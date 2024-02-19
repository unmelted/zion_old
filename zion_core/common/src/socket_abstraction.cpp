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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/19.
 *
 */

#include "socket_abstraction.h"

int SocketHandlerAbs::receive(int clnt_sock, char* pRecv, int nSize, int flags)
{
    int nReclen = 0, nTotalRecvSize = 0, nRemainSize = nSize;
    int nRecvSize = 1024;
    if (nRecvSize > nSize)
        nRecvSize = nSize;

    while (1)
    {
        if ((nReclen = recv(clnt_sock, pRecv, nRecvSize, MSG_NOSIGNAL)) == 0)
            return 0;

        if (nReclen <= 0)
            break;

        pRecv += nReclen;
        nTotalRecvSize += nReclen;
        nRemainSize -= nReclen;
        if (nRemainSize <= 0)
            break;

        if (nRecvSize > nRemainSize)
            nRecvSize = nRemainSize;
    }

    return nTotalRecvSize;
}

std::list<std::string> SocketHandlerAbs::getIPList()
{
    std::list<std::string> lstIPAddress;

    int             sock, nRet, nFamily = PF_INET;
    size_t          nNIC;
    const size_t    nMaxNIC = 256;
    struct ifconf   ifc;
    struct ifreq    ifr[nMaxNIC];
    struct sockaddr* pAddr(NULL);
    sock = socket(nFamily, SOCK_STREAM, 0);
    if (sock == -1)
        return lstIPAddress;

    ifc.ifc_len = sizeof(ifr);
    ifc.ifc_ifcu.ifcu_req = ifr;

    nRet = ioctl(sock, SIOCGIFCONF, &ifc);
    if (nRet == -1)
        return lstIPAddress;

    close(sock);
    nNIC = ifc.ifc_len / sizeof(struct ifreq);
    for (size_t i = 0; i < nNIC; i++)
    {
        if (nFamily == ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family)
        {
            pAddr = (&ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr);
            sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(pAddr);
            lstIPAddress.push_back(inet_ntoa(sin->sin_addr));
        }
    }

    return lstIPAddress;
}

std::string SocketHandlerAbs::getLocalCompare(std::string ip)
{
    std::string str_ip;
    std::list<std::string> _list = getIPList();
    std::list<std::string>::iterator iter;
    std::string strC = ip.substr(0, ip.rfind('.'));

    for (iter = _list.begin(); iter != _list.end(); iter++)
    {
        std::string strData = *iter;
        std::string strDiff = strData.substr(0, strData.rfind('.'));

        if (strDiff == strC)
        {
            str_ip = strData;
            break;
        }
    }

    return str_ip;
}
