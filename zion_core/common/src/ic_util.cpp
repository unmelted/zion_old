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

#include "ic_util.h"

Configurator&  Configurator::get()
{
    static Configurator _instance;
    return _instance;
}

void Configurator::setDirectory()
{
    std::cout << "setDirectory create directory : " << LOG << std::endl;
    std::filesystem::create_directories(LOG);
    std::filesystem::create_directories(DB);
    std::filesystem::path db_src1 = "../../db/livsmed.db";
    std::filesystem::path db_src2 = "../../db/livsmed_log.db";
    std::filesystem::path db_src3 = "../../db/db_config.json";
    std::filesystem::path db_dst = DB;

    try
    {
//        std::filesystem::copy(db_src1, db_dst, std::filesystem::copy_options::update_existing);
//        std::filesystem::copy(db_src2, db_dst, std::filesystem::copy_options::update_existing);
        std::filesystem::copy(db_src3, db_dst, std::filesystem::copy_options::update_existing);
    }
    catch (std::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << '\n';
    }

}

std::string Configurator::getCurrentDateTime(std::string s)
{
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    if(s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if(s == "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    else if (s == "datetime")
        strftime(buf, sizeof(buf), "%Y%m%d_%H%M", &tstruct);
    return std::string(buf);
}

std::string Configurator::generateToken() {
    char tk[100];
    std::string base = getCurrentDateTime("date");
    if(token_serial_ >= 9999)
        token_serial_ = 0;
    token_serial_ ++;
    snprintf(tk, sizeof(tk), "%s_%04d", base.c_str(), token_serial_);
    std::string str_tk(tk);   
    return str_tk;    
}

void Configurator::startTimer(TIMER *times)
{
    times->last_check = true;
    times->last_time = clock();
}

int Configurator::endTimer(TIMER *times)
{
    if(times->last_check == 0) return -1;
    int ms = clock() - times->last_time;
    times->last_check = 0;
    return ms;
}

float Configurator::lapTimer(TIMER *times)
{
    if (times->last_check == 0)
        return -1;
    clock_t ctime = clock();
    float ms = ((float)ctime - (float)times->last_time) / 1000.0;
    times->last_time = ctime;
    times->last_check = true;
    return ms;
}
