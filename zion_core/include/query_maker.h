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
    std::string makeQuery(QueryType type, const std::string& tableName, Args... args) {
        std::string query;

        switch (type) {
            case INSERT:
                query = makeInsertQuery(tableName, std::forward<Args>(args)...);
                break;
            case UPDATE:
                query = makeUpdateQuery(tableName, std::forward<Args>(args)...);
                break;
            case DELETE:
                query = makeDeleteQuery(tableName);
                break;
        }

        return query;
    }

private:

    template<typename... Args>
    std::string makeInsertQuery(const std::string& tableName, Args... args) {
        std::stringstream columns, values;

        processArgs(columns, values, std::forward<Args>(args)...);

        return "INSERT INTO " + tableName + " (" + columns.str() + ") VALUES (" + values.str() + ");";
    }

    template<typename... Args>
    std::string makeUpdateQuery(const std::string& tableName, Args... args) {
        std::stringstream setClause;

        processUpdateArgs(setClause, std::forward<Args>(args)...);

        return "UPDATE " + tableName + " SET " + setClause.str() + " WHERE [CONDITION];";
    }

    std::string makeDeleteQuery(const std::string& tableName) {
        return "DELETE FROM " + tableName + " WHERE [CONDITION];";
    }


    template<typename T, typename... Args>
    void processArgs(std::stringstream& columns, std::stringstream& values, T first, T second, Args... args) {
        columns << first;
        values << "'" << second << "'";

        if constexpr (sizeof...(args) > 0) {
            columns << ", ";
            values << ", ";
            processArgs(columns, values, args...);
        }
    }

    template<typename T, typename... Args>
    void processUpdateArgs(std::stringstream& setClause, T first, T second, Args... args) {
        setClause << first << " = '" << second << "'";

        if constexpr (sizeof...(args) > 0) {
            setClause << ", ";
            processUpdateArgs(setClause, args...);
        }
    }
};
