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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/14.
 *
 */

#pragma once

#include "ic_define.h"
#include "event_manager.h"
#include "task_manager.h"
#include "db_manager.h"

using namespace rapidjson;

template <typename T, typename U>
class ICManager
{
public:
    ICManager();
    virtual int initialize() = 0;

protected:
    virtual int doManage(int mode, const ic::ClientInfo& info, char* pData, int nDataSize);

    std::vector<std::shared_ptr<ic::ServerInfo>> server_info_list_;
    std::vector<std::shared_ptr<T>> socket_list_;
    std::unique_ptr<U> task_manager_;
    std::shared_ptr<DBManager> db_manager_;
};

#include "ic_manager.tpp"