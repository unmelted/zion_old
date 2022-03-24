
#ifndef	_RTPAPI_H_
#define	_RTPAPI_H_

#include "SystemAPI.h"

#define		RTP_UDP_PORT				16000
#define		RTP_TCP_PORT				0
#define		MAX_RTP_PKT_SIZE			2000

#define		RTP_HEADER_LENGTH			12
#define		RTP_MAX_MTU_SIZE			1400


#ifdef _WIN32_WCE
#define		_NOT_BIT_OP_
#endif

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

typedef struct {
#ifdef	_NOT_BIT_OP_
	WORD	wHead;
#else
	WORD	cc:4;			/* CSRC count */
	WORD	x:1;			/* header extension flag */
	WORD	p:1;			/* padding flag */
	WORD	version:2;		/* protocol version */
	WORD	pt:7;			/* payload type */
	WORD	m:1;			/* marker bit */
#endif
	WORD	wSeq;			/* sequence number */
	DWORD32	dwTimestamp;	/* timestamp */
	DWORD32	dwSSRC;			/* synchronization source */
	//DWORD32	dwCSRC[1];		/* optional CSRC list */
} RTP_HEADER;

typedef	void*	FrRTPHandle;

// RTP
LRSLT FrRTPOpen(FrRTPHandle* phRTP, FrMediaHandle hMedia);
void FrRTPClose(FrRTPHandle hHandle);

void FrRTPMakeRTPPacket(FrRTPHandle hHandle, BYTE* pPacket, DWORD32 dwCTS, BOOL bMBit);
void FrRTPMakeRTPPacketUseInputCTS(FrRTPHandle hHandle, BYTE* pPacket, DWORD32 dwCTS, BOOL bMBit);
LRSLT FrRTPParseRTPPacket(FrRTPHandle hHandle, BYTE* pPacket, DWORD32 dwPacket, WORD* pwSeq, DWORD32 dwTick, DWORD32* pdwStartTS);
DWORD32 FrRTPMakeRTCPPacket(FrRTPHandle hHandle, BYTE* pBuffer, DWORD32 dwBufDuration, WORD wBufRemain, BOOL bSend, BOOL bBYE);
DWORD32 FrRTPMakeFeedbackRTCPPacket(FrRTPHandle hHandle, BYTE* pBuffer, WORD wFbType);	// nhn on air
BOOL FrRTPParseRTCPPacket(FrRTPHandle hHandle, BYTE* pPacket, DWORD32 dwPacket);

void FrRTPSetStartTS(FrRTPHandle hHandle, DWORD32 dwStartTS);
void FrRTPSetPT(FrRTPHandle hHandle, WORD wPT);
DWORD32 FrRTPGetCTS(FrRTPHandle hHandle, BYTE* pPacket, BOOL bRewind);
DWORD32 FrRTPGetRtpTime(BYTE* pPacket);
DWORD32 FrRTPGetBFrames(FrRTPHandle hHandle);
DWORD32 FrRTPGetRecvTS(FrRTPHandle hHandle);
DWORD32 FrRTPGetLastRecvTS(FrRTPHandle hHandle);
WORD FrRTPGetRecvSeq(FrRTPHandle hHandle);
BOOL FrRTPGetRecvMBit(FrRTPHandle hHandle);
BOOL FrRTPGetLastRecvMBit(FrRTPHandle hHandle);
DWORD32 FrRTPGetSendTS(FrRTPHandle hHandle);
WORD FrRTPGetSendSeq(FrRTPHandle hHandle);
DWORD32 FrRTPGetSSRC(FrRTPHandle hHandle);
BOOL FrRTPIsRTCPBye(FrRTPHandle hHandle);
DWORD32 FrRTPGetLastCTS(FrRTPHandle hHandle);
DWORD32 FrRTPGetBufferedDurationInMsec(FrRTPHandle hHandle);
DWORD32 FrRTPGetInitialDurationInMsec(FrRTPHandle hHandle, DWORD32 dwStartCTS);
DWORD32 FrRTPGetSSRCFromRTCP(BYTE* pBuffer);
WORD FrRTPGetPT(FrRTPHandle hHandle);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _RTPAPI_H_
