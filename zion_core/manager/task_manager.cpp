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

#include "task_manager.hpp"
#include "message_manager.hpp"

using namespace rapidjson;

TaskManager::TaskManager(size_t num_worker_, MsgManager *msg_manager)
: num_worker_(num_worker_)
, stop_all_(false)
, watching_(true)
{

    msgmanager_ = msg_manager;
    cur_worker_ = 0;
    worker_.reserve(num_worker_);
    for (size_t i = 0; i < num_worker_; ++i)
    {
        worker_.emplace_back(std::make_unique<std::thread>([this]()
                            { this->workerThread(); }));
    }

    watcher_ = std::make_unique<std::thread>(&TaskManager::watchFuture, this);
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

// message_manager call this function with task message
void TaskManager::onRcvTask(std::shared_ptr<ic::MSG_T> ptrMsg)
{
    queTaskMSG_.Enqueue(ptrMsg);
}

template <class F, class... Args>
void TaskManager::enqueueJob(MessageQueue<int> *fu, F &&f, Args &&...args)
{
    if (stop_all_)
    {
        throw std::runtime_error("Can't add job in ThreadPool");
    }

    using return_type = typename std::result_of<F(Args...)>::type;
    auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> job_result_future = job->get_future();
    {
        std::lock_guard<std::mutex> lock(jobMutex_);
        jobs.push([job]()
                  { (*job)(); });
    }
    cv_job_.notify_one();
    fu->Enqueue(job_result_future.get());
}

// message_manager call this function for doing task after message parsing
int TaskManager::commandTask(int mode, std::string arg)
{

    if (cur_worker_ == num_worker_)
        LOG_DEBUG("CMd Job Queue is fool. working worker + job = : {}", cur_worker_);
    cur_worker_++;

    if (mode == (int)ic::COMMAND_STRUCTURE::COMMAND_START)
    {
//         ExpUtil in;
//         shared_ptr<VIDEO_INFO> info = make_shared<VIDEO_INFO>();
//         int result = in.ImportVideoInfo(arg, info.get());
//         LOG_INFO(" swipe period size {} ", info->swipe_period.size());
//         if (result == ic::ERR_NONE)
//             EnqueueJob(&future_, &TaskManager::RunStabilize, this, info);
//         else
//         {
//             LOG_WARN(" Stabilization Message is not compatible ERR: {} ", result);
//             queTaskMSG_.Dequeue();
//         }
    }
    else if (mode == (int)ic::COMMAND_STRUCTURE::COMMAND_STOP)
    {
    }
    else if (mode == (int)ic::COMMAND_STRUCTURE::COMMAND_VERSION)
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
        if (future_.IsQueue())
        {
            makeSendMsg(queTaskMSG_.Dequeue(), future_.Dequeue());
            cur_worker_--;
        }
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
}

void TaskManager::makeSendMsg(std::shared_ptr<ic::MSG_T> ptrMsg, int result)
{

    if (result < (int)ErrorCommon::COMMON_ERR_NONE)
    {
        LOG_WARN(" Captured future return is ERR {} ", result);
        return;
    }

    Document sndDoc(kObjectType);
    Document::AllocatorType &allocator = sndDoc.GetAllocator();

    if (result == (int)ErrorCommon::COMMON_ERR_TEMPORARY)
    {
        Document recvDoc;
        recvDoc.Parse(ptrMsg->txt);
        std::string outfile; 
        if (recvDoc.HasMember("output")) 
            outfile = recvDoc["output"].GetString();
        
        std::string str_token = Configurator::get().generateToken();
        LOG_INFO(" Generated token {} ", str_token.c_str());

        sndDoc.AddMember(PROTOCOL_SECTION1, "4DReplay", allocator);
        sndDoc.AddMember(PROTOCOL_SECTION2, "CM", allocator);
        sndDoc.AddMember(PROTOCOL_SECTION3, "StabilizeDone", allocator);
        sndDoc.AddMember(PROTOCOL_ACTION, "action", allocator);
        sndDoc.AddMember(PROTOCOL_TOKEN, str_token, allocator); // token..
        sndDoc.AddMember(PROTOCOL_FROM, "CMd", allocator);
        sndDoc.AddMember(PROTOCOL_TO, "4DPD", allocator);
        sndDoc.AddMember(PROTOCOL_DATA, "set", allocator);
        sndDoc.AddMember("output", outfile, allocator);
    }

    std::string strSendString = getDocumentToString(sndDoc);
    msgmanager_->onRcvSndMessage(strSendString);
}

std::string TaskManager::getDocumentToString(Document &document)
{
    StringBuffer strbuf;
    strbuf.Clear();
    PrettyWriter<StringBuffer> writer(strbuf);
    document.Accept(writer);
    std::string ownShipRadarString = strbuf.GetString();

    return ownShipRadarString;
}
