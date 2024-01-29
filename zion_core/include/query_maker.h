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

class QueryMaker {
public:
    enum QueryType { INSERT, UPDATE, DELETE };

    template<typename... Args>
    static std::string makeQuery(QueryType type, const std::string& tableName, Args... args) {
        std::stringstream query;

        if (type == INSERT) {
            query << makeInsertQuery(tableName, std::forward<Args>(args)...);
        }

        return query.str();
    }

    template<typename... Args>
    static std::string makeQuery(QueryType type, const std::string& tableName, const std::string& whereClause, Args... args) {
        std::stringstream query;

        if (type == UPDATE) {
            query << makeUpdateQuery(tableName, whereClause, std::forward<Args>(args)...);
        } else if (type == DELETE) {
            query << makeDeleteQuery(tableName, whereClause);
        }

        return query.str();
    }

private:

    template<typename... Args>
    static std::string makeInsertQuery(const std::string& tableName, Args... args) {
        std::stringstream columns, values;

        processArgs(columns, values, std::forward<Args>(args)...);

        return "INSERT INTO " + tableName + " (" + columns.str() + ") VALUES (" + values.str() + ");";
    }

    // UPDATE 쿼리 생성
    template<typename... Args>
    static std::string makeUpdateQuery(const std::string& tableName, const std::string& whereClause, Args... args) {
        std::stringstream setClause;

        processUpdateArgs(setClause, std::forward<Args>(args)...);

        return "UPDATE " + tableName + " SET " + setClause.str() + " WHERE " + whereClause + ";";
    }

    // DELETE 쿼리 생성
    static std::string makeDeleteQuery(const std::string& tableName, const std::string& whereClause) {
        return "DELETE FROM " + tableName + " WHERE " + whereClause + ";";
    }


    template<typename T, typename... Args>
    static void processArgs(std::stringstream& columns, std::stringstream& values, T first, T second, Args... args)
    {
        columns << first;
        values << "'" << second << "'";

        if constexpr (sizeof...(args) > 0)
        {
            columns << ", ";
            values << ", ";
            processArgs(columns, values, args...);
        }
    }

    template<typename T, typename... Args>
    static void processUpdateArgs(std::stringstream& setClause, T first, T second, Args... args)
    {
        setClause << first << " = '" << second << "'";

        if constexpr (sizeof...(args) > 0)
        {
            setClause << ", ";
            processUpdateArgs(setClause, args...);
        }
    }
};
