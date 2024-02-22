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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/20.
 *
 */

#pragma once
#include "ic_define.h"
#include "task_manager.h"

using namespace rapidjson;

class ClientTaskManager : public TaskManager
{
public:
    ClientTaskManager();
    ~ClientTaskManager();
    int eventTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task);
    int errorTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task);
};