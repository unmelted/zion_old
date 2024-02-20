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

#include <functional>
#include "ic_manager.h"
#include "ic_server.h"
#include "server_task.h"
#include "server_message.h"

using namespace rapidjson;

class ServerManager : public ICManager<ICServer, ServerTaskManager>
{
public:
    ServerManager();
	~ServerManager();
    int initialize() override;

private:
    int classifier(const ic::ClientInfo& info, char* pData, int nDataSize) override;

private:
    std::unique_ptr<SeverMsgManager> msg_manager_;
//    std::unique_ptr<MessageResponder<ICServer>> msg_rspndr_;

};

