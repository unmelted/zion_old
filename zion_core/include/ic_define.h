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

#include <map>
#include <cstdio>
#include <string>
#include <time.h>
#include <cstring>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>
#include <set>
#include <cstdint>
#include <map>
#include <sqlite3.h>

#define RAPIDJSON_HAS_STDSTRING 1
#include "_3rdparty_/rapidjson/include/rapidjson/document.h"
#include "_3rdparty_/rapidjson/include/rapidjson/writer.h"
#include "_3rdparty_/rapidjson/include/rapidjson/stringbuffer.h"
#include "_3rdparty_/rapidjson/include/rapidjson/prettywriter.h"

#include "ic_server.h"
#include "logger.h"
#include "message_queue.h"
#include "ic_util.h"
#include "error_manager.h"


#define TASKPOOL_SIZE 10
#define CURRENTVERSION "0.0.1.T"


using namespace std;

namespace ic
{

enum class ROBOT_TYPE
{
    ROBOT_TYPE_NONE,
    ROBOT_TYPE_MC,
    ROBOT_TYPE_SR,
    ROBOT_TYPE_CR,
    ROBOT_TYPE_VCB,
    ROBOT_TYPE_IC,
    ROBOT_TYPE_SIZE,
};


enum class DB_TYPE
{
    DB_TYPE_LIVSMED,
    DB_TYPE_LOG,
    DB_TYPE_SIZE,
};

const string DB_DIRECTORY = "../db/";
const std::array<std::string, (int)DB_TYPE::DB_TYPE_SIZE> DB_NAME =
{
    DB_DIRECTORY + "livsmed.db",
    DB_DIRECTORY + "livsmed_log.db",
};

enum class SERVER_TYPE
{
    SERVER_TYPE_NONE,
    SERVER_ROBOT_CONTROL,
    SERVER_ROBOT_ALIVE,
};

const int SERVER_PORT[] =
{
    -1,
    0x4D15,
    0x4D16
};

enum class COMMAND_CLASS
{
    COMMAND_NONE = 0,
    COMMAND_CONNECT,
    COMMAND_START,
    COMMAND_INITIALIZE,
    COMMAND_STOP,
    COMMAND_RESTART,
    COMMAND_GETINFO,
    COMMAND_SETINFO,
    COMMAND_HEARTBEAT,
    COMMAND_VERSION,
    COMMAND_NOTIFY,
    COMMAND_SIZE,
};

struct Protocol
{
    std::string Type;
    std::string Command;
    std::string SubCommand;
    std::string Action;
    std::string Token;
    std::string From;
    std::string To;
    std::string Data;

};


#define PROTOCOL_SECTION1    "Type"
#define PROTOCOL_SECTION2    "Command"
#define PROTOCOL_SECTION3    "SubCommand"
#define PROTOCOL_ACTION      "Action"
#define PROTOCOL_TOKEN       "Token"
#define PROTOCOL_FROM        "From"
#define PROTOCOL_TO          "To"
#define PROTOCOL_DATA        "Data"

#define PROTOCOL_RESULTCODE  "ResultCode"
#define PROTOCOL_ERRORMSG    "ErrorMsg"

enum class MSG_TYPE
{
    MSG_TYPE_NONE,
    MSG_TYPE_SND,
    MSG_TYPE_RCV,
    MSG_TYPE_LOG,
    MSG_TYPE_DB_INSERT,
    MSG_TYPE_DB_UPDATE,
    MSG_TYPE_DB_DELETE,
};

typedef struct _MSG_T
{
    int type;
    std::string target;
    std::string txt;

    _MSG_T(void)
    : txt{}
    , type(-1)
    {
    }

} MSG_T;


// Packet Separator Type
enum class PACKET_SEPARATOR
{
    PACKETTYPE_JSON,
    PACKETTYPE_BINALY,
    PACKETTYPE_ABAILABLE_INQUIRY_VP = 20,
    PACKETTYPE_VP_SET_BUFFERCOUNT,
    PACKETTYPE_VP_ALIVE,
    PACKETTYPE_CALIBRATION_VALUE = 100,

    PACKETTYPE_SIZE,
};

enum class DB_LOG_COLUMN
{
    DB_LOG_COLUMN_DATE,
    DB_LOG_COLUMN_PID,
    DB_LOG_COLUMN_TID,
    DB_LOG_COLUMN_LEVEL,
    DB_LOG_COLUMN_FILE,
    DB_LOG_COLUMN_MSG,
    DB_LOG_COLUMN_SIZE,
};


#pragma pack(push, 1)
struct ProtocolHeader
{
    int nSize;
    char cSeparator;
};

#pragma pack(pop)

}