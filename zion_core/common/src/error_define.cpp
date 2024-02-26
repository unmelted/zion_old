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

#include "error_define.h"

void GetCommonErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
    case static_cast<int>(ERROR_COMM::COMMON_ERR_NONE):				strError = ""; break;
    case static_cast<int>(ERROR_COMM::COMMON_ERR_PARSE_FAIL):		strError = "Message Parsing Fail"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_TYPE):	strError = "Not Found: Type"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_COMMAND):	strError = "Not Found: Command"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_SUBCOMMAND):	strError = "Not Found: SubCommand"; break;
    case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_ACTION):	strError = "Not Found: Action"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_TOKEN):	strError = "Not Found: Token"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_FROM):	strError = "Not Found: From"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_TO):		strError = "Not Found: To"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_DATA):	strError = "Not Found: Data"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_TYPE):		strError = "Unknown: Type"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_COMMAND):		strError = "Unknown: Command"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_SUBCOMMAND):		strError = "Unknown: SubCommand"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_ACTION):	strError = "Unknown: Action"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_TOKEN):	strError = "Unknown: Token"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_FROM):		strError = "Unknown: From"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_TO):		strError = "Unknown: To"; break;
	case static_cast<int>(ERROR_COMM::COMMON_ERR_UNKNOWN_DATA):		strError = "Unknown: Data"; break;
    default:								strError = "Undefined Error"; break;
	}
}

void GetICErrorString(const int nErrorCode, std::string& strError)
{
	switch (nErrorCode) {
    case static_cast<int>(ERROR_IC::IC_ERR_SLAVE_NOT_CONNECTED):strError = "Slave is not Connected"; break;
    case static_cast<int>(ERROR_IC::IC_ERR_APP_NOT_CONNECTED):	strError = "Application is not Connected"; break;
    case static_cast<int>(ERROR_IC::IC_ERR_DISCONNECT_SLAVE) :   strError = "Slave is Disconnected"; break;
    case static_cast<int>(ERROR_IC::IC_ERR_NOT_FOUND_SLAVE_LIST):strError = "Not Found: Slave List"; break;
	default:								strError = "Undefined Error"; break;
	}
}

std::string getErrorCodeToString(const int nErrorCode)
{
	std::string strError = "";
	if (nErrorCode != static_cast<int>(ERROR_COMM::COMMON_ERR_NONE))
    {
		if (nErrorCode > static_cast<int>(ERROR_COMM::COMMON_ERR_NONE) &&
            nErrorCode < static_cast<int>(ERROR_COMM::COMMON_ERR_END))
        {
            GetCommonErrorString(nErrorCode, strError);
        }
		else if (nErrorCode > static_cast<int>(ERROR_IC::IC_ERR_START) &&
                 nErrorCode < static_cast<int>(ERROR_IC::IC_ERR_END))
         {
            GetICErrorString(nErrorCode, strError);
         }
		else
            strError = "Undefined Error";
	}

	return strError;
}
