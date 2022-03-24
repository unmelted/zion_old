
#ifndef _MEDIAINFO_H_
#define _MEDIAINFO_H_

#include "SystemAPI.h"
#include "mediafourcc.h"
#include "mediaerror.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions for Numbers
///////////////////////////////////////////////////////////////////////////////////////////////////
#define		MAX_CONFIG_SIZE				5120
#define		MAX_TRACKS					10
#define		MAX_FRAMES					64

///////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions for enumeration
///////////////////////////////////////////////////////////////////////////////////////////////////
// Video Pixel Format
typedef enum  {
	VideoFmtUnknown = 0,
	VideoFmtIYUV,
	VideoFmtYV12,
	VideoFmtRGB555LE,
	VideoFmtBGR555LE,
	VideoFmtRGB565LE,
	VideoFmtBGR565LE,
	VideoFmtRGB555BE,
	VideoFmtBGR555BE,
	VideoFmtRGB565BE,
	VideoFmtBGR565BE,
	VideoFmtRGBA,
	VideoFmtBGRA,
	VideoFmtABGR,
	VideoFmtARGB,
	VideoFmtRGB24,
	VideoFmtBGR24,
	VideoFmtYUV410P,
	VideoFmtYUV411P,
	VideoFmtYUV420P,
	VideoFmtYUV422P,
	VideoFmtYUV420P10LE,				// 420P 10 bps LE
	VideoFmtYUV422P10LE,				// 422P 10 bps LE
	VideoFmtNV12,						// android
	VideoFmtNV12Tiled,					// android :: MB-tiled NV12
	VideoFmtNV21,						// UV swapped
	VideoFmtQSV,						// QSV mfxFrameSurface1
} VideoOutFormat;

// URL Type
typedef	enum {
	_NONE_FILE  = 0,
	_MP4_FF_FILE  = 1,
	_MP4_FILE = 2,
	_TS_FILE = 10,
	
	_H264_FILE		= 35,
	
	_RTSP_URL  = 101,
	_HLS_URL = 107,
	
} FrURLType;

// Media Type
typedef enum {
	ALL_MEDIA		= 0,
	AUDIO_MEDIA		= 1,
	VIDEO_MEDIA		= 2,
	TEXT_MEDIA		= 3
} FrMediaType;

// Video Frame Type
typedef enum {
	FRAME_SKIP		= -1,
	FRAME_P			= 0,
	FRAME_B			= 1,
	FRAME_I			= 2,
	FRAME_UNKNOWN	= 100
} FrFrameType;

// TimeStamp Mode
typedef enum {
	TIMESTAMP_DTS	= 0,
	TIMESTAMP_PTS,
	TIMESTAMP_PTS_DTS
} FrTimeStamp;

// AAC BSType(ADTS or RAW)
typedef enum {
	MP4_TYPE	= 10,
	TS_TYPE		= 11
} FrBitStreamType;

typedef enum {
	CODEC_SW = 0,
	CODEC_SW_ESM = 1,
	CODEC_HW = 2,
	CODEC_HW_QSV = 3
} FrCodecType;

// File Format is M3U
typedef enum {
	M3U_START_TIME_NO_OP = 0,
	M3U_START_TIME_GET,
	M3U_START_TIME_SET
}	M3UStartTime;

typedef enum {
	M3U_FORMAT_NONE = 0,
	M3U_FORMAT_IS_TS,
	M3U_FORMAT_IS_MP4,
	M3U_FORMAT_IS_TS_AND_TS_H264
} eM3UFileFormat;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Common Media Structures
///////////////////////////////////////////////////////////////////////////////////////////////////
// Url Information
typedef	struct {
	FrURLType	URLType;
	CHAR*		pUrl;
	
	BOOL		bStreaming;
	DWORD32		dwInitBufferTime;
	DWORD32		dwConnTimeOut;

	// Add Timeout and customizable values following m_dwConnTimeOut - hjchoi 2007.08.14
	DWORD32		m_dwConnTimeOut;
	DWORD32		m_dwMResTimeOut;
	DWORD32		m_dwRecvTimeOut;
	DWORD32		m_dwRetryCnt;
	DWORD32		m_dwReCntCnt;
	DWORD32		m_dwRecvDelay;
	DWORD32		m_dwRecvMaxDelay;
	DWORD32		m_dwRecvMaxSize;
	DWORD32		m_dwRecvBufLeng;
	double		m_dlDownloadSafe;


	BOOL		m_bProgressive;
	BYTE* m_pMem;
	BOOL		m_bMem;
	QWORD		m_qwSize;
	DWORD32* m_pWrittenSize;
	DWORD32		m_dwStart;
	DWORD32		m_dwEnd;
	BOOL		m_bMgosp;
	BOOL		m_bMInfoSvr;
	/*DWORD32		m_dwMMSSupported;
	DWORD32		m_dwRTSPSupported;
	DWORD32		m_dwHTTPSupported;
	DWORD32		m_dwLocalSupported;*/
	BOOL		m_bLV;
	BOOL		m_bJPEGConv;
	BOOL		m_bNoSeek;
	WORD		m_wUDPStartPort;
	WORD		m_wUDPEndPort;

	QWORD		m_qwOffset;
	QWORD		m_qwFileSize;
	DWORD32		m_dwSeekCTS;

	BOOL		m_bNotPrintUrlInfo;			// Turn off PrintURLInfo at McReaderOpen()


	// Buffer File
	//ePDFileMode		m_ePDFileMode;
	eM3UFileFormat	m_eM3UFormat;		// none, ts or mp4
	BOOL			m_bM3UMemOptimize;	// for m3u relay or live server

	// PSReader reoder buffer
	BOOL			m_bReorderBuffer;	// set by 1 when Reorder buffer is on
	BOOL			m_bDisableNoData;	// set by 1 when Video is in nosync mode
	DWORD32			m_dwInitBufferTime;
	DWORD32			m_dwSeekBufferTime;
	DWORD32			m_dwBufferTime;
	BOOL			m_bLiveBufferTime;	// set by 1 when different buffer times need to be set for VoD and Live streams
	DWORD32			m_dwInitBufferTimeLive;
	DWORD32			m_dwSeekBufferTimeLive;
	DWORD32			m_dwBufferTimeLive;
	DWORD32			m_dwReOpenCount;	// How many times to try to reopen a socket for live srt, tcp etc streams

	
} FrURLInfo;


// Audio info
typedef	struct {
	// input audio info
	DWORD32		dwTrackID;
	char		szCodec[MAX_STRING_LENGTH];
	DWORD32		dwFourCC;
	DWORD32		dwSubType;						// mainly for mp3 layer info
	DWORD32		dwSampleRate;
	DWORD32		dwChannelNum;
	DWORD32		dwBitRate;
	DWORD32		dwHeaderBitRate;					// have bitrate in both Header and stream
	DWORD32		dwBitPerSample;
	DWORD32		dwBlockAlign;
	DWORD32		dwSampleNum;
	BYTE		pConfig[MAX_CONFIG_SIZE];
	DWORD32		dwConfig;
	DWORD32		dwInitialBuffering;
	DWORD32		dwMaxBuffSize;
	DWORD32		dwDuration;
	
} FrAudioInfo;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Codec Info
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	// H264 config info
	BOOL		m_bSeparateColourPlaneFlag;
	BOOL		m_bFrameMbsOnlyFlag;
	BOOL		m_bIdrPicFlag;			// Must be erived from MVC nal header extension. 0 otherwise.
	DWORD32		m_dwPicOrderCntType;	// Must be 0, 1 (probably GOP structure), or 2 (no-B frame)
	DWORD32		m_dwReorderBuffer;		// Meaningful for m_dwPicOrderCntType == 1
	BOOL		m_bBottomFieldPicOrderInFramePresentFlag;
	BOOL		m_bDeltaPicOrderAlwaysZeroFlag;
	DWORD32		m_dwFrameNumBitLength;
	DWORD32		m_dwPOCBitLength;
}	tH264DecoderInfo;

typedef struct {
	// H265 config info
	BOOL		m_bDependentSliceSegmentsEnabledFlag;
	DWORD32		m_dwNumExtraSliceHeaderBits;
	BOOL		m_bOutputFlagPresentFlag;
	BOOL		m_bSeparateColourPlaneFlag;
	DWORD32		m_dwPOCBitLength;
	DWORD32		m_dwPicOrderCntType;	// 2 (no-B frame)
}	tH265DecoderInfo;

typedef union {
	tH264DecoderInfo	H264;
	tH265DecoderInfo	H265;
}	uDecoderInfo;

// Video info
typedef	struct {
	// input video info
	DWORD32		dwTrackID;
	char		szCodec[MAX_STRING_LENGTH];
	DWORD32		dwFourCC;
	FrCodecType	eCodecType;
	
	DWORD32		dwWidth;
	DWORD32		dwHeight;
	DWORD32		dwFrameRate;
	DWORD32		dwBitRate;
	DWORD32		dwBitCount;
	DWORD32		dwDuration;
	DWORD32		dwGopLength;
	
	BYTE		pConfig[MAX_CONFIG_SIZE];
	DWORD32		dwConfig;
	
	DWORD32		dwInitialBuffering;		//3
	DWORD32		dwMaxBuffSize;
	
	VideoOutFormat	eColorFormat;			// Color format from config if exists

	BOOL		bLowDelayDecoding;		// ffmpeg video low delay decoding

	// Vpd option
	UINT32 gopIdx;
	USHORT gopPos;
	UINT64 timestamp;
	UINT32 frameNum;
	UINT32 camCh;
	UINT32 postSdIP;
	UINT32 recordFrameNo;
	
	// encoding
	INT32		nProfile;
	INT32		nLevel;
	INT32		nCompatibility;
	DWORD32		dwNumPasses;
	float		fIntraFrameRefresh;
	DWORD32		dwBFrame;

	// CUDA
	void*		pCtx;
	int			memDir;	// host or device

	INT32		nPreset;				// H.264:Preset (default:faster), WMV:Complexity
	INT32		nTune;				// H.264:Tune (default:film)
	INT32		nProfileType;			// H.264:Profile (default:baseline)
	DWORD32		dwVidEncThread;		// Encoding thred mode
	DWORD32		dwRCMode;				// CQP, CRF or ABR. default (ABR)
	DWORD32		dwQualityFactor;		// QFacter * 1000. meaningful when rc mode is CQP or CRF.
	FrBitStreamType BSType;
	BOOL		bSpeedOptimization;	// Speed or Delay optimized. default (DelayOptimization)
	BOOL        bAdaptive;            // equal key max value to min value, set no-scenecut

	// Decoder info for reordering frames for IOS
	uDecoderInfo	m_DecoderInfo;
} FrVideoInfo;


#define MAX_EXT_LENGTH	10

// Media info for multimedia contents
typedef	struct {
	char		szFileFormat[MAX_EXT_LENGTH];		// for file extension for NHN

	DWORD32		dwTotalRange;
	DWORD32		dwAudioNum;						// Recommanded Audio Index in m_Audio by GetInfo
	DWORD32		dwVideoNum;						// Recommanded Video Index in m_Video by GetInfo
	
	FrAudioInfo	FrAudio;
	FrVideoInfo FrVideo;

	DWORD32		dwAudioTotal;
	DWORD32		dwVideoTotal;
	DWORD32		dwTextTotal;

	FrURLType	URLType;
	DWORD32		dwBufferTime;

	// for hls
	DWORD32     dwSessionTimeOut;
	DWORD32     dwTsDuration;
	DWORD32		dwMinDuration;
	DWORD32		dwStartTime;
	DWORD32		dwEndTime;

	M3UStartTime	eM3UStartTime;				// get or set start time to make time stamp continuous between m3u segments
	DWORD32			dwM3UStartTime;				// start time for m3u segments. That must be a starting time stamp in the 1st segment	eM3UFileFormat	m_eM3UFormat;
	eM3UFileFormat	eM3UFormat;					// none, ts or mp4
	DWORD32		dwTsPacketSize;
} FrMediaInfo;

// Media bitstream data
typedef	struct {
	FrMediaType	tMediaType;					// audio, video and text.
	BYTE*		pFrame;						// frame pointer.
	DWORD32		dwFrameNum;					// number of frames in elementary stream.
	DWORD32		dwFrameLen[MAX_FRAMES];		// length of each frames in elementary stream.
	FrFrameType	tFrameType[MAX_FRAMES];		// I, P, B
	DWORD32		dwCTS;						// media timestamp.

	// add hls..
	BOOL		bDiscontinuity;
	FrVideoInfo* pVideoInfoVideoConfig;	// Pointer to Video config length when not Null
	BOOL		bVideoFrameDrop;				// Video Frame Drop from Reader
	
} FrMediaStream;

// Aidio pcm data
typedef	struct {
	BYTE*		pPCM;
	DWORD32		dwPCMLength;
	DWORD32		dwCTS;
	//ePCMFormat	m_eFormat;
	DWORD32		dwBitWidth;
	//DOUBLE		dbTime;
} FrRawAudio;

// Video raw data
typedef	struct {
	BYTE*		pY;
	BYTE*		pU;
	BYTE*		pV;
	int			nPhyAddr;
	DWORD32		dwPitch;
	DWORD32		dwCTS;
	FrFrameType	tFrameType;		// set as FRAME_I to request I frame encoding
	VideoOutFormat eColorFormat;
	DWORD32		dwSrcCnt;

	DWORD32		dwDecodedWidth;
	DWORD32		dwDecodedHeight;

	// GPU
	BYTE** ppDecoded;
	int	nDecoded;
	INT64* pDecodedUids;
} FrRawVideo;


#endif	// _MEDIAINFO_H_
