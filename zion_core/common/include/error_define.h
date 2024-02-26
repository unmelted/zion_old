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

#include <string>

#define SUCCESS			100
#define FAIL			-100
#define MAX_ERR_COUNT	100

enum class ErrorCodeIndex{
	ERR_INDEX_COMMON	= 1000,
	ERR_INDEX_IC		= 1000 + 1 * MAX_ERR_COUNT,
	ERR_INDEX_SLAVE		= 1000 + 3 * MAX_ERR_COUNT,
};

////////////////////////////////////////////////////////
// Common
enum class ERROR_COMM
{
    COMMON_ERR_NONE = SUCCESS,

    COMMON_ERR_PARSE_FAIL = 1001,

    COMMON_ERR_NOT_FOUND_TYPE,
    COMMON_ERR_NOT_FOUND_COMMAND,
    COMMON_ERR_NOT_FOUND_SUBCOMMAND,
    COMMON_ERR_NOT_FOUND_ACTION,
    COMMON_ERR_NOT_FOUND_TOKEN,
    COMMON_ERR_NOT_FOUND_FROM,
    COMMON_ERR_NOT_FOUND_TO,
    COMMON_ERR_NOT_FOUND_DATA,

    COMMON_ERR_UNKNOWN_TYPE,
    COMMON_ERR_UNKNOWN_COMMAND,
    COMMON_ERR_UNKNOWN_SUBCOMMAND,
    COMMON_ERR_UNKNOWN_ACTION,
    COMMON_ERR_UNKNOWN_TOKEN,
    COMMON_ERR_UNKNOWN_FROM,
    COMMON_ERR_UNKNOWN_TO,
    COMMON_ERR_UNKNOWN_DATA,

    COMMON_ERR_TEMPORARY,

    COMMON_ERR_END = ((int)ErrorCodeIndex::ERR_INDEX_COMMON + MAX_ERR_COUNT),
};

////////////////////////////////////////////////////////
enum class ERROR_IC
{
    IC_ERR_START = (int)ErrorCodeIndex::ERR_INDEX_IC,
    ///////////////////////////////////

    IC_ERR_SLAVE_NOT_CONNECTED,
    IC_ERR_APP_NOT_CONNECTED,
    IC_ERR_DISCONNECT_SLAVE,
    IC_ERR_NOT_FOUND_SLAVE_LIST,

    ///////////////////////////////////
    IC_ERR_END = (IC_ERR_START + MAX_ERR_COUNT),
};

enum class ERROR_SLAVE
{
    SLAVE_ERR_START = (int)ErrorCodeIndex::ERR_INDEX_SLAVE,
    ///////////////////////////////////


    ///////////////////////////////////
    SLAVE_ERR_END = (SLAVE_ERR_START + MAX_ERR_COUNT),
};

std::string getErrorCodeToString(const int nErrorCode);
