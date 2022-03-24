
#ifndef _CODECENCODERAPI_H_
#define _CODECENCODERAPI_H_

#include "SystemAPI.h"
#include "mediainfo.h"


#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////////////////////////
// Video Encoder
typedef	void*	FrVideoEncHandle;

LRSLT FrVideoEncOpen(FrVideoEncHandle* phEncoder, FrVideoInfo* hInfo);
void FrVideoEncClose(FrVideoEncHandle hEncoder);
//LRSLT FrVideoEncInit(FrVideoEncHandle hEncoder, FrVideoInfo* hInfo, DWORD32 dwPass);
//BOOL FrVideoEncDestroy(FrVideoEncHandle hEncoder, BOOL bEncCancel);
BOOL FrVideoEncReset(FrVideoEncHandle hEncoder, int nStartCTS);
LRSLT FrVideoEncEncode(FrVideoEncHandle hEncoder, FrRawVideo* pSrc, FrMediaStream* pMedia);
BOOL FrVideoEncFlush(FrVideoEncHandle hEncoder, FrMediaStream* pDst, FrVideoInfo *hVideo);

//DWORD32 McVideoEncGetStartTime(McVideoEncHandle hEncoder);
//BOOL McVideoEncIntra(McVideoEncHandle hEncoder);
//DWORD32 DtbVideoEncGetSupportResolution(DWORD32 dwFourCC, int nSubType);
//BOOL McVideoEncSetPSNR(McVideoEncHandle hEncoder, CHAR *pStr, DWORD32 dwStr);
//DWORD32	McVideoEncAlignVideoCTSwithFrameRate(DWORD32 dwCTS, DWORD32 dwFrameRate);

#ifdef __cplusplus
}
#endif

#endif	// _CODECENCODERAPI_H_
