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

    enum class EventHandlerID
    {
        HANDLER_EVENT_TASK = 0,
        HANDLER_COMMAND_TASK,
    };

    static void initialize()
    {
        getInstance().ev_map_[static_cast<int>(ic::EVENT_ID::EVENT_ID_WHO)] =
                static_cast<int>(EventHandlerID::HANDLER_EVENT_TASK);
        getInstance().ev_map_[static_cast<int>(ic::EVENT_ID::EVENT_ID_TCP_LOG_START)] =
                static_cast<int>(EventHandlerID::HANDLER_EVENT_TASK);
    }

    static bool addEventHandler(EventHandler handler)
    {
        std::lock_guard<std::mutex> guard(getInstance().mutex_);
//        bool ret = (getInstance().ev_map_.find(id) == getInstance().ev_map_.end());
//        getInstance().ev_map_[id] = handler;
        getInstance().ev_handlers_.push_back(handler);
        return true;
    }

    static int callEvent(int id, void* context1, void* context2)
    {
        auto& instance = getInstance();
        std::lock_guard<std::mutex> guard(instance.mutex_);
        auto it = instance.ev_map_.find(id);
        if (it != instance.ev_map_.end())
        {
//            return it->second(id, context1, context2);
            return instance.ev_handlers_[it->second](id, context1, context2);
        }
        return -1;
    }

private:
    std::vector<EventHandler> ev_handlers_;
    std::map<int, int> ev_map_;
    std::mutex mutex_;

    static EventManager& getInstance()
    {
        static EventManager instance;
        return instance;
    }
};