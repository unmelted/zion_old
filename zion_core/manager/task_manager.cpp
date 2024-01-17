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

TaskManager::TaskManager(size_t num_worker_, MsgManager *a)
    : num_worker_(num_worker_), stop_all_(false), watching_(true)
{

    msgmanager_ = a;
    cur_worker_ = 0;
    worker_.reserve(num_worker_);
    for (size_t i = 0; i < num_worker_; ++i)
    {
        worker_.emplace_back([this]()
                            { this->workerThread(); });
    }

    watcher_ = new std::thread(&TaskManager::watchFuture, this);
}

TaskManager::~TaskManager()
{
    stop_all_ = true;
    watching_ = false;

    cv_job_.notify_all();

    for (auto &t : worker_)
    {
        t.join();
    }

    if (watcher_ != nullptr)
    {
        watcher_->join();
        delete watcher_;
        watcher_ = nullptr;
    }
    CMd_DEBUG("TaskManager Destroyed Done.");
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
        std::lock_guard<std::mutex> lock(jobMutex);
        jobs.push([job]()
                  { (*job)(); });
    }
    cv_job_.notify_one();
    fu->Enqueue(job_result_future.get());
}

void TaskManager::workerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(jobMutex);
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

// int TaskManager::RunStabilize(shared_ptr<VIDEO_INFO> arg)
// {
//     int result = -1;
//     CMd_DEBUG(" Stabil Swipe period size {}", arg->swipe_period.size());
//     unique_ptr<Dove> stblz(new Dove());
//     stblz->SetInfo(arg.get());
//     result = stblz->Process();
//     CMd_INFO("Run stabilize Done. Result : {}" , result);
//     return result;
// }

int TaskManager::commandTask(int mode, std::string arg)
{

    if (cur_worker_ == num_worker_)
        CMd_DEBUG("CMd Job Queue is fool. working worker + job = : {}", cur_worker_);
    cur_worker_++;

    if (mode == (int)ic::COMMAND_TYPE::COMMAND_START)
    {
        // ExpUtil in;
        // shared_ptr<VIDEO_INFO> info = make_shared<VIDEO_INFO>();
        // int result = in.ImportVideoInfo(arg, info.get());
        // CMd_INFO(" swipe period size {} ", info->swipe_period.size());
        // if (result == ic::ERR_NONE)
        //     EnqueueJob(&m_future, &TaskManager::RunStabilize, this, info);
        // else
        // {
        //     CMd_WARN(" Stabilization Message is not compatible ERR: {} ", result);
        //     m_qTMSG.Dequeue();
        // }
    }
    else if (mode == (int)ic::COMMAND_TYPE::COMMAND_STOP)
    {
    }
    else if (mode == (int)ic::COMMAND_TYPE::COMMAND_VERSION)
    {
        sendVersionMessage(arg);
    }

    return (int)ErrorCommon::COMMON_ERR_NONE;
}

void TaskManager::watchFuture()
{

    while (watching_)
    {
        if (m_future.IsQueue())
        {
            makeSendMsg(m_qTMSG.Dequeue(), m_future.Dequeue());
            cur_worker_--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TaskManager::onRcvTask(std::shared_ptr<ic::MSG_T> ptrMsg)
{
    m_qTMSG.Enqueue(ptrMsg);
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

void TaskManager::makeSendMsg(std::shared_ptr<ic::MSG_T> ptrMsg, int result)
{

    if (result < (int)ErrorCommon::COMMON_ERR_NONE)
    {
        CMd_WARN(" Captured future return is ERR {} ", result);
        return;
    }

    Document sndDoc(kObjectType);
    Document::AllocatorType &allocator = sndDoc.GetAllocator();

    if (result == (int)ErrorCommon::COMMON_ERR_TEMPORARY)
    {
        nlohmann::json j = nlohmann::json::parse(ptrMsg->txt);
        std::string outfile = j["output"];
        std::string str_token = Configurator::get().generateToken();
        CMd_INFO(" Generated token {} ", str_token.c_str());

        sndDoc.AddMember(PROTOCOL_SECTION1, "4DReplay", allocator);
        sndDoc.AddMember(PROTOCOL_SECTION2, "CM", allocator);
        sndDoc.AddMember(PROTOCOL_SECTION3, "StabilizeDone", allocator);
        sndDoc.AddMember(PROTOCOL_SENDSTATE, "request", allocator);
        sndDoc.AddMember(PROTOCOL_TOKEN, str_token, allocator); // token..
        sndDoc.AddMember(PROTOCOL_FROM, "CMd", allocator);
        sndDoc.AddMember(PROTOCOL_TO, "4DPD", allocator);
        sndDoc.AddMember(PROTOCOL_ACTION, "set", allocator);
        sndDoc.AddMember("output", outfile, allocator);
    }

    std::string strSendString = getDocumentToString(sndDoc);
    msgmanager_->onRcvSndMessage(strSendString);
}

void TaskManager::sendVersionMessage(std::string ptrMsg)
{
    Document document;
    document.Parse(ptrMsg.c_str());
    Document sndDoc(kObjectType);
    Document::AllocatorType &allocator = sndDoc.GetAllocator();

    Value ver(kObjectType);
    Value cmd(kObjectType);
    cmd.AddMember("verion", CURRENTVERSION, allocator);
    cmd.AddMember("date", Configurator::get().getCurrentDateTime("now"), allocator);
    ver.AddMember("CMd", cmd, allocator);

    sndDoc.AddMember(PROTOCOL_SECTION1, document[PROTOCOL_SECTION1], allocator);
    sndDoc.AddMember(PROTOCOL_SECTION2, document[PROTOCOL_SECTION2], allocator);
    sndDoc.AddMember(PROTOCOL_SECTION3, document[PROTOCOL_SECTION3], allocator);
    sndDoc.AddMember(PROTOCOL_SENDSTATE, "response", allocator);
    sndDoc.AddMember(PROTOCOL_TOKEN, document[PROTOCOL_TOKEN], allocator);
    sndDoc.AddMember(PROTOCOL_FROM, document[PROTOCOL_TO], allocator);
    sndDoc.AddMember(PROTOCOL_TO, document[PROTOCOL_FROM], allocator);
    sndDoc.AddMember(PROTOCOL_ACTION, document[PROTOCOL_ACTION], allocator);
    sndDoc.AddMember("Version", ver, allocator);
    sndDoc.AddMember(PROTOCOL_RESULTCODE, (int)ErrorCommon::COMMON_ERR_NONE, allocator);
    sndDoc.AddMember(PROTOCOL_ERRORMSG, "SUCCESS", allocator);
    std::string strSendString = getDocumentToString(sndDoc);
    msgmanager_->onRcvSndMessage(strSendString);
}