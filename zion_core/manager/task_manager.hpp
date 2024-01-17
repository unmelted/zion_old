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

#include "ics_define.h"

class MsgManager;

using namespace rapidjson;

class TaskManager
{

public:
    TaskManager(size_t num_worker_, MsgManager *a);
    ~TaskManager();

    template <class F, class... Args>
    void enqueueJob(MessageQueue<int> *fu, F &&f, Args &&...args);
    void onRcvTask(std::shared_ptr<ic::MSG_T> pData);
    int commandTask(int mode, std::string arg); // shared_ptr<VIDEO_INFO> arg);
    void setSndQue(std::function<void(MsgManager &, const std::string msg)> que)
    {
        fSendQue = que;
    };

private:
    void workerThread();
    void watchFuture();
    // int RunStabilize(shared_ptr<VIDEO_INFO> arg);
    void makeSendMsg(std::shared_ptr<ic::MSG_T> ptrMsg, int result);

    void sendVersionMessage(std::string ptrMsg);
    std::string getDocumentToString(Document &document);

private:
    size_t num_worker_;
    size_t cur_worker_;
    std::vector<std::thread> worker_;
    std::thread *watcher_{nullptr};
    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job_;
    std::mutex jobMutex;
    MessageQueue<int> m_future;
    MessageQueue<std::shared_ptr<ic::MSG_T>> m_qTMSG;
    std::function<void(MsgManager &, const std::string msg)> fSendQue;

    MsgManager *msgmanager_;
    bool stop_all_;
    bool watching_;

};

