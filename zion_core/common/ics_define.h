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


#define RAPIDJSON_HAS_STDSTRING 1
#include "_3rdparty_/rapidjson/include/rapidjson/document.h"
#include "_3rdparty_/rapidjson/include/rapidjson/writer.h"
#include "_3rdparty_/rapidjson/include/rapidjson/stringbuffer.h"
#include "_3rdparty_/rapidjson/include/rapidjson/prettywriter.h"
#include "ic_server.h"
#include "logger.hpp"
#include "json.hpp"
#include "message_queue.h"
#include "ics_util.hpp"

#include "error_manager.h"


#define MESSAGELTRANSFER_DAEMON_PORT        0x4D01

#define ROBOT_CONTROL_PORT              0x4D15

#define CONTROLLER_PORT                     0x4D20
#define PRODUCING_PORT                      0x4D21
#define VAR_PORT                            0x4D22
#define EMS_PORT                            0x4D23
#define DAEMONVIEWER_PORT                   0x4D24
#define BUFFER_VIEWER_PORT                  0x4D25
#define LOGVIEWER_PORT                      0x4D26

#define VP_BUFFER_MAXSIZE	10
#define TASKPOOL_SIZE 3
#define CURRENTVERSION "0.0.1.T"


using namespace std;

namespace ic
{
enum class COMMAND_TYPE
{
    COMMAND_NONE = 0,
    COMMAND_VERSION,
    COMMAND_START,
    COMMAND_STOP,
    COMMAND_RESTART,
    COMMAND_STATUS,
};

struct Protocol
{
    std::string Type;
    std::string Command;
    std::string SubCommand;
    std::string SendState;
    std::string From;
    std::string To;
    std::string action;
    std::string Token;
};


typedef struct PACKET_TYPE
{
    static const int TEXT = 0;
    static const int BINARY = 1;
} PACKET_TYPE;


typedef struct _MSG_T
{
    int32_t type;
    std::string txt;
    char* bin;
    int32_t bin_size;

    _MSG_T(void)
            : type(PACKET_TYPE::TEXT)
            , txt{}
            , bin(nullptr)
            , bin_size(0)
    {
    }

    ~_MSG_T(void)
    {
        if (bin && bin_size > 0)
            free(bin);
        bin = nullptr;
        bin_size = 0;
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

#define PROTOCOL_SECTION1    "Type"
#define PROTOCOL_SECTION2    "Command"
#define PROTOCOL_SECTION3    "SubCommand"
#define PROTOCOL_SENDSTATE   "SendState"
#define PROTOCOL_FROM        "From"
#define PROTOCOL_TO          "To"
#define PROTOCOL_ACTION      "Action"
#define PROTOCOL_TOKEN       "Token"
#define PROTOCOL_RESULTCODE  "ResultCode"
#define PROTOCOL_ERRORMSG    "ErrorMsg"

#define PROTOCOL_SENDSTATE_REQUEST       "request"
#define PROTOCOL_SENDSTATE_RESPONSE      "response"

#define MTDHEADERSIZE           5
//#define MTDPROTOCOL                           "YJS"

#pragma pack(push, 1)
struct ProtocolHeader
{
    int nSize;
    char cSeparator;
};

#pragma pack(pop)

class Version
{
    Version(std::string versionStr)
    {
        sscanf(versionStr.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
    }

    bool operator<(const Version& otherVersion)
    {
        if (major < otherVersion.major)
            return true;
        if (minor < otherVersion.minor)
            return true;
        if (revision < otherVersion.revision)
            return true;
        if (build < otherVersion.build)
            return true;
        return false;
    }

    bool operator==(const Version& other)
    {
        return major == other.major && minor == other.minor && revision == other.revision && build == other.build;
    }

    int major, minor, revision, build;
};

}