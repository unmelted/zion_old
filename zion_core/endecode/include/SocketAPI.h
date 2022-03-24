
#ifndef _SOCKETAPI_H_
#define _SOCKETAPI_H_

/****************************************/
/*		Default Header Files			*/
/****************************************/
#if defined(WIN32)

#ifdef	MULTI_CAST
#include <winsock2.h>
#else
#include <winsock.h>
#endif
#endif

#ifdef WIN32
# ifndef _WIN32_WCE
#  pragma comment(lib, "Ws2_32.lib")
# endif
#endif

#include "SystemAPI.h"
#include "TraceAPI.h"
#include "mediaerror.h"

#if defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/poll.h>

// #include <openssl/rsa.h>       /* SSLeay stuff */
// #include <openssl/crypto.h>
// #include <openssl/x509.h>
// #include <openssl/pem.h>
// #include <openssl/ssl.h>
// #include <openssl/err.h>

#define SOCKET				INT32
#define SOCKET_HANDLE		INT32
#define INVALID_SOCKET		(-1)
#define SOCKET_ERROR		(-1)

#endif

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

#ifndef	FR_SOCK_STREAM
#define	FR_SOCK_STREAM		0
#endif

#ifndef	FR_SOCK_DGRAM
#define	FR_SOCK_DGRAM		1
#endif

#ifndef	INVALID_SOCK
#define INVALID_SOCK		0
#endif

#define	MAX_ADDR_LEN		16
#define	MAX_MSG_LEN			(1024*4)

#ifndef	ntohs
#define ntohs(x)	((((x)&0xFF)<<8) | (((x)&0xFF00)>>8))
#endif
#ifndef	ntohl
#define ntohl(x)	((((x)&0xFF)<<24) | (((x)&0xFF00)<<8) | (((x)&0xFF0000)>>8) | (((x)&0xFF000000)>>24))
#endif
#ifndef	htonl
#define htonl(x)	((((x)&0xFF)<<24) | (((x)&0xFF00)<<8) | (((x)&0xFF0000)>>8) | (((x)&0xFF000000)>>24))
#endif
#ifndef	htons
#define htons(x)	((((x)&0xFF)<<8) | (((x)&0xFF00)>>8))
#endif

/****************************************/
/*		Type Definition					*/
/****************************************/

#if !defined(SOCKET_HANDLE) && !defined(_SOCKET_HANDLE_DEFINED)
#define _SOCKET_HANDLE_DEFINED
typedef	UINT_PTR				SOCKET_HANDLE, *PSOCKET_HANDLE;
#endif

#if !defined(FD_SET_TYPE) && !defined(_FD_SET_TYPE_DEFINED)
#define _FD_SET_TYPE_DEFINED
typedef	fd_set				FD_SET_TYPE;
#endif

/****************************************/
/*		Socket Struct					*/
/****************************************/
typedef void DTB_SSL_CTX, *DTB_SSL_CTX_HANDLE;
typedef void DTB_SSL, *DTB_SSL_HANDLE;
typedef void DTB_WEBSOCKET, *DTB_WEBSOCKET_HANDLE;

#ifndef	SRTPROTO
typedef	int	SRTSOCKET;
#endif

typedef struct {
    char                m_strSSLCertificateFile[_MAX_FNAME];           //* SSL:// CertificateFile */
    char                m_strSSLCertificateChainFile[_MAX_FNAME];      //* SSL:// CertificateChainFile */
    char                m_strSSLKeyFile[_MAX_FNAME];                   //* SSL:// KeyFile */
    char                m_strSSLKeyFilePassword[_MAX_FNAME];           //* SSL:// KeyFilePassword */
} DTB_SSL_SERVER_INFO, *DTB_SSL_SERVER_INFO_HANDLE;

typedef	struct {
	SOCKET_HANDLE		        m_hSocket;
    BOOL				        m_bSSL;
    BOOL                        m_bSRT;
    BOOL                        m_bServer;                              // Server Mode or Client Mode
    BOOL                        m_bWebSocketServer;
    SRTSOCKET                   m_hSrtSocket;
	DWORD32				        m_nSrtRecvPrevPkgSeq;
	DWORD32				        m_nSrtSendPrevPkgSeq;
    DTB_SSL_CTX_HANDLE	        m_hCtx;
    DTB_SSL_HANDLE		        m_hSsl;
    DTB_WEBSOCKET_HANDLE        m_hWebSocket;
#ifdef _P2P_SOCKET
    BOOL                        m_bP2P;
    DTB_P2PSOCKET_HANDLE        m_hP2PSocket;
#endif
} SOCKET_STRUCT, *SOCK_HANDLE;

/****************************************/
/*		Func Proto Type Definition		*/
/****************************************/
LRSLT FrSocketInitNetwork(void);
void FrSocketCloseNetwork(void);


LRSLT McSocketAccept(SOCK_HANDLE* phSock, SOCK_HANDLE hSock, DWORD32* pdwAddr, WORD* pwPort);
BOOL McSocketBind(SOCK_HANDLE hSock, WORD wPort, BOOL bTCP);
BOOL McSocketBind2(SOCK_HANDLE hSock, char *pAddr, WORD wPort, BOOL bTCP);
void McSocketClose(SOCK_HANDLE hSock);
BOOL McSocketCheckClosed(SOCK_HANDLE hSock, DWORD32 dwTimeout);
LRSLT McSocketConnect(SOCK_HANDLE hSock, char* pAddr, WORD wPort, DWORD32 dwTimeOut);
const char* McSocketGetHostByAddr(char* pAddr);
DWORD32 McSocketGetHostByName(const char* pchName);
DWORD32 McSocketGetPeerAddr(SOCK_HANDLE hSock);
DWORD32 McSocketGetSockAddr(SOCK_HANDLE hSock);
WORD McSocketGetSockPort(SOCK_HANDLE hSock);
BOOL McSocketListen(SOCK_HANDLE hSock);
BOOL McSocketMulticastTTL(SOCK_HANDLE hSock, char* pMultiAddr, char* pLocalAddr, WORD wPort, BYTE nMulticastTTL);
BOOL McSocketMulticastJoin(SOCK_HANDLE hSock, char* pMultiAddr, char* pLocalAddr, WORD wPort);
void McSocketMulticastDrop(SOCK_HANDLE hSock, char* pMultiAddr, char* pLocalAddr, WORD wPort);
LRSLT McSocketOpen(SOCK_HANDLE* phSock, INT32 iType);
LRSLT McSocketTcpOpen(SOCK_HANDLE* phSock, char* pAddr, WORD wPort, BOOL bServer, DWORD32 dwTimeout);
LRSLT McSocketTcpOpen2(SOCK_HANDLE* phSock, char* pAddr, WORD wPort, BOOL bServer, DWORD32 dwTimeout, BOOL bSSL);
LRSLT McSocketWebSocketOpen(SOCK_HANDLE* phSock, char* pAddr, WORD wPort, char* pUri, BOOL bServer, DWORD32 dwTimeout, BOOL bSSL, DTB_SSL_SERVER_INFO_HANDLE hSSLServerInfo);
LRSLT McSocketSRTOpen(SOCK_HANDLE* phSock, char* pAddr, WORD wPort, BOOL bServer, DWORD32 dwTimeout);
LRSLT McSocketSRTFileModeOpen(SOCK_HANDLE* phSock, char* pAddr, WORD wPort, BOOL bServer, DWORD32 dwTimeout);
LRSLT McSocketSRTFileMsgModeOpen(SOCK_HANDLE* phSock, char* pAddr, WORD wPort, BOOL bServer, DWORD32 dwTimeout);
LRSLT McSocketSRTRendezvousOpen(SOCK_HANDLE* phSock, WORD wPort, INT32 iMode);

LRSLT McSocketP2PServerInit(char *pDeviceID);
LRSLT McSocketP2POpen(SOCK_HANDLE* phSock, char *pDeviceID, WORD *pwServiceID, BOOL bServer, DWORD32 dwTimeout, BOOL bTCPMode);
LRSLT McSocketUdpOpen(SOCK_HANDLE* phSock, char* pMultiAddr, char* pLocalAddr, WORD* pwPort, BOOL bMulti);
LRSLT McSocketUdpOpen2(SOCK_HANDLE* phSock, char* pMultiAddr, char* pLocalAddr, WORD* pwPort, BOOL bMulti, BYTE nMulticastTTL);
LRSLT McSocketUdpOpenFixed(SOCK_HANDLE* phSock, char* pMultiAddr, char* pLocalAddr, WORD* pwPort, BOOL bMulti);
LRSLT McSocketUdpOpenFixed2(SOCK_HANDLE* phSock, char* pMultiAddr, char* pLocalAddr, WORD* pwPort, BOOL bMulti);
LRSLT McSocketUdpOpenRangeFixed(SOCK_HANDLE* phSock, char* pMultiAddr, char* pLocalAddr, WORD* pwPort, BOOL bMulti, DWORD32 dwRange);
LRSLT McSocketRecv(SOCK_HANDLE hSock, char* pBuf, INT32 iLen);
LRSLT McSocketRecvFrom(SOCK_HANDLE hSock, char *pBuf, INT32 iLen, DWORD32* pdwAddr, WORD* pwPort);
LRSLT McSocketSelect(FD_SET_TYPE *rSet, FD_SET_TYPE *wSet, FD_SET_TYPE *eSet);
LRSLT McSocketSelect2(FD_SET_TYPE *rSet, FD_SET_TYPE *wSet, FD_SET_TYPE *eSet, DWORD32 dwTimeout);
LRSLT McSocketSend(SOCK_HANDLE hSock, char* pBuf, INT32 iLen);
LRSLT McSocketOneSend(SOCK_HANDLE hSock, char *pBuf, INT32 iLen);
LRSLT McSocketSendTo(SOCK_HANDLE hSock, char* pBuf, INT32 iLen, char* pAddr, WORD wPort);
LRSLT McSocketWait(SOCK_HANDLE hSock, DWORD32 dwTimeout);
LRSLT McSocketWait2(SOCK_HANDLE hSock, DWORD32 dwTimeout);
LRSLT McSocketWait3(SOCK_HANDLE hSock, DWORD dwTimeout);
LRSLT McSocketGetHostName(char* pchHostName, DWORD32 dwBufLen);
LRSLT McSocketMsgSend(SOCK_HANDLE hSock, char *pBuf, INT32 iLen);
INT32 DtbSocketSend(SOCK_HANDLE hSock, const char* ptr, INT32 nbytes);
INT32 DtbSocketRecv(SOCK_HANDLE hSock, char* ptr, INT32 nbytes);
LONG McSocketSendNonBlock(SOCK_HANDLE hSock, char *pBuf, int iLen, int mstime);

BOOL McSocketIsValid(SOCK_HANDLE hSock);
VOID McSocketSetInValid(SOCK_HANDLE hSock);

DWORD32 INET_ADDR(char *psz);
void INET_NTOA(char* psz, DWORD32 dwAddr);
ULONG GetErrorMessage(char* szFunc);


#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _SOCKETAPI_H_
