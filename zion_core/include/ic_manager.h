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

#include "ic_server.h"
#include "ic_define.h"
#include <functional>
#include "message_responder.h"
#include "message_manager.h"
#include "db_manager.h"

using namespace rapidjson;

class ICManager
{
public:
	ICManager();
	~ICManager();

private:
	int validateMsg(char cSeparator, char* pData, int nDataSize);

    std::shared_ptr<ICServer> icServer_;
    std::unique_ptr<MessageResponder> msg_rspndr_;
    std::unique_ptr<MsgManager> msg_manager_;
    std::unique_ptr<DBManager> db_manager_;

};

