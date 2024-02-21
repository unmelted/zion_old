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

#define SUCCESS			0
#define MAX_ERR_COUNT	100

enum class ErrorCodeIndex{
	ERR_INDEX_COMMON	= 1,
	ERR_INDEX_IC		= 2 * MAX_ERR_COUNT,
	ERR_INDEX_MC		= 3 * MAX_ERR_COUNT,
    ERR_INDEX_SR		= 4 * MAX_ERR_COUNT,
    ERR_INDEX_CR	    = 5 * MAX_ERR_COUNT,
};

////////////////////////////////////////////////////////
// Common
enum class ErrorCommon {
	COMMON_ERR_NONE = SUCCESS,

	COMMON_ERR_PARSE_FAIL = 1001,

	COMMON_ERR_NOT_FOUND_SEC1,
	COMMON_ERR_NOT_FOUND_SEC2,
	COMMON_ERR_NOT_FOUND_SEC3,
	COMMON_ERR_NOT_FOUND_ACTION,
	COMMON_ERR_NOT_FOUND_TOKEN,
	COMMON_ERR_NOT_FOUND_FROM,
	COMMON_ERR_NOT_FOUND_TO,
	COMMON_ERR_NOT_FOUND_DATA,

	COMMON_ERR_UNKNOWN_SEC1,
	COMMON_ERR_UNKNOWN_SEC2,
	COMMON_ERR_UNKNOWN_SEC3,
	COMMON_ERR_UNKNOWN_ACTION,
	COMMON_ERR_UNKNOWN_TOKEN,
	COMMON_ERR_UNKNOWN_FROM,
	COMMON_ERR_UNKNOWN_TO,
	COMMON_ERR_UNKNOWN_DATA,
	COMMON_ERR_UNKNOWN_TYPE,

	COMMON_ERR_TYPE_NAME_STRING,
    COMMON_ERR_TEMPORARY,

	COMMON_ERR_END = ((int)ErrorCodeIndex::ERR_INDEX_COMMON + MAX_ERR_COUNT),
};

////////////////////////////////////////////////////////
enum class ErrorIC {
	IC_ERR_START = (int)ErrorCodeIndex::ERR_INDEX_IC,
	///////////////////////////////////

	IC_ERR_DAEMON_NOT_CONNECTED,
	IC_ERR_APP_NOT_CONNECTED,

	IC_ERR_NOT_FOUND_DAEMONLIST,

	///////////////////////////////////
	IC_ERR_END = (IC_ERR_START + MAX_ERR_COUNT),
};

std::string getErrorCodeToString(const int nErrorCode);
