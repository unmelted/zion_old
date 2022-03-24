
#ifndef _CODECCONVERTERAPI_H_
#define _CODECCONVERTERAPI_H_

#include "SystemAPI.h"
#include "mediainfo.h"

#ifdef __cplusplus
extern "C"
{
#endif


/////////////////////////////////////////////////////
// Video Converter
typedef	void*	FrVideoResizeHandle;
typedef	void*	FrVideoConvHandle;

LRSLT FrVideoResizeOpen(FrVideoResizeHandle* phResizer, FrVideoInfo* hSrcInfo, FrVideoInfo* hDstInfo);
void FrVideoResizeClose(FrVideoResizeHandle hHandle);
LRSLT FrVideoResizeVideo(FrVideoResizeHandle hHandle, FrRawVideo* pSrc, FrRawVideo* pDst);


LRSLT FrVideoConvOpen(FrVideoConvHandle* phConv, FrVideoInfo* Info, int color);
void FrVideoConvClose(FrVideoConvHandle hHandle);
LRSLT FrVideoConvColor(FrVideoConvHandle hHandle, FrRawVideo* pSrc, FrRawVideo* pDst);

#ifdef __cplusplus
}
#endif



#endif	// _CODECCONVERTERAPI_H_
