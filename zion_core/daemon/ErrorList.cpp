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

#include "ErrorList.h"

void GetCommonErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case COMMON_ERR_NONE:					strError = ""; break;
	case COMMON_ERR_PARSE_FAIL:				strError = "Message Parsing Fail"; break;
	case COMMON_ERR_NOT_FOUND_SEC1:			strError = "Not Found: Section 1"; break;
	case COMMON_ERR_NOT_FOUND_SEC2:			strError = "Not Found: Section 2"; break;
	case COMMON_ERR_NOT_FOUND_SEC3:			strError = "Not Found: Section 3"; break;
	case COMMON_ERR_NOT_FOUND_SENDSTATE:	strError = "Not Found: Sendstate"; break;
	case COMMON_ERR_NOT_FOUND_TOKEN:		strError = "Not Found: Token"; break;
	case COMMON_ERR_NOT_FOUND_FROM:			strError = "Not Found: From"; break;
	case COMMON_ERR_NOT_FOUND_TO:			strError = "Not Found: To"; break;
	case COMMON_ERR_NOT_FOUND_ACTION:		strError = "Not Found: Action"; break;
	case COMMON_ERR_UNKNOWN_SEC1:			strError = "Unknown: Section 1"; break;
	case COMMON_ERR_UNKNOWN_SEC2:			strError = "Unknown: Section 2"; break;
	case COMMON_ERR_UNKNOWN_SEC3:			strError = "Unknown: Section 3"; break;
	case COMMON_ERR_UNKNOWN_SENDSTATE:		strError = "Unknown: Sendstate"; break;
	case COMMON_ERR_UNKNOWN_TOKEN:			strError = "Unknown: Token"; break;
	case COMMON_ERR_UNKNOWN_FROM:			strError = "Unknown: From"; break;
	case COMMON_ERR_UNKNOWN_TO:				strError = "Unknown: To"; break;
	case COMMON_ERR_UNKNOWN_ACTION:			strError = "Unknown: Action"; break;
	case COMMON_ERR_UNKNOWN_TYPE:			strError = "Unknown: Type"; break;
	case COMMON_ERR_TYPE_NAME_STRING:		strError = "Value Name is not String"; break;
	default:								strError = "Undefined Error"; break;
	}
}

void GetMtdErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
	case MTD_ERR_DAEMON_NOT_CONNECTED:		strError = "Daemon is not Connected"; break;
	case MTD_ERR_APP_NOT_CONNECTED:			strError = "Application is not Connected"; break;
	case MTD_ERR_NOT_FOUND_DAEMONLIST:		strError = "Not Found: DaemonList"; break;
	default:								strError = "Undefined Error"; break;
	}
}

std::string GetErrorCodeToString(const int nErrorCode)
{
	std::string strError = "";
	if (nErrorCode != COMMON_ERR_NONE) {
		if (nErrorCode > COMMON_ERR_NONE && nErrorCode < COMMON_ERR_END) { GetCommonErrorString(nErrorCode, strError); }
		else if (nErrorCode > MTD_ERR_START && nErrorCode < MTD_ERR_END) { GetMtdErrorString(nErrorCode, strError); }
		else strError = "Undefined Error";
	}

	return strError;
}
