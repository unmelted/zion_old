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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/23.
 *
 */

#pragma once

#include "ic_define.h"

class EventManager
{
public:
    using EventHandler = std::function<int(int, void*, void*)>;
    using ErrorHandler = std::function<int(int, const char*, int)>;

    enum class EventHandlerID
    {
        HANDLER_EVENT_TASK = 0,
        HANDLER_ERROR_TASK,
    };

    enum class ErrHandlerID
    {
        HANDLER_MSG_STORE = 0,
    };

    static void initialize()
    {
        getInstance().ev_map_[static_cast<int>(ic::EVENT_ID::EVENT_ID_WHO)] =
                static_cast<int>(EventHandlerID::HANDLER_EVENT_TASK);
        getInstance().ev_map_[static_cast<int>(ic::EVENT_ID::EVENT_ID_TCP_LOG_START)] =
                static_cast<int>(EventHandlerID::HANDLER_EVENT_TASK);
        getInstance().ev_map_[static_cast<int>(ic::EVENT_ID::EVENT_ID_REQUEST_INFO)] =
                static_cast<int>(EventHandlerID::HANDLER_EVENT_TASK);


        getInstance().err_map_[static_cast<int>(ERROR_IC::IC_ERR_DISCONNECT_SLAVE)] =
                static_cast<int>(ErrHandlerID::HANDLER_MSG_STORE);


    }

    static int addEventHandler(EventHandler handler)
    {
        std::lock_guard<std::mutex> guard(getInstance().mutex_);
        getInstance().ev_handlers_.push_back(handler);
        return SUCCESS;
    }

    static int addErrorHandler(ErrorHandler handler)
    {
        std::lock_guard<std::mutex> guard(getInstance().mutex_);
        getInstance().err_handlers_.push_back(handler);
        return SUCCESS;
    }

    static int callEvent(int id, void* context1, void* context2)
    {
        auto& instance = getInstance();
        std::lock_guard<std::mutex> guard(instance.mutex_);
        auto it = instance.ev_map_.find(id);
        if (it != instance.ev_map_.end())
        {
            return instance.ev_handlers_[it->second](id, context1, context2);
        }
        return FAIL;
    }

    static int callError(int id, const char* file, int line)
    {
        auto& instance = getInstance();
        std::lock_guard<std::mutex> guard(instance.mutex_);
        auto it = instance.err_map_.find(id);
        if (it != instance.err_map_.end())
        {
            return instance.err_handlers_[it->second](id, file, line);
        }
        return FAIL;
    }

private:
    std::vector<EventHandler> ev_handlers_;
    std::vector<ErrorHandler> err_handlers_;
    std::map<int, int> ev_map_;
    std::map<int, int> err_map_;
    std::mutex mutex_;

    static EventManager& getInstance()
    {
        static EventManager instance;
        return instance;
    }
};