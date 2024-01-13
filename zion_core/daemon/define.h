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
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>
#include <string>
#include <set>
#include <cstdint>
#include <map>

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "DMServer.h"
#include "DaemonDefine.h"
#include "CMdLogger.hpp"
#include "json.hpp"
#include "MessageQueue.h"
#include "ics_util.hpp"

#include "DaemonDefine.h"
#include "Util/logger.h"
#include "ErrorList.h"

#define CURRENTVERSION "0.0.1.T"

using namespace std;

namespace ic
{
enum {
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

#define TASKPOOL_SIZE 3

typedef struct PACKET_TYPE
{
    static const int TEXT = 0;
    static const int BINARY = 1;
} PACKET_TYPE;


typedef struct _MSG_T
{
    int32_t type;
    std::string txt;
    char *bin;
    int32_t bin_size;
    _MSG_T(void)
            : type(PACKET_TYPE::TEXT), txt{}, bin(nullptr), bin_size(0)
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

}