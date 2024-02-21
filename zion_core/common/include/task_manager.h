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


#pragma once
#include "ic_define.h"
#include "message_sender.h"

using namespace rapidjson;

class TaskManager
{
private:
    struct TaskInfo
    {
        std::future<int> resultFuture;
        std::string token;
        std::shared_ptr<ic::MSG_T> taskMsg;
        ic::ServerInfo info;

        TaskInfo() : token(""), taskMsg(nullptr), info() {}
        TaskInfo(std::future<int> future, std::string tk, std::shared_ptr<ic::MSG_T> msg, ic::ServerInfo info_)
                : resultFuture(std::move(future))
                , token(std::move(tk))
                , taskMsg(std::move(msg))
                , info(info_) {}
    };

public:
    TaskManager(size_t num_worker_);
    ~TaskManager();

    int commandTask(int id, const ic::ServerInfo& info, const ic::MSG_T& task);
    virtual int eventTask(int id, const ic::ServerInfo& info, const ic::MSG_T& task) = 0;

protected:
    std::unique_ptr<MessageSender> msgSender_;

private:
    void watchFuture();
    void workerThread();
    void makeSendMsg(ic::ServerInfo& info, std::shared_ptr<ic::MSG_T> ptrMsg, int result);
//    std::string getDocumentToString(Document &document);

    template <class F, class... Args>
    void enqueueJob(MessageQueue<int>* fu, shared_ptr<ic::MSG_T> task, const ic::ServerInfo& info, F &&f, Args &&...args);
    int taskStart(int argument);

private:
    std::vector<std::unique_ptr<std::thread>> worker_;
    std::unique_ptr<std::thread> watcher_{nullptr};

    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job_;
    std::mutex jobMutex_;

    MessageQueue<int> future_;
    MessageQueue<std::shared_ptr<ic::MSG_T>> queTaskMSG_;

    std::vector<TaskInfo> taskInfo;
    std::mutex taskInfoMutex;

    size_t num_worker_;
    size_t cur_worker_;
    bool stop_all_;
    bool watching_;

};

