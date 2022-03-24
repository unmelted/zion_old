
#ifndef _STREAMWRITERAPI_H_
#define _STREAMWRITERAPI_H_

#include "SystemAPI.h"
#include "mediainfo.h"

typedef	void*	FrWriterHandle;

#ifdef __cplusplus
extern "C"
{
#endif

LRSLT FrWriterOpen(FrWriterHandle* phWriter, FrURLInfo *hUrlInfo);
void FrWriterClose(FrWriterHandle hWriter);
LRSLT FrWriterSetInfo(FrWriterHandle hWriter, FrMediaInfo* Info, DWORD32 dwStartCTS);
LRSLT FrWriterUpdateInfo(FrWriterHandle hWriter);
LRSLT FrWriterWriteFrame(FrWriterHandle hWriter, FrMediaStream* pMedia);
LRSLT FrWriterUpdateData(FrWriterHandle hWriter, FrMediaStream* pMedia);
//LRSLT McWriterGetHeaderInfo(McWriterHandle hWriter, McMediaData* pMedia);
//LRSLT McWriterClearBuffer(McWriterHandle hWriter, DWORD32 dwVideoID, DWORD32 dwAudioID);
//LRSLT McWriterEstimateMoov(McWriterHandle hWriter, McMediaData* pMedia, DWORD32 dwTotalVideoFrame, DWORD32 dwTotalAudioSample, DWORD32 dwIDRCount, DWORD32 dwVideoConfigSize);
//LRSLT McWriterWriteEstimateMoov(McWriterHandle hWriter);
//LRSLT McWriterGetSeekTime(McWriterHandle hWriter, DWORD32* dwOffset, DWORD32* dwFrameType, DWORD32* dwTime, DWORD32* dwAudioNum, DWORD32* dwVideoNum, DWORD32* dwNextIFrame);
//LRSLT McWriterGetFileSize(McWriterHandle hWriter);

#ifdef __cplusplus
}
#endif

#endif	// _STREAMWRITERAPI_H_
