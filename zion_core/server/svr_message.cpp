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
#include "svr_message.h"

using namespace rapidjson;

SvrMsgManager::SvrMsgManager()
: SocketMsgManager<ICServer>()
, taskmanager_(3, this)
{
    setDBManager(std::make_shared<DBManager>());
    setSocketServer(std::make_shared<ICServer>(1));
}

SvrMsgManager::~SvrMsgManager()
{

}