
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define RAPIDJSON_HAS_STDSTRING 1

#define MTDPROTOCOL_SECTION1    "Type"
#define MTDPROTOCOL_SECTION2    "Command"
#define MTDPROTOCOL_SECTION3    "SubCommand"
#define MTDPROTOCOL_ACTION      "Action"
#define MTDPROTOCOL_FROM        "From"
#define MTDPROTOCOL_TO          "To"
#define MTDPROTOCOL_DATA        "Data"
#define MTDPROTOCOL_TOKEN       "Token"


#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <thread>

#include "common.h"

using namespace rapidjson;

#pragma pack(push, 1)  
struct MTdProtocolHeader
{
    int nSize;
    char cSeparator;
};

#pragma pack(pop)  

enum {
    PACKETTYPE_JSON,
    PACKETTYPE_BINALY,
    PACKETTYPE_ABAILABLE_INQUIRY_VP = 20,
    PACKETTYPE_VP_SET_BUFFERCOUNT,
    PACKETTYPE_VP_ALIVE,
    PACKETTYPE_CALIBRATION_VALUE = 100,

    PACKETTYPE_SIZE,
};

std::string GetDocumentToString(Document &document)
{
    StringBuffer strbuf;
    strbuf.Clear();
    PrettyWriter<StringBuffer> writer(strbuf);
    document.Accept(writer);
    std::string ownShipRadarString = strbuf.GetString();

    return ownShipRadarString;
}

bool SendData(std::string strJson, int sock)
{
    int nSize = (int)strlen(strJson.c_str());
    char cType = PACKETTYPE_JSON;
    m_SendMutex.lock();

    int nSendSize = sizeof(int) + 1 + nSize;

    if (m_nSendBufferSize < nSendSize)
    {

        if (m_pSendBuffer != nullptr)
            delete[] m_pSendBuffer;

        m_pSendBuffer = new char[nSendSize];
        m_nSendBufferSize = nSendSize;

    }
    memcpy(m_pSendBuffer, (char*)&nSize, sizeof(int));

    int nBufPos = sizeof(int);

    memcpy(m_pSendBuffer + nBufPos, &cType, 1);
    nBufPos++;
    memcpy(m_pSendBuffer + nBufPos, strJson.c_str(), nSize);
    m_Sockmutx.lock();

    int nSend = send(sock, m_pSendBuffer, nSendSize, MSG_NOSIGNAL);

    m_Sockmutx.unlock();
    m_SendMutex.unlock();

    if (nSend != nSendSize)
    {
        //ErrorL << "[ERROR]Send Fail MTD";
        return false;

    }
    return true;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(19733); // 서버 포트 번호

    // 서버 주소 변환
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nConnection Failed \n";
        return -1;
    }

    // 데이터 송수신
    MTdProtocolHeader mtdProtoHeader;
    mtdProtoHeader.nSize = strlen("hello server\n") + sizeof(MTdProtocolHeader);
    mtdProtoHeader.cSeparator = PACKETTYPE_SIZE;
    std::cout << "mtdProtoHeader.nSize : " << mtdProtoHeader.nSize << std::endl;


    Document document;
    Document sndDoc(kObjectType);
    Document::AllocatorType &allocator = sndDoc.GetAllocator();

    Value ver(kObjectType);
    Value cmd(kObjectType);
    cmd.AddMember("verion", "0.0.1.T", allocator);
    cmd.AddMember("date", "2024-01--14", allocator);
    ver.AddMember("CMd", cmd, allocator);

    sndDoc.AddMember(MTDPROTOCOL_SECTION1, "REQUEST", allocator);
    sndDoc.AddMember(MTDPROTOCOL_SECTION2, "CONNECT", allocator);
    sndDoc.AddMember(MTDPROTOCOL_SECTION3, "VERSION", allocator);
    sndDoc.AddMember(MTDPROTOCOL_ACTION, "response", allocator);
    sndDoc.AddMember(MTDPROTOCOL_TOKEN, "TEST_TOKEN", allocator);
    sndDoc.AddMember(MTDPROTOCOL_FROM, "SR1", allocator);
    sndDoc.AddMember(MTDPROTOCOL_TO, "ICS_SERVER", allocator);
    sndDoc.AddMember(MTDPROTOCOL_DATA, "TEST_SEND_MSG", allocator);
    sndDoc.AddMember("Version", ver, allocator);
    std::string strSendString = GetDocumentToString(sndDoc);

    SendData(strSendString.c_str(), sock);
    std::cout <<"send connect message " << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<< "sleep 1 second " << std::endl;
    sndDoc.SetObject();
    sndDoc.AddMember(MTDPROTOCOL_SECTION1, "REQUEST", allocator);
    sndDoc.AddMember(MTDPROTOCOL_SECTION2, "START", allocator);
    sndDoc.AddMember(MTDPROTOCOL_SECTION3, "", allocator);
    sndDoc.AddMember(MTDPROTOCOL_ACTION, "", allocator);
    sndDoc.AddMember(MTDPROTOCOL_TOKEN, "TEST_TOKEN", allocator);
    sndDoc.AddMember(MTDPROTOCOL_FROM, "SR1", allocator);
    sndDoc.AddMember(MTDPROTOCOL_TO, "ICS_SERVER", allocator);
    sndDoc.AddMember(MTDPROTOCOL_DATA, "TEST_SEND_MSG", allocator);
//    sndDoc.AddMember("Version", ver, allocator);
    strSendString = GetDocumentToString(sndDoc);

    SendData(strSendString.c_str(), sock);

    //send(sock, &mtdProtoHeader, mtdProtoHeader.nSize, 0);
    std::cout << "Hello message sent\n";
    int valread = read(sock, buffer, 1024);

    Document rcvDoc;
    rcvDoc.Parse(buffer);

    std::cout << "test _main read end"  << std::endl;

    valread = read(sock, buffer, 1024);
    // 소켓 닫기
    close(sock);

    return 0;
}

