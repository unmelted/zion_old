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
#include "db_manager.h"
#include "message_sender.h"

class TaskManager
{
private:
    struct TaskInfo
    {
        std::future<int> resultFuture;
        std::string token;
        std::shared_ptr<ic::IC_MSG> taskMsg;
        ic::ServerInfo info;

        TaskInfo() : token(""), taskMsg(nullptr), info() {}
        TaskInfo(std::future<int> future, std::string tk, std::shared_ptr<ic::IC_MSG> msg, ic::ServerInfo info_)
                : resultFuture(std::move(future))
                , token(std::move(tk))
                , taskMsg(std::move(msg))
                , info(info_) {}
    };

public:
    TaskManager(size_t num_worker_);
    ~TaskManager();

    void setDBManager(std::shared_ptr<DBManager>& db_manager);
    int commandTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task);
    virtual int eventTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task) = 0;
    virtual int errorTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task) = 0;

protected:
    std::unique_ptr<MessageSender> msg_sender_;
    std::shared_ptr<DBManager> db_manager_;

private:
    void watchFuture();
    void workerThread();

    template <class F, class... Args>
    void enqueueJob(MessageQueue<int>* fu, shared_ptr<ic::IC_MSG> task, const ic::ServerInfo& info, F &&f, Args &&...args);

private:
    std::vector<std::unique_ptr<std::thread>> worker_;
    std::unique_ptr<std::thread> watcher_{nullptr};

    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job_;
    std::mutex jobMutex_;

    MessageQueue<int> future_;
    MessageQueue<std::shared_ptr<ic::IC_MSG>> queTaskMSG_;

    std::vector<TaskInfo> taskInfo;
    std::mutex taskInfoMutex;

    size_t num_worker_;
    size_t cur_worker_;
    bool stop_all_;
    bool watching_;

};

