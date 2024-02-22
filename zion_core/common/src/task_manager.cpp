/*
 * LIVSMED CONFIDENTIAL
 *
 * Copyright (c) 2024 LIVSMED, INC.
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the property
 * of LIVSMED and its suppliers, if any. The intellectual and technical concepts
 * contained herein are proprietary to LIVSMED and its suppliers and may be
 * covered by S.Korea and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from LIVSMED.
 *
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */

#include "task_manager.h"

TaskManager::TaskManager(size_t num_worker_)
: num_worker_(num_worker_)
, stop_all_(false)
, watching_(true)
{

    cur_worker_ = 0;
    worker_.reserve(num_worker_);
    for (size_t i = 0; i < num_worker_; ++i)
    {
        worker_.emplace_back(std::make_unique<std::thread>([this]()
                            { this->workerThread(); }));
    }

    watcher_ = std::make_unique<std::thread>(&TaskManager::watchFuture, this);
    msgSender_ = std::make_unique<MessageSender>();
}

TaskManager::~TaskManager()
{
    stop_all_ = true;
    watching_ = false;

    cv_job_.notify_all();

    for (auto &t : worker_)
    {
        t->join();
    }

    if (watcher_ != nullptr)
    {
        watcher_->join();
    }
    LOG_DEBUG("TaskManager Destroyed Done.");
}

template <class F, class... Args>
void TaskManager::enqueueJob(MessageQueue<int>* fu, shared_ptr<ic::IC_MSG> task, const ic::ServerInfo& info, F &&f, Args &&...args)
{
    if (stop_all_)
    {
        throw std::runtime_error("Cannotadd job in ThreadPool");
    }

    using return_type = typename std::invoke_result<F, Args...>::type;
    auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> job_result_future = job->get_future();

    std::lock_guard<std::mutex> lock(taskInfoMutex);
    std::string token = "abcd"; //teporary
    taskInfo.push_back(TaskInfo(std::move(job_result_future), token, task, info));

    {
        std::lock_guard<std::mutex> lock(jobMutex_);
        jobs.push([job]()
                  { (*job)(); });
    }
    cv_job_.notify_one();
    fu->Enqueue(job_result_future.get());
}

// message_manager call this function for doing task after message parsing
// or EventManager call this function by EventHandler
// so, id could be different by the caller. COMMAND_ID or EVENT_ID
int TaskManager::commandTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task)
{

    if (cur_worker_ == num_worker_)
        LOG_DEBUG("Job Queue is fool. working worker + job = : {}", cur_worker_);
    cur_worker_++;

    if (id == (int)ic::COMMAND_CLASS::COMMAND_VERSION)
    {
        LOG_INFO("TEST API COMMAND_VERSION {} ", task.Command);
    }
    else if (id == (int)ic::COMMAND_CLASS::COMMAND_STOP)
    {

    }
    else if (id == (int)ic::COMMAND_CLASS::COMMAND_VERSION)
    {

    }

    return (int)ErrorCommon::COMMON_ERR_NONE;
}

// future que detect the message after complete task,
// pop the data from both taskMsg and future que.
void TaskManager::watchFuture()
{
    while (watching_)
    {
        std::unique_lock<std::mutex> lock(taskInfoMutex);
        for (auto it = taskInfo.begin(); it != taskInfo.end(); )
        {
            if (it->resultFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                int result = it->resultFuture.get();
                // originally, intend to send message after task.
                // but message_sender have responsiblity to send message,
                // until now, there is no process with sending message after task.
                // someday i have to implement with correct context
//                makeSendMsg(it->info, it->taskMsg, result);
                it = taskInfo.erase(it); // 작업 처리 후 TaskInfo 제거
            }
            else
            {
                ++it;
            }
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// workerThread is created when TaskManager is created.
// workerThread is cloned as much as num_worker
// if there is a job in jobs que, pop the job and execute job.
void TaskManager::workerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(jobMutex_);
        cv_job_.wait(lock, [this]()
        { return !this->jobs.empty() || stop_all_; });

        if (stop_all_ && this->jobs.empty())
        {
            return;
        }
        std::function<void()> job = std::move(jobs.front());
        jobs.pop();
        lock.unlock();
        job();
    }
//}
//
//void TaskManager::makeSendMsg(ic::ServerInfo& info, std::shared_ptr<ic::IC_MSG> ptrMsg, int result)
//{
//
//    if (result < (int)ErrorCommon::COMMON_ERR_NONE)
//    {
//        LOG_WARN(" Captured future return is ERR {} ", result);
//        return;
//    }
//
//    Document sndDoc(kObjectType);
//    Document::AllocatorType &allocator = sndDoc.GetAllocator();
//
//    if (result == (int)ErrorCommon::COMMON_ERR_TYPE_NAME_STRING)
//    {
//        Document recvDoc;
////        recvDoc.Parse(ptrMsg->txt);
//        std::string outfile;
//        if (recvDoc.HasMember("output"))
//            outfile = recvDoc["output"].GetString();
//
//        std::string str_token = Configurator::get().generateToken();
//        LOG_INFO(" Generated token {} ", str_token.c_str());
//
//        sndDoc.AddMember(PROTOCOL_TYPE, "REQUEST", allocator);
//        sndDoc.AddMember(PROTOCOL_COMMAND, "NOTIFY", allocator);
//        sndDoc.AddMember(PROTOCOL_SUBCOMMAND, "PROCESS_DONE", allocator);
//        sndDoc.AddMember(PROTOCOL_ACTION, "action", allocator);
//        sndDoc.AddMember(PROTOCOL_TOKEN, str_token, allocator); // token..
//        sndDoc.AddMember(PROTOCOL_FROM, "ICS", allocator);
//        sndDoc.AddMember(PROTOCOL_TO, "SR1", allocator);
//        sndDoc.AddMember(PROTOCOL_DATA, "set", allocator);
//        sndDoc.AddMember("output", outfile, allocator);
//    }
//
//    std::string strSendString = convertDocumentForSend(sndDoc);
////    msgmanager_->insertEventTable(sndDoc, (int)ic::MSG_TYPE::MSG_TYPE_SND);
////    msgmanager_->onRcvSndMessage(ptrMsg->socket, strSendString);

}
