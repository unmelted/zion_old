/*
 * LIVSMED CONFIDENTIAL
 *
 * Copyright (c) 2024 LIVSMED, INC.
 * All Rights Reserved.
 *≤
 * NOTICE: All information contained herein is, and remains the property
 * of LIVSMED and its suppliers, if any. The intellectual and technical concepts
 * contained herein are proprietary to LIVSMED and its suppliers and may be
 * covered by S.Korea and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from LIVSMED.
 *
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/22.
 *
 */

#include <utility>
#include "db_manager.h"

DBManager::DBManager()
{
    const std::string& db_path1 = ic::DB_NAME[static_cast<int>(ic::DB_TYPE::DB_TYPE_LIVSMED)];
    std::cout <<"try to open DB: " << db_path1 << std::endl;

    if (!openDB(db_path1))
    {
        std::cout <<"Failed to open DB: " << db_path1 << std::endl;
        return;
    }

    const std::string& db_path2 = ic::DB_NAME[static_cast<int>(ic::DB_TYPE::DB_TYPE_LOG_MONITOR)];
    std::cout <<"try to open DB: " << db_path2 << std::endl;

    if (!openDB(db_path2))
    {
        std::cout <<"Failed to open DB: " << db_path2 << std::endl;
        return;
    }

    // until this process, can't use logger because logger is not initialized yet.

    isQueryThread_ = true;
    queryThread_ = std::make_unique<std::thread>(&DBManager::queryThread, this);
}

DBManager::DBManager(int db_name_idx)
{
    const std::string& db_path = ic::DB_NAME[db_name_idx];
    std::cout <<"try to open DB: " << db_path << std::endl;

    if (!openDB(db_path))
    {
        std::cout <<"Failed to open DB: " << db_path << std::endl;
        return;
    }
    if (db_name_idx != static_cast<int>(ic::DB_TYPE::DB_TYPE_LOG))
    {
        createTable(db_name_idx);
    }

    // until this process, can't use logger because logger is not initialized yet.

    isQueryThread_ = true;
    queryThread_ = std::make_unique<std::thread>(&DBManager::queryThread, this);
}

DBManager::~DBManager()
{
    closeDB();
    isQueryThread_ = false;
    cv_query_.notify_all();
    queryThread_->join();

    LOG_DEBUG("DBManager Destroyed Done.");
}

sqlite3* DBManager::getDB()
{
    return db_;
}


bool DBManager::openDB(std::string db_path)
{
    int nRet = sqlite3_open(db_path.c_str(), &db_);
    if (nRet != SQLITE_OK)
    {
        std::cout<<"Can't open database: {}" << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    isOpen_ = true;
    std::cout <<"DBManager Opened : " <<  db_path << std::endl;
    return true;
}

int DBManager::createTable(int db_name_idx)
{
    if(db_name_idx == static_cast<int>(ic::DB_TYPE::DB_TYPE_LIVSMED))
    {
        rapidjson::Document doc = parseJsonFile(ic::DB_CONFIG);

        if (doc.HasMember("create_table1"))
        {
            const rapidjson::Value& tables = doc["create_table1"];
            for (auto it = tables.MemberBegin(); it != tables.MemberEnd(); ++it)
            {
                try
                {
                    std::string query = it->value.GetString();
                    char* errMsg = nullptr;
                    int result = sqlite3_exec(db_, query.c_str(), NULL, NULL, &errMsg);
                    if (result != SQLITE_OK)
                    {
                        std::string errorStr = errMsg;
                        sqlite3_free(errMsg);
                        throw std::runtime_error(errorStr);
                    }
                }
                catch (const std::runtime_error& e)
                {
                    LOG_ERROR("Failed to execute query: {}", e.what());
                    return FAIL;
                }
            }
        }
    }
    else if (db_name_idx == static_cast<int>(ic::DB_TYPE::DB_TYPE_LOG_MONITOR))
    {
        try
        {
            char* errMsg = nullptr;
            table_name_of_tcplog_ = "Log_monitor" + Configurator::get().getCurrentDateTime("datetime");
            std::string createQuery = "CREATE TABLE IF NOT EXISTS " + table_name_of_tcplog_ + "(date DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now')), from_where TEXT, data TEXT)";
            int result = sqlite3_exec(db_, createQuery.c_str(), NULL, NULL, &errMsg);
            std::cout << "Create table query : " << createQuery << " Result : " << result << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "create table query error : " << e.what() << std::endl;
            return FAIL;
        }
    }
    LOG_DEBUG("Create Table Done.");
    return SUCCESS;
}

std::string& DBManager::getTableNameOfTcpLog()
{
    return table_name_of_tcplog_;
}

bool DBManager::closeDB()
{
    int nRet = sqlite3_close(db_);
    if (nRet != SQLITE_OK)
    {
        LOG_ERROR("Cannotclose database: {}", sqlite3_errmsg(db_));
        return false;
    }
return true;
}

int DBManager::enqueueQuery(const std::string& query)
{
    if(!isQueryThread_)
    {
        LOG_ERROR("DBManager is already stopped.");
        return FAIL;
    }

    std::lock_guard<std::mutex> lock(queryMutex_);
    auto p_query = make_shared<std::string>(query);
    queQuery_.Enqueue(p_query);
    cv_query_.notify_one();

    return SUCCESS;
}

void DBManager::queryThread()
{
    while (isQueryThread_)
    {
        std::unique_lock<std::mutex> lock(queryMutex_);
        cv_query_.wait(lock);

        if (queQuery_.IsQueue())
        {
            auto query = queQuery_.Dequeue();
            if (query == nullptr)
                continue;

            runQuery(query);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(ic::QUEUE_EMPTY_CHECK));
    }
}

int DBManager::runQuery(const std::shared_ptr<std::string>& query)
{
    int result = -1;
    std::string str_query;
    try
    {
        char* errMsg = nullptr;
        if (query)
        {
            str_query = *query;
            result = sqlite3_exec(db_, str_query.c_str(), 0, 0, &errMsg);
            if (result != SQLITE_OK)
            {
                std::cout << "Failed to execute query: " << str_query << std::endl;
                std::string errorStr = errMsg;
                sqlite3_free(errMsg); // 메모리 누수 방지를 위해 errMsg 해제
                throw std::runtime_error(errorStr);
            }
        }
        else
        {
            LOG_ERROR("delivered query is null.");
        }

    }
    catch (const std::runtime_error& e)
    {
        LOG_ERROR("Failed to execute query: {}", e.what());
        return FAIL;
    }

    return SUCCESS;
}
