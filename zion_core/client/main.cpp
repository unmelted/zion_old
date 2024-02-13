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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/12
 *
 */

#include <sstream>
#include <csignal>
#include "ic_client.h"

std::unique_ptr<ICClient> ic_cli;

void signalHandler(int signum) {
    LOG_TRACE("Interrupt signal {} received.", signum);

    signal(SIGINT, signalHandler);
//        ic_cli.reset();

    exit(signum);
}

int main()
{
    // teporary data for initialize
    std::ostringstream ss;
    ss << R"(
    {
        "servers": [
        {
            "name": "Server1",
            "ip": "192.168.1.100",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_CONTROL)] << R"(
        },
        {
            "name": "Server2",
            "ip": "192.168.1.101",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_ALIVE)] << R"(
        }
        ]
    }
    )";

    std::string configContent = ss.str();
    LOG_INFO("ICManager Start!");

    signal(SIGINT, signalHandler);
    ic_cli = std::make_unique<ICClient>(configContent);
    Logger logger;

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    logger.~Logger();
    return 1;
}
