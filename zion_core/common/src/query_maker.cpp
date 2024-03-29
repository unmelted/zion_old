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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/23.
 *
 */

#include "query_maker.h"

std::pair<string, string> QueryMaker::makeInsertProtocolQuery(const Document& doc, const vector<string>& columns)
{
    string query_col = "(";
    string query_val = "VALUES (";
    int index = 0;

    if (doc.IsObject())
    {
        for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
        {
            if(index < columns.size())
            {
                query_col += columns[index] + ", ";
                if (columns[index] == "data")
                {
                    if (itr->value.IsString())
                    {
                        std::string value = itr->value.GetString();
                        query_val += "'" + value + "', ";
                    }
                    else
                    {
                        StringBuffer buffer;
                        Writer<StringBuffer> writer(buffer);
                        itr->value.Accept(writer);
                        query_val += std::string("'") + buffer.GetString() + "', ";
                    }
                }
                else
                {
                    std::string value = itr->value.GetString();
                    query_val += "'" + value + "', ";
                }

                index++;
            }
        }
    }

    query_col = query_col.substr(0, query_col.size() - 2) + ")";
    query_val = query_val.substr(0, query_val.size() - 2) + ")";
    return {query_col, query_val};
}


std::string QueryMaker::makeLogMonitorInsertQuery(std::string& table, const Document& doc)
{
    std::vector<string> columns(DB_LOG_MONITOR_COLUMN_NAME.begin(), DB_LOG_MONITOR_COLUMN_NAME.end());
    auto [query_col, query_val] = makeInsertProtocolQuery(doc, columns);
    std::string query = "INSERT INTO " + table +  " " + query_col + " " + query_val + ";";
    return query;
}

std::string QueryMaker::makeEventInsertQuery(std::string& table, const Document& doc)
{
    std::vector<string> columns(DB_EVENT_HISTORY_COLUMN_NAME.begin(), DB_EVENT_HISTORY_COLUMN_NAME.end());
    auto [query_col, query_val] = makeInsertProtocolQuery(doc, columns);
    std::string query = "INSERT INTO " + table +  " " + query_col + " " + query_val + ";";
    return query;
}

std::string QueryMaker::makeLogInsertQuery(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& columns)
{
    std::string query_col = "INSERT INTO " + tableName + " (";
    std::string query_val = " VALUES (";

    for (const auto& column : columns)
    {
        query_col += column.first + ", ";
        query_val += "'" + column.second + "', ";
    }

    std::string query = query_col.substr(0, query_col.size() - 2) + ")" + query_val.substr(0, query_val.size() - 2) + ");";

    return query;
}

std::string QueryMaker::makeErrorMsgQuery(const Document& doc)
{
    std::string query_col = "INSERT INTO error (";
    std::string query_val = " VALUES (";
    int index = 0;

    if(doc.IsObject())
    {
        for (auto& col : DB_ERROR_COLUMN_NAME)
        {
            const Value& value = doc[col.c_str()];
            query_col += col + ", ";

            if (value.IsInt())
            {
                int intVal = value.GetInt();
                query_val += "'" + std::to_string(intVal) + "', ";
            }
            else if (value.IsString())
            {
                std::string strVal = value.GetString();
                query_val += "'" + strVal + "', ";
            }
            else
            {
                query_val += "'', ";
            }
            index++;
        }
    }

    std::string query = query_col.substr(0, query_col.size() - 2) + ")" + query_val.substr(0, query_val.size() - 2) + ");";
    return query;
}