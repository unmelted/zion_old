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


#ifndef __ERROR_MANAGER_H__
#define __ERROR_MANAGER_H__

#include <string>

#define SUCCESS			1000
#define MAX_ERR_COUNT	10000

enum class ErrorCodeIndex{
	ERR_INDEX_COMMON	= 0,
	ERR_INDEX_MTD		= 1 * MAX_ERR_COUNT,
	ERR_INDEX_PCD		= 2 * MAX_ERR_COUNT,
	ERR_INDEX_EMD		= 3 * MAX_ERR_COUNT,
	ERR_INDEX_SCD		= 4 * MAX_ERR_COUNT,
	ERR_INDEX_CCD		= 5 * MAX_ERR_COUNT,
	ERR_INDEX_GCD		= 6 * MAX_ERR_COUNT,
	ERR_INDEX_PRESD		= 7 * MAX_ERR_COUNT,
	ERR_INDEX_POSTSD	= 8 * MAX_ERR_COUNT,
};

////////////////////////////////////////////////////////
// Common
enum class ErrorCommon {
	COMMON_ERR_NONE = SUCCESS,

	COMMON_ERR_PARSE_FAIL = 1001,

	COMMON_ERR_NOT_FOUND_SEC1,
	COMMON_ERR_NOT_FOUND_SEC2,
	COMMON_ERR_NOT_FOUND_SEC3,
	COMMON_ERR_NOT_FOUND_SENDSTATE,
	COMMON_ERR_NOT_FOUND_TOKEN,
	COMMON_ERR_NOT_FOUND_FROM,
	COMMON_ERR_NOT_FOUND_TO,
	COMMON_ERR_NOT_FOUND_ACTION,

	COMMON_ERR_UNKNOWN_SEC1,
	COMMON_ERR_UNKNOWN_SEC2,
	COMMON_ERR_UNKNOWN_SEC3,
	COMMON_ERR_UNKNOWN_SENDSTATE,
	COMMON_ERR_UNKNOWN_TOKEN,
	COMMON_ERR_UNKNOWN_FROM,
	COMMON_ERR_UNKNOWN_TO,
	COMMON_ERR_UNKNOWN_ACTION,
	COMMON_ERR_UNKNOWN_TYPE,

	COMMON_ERR_TYPE_NAME_STRING,
    COMMON_ERR_TEMPORARY,

	COMMON_ERR_END = ((int)ErrorCodeIndex::ERR_INDEX_COMMON + MAX_ERR_COUNT),
};

////////////////////////////////////////////////////////
// MTd
enum class ErrorMTd {
	MTD_ERR_START = (int)ErrorCodeIndex::ERR_INDEX_MTD,
	///////////////////////////////////

	MTD_ERR_DAEMON_NOT_CONNECTED,
	MTD_ERR_APP_NOT_CONNECTED,

	MTD_ERR_NOT_FOUND_DAEMONLIST,

	///////////////////////////////////
	MTD_ERR_END = (MTD_ERR_START + MAX_ERR_COUNT),
};

std::string GetErrorCodeToString(const int nErrorCode);

#endif // __ERROR_MANAGER_H__
