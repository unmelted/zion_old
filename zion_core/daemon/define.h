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
 * Created by Eunkyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */

#pragma once
#include "DaemonDefine.h"
#include "Util/logger.h"
#include "ErrorList.h"

using namespace std;
using namespace toolkit;

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

#define CURRENTVERSION "4.3.0.A"