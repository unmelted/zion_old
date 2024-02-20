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

#include <array>
#include <map>
#include <unordered_map>
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
#include <sqlite3.h>

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "logger.h"
#include "message_queue.h"
#include "ic_util.h"
#include "error_manager.h"



#define CURRENTVERSION "0.0.1.T"

using namespace std;

namespace ic
{

const int TASKPOOL_SIZE = 5;
const int CONNECT_WAIT_TIME = 5; //seconds
const int QUEUE_EMPTY_CHECK = 50; //milliseconds
//const int

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

const string DB_DIRECTORY = "db/";
const string DB_CONFIG = DB_DIRECTORY + "db_config.json";
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
    SERVER_ROBOT_LOGMONITOR,
};

const int SERVER_PORT[] =
{
    -1,
    0x4D15,
    0x4D16,
    0x4D17,
};

struct ServerInfo {
    std::string name;
    std::string ip;
    int port;
    int socket;
    bool isAvailable;
    bool waitWhenDisconnect;

    ServerInfo() : port(0), socket(-1), isAvailable(false), waitWhenDisconnect(false) {}

    ServerInfo(const std::string& name, const std::string& ip, int port)
            : name(name), ip(ip), port(port), socket(-1), isAvailable(false), waitWhenDisconnect(false) {}
    ServerInfo(const std::string& name, const std::string& ip, int port, int sock)
            : name(name), ip(ip), port(port), isAvailable(false), waitWhenDisconnect(false), socket(sock) {}

};

typedef ServerInfo ClientInfo;

enum class EVENT_ID
{
    EVENT_ID_NONE,
    EVENT_ID_WHO,
    EVENT_ID_TCP_LOG_START,
    EVENT_ID_SIZE,
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

typedef Protocol  MSG_T;

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

enum class DB_EVENT_HISTORY_COLUMN
{
//    DB_EVENT_HISTORY_COLUMN_DATE, // date is default value in sqlite3
    DB_EVENT_HISTORY_COLUMN_TYPE,
    DB_EVENT_HISTORY_COLUMN_COMMAND,
    DB_EVENT_HISTORY_COLUMN_SUBCOMMAND,
    DB_EVENT_HISTORY_COLUMN_ACTION,
    DB_EVENT_HISTORY_COLUMN_TOKEN,
    DB_EVENT_HISTORY_COLUMN_FROM,
    DB_EVENT_HISTORY_COLUMN_TO,
    DB_EVENT_HISTORY_COLUMN_DATA,
    DB_EVENT_HISTORY_COLUMN_SIZE,
};

#pragma pack(push, 1)
struct ProtocolHeader
{
    int nSize;
    char cSeparator;
};

#pragma pack(pop)

}