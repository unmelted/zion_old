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

#include "define.h"

class MsgManager;

using namespace rapidjson;

namespace TaskPool
{
    class TaskManager
    {

    public:
        TaskManager(size_t num_worker_, MsgManager *a);
        ~TaskManager();

        template <class F, class... Args>
        void EnqueueJob(MessageQueue<int> *fu, F &&f, Args &&...args);
        void OnRcvTask(std::shared_ptr<ic::MSG_T> pData);
        int CommandTask(int mode, std::string arg); // shared_ptr<VIDEO_INFO> arg);
        void SetSndQue(std::function<void(MsgManager &, const std::string msg)> que)
        {
            fSendQue = que;
        };

    private:
        size_t num_worker;
        size_t cur_worker;
        std::vector<std::thread> worker;
        std::thread *watcher{nullptr};
        std::queue<std::function<void()>> jobs;
        std::condition_variable cv_job;
        std::mutex m_job;
        MessageQueue<int> m_future;
        MessageQueue<std::shared_ptr<ic::MSG_T>> m_qTMSG;
        std::function<void(MsgManager &, const std::string msg)> fSendQue;

        MsgManager *m_msgmanager;
        bool stop_all;
        bool watching;
        void WorkerThread();
        void WatchFuture();
        // int RunStabilize(shared_ptr<VIDEO_INFO> arg);
        void MakeSendMsg(std::shared_ptr<ic::MSG_T> ptrMsg, int result);

        void SendVersionMessage(std::string ptrMsg);
        std::string GetDocumentToString(Document &document);
    };

} // namespace
