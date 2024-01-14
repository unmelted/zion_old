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
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "DMServer.h"
#include "CMdLogger.hpp"
#include "json.hpp"
#include "message_queue.h"
#include "ics_util.hpp"

#include "error_manager.h"


#define MESSAGELTRANSFER_DAEMON_PORT        0x4D01
#define ENTERPRISE_MONITOR_DAEMON_PORT      0x4D02
#define SWITCH_CONTROL_DAEMON_PORT          0x4D03
#define CAMERA_CONTROL_DAEMON_PORT          0x4D04
#define GIMBAL_CONTROL_DAEMON_PORT          0x4D05
#define PROCESS_CONTROL_DAEMON_PORT         0x4D06
#define PRE_STOARGE_DAEMON_PORT             0x4D07
#define VIDEO_PROCESS_FOR_PRESD_DAEMON_PORT  0x4D08
#define VIDEO_PROCESS_FOR_PCD_DAEMON_PORT    0x4D09
#define VIDEO_PROCESS_FOR_POSTSD_DAEMON_PORT 0x4D10
#define POST_STOARGE_DAEMON_PORT            0x4D11
#define SDI_PROCESS_DAEMON_PORT             0x4D12
#define SPD_FRAME_DAEMON_PORT               0x4D13
#define VIDEO_PROCESS_FOR_MMD_DAEMON_PORT   0x4D14
#define CM_CONTROL_DAEMON_PORT              0x4D15

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
enum
{
    COMMAND_NONE = 0,
    COMMAND_VERSION,
    COMMAND_START,
    COMMAND_STOP,
    COMMAND_RESTART,
    COMMAND_STATUS,
};

struct MTdProtocol
{
    std::string Section1;
    std::string Section2;
    std::string Section3;
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
enum
{
    PACKETTYPE_JSON,
    PACKETTYPE_BINALY,
    PACKETTYPE_ABAILABLE_INQUIRY_VP = 20,
    PACKETTYPE_VP_SET_BUFFERCOUNT,
    PACKETTYPE_VP_ALIVE,
    PACKETTYPE_CALIBRATION_VALUE = 100,

    PACKETTYPE_SIZE,
};

//Client Port
enum
{
    MESSAGELTRANSFER_DAEMON,
    ENTERPRISE_MONITOR_DAEMON,
    SWITCH_CONTROL_DAEMON,
    CAMERA_CONTROL_DAEMON,
    GIMBAL_CONTROL_DAEMON,
    SDI_PROCESS_DAEMON,
    PROCESS_CONTROL_DAEMON,
    PRE_STOARGE_DAEMON,
    POST_STOARGE_DAEMON,
    VIDEO_PROCESS_FORPRESD_DAEMON,
    VIDEO_PROCESS_FORPCD_DAEMON,
    VIDEO_PROCESS_FORPOSTSD_DAEMON,
    CORE_MODULE_DAEMON,
    SOCKNUM_CLIENT_SIZE,
};

// Server Port
enum
{
    SOCKNUM_CONTROLLER_APP,
    SOCKNUM_PRODUCING_APP,
    SOCKNUM_VAR_APP,
    SOCKNUM_EMS_APP,
    SOCKNUM_DAEMONVIEWER_APP,
    SOCKNUM_BUFFERVIWER_APP,
    SOCKNUM_LOGVIEWER_APP,
    SOCKNUM_SERVER_SIZE,
};

static char arrDaemonObject[20][100] = {"MTd", "EMd", "SCd", "CCd", "GCd", "SPd", "PCd", "PreSd", "PostSd", "VPd", "VPd", "VPd", "CMd"};


static char arrModelObject[11][100] = {"4DDM",                     // Controller Daemon
                                       "4DPD",                     // Producing Daemon
                                       "VARApp",                   // VAR Daemon
                                       "EMSApp",                   // EMS Daemon
                                       "DaemonViewer",             // DaemonViwer
                                       "4DBUFFER",             // DaemonBufferViewer
                                       "4DLOG",                    // LogViewer
};

#define MTDPROTOCOL_SECTION1    "Section1"
#define MTDPROTOCOL_SECTION2    "Section2"
#define MTDPROTOCOL_SECTION3    "Section3"
#define MTDPROTOCOL_SENDSTATE   "SendState"
#define MTDPROTOCOL_FROM        "From"
#define MTDPROTOCOL_TO          "To"
#define MTDPROTOCOL_ACTION      "Action"
#define MTDPROTOCOL_TOKEN       "Token"
#define MTDPROTOCOL_RESULTCODE  "ResultCode"
#define MTDPROTOCOL_ERRORMSG    "ErrorMsg"
#define MTDPROTOCOL_TARGETIP    "TargetIP"

#define MTDPROTOCOL_PRESET      "Preset"

#define MTDPROTOCOL_SENDSTATE_REQUEST       "request"
#define MTDPROTOCOL_SENDSTATE_RESPONSE      "response"

#define MTDHEADERSIZE           5
//#define MTDPROTOCOL                           "YJS"

#pragma pack(push, 1)
struct MTdProtocolHeader
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