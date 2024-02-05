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
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

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

    static inline const std::filesystem::path LOG {std::filesystem::current_path().append("log") };

private :
    int token_serial_ = 0;
};

static rapidjson::Document parsingJsonFile(std::string filename)
{
    std::ifstream ifs("config.json");
    if (!ifs.is_open()) {
        std::cerr << "Can not open the file : " << filename << std::endl;
        return nullptr;
    }

    // 파일 내용을 Document 객체로 파싱
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        std::cerr << "Parsing Error" << std::endl;
        return nullptr;
    }

    return doc;
}