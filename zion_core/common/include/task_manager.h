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

using namespace rapidjson;

class TaskManager
{
private:
    struct TaskInfo
    {
        std::future<int> resultFuture;
        std::string token;
        std::shared_ptr<ic::MSG_T> taskMsg;

        TaskInfo(std::future<int> future, std::string tk, std::shared_ptr<ic::MSG_T> msg)
                : resultFuture(std::move(future)), token(std::move(tk)), taskMsg(std::move(msg)) {}
    };

public:
    TaskManager(size_t num_worker_);
    ~TaskManager();

    void onRcvTask(std::shared_ptr<ic::MSG_T> pData);
    int commandTask(int mode, const ic::MSG_T& task);

private:
    void watchFuture();
    void workerThread();
    void makeSendMsg(std::shared_ptr<ic::MSG_T> ptrMsg, int result);
    std::string getDocumentToString(Document &document);
    template <class F, class... Args>
    void enqueueJob(MessageQueue<int>* fu, shared_ptr<ic::MSG_T> task, F &&f, Args &&...args);

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

