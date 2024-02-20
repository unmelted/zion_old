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
    using EventHandler = std::function<int(int, void*)>;

    static bool addEventHandler(int evid, EventHandler handler)
    {
        std::lock_guard<std::mutex> guard(getInstance().mutex_);
        bool ret = (getInstance().ev_map_.find(evid) == getInstance().ev_map_.end());
        getInstance().ev_map_[evid] = handler;
        return ret;
    }

    static int setEvent(int evid, void* context)
    {
        auto& instance = getInstance();
        std::lock_guard<std::mutex> guard(instance.mutex_);
        auto it = instance.ev_map_.find(evid);
        if (it != instance.ev_map_.end())
        {
            return it->second(evid, context);
        }
        return -1;
    }

private:
    std::map<int, EventHandler> ev_map_;
    std::mutex mutex_;

    static EventManager& getInstance()
    {
        static EventManager instance;
        return instance;
    }
};