﻿/*
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

#include <csignal>
#include "server_manager.h"

std::unique_ptr<ServerManager> svrManager;

void signalHandler(int signum)
{
    LOG_TRACE("Interrupt signal {} received.", signum);

    signal(SIGINT, signalHandler);
    svrManager.reset();

    exit(signum);
}

int main()
{
    // array<bool, 4> represent the sink of log
    // refer the enum Logger::sink_enum
    Logger logger({true, false, true, false});
    signal(SIGINT, signalHandler);
    svrManager = std::make_unique<ServerManager>();
    svrManager->initialize();

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}