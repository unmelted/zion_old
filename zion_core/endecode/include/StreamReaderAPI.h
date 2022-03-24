/*****************************************************************************

    File Name       :
    Author(s)       :
    Created         :

    Description     :
    Notes           :

==============================================================================
                      Modification History (Reverse Order)
==============================================================================
    Date        Author      Location (variables) / Description
------------------------------------------------------------------------------

------------------------------------------------------------------------------*/

#ifndef _STREAMREADERAPI_H_
#define _STREAMREADERAPI_H_

#include "SystemAPI.h"
#include "mediainfo.h"

#define		READ_INIT				0
#define		READ_STOP				1
#define		READ_PLAY				2
#define		READ_PAUSE				3
#define		READ_BUFFERING			4
#define     READ_SEEK				5			// Nothing to do
#define     READ_STEP				6
#define		READ_SCALE				7
#define		READ_INFO				8			// Http download : inform of current file size

#define		READ_MODE_NORMAL		100			// Back to noraml read
#define		READ_MODE_INTRA			101			// Read Intra Only
#define		READ_MODE_INTRA_END		102			// Read Intra and End of stream, meaningful only for backward and end

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

typedef	enum {
	READER_BUFFERING_INIT = 0,
	READER_BUFFERING_SEEK,
	READER_BUFFERING_NODATA
} ReaderBufferingMode;

typedef	struct {
	QWORD	m_qwFileSize;
	QWORD	m_qwCurSize;

	DWORD32	m_dwAudioBuffRatio;
	DWORD32	m_dwVideoBuffRatio;
	DWORD32	m_dwBufferingRatio;
	DWORD32	m_dwCurFSizeRatio;
	DWORD32	m_dwBufferedTime;		// for progressive download in sec

	DWORD32	m_dwAudioBufferedTime;	// Audio Buffered Time for streaming source
	DWORD32	m_dwVideoBufferedTime;	// Video Buffered Time for streaming source


	LRSLT	m_lError;
	ReaderBufferingMode	m_eMode;
} FrReaderStatus;

typedef struct {
	DWORD32				m_dwBufferBaseAudio;
	DWORD32				m_dwBufferBaseVideo;
} ReaderStatusInfo;

typedef	void*	FrReaderHandle;

LRSLT FrReaderOpen(FrReaderHandle* phReader, FrURLInfo* pURL);
void FrReaderClose(FrReaderHandle hReader);
LRSLT FrReaderGetInfo(FrReaderHandle hReader, FrMediaInfo* hInfo);
LRSLT FrReaderStart(FrReaderHandle hReader, DWORD32* pdwStart);
LRSLT FrReaderReadFrame(FrReaderHandle hReader, FrMediaStream* pMedia);
BOOL FrReaderGetStatus(FrReaderHandle hReader, FrReaderStatus* pStatus, ReaderStatusInfo* hInfo);
LRSLT FrReaderSetStatus(FrReaderHandle hReader, INT32 nStatus, INT32 nScale);

//BOOL McReaderSeekByTime(McReaderHandle hReader, DWORD32* dwCTS, BOOL bKeyFrame);
//BOOL McReaderSeekByKeyFrame(McReaderHandle hReader, DWORD32* dwCTS, BOOL bNext);
//BOOL McReaderSeekByMedia(McReaderHandle hReader, DWORD32* pdwCTS, McMediaType eType);
//BOOL McReaderSeekBySubtitle(McReaderHandle hReader, DWORD32* dwCTS, BOOL bNext);
//LRSLT McReaderSeekByClock(McReaderHandle hReader, DWORD32 dwDate, DWORD32 dwTime);
//BOOL McReaderSetTrackID(McReaderHandle hReader, DWORD32 dwTrackID, McMediaType eType);
//BOOL McReaderOpenSubtitle(McReaderHandle hReader, TCHAR* pFilename, McMediaInfo* hInfo);
//LRSLT McReaderGetCurTime(McReaderHandle hReader, DWORD32* pdwDate, DWORD32* pdwTime);
//BOOL McReaderSetSkip(McReaderHandle hReader, DWORD32 dwStart, DWORD32 dwEnd, BOOL bSkip);
//BOOL McReaderSetTimeDelta(McReaderHandle hReader, INT32 iTimeDelta, INT32 iMediaType);
//LRSLT McReaderReadFile(McReaderHandle hReader, BYTE* pData, DWORD32* dwData);
//BOOL McReaderSeekFile(McReaderHandle hReader, QWORD qwPos);
//LRSLT McReaderGetInfoFile(TCHAR* pURL, McMediaInfo* hInfo);
//BOOL McReaderSeekByVideo(McReaderHandle hReader, DWORD32* pdwCTS);
//LRSLT McReaderGetInfoFileEx(McURLInfo URL, McMediaInfo* hInfo);
//BOOL McReaderGetJpegImageSize(BYTE* pIn, DWORD32 dwSize, DWORD32 *pdwWidth, DWORD32 *pdwHeight);
//BOOL McReaderSetFeedback(McReaderHandle hReader, WORD wFbType, BOOL bState);	// nhn on-air
//BOOL McReaderGetFeedback(McReaderHandle hReader, EVENT_HANDLE *phAEvent, EVENT_HANDLE *phVEvent);
//void McReaderGetInnerHandle(McReaderHandle hReader, void **inner);
//void McReaderSetFactor(McReaderHandle hReader, float fa, float fb);				// for etri
//INT32	McReaderGetBitrate(McReaderHandle hReader);
//BOOL McReaderSetDFSReadSize(McReaderHandle pReader, DWORD32 dwAudioReadSize, DWORD32 dwVideoReadSize);
//BOOL McReaderIsLocalFile(McReaderHandle pReader); 
//BOOL McReaderIsLive(McReaderHandle pReader);
//BOOL McReaderSetReadMode(McReaderHandle pReader, BOOL bReadIntra, BOOL bEnd, INT32 nReadSpeed);
//BOOL McReaderGetReadMode(McReaderHandle pReader, BOOL *pbReadIntra);
//BOOL McReaderSetStartReadMode(McReaderHandle pReader, BOOL bReadIntra, INT32 nReadSpeed);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _STREAMREADERAPI_H_
