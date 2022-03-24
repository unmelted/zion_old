
#ifndef	_SDPAPI_H_
#define	_SDPAPI_H_

#include "SystemAPI.h"
#include "StreamUtilAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

typedef	enum
{
	MPEG4V_OBJECT,
	H263_OBJECT,
	H264_OBJECT,
	H265_OBJECT,
	SVC_OBJECT,
	VCO_OBJECT,			// nhn

	AAC_LATM_OBJECT,
	HEAAC_LATM_OBJECT,
	PSAAC_LATM_OBJECT,
	AMR_OBJECT,
	AMR_WB_OBJECT,
	EVRC_OBJECT,
	QCELP_OBJECT,
	G711A_OBJECT,
	G711U_OBJECT,
	ACO_OBJECT,			// nhn
	OPUS_OBJECT,

	AAC_HBR_GENERIC_OBJECT,
	AAC_LBR_GENERIC_OBJECT,
	MPEG_CELP_CBR_GENERIC_OBJECT,
	MPEG_CELP_VBR_GENERIC_OBJECT,

	TEXT_OBJECT,

	ONVIF_META_OBJECT,	// s one

	UNKNOWN_OBJECT,

} OBJECT_TYPE;

typedef enum {
	MI_AUDIO = 0,
	MI_VIDEO,
	MI_TEXT,
	MI_APPLICATION,
	MI_DATA,
	MI_CONTROL
} MEDIA_TYPE2;

typedef	void*	FrMediaHandle;
typedef	void*	FrSDPHandle;

LRSLT FrSDPOpen(FrSDPHandle* phSDP, char* pSDP, DWORD32 dwSDPLen, DWORD32 dwMedia);
void FrSDPClose(FrSDPHandle hHandle);
LRSLT FrSDPGetSDP(FrSDPHandle hHandle, char* pBuffer, DWORD32 dwLen);

// SDP
char* FrSDPGetSDPControl(FrSDPHandle hHandle);
DWORD32 FrSDPGetSDPRange(FrSDPHandle hHandle);
void FrSDPSetSDPRange(FrSDPHandle hHandle, DWORD32 dwRange);
DWORD32 FrSDPGetMediaNum(FrSDPHandle hHandle);
DWORD32 FrSDPGetMediaNumOrg(FrSDPHandle hHandle);
void FrSDPSetLocalAddr(FrSDPHandle hHandle, char* pAddr);
void FrSDPSetCharge(FrSDPHandle hHandle, BOOL bCharge);
void FrSDPDelSKInfo(FrSDPHandle hHandle);
void FrSDPCheck3GP(FrSDPHandle hHandle);
char* FrSDPGetSessionFieldName(FrSDPHandle hHandle);
char* FrSDPGetInfoFieldName(FrSDPHandle hHandle);
char* FrSDPGetAuthToken(FrMediaHandle hHandle);
void FrSDPSetAuthToken(FrSDPHandle hHandle, char* pToken);
INT32 FrSDPGetInitSeekDiffTime(FrSDPHandle hHandle);
void FrSDPSetInitSeekDiffTime(FrSDPHandle hHandle, INT32 nDiffTime);

// Media
FrMediaHandle FrSDPGetMediaHandle(FrSDPHandle hHandle, DWORD32 dwIndex);
BOOL FrSDPGetMediaIsLive(FrSDPHandle hHandle);

char* FrSDPGetMediaControl(FrMediaHandle hHandle);
MEDIA_TYPE2 FrSDPGetMediaType(FrMediaHandle hHandle);
void FrSDPSetMediaType(FrMediaHandle hHandle, MEDIA_TYPE2 tType);
BOOL FrSDPGetSendOnly(FrMediaHandle hHandle);
DWORD32 FrSDPGetMediaRange(FrMediaHandle hHandle);
void FrSDPSetMediaRange(FrMediaHandle hHandle, DWORD32 dwRange);
DWORD32 FrSDPGetMediaAvgBitrate(FrMediaHandle hHandle);
void FrSDPSetMediaAvgBitrate(FrMediaHandle hHandle, DWORD32 dwAvgBitrate);
DWORD32 FrSDPGetMediaBuffSize(FrMediaHandle hHandle);
void FrSDPSetMediaBuffSize(FrMediaHandle hHandle, DWORD32 dwBuffSize);
OBJECT_TYPE FrSDPGetMediaObjectType(FrMediaHandle hHandle);
DWORD32 FrSDPGetMediaPayloadType(FrMediaHandle hHandle);
void FrSDPSetMediaPayloadType(FrMediaHandle hHandle, DWORD32 dwPayload);
void FrSDPSetMediaObjectType(FrMediaHandle hHandle, OBJECT_TYPE eObjectType);
DWORD32 FrSDPGetMediaSampleRate(FrMediaHandle hHandle);
void FrSDPSetMediaSampleRate(FrMediaHandle hHandle, DWORD32 dwSampleRate);
BYTE* FrSDPGetMediaConfig(FrMediaHandle hHandle);
DWORD32 FrSDPGetMediaConfigLen(FrMediaHandle hHandle);
BOOL FrSDPGetMediaH264ParamSet(FrMediaHandle hHandle);
void FrSDPSetMediaConfig(FrMediaHandle hHandle, BYTE* pConfig, DWORD32 dwConfigLen);
void FrSDPSetMediaConfigLen(FrMediaHandle hHandle, DWORD32 dwConfigLen);
void FrSDPSetMediaH264ParamSet(FrMediaHandle hHandle, BOOL bH264ParamSet);
hvcCStruct* FrSDPGetH265hvcCInfo(FrMediaHandle hHandle);
int FrSDPGetMediaProfile(FrMediaHandle hHandle);
void FrSDPSetMediaProfile(FrMediaHandle hHandle, int nProfile);
int FrSDPGetMediaLevel(FrMediaHandle hHandle);
void FrSDPSetMediaLevel(FrMediaHandle hHandle, int nLevel);
int FrSDPGetMediaCompatibility(FrMediaHandle hHandle);
void FrSDPSetMediaCompatibility(FrMediaHandle hHandle, int nCompatibility);
DWORD32 FrSDPGetMediaVideoWidth(FrMediaHandle hHandle);
void FrSDPSetMediaVideoWidth(FrMediaHandle hHandle, DWORD32 dwWidth);
DWORD32 FrSDPGetMediaVideoHeight(FrMediaHandle hHandle);
void FrSDPSetMediaVideoHeight(FrMediaHandle hHandle, DWORD32 dwHeight);
DWORD32 FrSDPGetMediaVideoFrameRate(FrMediaHandle hHandle);
void FrSDPSetMediaVideoFrameRate(FrMediaHandle hHandle, DWORD32 dwFrameRate);
DWORD32 FrSDPGetMediaAudioChannelNum(FrMediaHandle hHandle);
void FrSDPSetMediaAudioChannelNum(FrMediaHandle hHandle, DWORD32	dwChannelNum);
void FrSDPGetMediaAACGenericInfo(FrMediaHandle hHandle, int* iMode, int* iSizeLen, int* iIndexLen, int* iIndexDeltaLen);
void FrSDPSetMediaAACGenericInfo(FrMediaHandle hHandle, int nMode, int nSizeLen, int nIndexLen, int nIndexDeltaLen);
DWORD32 FrSDPGetMediaAACObject(FrMediaHandle hHandle);

OBJECT_TYPE ConvCodecToObject(DWORD32 dwFourCC);
DWORD32 ConvObjectToCodec(OBJECT_TYPE eObject);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _SDPAPI_H_
