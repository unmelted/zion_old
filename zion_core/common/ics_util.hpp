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
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include "logger.hpp"

class Configurator {

public:
    typedef struct _TIMER {
        unsigned int expire;
        void* routine;
        void* arg;
        short timer_id;
        clock_t last_time;
        bool last_check = 0;
        
    } TIMER;
   
    static Configurator& get();
    void setDirectory();
    std::string generateToken();
    std::string getCurrentDateTime(std::string s);   
    void updateConfiture(int mode, int category, double value);

    void startTimer(TIMER *times);
    int endTimer(TIMER *times);
    float lapTimer(TIMER *times);

    struct Path {
        static inline const std::filesystem::path LOG {std::filesystem::current_path().append("log") };
        static inline const std::filesystem::path ANLS {std::filesystem::current_path().append("analysis") };
        static inline const std::filesystem::path DUMP {std::filesystem::current_path().append("dump") };
        static inline const std::filesystem::path INIF {std::filesystem::current_path().append("ini.json") };
    };

    struct ConfigureValue{
        enum class cfg_mode {
            SERVER      = 10,
            STABIL      = 11,
        };
        enum class category {
            ROI_WIDTH    = 100,
            ROI_HEIGHT   = 101,
            SWIPE_PT_X   = 102,
            SWIPE_PT_Y   = 103,
            DUMP_TRUE    = 104,
        };        
    };

private :
    int serial = 0;
};