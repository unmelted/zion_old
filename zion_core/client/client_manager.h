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
#include "ic_manager.h"
#include "ic_client.h"
#include "client_task.h"
#include "client_message.h"

using namespace rapidjson;

class ClientManager : public ICManager<ICClient, ClientTaskManager>
{
public:
    ClientManager();
    ~ClientManager();
    int initialize() override;

private:
    int processor(int mode, const ic::ClientInfo& info, char* pData, int nDataSize) override;

private :
    std::unique_ptr<ClientMsgManager> msg_manager_;
};

