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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/22.
 *
 */

#pragma once
#include <filesystem>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "error_define.h"

#define PROTOCOL_TYPE    "Type"
#define PROTOCOL_COMMAND    "Command"
#define PROTOCOL_SUBCOMMAND    "SubCommand"
#define PROTOCOL_ACTION      "Action"
#define PROTOCOL_TOKEN       "Token"
#define PROTOCOL_FROM        "From"
#define PROTOCOL_TO          "To"
#define PROTOCOL_DATA        "Data"

#define PROTOCOL_RESULTCODE  "ResultCode"
#define PROTOCOL_ERRORMSG    "ErrorMsg"

using namespace rapidjson;

struct Protocol
{
    std::string Type;
    std::string Command;
    std::string SubCommand;
    std::string Action;
    std::string Token;
    std::string From;
    std::string To;
    std::string Data;
};

static std::string convertDocumentForSend(Document& document)
{
    StringBuffer strbuf;
    strbuf.Clear();
    PrettyWriter<StringBuffer> writer(strbuf);
    document.Accept(writer);
    std::string ownShipRadarString = strbuf.GetString();

    return ownShipRadarString;
}

static Document parseJsonFile(const std::string& filename)
{
    std::cout<<" parseJsonFile  : " << filename << std::endl;
    std::cout << "file exists :: " << std::filesystem::exists(filename) << std::endl;

    if (!std::filesystem::exists(filename))
        return nullptr;

    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "Can not open the file : " << filename << std::endl;
        return nullptr;
    }

    IStreamWrapper isw(ifs);
    Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        std::cerr << "Parsing Error" << std::endl;
        return nullptr;
    }

    return doc;
}

static int convertDocumentToMSG(Document& document, Protocol& msg)
{
    if (document.HasMember(PROTOCOL_TYPE))
        msg.Type = document[PROTOCOL_TYPE].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_TYPE);

    if (document.HasMember(PROTOCOL_COMMAND))
        msg.Command = document[PROTOCOL_COMMAND].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_COMMAND);

    if (document.HasMember(PROTOCOL_SUBCOMMAND))
        msg.SubCommand = document[PROTOCOL_SUBCOMMAND].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_SUBCOMMAND);

    if (document.HasMember(PROTOCOL_ACTION))
        msg.Action = document[PROTOCOL_ACTION].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_ACTION);

    if (document.HasMember(PROTOCOL_TOKEN))
        msg.Token = document[PROTOCOL_TOKEN].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_TOKEN);

    if (document.HasMember(PROTOCOL_FROM))
        msg.From = document[PROTOCOL_FROM].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_FROM);

    if (document.HasMember(PROTOCOL_TO))
        msg.To = document[PROTOCOL_TO].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_TO);

    if (document.HasMember(PROTOCOL_DATA))
        msg.Data = document[PROTOCOL_DATA].GetString();
    else
        return static_cast<int>(ERROR_COMM::COMMON_ERR_NOT_FOUND_ACTION);

    return SUCCESS;
}

static int convertMSGToDocument(const Protocol msg, Document& document)
{
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember(PROTOCOL_TYPE, Value(msg.Type.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_COMMAND, Value(msg.Command.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_SUBCOMMAND, Value(msg.SubCommand.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_ACTION, Value(msg.Action.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_TOKEN, Value(msg.Token.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_FROM, Value(msg.From.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_TO, Value(msg.To.c_str(), allocator), allocator);
    document.AddMember(PROTOCOL_DATA, Value(msg.Data.c_str(), allocator), allocator);

    return SUCCESS;
}
