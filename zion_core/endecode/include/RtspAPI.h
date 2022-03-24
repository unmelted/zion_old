
#ifndef	_RTSPAPI_H_
#define	_RTSPAPI_H_

#include "SystemAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

typedef	enum {
	ANNOUNCE_METHOD,
	REDIRECT_METHOD,
	DESCRIBE_METHOD,
	SETUP_METHOD,
	PLAY_METHOD,
	RECORD_METHOD,
	PAUSE_METHOD,
	TEARDOWN_METHOD,
	OPTIONS_METHOD,
	SET_PARAMETER_METHOD,
	GET_PARAMETER_METHOD,
	NOT_METHOD,
} METHOD_TYPE;

typedef enum {
    SS_NONE = 0,
    SS_WAIT,
    SS_INIT,
    SS_READY,
    SS_PAUSE,
    SS_PAUSING
} SESSION_STATE;

#define		RTSP_DEFAULT_PORT			554
#define		HTTP_DEFAULT_PORT			80
#define		MAX_RTSP_MSG_SIZE			10000
#define		RTP_TCP_HEADER_LEN			4
#define		RTP_SRT_HEADER_LEN			4

typedef	struct {
	TCHAR*		m_pURL;
	char*		m_pUserAgent;
	char*		m_pUserName;
	char*		m_pUserPasswd;
	DWORD32		m_dwStartDate;
	DWORD32		m_dwStartTime;
	DWORD32		m_dwEndDate;
	DWORD32		m_dwEndTime;

	BOOL		m_bServer;
	BOOL		m_bUDP;
    BOOL        m_bUDPfec;
	BOOL		m_bHTTP;
	BOOL		m_bMulti;
	BOOL		m_bTS;
	BOOL		m_bMgosp;
	BOOL		m_bMMS;
	DWORD32		m_dwSession;
	BOOL		m_b3GP;		// distinguish SKT and 3GP
	BOOL		m_bPublishing;
	BOOL		m_bSRT;		// low latency
	DWORD32		m_dwBufferTime;
	BOOL		m_bRelay;	// WebSocketRelay
} RTSP_INFO;

typedef	void*	FrRTSPHandle;

LRSLT FrRTSPOpen(FrRTSPHandle* phHandle, RTSP_INFO* pInfo);
void FrRTSPClose(FrRTSPHandle hHandle);
LRSLT FrRTSPInit(FrRTSPHandle hHandle, DWORD32 dwChannelNum);
void FrRTSPGetConfigInfo(FrRTSPHandle hHandle, RTSP_INFO* pInfo);
void FrRTSPSetConfigInfo(FrRTSPHandle hHandle, RTSP_INFO* pInfo);

// method
LRSLT FrRTSPHttpGetMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPAnnounceMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPOptionsMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPDescribeMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPSetupMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, DWORD32 dwIndex);
LRSLT FrRTSPPlayMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, BOOL bResume, DWORD32 dwPos);
LRSLT FrRTSPRecordMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPPlayClockMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, DWORD32 dwDate, DWORD32 dwTime);
LRSLT FrRTSPPlayScaleMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, int nScale);
LRSLT FrRTSPPauseMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPTeardownMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);
LRSLT FrRTSPGetParamMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, char* pContent);
LRSLT FrRTSPSetParamMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, char* pContent);
LRSLT FrRTSPPlayTrickPlayMethod(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen, BOOL bResume, DWORD32 dwPos, INT32 nScale, BOOL bIntra, DWORD32 dwIntraInterval);

// message info
LRSLT FrRTSPGetErrorCode(FrRTSPHandle hHandle);
void FrRTSPSetErrorCode(FrRTSPHandle hHandle, DWORD32 dwErrorCode);
METHOD_TYPE FrRTSPGetReqMethod(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetReqIndex(FrRTSPHandle hHandle);
char* FrRTSPParseMessage(FrRTSPHandle hHandle, char* pBuffer, DWORD32* pdwLen);
BOOL FrRTSPMakeResponse(FrRTSPHandle hHandle, char* pBuffer, DWORD32 dwBuffLen);

// SDP
char* FrRTSPGetSDP(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetSDPLength(FrRTSPHandle hHandle);
void FrRTSPSetSDP(FrRTSPHandle hHandle, char* pSDP);
void FrRTSPSetSDPSampleRate(FrRTSPHandle hHandle, DWORD32 dwIndex, DWORD32 dwSampleRate);

// Announce & Describe Info
char* FrRTSPGetContentURL(FrRTSPHandle hHandle);
char* FrRTSPGetContentLocation(FrRTSPHandle hHandle);
char* FrRTSPGetUserAgent(FrRTSPHandle hHandle);
char* FrRTSPGetUserName(FrRTSPHandle hHandle);
BOOL FrRTSPIsPreContent(FrRTSPHandle hHandle);
void FrRTSPSetPreContent(FrRTSPHandle hHandle, char* pContentDate, BOOL bPreContent);
BOOL FrRTSPIsJoinContent(FrRTSPHandle hHandle);
void FrRTSPSetJoinContent(FrRTSPHandle hHandle, BOOL bJoinContent);
void FrRTSPSetServername(FrRTSPHandle hHandle, char* pszServer);

// Setup Info
void FrRTSPSetControl(FrRTSPHandle hHandle, char* pControl);
void FrRTSPSetTrackControl(FrRTSPHandle hHandle, DWORD32 dwIndex, char* pControl);
void FrRTSPSetLocalAddress(FrRTSPHandle hHandle, char* pAddr);
void FrRTSPSetMultiAddress(FrRTSPHandle hHandle, char* pAddr);
void FrRTSPSetRTPLocalPort(FrRTSPHandle hHandle, DWORD32 dwIndex, WORD wPort);
void FrRTSPSetRTCPLocalPort(FrRTSPHandle hHandle, DWORD32 dwIndex, WORD wPort);
void FrRTSPSetRTPMultiPort(FrRTSPHandle hHandle, DWORD32 dwIndex, WORD wPort);
void FrRTSPSetRTCPMultiPort(FrRTSPHandle hHandle, DWORD32 dwIndex, WORD wPort);
char* FrRTSPGetMultiAddress(FrRTSPHandle hHandle);
WORD FrRTSPGetRTPRemotePort(FrRTSPHandle hHandle, DWORD32 dwIndex);
WORD FrRTSPGetRTCPRemotePort(FrRTSPHandle hHandle, DWORD32 dwIndex);
WORD FrRTSPGetRTPMultiPort(FrRTSPHandle hHandle, DWORD32 dwIndex);
WORD FrRTSPGetRTCPMultiPort(FrRTSPHandle hHandle, DWORD32 dwIndex);

void FrRTSPUpdateTransport(FrRTSPHandle hHandle, BOOL bFlag);	// mwkang
void FrRTSPSetAnnounceCSeq(FrRTSPHandle hHandle);

// Play info
BOOL FrRTSPIsPlayRange(FrRTSPHandle hHandle);
BOOL FrRTSPIsPlayRangeNtp(FrRTSPHandle hHandle);
void FrRTSPSetPlayRange(FrRTSPHandle hHandle, DWORD32 dwRange);
void FrRTSPSetPlayFromRange(FrRTSPHandle hHandle, DWORD32 dwRange);
void FrRTSPSetPlayFromDateTime(FrRTSPHandle hHandle, DWORD32 dwDate, DWORD32 dwTime);
void FrRTSPSetPlayToDateTime(FrRTSPHandle hHandle, DWORD32 dwDate, DWORD32 dwTime);
DWORD32 FrRTSPGetPlayFromRange(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetPlayToRange(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetPlayFromDate(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetPlayFromTime(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetPlayToDate(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetPlayToTime(FrRTSPHandle hHandle);
INT32 FrRTSPGetScale(FrRTSPHandle hHandle);
char* FrRTSPGetFramesMode(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetFramesInterval(FrRTSPHandle hHandle);
BOOL FrRTSPGetRTPInfoComplete(FrRTSPHandle hHandle, DWORD32 dwIndex);
WORD FrRTSPGetRTPInfoSeq(FrRTSPHandle hHandle, DWORD32 dwIndex);
void FrRTSPSetRTPInfoSeq(FrRTSPHandle hHandle, DWORD32 dwIndex, WORD wSeq);
DWORD32 FrRTSPGetRTPInfoTime(FrRTSPHandle hHandle, DWORD32 dwIndex);
void FrRTSPSetRTPInfoTime(FrRTSPHandle hHandle, DWORD32 dwIndex, DWORD32 dwRTPTime);
DWORD32 FrRTSPGetRTPInfoSSRC(FrRTSPHandle hHandle, DWORD32 dwIndex);
void FrRTSPSetRTPInfoSSRC(FrRTSPHandle hHandle, DWORD32 dwIndex, DWORD32 dwRTPSSRC);
char *FrRTSPGetRTSPSessionID(FrRTSPHandle hHandle);

BOOL FrRTSPGetCheckSttup(FrRTSPHandle hHandle, DWORD32 dwIndex);
void FrRTSPSetCheckSetup(FrRTSPHandle hHandle, DWORD32 dwIndex);

// Content
BOOL FrRTSPIsContent(FrRTSPHandle hHandle);
char* FrRTSPGetContent(FrRTSPHandle hHandle);
DWORD32 FrRTSPGetContentLength(FrRTSPHandle hHandle);

// RTSP Writing Log API
BOOL FrRTSPMakeLog(FrRTSPHandle hHandle, char* pBuffer, char* pRet, DWORD32 dwBuffLen, FILE_HANDLE hFile, BOOL bPlay);
// nhn
void FrRTSPSetDtbResultCode(FrRTSPHandle hHandle, DWORD32 dwResultCode);
void FrRTSPSetDtbKeyCode(FrRTSPHandle hHandle, DWORD32 dwKeyCode);
char* FrRTSPGetUDPSourcAddr(FrRTSPHandle hHandle);
// s1
BOOL FrRTSPIsFindRTPInfo(FrRTSPHandle hHandle);
BOOL FrRTSPIsFindRange(FrRTSPHandle hHandle);

// alive
BOOL FrRTSPGetNeedToGetParameter(FrRTSPHandle hHandle);

// srt
WORD FrRTSPGetSRTServerPort(FrRTSPHandle hHandle);
BOOL FrRTSPParseRtpInfo(FrRTSPHandle hHandle);
//DWORD32 FrRTSPGetBufferTime(FrRTSPHandle hHandle);

// Chrome and Not use WaitEvent
VOID FrRTSPUnsetResponse(FrRTSPHandle hHandle);
VOID FrRTSPSetResponse(FrRTSPHandle hHandle);
BOOL FrRTSPGetResponse(FrRTSPHandle hHandle);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _RTSPAPI_H_
