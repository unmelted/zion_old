#pragma once

#include "SystemAPI.h"
#include "TraceAPI.h"
#include "SocketAPI.h"
#include "mediainfo.h"
#include "SdpAPI.h"
#include "RtspAPI.h"
#include "RtpAPI.h"
#include "PacketAPI.h"
#include "StreamUtilAPI.h"
#include <queue>
#include <vector>
#include <algorithm>

#ifdef min
#undef min
#endif

#define		RTSP_RECV_TIMEOUT			5000 //10000	// 5 sec

#define		RTSP_TASK_PRIORITY			THREAD_PRIORITY_NORMAL
#define		RTP_TASK_PRIORITY			THREAD_PRIORITY_HIGHEST
#define		RTCP_TASK_PRIORITY			THREAD_PRIORITY_LOWEST
#define		SERVER_TASK_PRIORITY		THREAD_PRIORITY_LOWEST

#define		RTSP_TASK_STACKSIZE			0x4000
#define		RTP_TASK_STACKSIZE			0x1000
#define		RTCP_TASK_STACKSIZE			0x400
#define		SERVER_TASK_STACKSIZE		0x400

#define		MAX_SESSION					16 //1 //16
#define		MAX_SDP_LEN					4096
#define		MAX_STREAM	2

const int SEND_RTP = 1;
const int SEND_TS = 2;

#ifdef _DEBUG
#define RTSP_SESESSION_MODEL
#endif

// callback
typedef void(*cbSessionStart)(void*, void*);
typedef void(*cbSessionStop)(void*, void*);

typedef struct {
	short wListenPort;
	int		mode;

	FrMediaInfo* hInfo;
} SenderParam;

typedef	struct {
	DWORD32		m_dwCTS;
	DWORD32		m_dwTS;
	DWORD32		m_dwSeq;
} PacketInfo;

typedef	struct {
	char*		pPacket;
	DWORD32		dwSize;
	DWORD32		dwCTS;
	DWORD32		dwTS;
	DWORD32		dwSeq;
} RtpPacket;

typedef	struct {
	std::vector<uint8_t> vPacket;
	DWORD32		dwSize;
	DWORD32		dwCTS;
	DWORD32		dwTS;
	DWORD32		dwSeq;
} RtpSample;

class CircularBuffer
{
public:
	CircularBuffer(size_t capacity);
	~CircularBuffer();

	size_t size() const { return size_; }
	size_t capacity() const { return capacity_; }
	// Return number of bytes written.
	size_t write(const char *data, size_t bytes);
	// Return number of bytes read.
	size_t read(char *data, size_t bytes);

private:
	size_t beg_index_, end_index_, size_, capacity_;
	char *data_;
};

// session socket params which are used between rtsp serve task and rtp send task.
class ChannelCtrl {
public:
	BYTE				m_pRTCPBuf[MAX_RTP_PKT_SIZE];

	BOOL				m_bUDP;
	BOOL				m_bSend;
	BOOL				m_bPlay;
	DWORD32				m_dwSeq;

	SOCK_HANDLE			m_hRTSPSocket = INVALID_SOCK;
	SOCK_HANDLE			m_hRTPSocket = INVALID_SOCK;
	SOCK_HANDLE			m_hRTCPSocket = INVALID_SOCK;
	WORD				m_wRTPLocalPort;
	WORD				m_wRTCPLocalPort;
	char*				m_szRemoteAddr;
	WORD				m_wRTPRemotePort;
	WORD				m_wRTCPRemotePort;
};

// a/v rtp sender task class
class RtpTaskStream {
public:
	FrMediaType			m_eStreamType;
	McQueHandle			m_hBuf;
	McQueHandle			m_hInfoBuf;
	FrPacketHandle		m_hPacket;
	FrRTPHandle			m_hRTP;
	TASK_HANDLE			m_hRTPSend = INVALID_TASK;
	BOOL				m_bEnd;
	BOOL				m_bWait;

	LONG                m_lError;
	DWORD32				m_dwVideoTick;

	MUTEX_HANDLE		m_xChannel;

	std::vector<ChannelCtrl *> m_Channel;
	
	// need to modify
	//std::vector<ChannelCtrl *> *m_pvChannel;
	std::queue<RtpPacket*> m_qPacket;
	CircularBuffer *m_hCirBuffer;

public:
	RtpTaskStream(FrMediaType eType);
	~RtpTaskStream();
	LRSLT Open();
	void Close();
};

// rtsp session class
class RtspSession {
public:
	std::vector<ChannelCtrl *> m_vChCtrl;
	DWORD				m_dwChannel;

	RTSP_INFO			m_RtspInfo;
	FrRTSPHandle		m_hRTSP;
	SOCK_HANDLE			m_hRTSPSocket;
	
	char				m_pSendBuffer[MAX_RTSP_MSG_SIZE];
	char				m_pRecvBuffer[MAX_RTSP_MSG_SIZE];
	DWORD32				m_dwRecvLen;

	char*				m_szRemoteAddr = NULL;
	DWORD32				m_dwRemoteAddr;
	WORD				m_wRTSPRemotePort;

	EVENT_HANDLE		m_hRecvEvent;

	char*				m_pSvrAddr = nullptr;

public :
	RtspSession();
	RtspSession(SOCK_HANDLE	hRTSPSocket, DWORD32 dwAddr, char* pSvrAddr, DWORD32 dwStream);
	~RtspSession();
	LRSLT Open(const char* pURL, FrSDPHandle hSDP, const char* pSDP);
	void Close();
	LRSLT Setup(WORD* pUDPPort, std::vector<RtpTaskStream *> &rtpStream);
	LRSLT Play(void *pData);
	char* RTSPParseRTPMessage(char* pBuffer, DWORD32* pdwBufLen);
};

// rtsp/rtp sender class
class FrStreamRTPSender {
public:
	char*				m_szURL;
	char*				m_szSvrAddr;	
	SOCK_HANDLE			m_hSvrSocket = INVALID_SOCK;
	
	TASK_HANDLE			m_hSvrTask;
	TASK_HANDLE			m_hRTSPRecv;
	TASK_HANDLE			m_hRTCPTask;
	BOOL				m_bEnd;
	LONG				m_lError;
	
	std::vector<RtpTaskStream *> m_vStream;
	std::vector<RtspSession	*> m_vSession;

	DWORD32				m_dwStream;
	DWORD				m_dwAudioIdx;
	DWORD				m_dwVideoIdx;
	BOOL				m_bAudioExist;
	BOOL				m_bVideoExist;
	BYTE*				m_p264Buffer = nullptr;
	DWORD32				m_dw264BuffSize = 1024 * 1024 * 2;
	
	FrSDPHandle			m_hSDP = nullptr;
	char				m_pSDP[MAX_SDP_LEN];

	WORD				m_wUDPPort;
	WORD				m_wTCPPort;
	
	cbSessionStart	m_cbStart;
	cbSessionStop	m_cbStop;
	void*			m_ctx;
public:
	FrStreamRTPSender();
	FrStreamRTPSender(void*, void*, void*);
	~FrStreamRTPSender();
	LRSLT Open(FrMediaInfo* hInfo, short port);
	void Close();
	LONG SendFrame(FrMediaStream* pMedia, void *extra);

	LRSLT SessionOpen();
	void SessionClose(RtspSession *hSession);
	//LONG SessionSetup();
	//LONG SessionPlay();
};

void TaskListener(void* lpVoid);
void TaskRTPSender(void* lpVoid);
void funcRTPSender(void* lpVoid);
void TaskRTSPServer(void* lpVoid);
