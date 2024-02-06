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

#include "ic_define.h"
#include <iostream>
#include <string>
#include <sstream>
#include <utility>

static std::array<std::string, (int)ic::DB_LOG_COLUMN::DB_LOG_COLUMN_SIZE> DB_LOG_COLUMN_NAME =
{
        "date", "pid", "tid" ,"level", "file", "msg",
};


class QueryMaker
{
public:

    enum QueryType { INSERT, UPDATE, DELETE };

    template<typename... Args>
    static std::string makeQuery(QueryType type, const std::string& tableName, Args... args)
    {
        std::string query;

//        if (type == INSERT)
//        {
//            query = makeInsertQuery(tableName, std::forward<Args>(args)...);
//        }

        return query;
    }

    template<typename... Args>
    static std::string makeQuery(QueryType type, const std::string& tableName, const std::string& whereClause, Args... args)
    {
        std::string query;

        if (type == UPDATE)
        {
            query = makeUpdateQuery(tableName, whereClause, std::forward<Args>(args)...);
        }

        return query;
    }

    template<typename T>
    static std::string toString(const T& value)
    {
        std::ostringstream oss;
        oss << *value;
        return oss.str();
    }

    static std::string makeInsertQuery(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& columns)
    {
        std::string query_col = "INSERT INTO " + tableName + " (";
        std::string query_val = " VALUES (";

        for (const auto& column : columns)
        {
            query_col += column.first + ", ";
            query_val += "'" + column.second + "', ";
        }

        std::string query = query_col.substr(0, query_col.size() - 2) + ")" + query_val.substr(0, query_val.size() - 2) + ");";

//        LOG_INFO("make insert query : {}", query);

        return query;
    }

private:
    template<typename... Args>
    static void processArgs(std::stringstream& columns, std::stringstream& values, Args... args)
    {
        std::string argsArray[] = { toString(args)... }; // 가변 인자를 배열로 변환
        for (size_t i = 0; i < sizeof...(args); i += 2)
        {
            columns << argsArray[i];
            values << "'" << argsArray[i + 1] << "'";
            if (i + 2 < sizeof...(args))
            {
                columns << ", ";
                values << ", ";
            }
        }
    }


    template<typename... Args>
    static void processUpdateArgs(std::string& setClause, Args... args)
    {
        std::string argsArray[] = { std::to_string(args)...};
        size_t argsCount = sizeof...(args);

        for (size_t i = 0; i < argsCount; i += 2)
        {
            setClause += argsArray[i];
            setClause += " = '";
            setClause += argsArray[i + 1];
            setClause += "'";

            if (i + 2 < argsCount)
            {
                setClause += ", ";
            }
        }
    }

    template<typename... Args>
    static std::string makeUpdateQuery(const std::string& tableName, const std::string& whereClause, Args... args)
    {
        std::string setClause;
        processUpdateArgs(setClause, std::forward<Args>(args)...);

        return "UPDATE " + tableName + " SET " + setClause + " WHERE " + whereClause + ";";
    }

};
