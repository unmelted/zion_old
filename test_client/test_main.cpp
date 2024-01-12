#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
 
    send(sock, mtdProtoHeader, 0);
    std::cout << "Hello message sent\n";
    int valread = read(sock, buffer, 1024);
    std::cout << buffer << std::endl;

    // 소켓 닫기
    close(sock);

    return 0;
}

