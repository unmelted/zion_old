
#ifndef _CODECDECODERAPI_H_
#define _CODECDECODERAPI_H_

#include "SystemAPI.h"
#include "mediainfo.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
	MEM_CPU = 100,
	MEM_GPU = 101
} FrMemDir;
/////////////////////////////////////////////////////
// Video Decoder
typedef	void*	FrVideoDecHandle;

BOOL GetVideoCodec(DWORD32 dwCompression, INT32 iBitCount, char* pCodecString);
DWORD32 GetVideoFourCC(DWORD32 dwFourCC);			// Get a representative FourCC
BOOL FrVideoDecGetInfo(BYTE* pConfig, DWORD32 dwConfigLen, FrVideoInfo* hInfo);

// decoder
LRSLT FrVideoDecOpen(FrVideoDecHandle* phDecoder, FrMediaStream *pData, FrVideoInfo *hInfo);
void FrVideoDecClose(FrVideoDecHandle hHandle);
LRSLT FrVideoDecDecode(FrVideoDecHandle hHandle, FrMediaStream* pSrc, FrRawVideo* pMedia);
BOOL FrVideoDecReset(FrVideoDecHandle hHandle);
void FrVideoDecFlush(FrVideoDecHandle hHandle);


#ifdef __cplusplus
}
#endif

#endif	// _CODECDECODERAPI_H_
