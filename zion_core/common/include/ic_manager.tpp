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

#include "ic_manager.h"

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
template <typename T>
int ICManager<T>::classifier(const ic::ServerInfo& info, char* pData, int nDataSize)
{
    std::string strMessage = pData;
    Document document;
    bool isSuccess = false;
    try
    {
        isSuccess = document.Parse(strMessage.c_str()).HasParseError();
    }
    catch (...)
    {
        isSuccess = false;
    }

    if (isSuccess)
    {
        LOG_ERROR("Json Parsing Faile {} ", strMessage);
        return 0;
    }

    if (!document.HasMember("Type")
        || !document.HasMember("Command")
        || !document.HasMember("SubCommand")
        || !document.HasMember("Action")
        || !document.HasMember("Token"))
    {
        LOG_ERROR("Json component missing. can't execute.");
        return 0;
    }

    return 1;
}
