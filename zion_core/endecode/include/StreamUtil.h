/*****************************************************************************
*                                                                            *
*                            StreamReader Library							 *
*                                                                            *
*   Copyright (c) 2004 by Mcubeworks, Incoporated. All Rights Reserved.      *
******************************************************************************

    File Name       : StreamUtil.h
    Author(s)       : Kim, Kun-Tae
    Created         : 11 Jan 2005

    Description     : File API
    Notes           : 

==============================================================================
                      Modification History (Reverse Order)                    
==============================================================================
    Date        Author      Location (variables) / Description
------------------------------------------------------------------------------

------------------------------------------------------------------------------*/

#ifndef	_STREAMUTIL_H_
#define	_STREAMUTIL_H_

#include "SystemAPI.h"
#include "TraceAPI.h"
#include "mediainfo.h"

// Xing
#define FRAMES_FLAG				0x0001
#define BYTES_FLAG				0x0002
#define TOC_FLAG				0x0004
#define VBR_SCALE_FLAG			0x0008
#define NUMTOCENTRIES			100
#define MAX_NUMTOCENTRIES		400			// must be bigger than NUMTOCENTRIES

#define	IS_3B_START_CODE(a)	( (a[0]==0) && (a[1]==0) && (a[2]==1) )
#define	IS_4B_START_CODE(a)	( (a[0]==0) && (a[1]==0) && (a[2]==0) && a[3]==1)

#define	Copy4BytesFromDWORD32(a, b)			(a)[0] = (BYTE)(((b)&0xff000000)>>24); \
											(a)[1] = (BYTE)(((b)&0x00ff0000)>>16); \
											(a)[2] = (BYTE)(((b)&0x0000ff00)>>8); \
											(a)[3] = (BYTE)(((b)&0x000000ff))

#define Copy2BytesFromDWORD32(a, b)			(a)[0] = (BYTE)(((b)&0xff00)>>8); \
											(a)[1] = (BYTE)(((b)&0x00ff))

#define Copy1BytesFromDWORD32(a, b)			(a)[0] = (BYTE)(((b)&0x00ff))


#define Copy6ByteTFromQWORD(a)				((QWORD)((a)[0] << 56) | ((a)[1] << 48) | ((a)[2] << 40) | ((a)[3] << 32)) \
												((a)[4] << 24) | ((a)[5] << 16) | ((a)[6] <<  8) | ((a)[7]))


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

typedef enum {
	H265_NAL_NONE		= 0,
	H265_NAL_BLA_W_LP   = 16,
	H265_NAL_BLA_W_RADL,
	H265_NAL_BLA_N_LP,

	H265_NAL_IDR_W_RADL = 19,
	H265_NAL_IDR_N_LP,
	H265_NAL_IDR_CRA,
	H265_NAL_IRAP_VCL22,							// IRAP
	H265_NAL_IRAP_VCL23,
	H265_NAL_VPS   = 32,
	H265_NAL_SPS,
	H265_NAL_PPS,
	H265_NAL_AUD,
} eH265NalType;

typedef struct
{
	INT32		h_id;						// from MPEG header, 0=MPEG2, 1=MPEG1
	INT32		samprate;					// determined from MPEG header
	INT32		flags;						// from Vbr header data
	INT32		frames;						// total bit stream frames from Vbr header data
	INT32		bytes;						// total bit stream bytes from Vbr header data
	INT32		vbr_scale;					// encoded vbr scale from Vbr header data
	INT32		toc_number;					// # of items in toc tables
	DWORD32		toc[MAX_NUMTOCENTRIES];		// may be NULL if toc not desired
	INT32		headersize;					// size of VBR header, in bytes
	INT32		frames_per_entry;			// frames per Toc entry
	INT32		is_xing;					// [0 : cbr] [1 : xing] [2 : VBRI]
	DOUBLE		frame_duation;				// # of frame in msec
} VBRTAGDATA;

//////////////////////////////////////////
// Stream Format
//////////////////////////////////////////
typedef struct {
	DWORD32	biSize; 
	INT32	biWidth; 
	INT32	biHeight; 
	WORD	biPlanes; 
	WORD	biBitCount; 
	DWORD32	biCompression; 
	DWORD32	biSizeImage; 
	INT32	biXPelsPerMeter; 
	INT32	biYPelsPerMeter; 
	DWORD32	biClrUsed; 
	DWORD32	biClrImportant; 
} VideoFormat;

#define	SIZE_OF_AudioFormat	18
typedef struct {
	WORD	wFormatTag;
	WORD	nChannels;
	DWORD32	nSamplesPerSec;
	DWORD32	nAvgBytesPerSec;
	WORD	nBlockAlign;
	WORD	wBitsPerSample;
	WORD	cbSize;
} AudioFormat;

BOOL GetXingFrame(BYTE* pBuffer, DWORD32 dwBuffLen, VBRTAGDATA *pTagData);
INT32 GetSeekpoint(VBRTAGDATA *pTagData, DWORD32 dwCTS, DWORD32 dwDuration);

void GetAudioFormat(BYTE* pFormat, DWORD32 dwLen, FrAudioInfo* hInfo);
void GetVideoFormat(BYTE* pFormat, DWORD32 dwLen, FrVideoInfo* hInfo);
DWORD32 get_hevc_config_size(BYTE *pData_i, DWORD32 size);
BOOL check_video_config_needed(DWORD32 dwFourCC, FrVideoInfo *hVideo);
BOOL make_video_config(FrVideoInfo *hVideo, DWORD32 dwData, BYTE *pData);

DWORD32 MakeVideoPSInfo(BYTE* pSrc, DWORD32 dwSrcLen, BYTE* pDst);

BOOL check_h264_is_rtp(BYTE *pData, DWORD32 size);
BOOL check_h264_config_only(BYTE *pData, DWORD32 size);
DWORD32 make_h264stream_rtp_format(BYTE *pOut, DWORD32 out_buff_size, BYTE *pInData, DWORD32 in_size);
DWORD32 make_h264stream_rtp_format_from_ts_format(BYTE *pOutData, DWORD32 out_buff_size, BYTE *pInData, DWORD32 in_size);
DWORD32 make_h265stream_rtp_format_from_ts_format(BYTE *pOutData, DWORD32 out_buff_size, BYTE *pInData, DWORD32 in_size);
DWORD32 convert_startcode2nalsize_in_h264stream(BYTE *pInData, DWORD32 in_size);
DWORD32 convert_nalsize2startcode_in_h264stream(BYTE *pInData, DWORD32 in_size);
BOOL get_h264_picture_type(BYTE* pData, DWORD32 dwSize);
BOOL get_h264_picture_type_nallenghttype(BYTE* pData, DWORD32 dwSize, DWORD32 dwNalLengthType);
BOOL is_a_h264_frame_intra(BYTE* pFrame, INT32 nFrameLen);
DWORD32 check_h264_config(BYTE *pConfig, DWORD32 dwConfig);
DWORD32 change_H264_config_from_mp4(BYTE* pBuffer, DWORD32 dwBuffer);
DWORD32 make_h264_config_data_from_skt(BYTE* pDst, DWORD32 dwDst, BYTE* pSrc, DWORD32 dwSrc);
DWORD32	make_h264_avcC_config_from_2B_nalsize(BYTE *pConfig, DWORD32 dwConfig);
DWORD32	make_h264_avcC_config_to_annexB(BYTE *pConfig, DWORD32 dwConfig);
//BOOL get_H264_frameinfo(BYTE* pData, DWORD32 dwData, FrFrameType *pFrameType, DWORD32 *pPOC, tH264DecoderInfo *hStreamInfo);
//BOOL get_H265_frameinfo(BYTE* pData, DWORD32 dwData, McFrameType *pFrameType, DWORD32 *pPOC, tH265DecoderInfo *hStreamInfo);

BOOL get_wvc1_config_data(FrVideoInfo* hInfo, BYTE *pData_o, DWORD32 size);

// JPEG
BOOL GetJpegImageSize(BYTE* pIn, DWORD32 dwSize, DWORD32 *pdwWidth, DWORD32 *pdwHeight);
BOOL GetH263PicType(BYTE* pBuffer, DWORD32 dwLen);
FrFrameType get_video_frame_type(BYTE* pBuffer, DWORD32 dwBuffer, DWORD32 dwFourCC);

DWORD32	 GetAMRFrameSize(BYTE *pFrame, DWORD32 dwFrame);

// VorBis
// VorBis Config Process
typedef struct VorbisParseContext {
    INT32	nExtradataParsed;		// we have attempted to parse extradata
    INT32	nValidExtradata;        // extradata is valid, so we can calculate duration
    INT32	nBlocksize[2];          // short and long window sizes
    INT32	nPreviousBlocksize;     // previous window size
    INT32	nModeBlocksize[64];     // window size mapping for each mode
    INT32	nModeCount;             // number of modes
    INT32	nModeMask;              // bitmask used to get the mode in each packet
    INT32	nPrevMask;              // bitmask used to get the previous mode flag in each packet
} VorbisParseStruct;

LRSLT VorbisParseSetupHeader(VorbisParseStruct *hVorbisParser, const BYTE *pBuf, INT32 nBufSize);
LRSLT VorbisParseIdHeader(VorbisParseStruct *hVorbisParser, const BYTE *pBuf, INT32 nBufSize);
INT32 GetVorbisFrameDuration(VorbisParseStruct *hVorbisParser, const BYTE *pBuf, INT32 nBufSize);

// AAC
DWORD32 MakeAACConfig(BYTE *pConfig, DWORD32 dwAudioObect, DWORD32 dwSampleRate, DWORD32 dwChannelNum);
BOOL CheckNullAudioStream(BYTE*	pBuffer, DWORD32 *dwBuffer);

// Opus
DWORD32 MakeOpusConfig(BYTE *pConfig, DWORD32 dwSampleRate, DWORD32 dwChannelNum);
DWORD32 MakeOpusConfigForTS(BYTE *pConfig, DWORD32 dwSampleRate, DWORD32 dwChannelNum);

void PrintFourCC(DWORD32 dwFourCC);

/*	BYTE			general_profile_space;
	BYTE			general_tier_flag;
	BYTE			general_profile_idc;
	UINT32			general_profile_compatibility_flags;
	UINT64			general_constraint_indicator_flags;
	BYTE			general_level_idc;
	USHORT			min_spatial_segmentation_idc;
	BYTE			parallelismType;
	BYTE			chromaFormat;
	BYTE			bitDepthLumaMinus8;
	BYTE			bitDepthChromaMinus8;
	USHORT			avgFrameRate;
	BYTE			constantFrameRate;
	BYTE			numTemporalLayers;
	BYTE			temporalIdNested;
	BYTE			lengthSizeMinusOne;
	BYTE			numOfArrays; */

typedef struct
{
	USHORT			m_sLength;
	BYTE*			m_pNal;
} paraSetStruct;

typedef	struct 
{
	BYTE			m_bConfigurationVersion;
	BYTE			m_bGeneralProfileSpace;					//  0 by default
	BYTE			m_bGeneralTierFlag;						//  0 by default
	BYTE			m_bGeneralProfileIdc;					//  1 by default
	UINT32			m_uGeneralProfileCompatibilityFlags;	// 
	UINT64			m_u64GeneralConstraintsIndicatorFlags;	// 
	BYTE			m_bGeneralLevelIdc;						// 93 by default
	USHORT			m_sMinSpatialSegmentationIdc;
	BYTE			m_bParallelismType;
	BYTE			m_bChromaFormat;
	BYTE			m_bBitDepthLumaMinus8;
	BYTE			m_bBitDepthChromaMinus8;
	USHORT			m_sAvgFrameRate;
	BYTE			m_bConstantFrameRate;
	BYTE			m_bNumTemporalLayers;
	BYTE			m_bTemporalIdNested;
	BYTE			m_bLengthSizeMinusOne;
	BYTE			m_bNumOfArrays;			
	DWORD32			m_dwSpropMaxDonDiff;					//  0 by default
	DWORD32			m_dwSpropVps;
	DWORD32			m_dwSpropSps;
	DWORD32			m_dwSpropPps;
	paraSetStruct*	m_pVpsPara;
	paraSetStruct*	m_pSpsPara;
	paraSetStruct*	m_pPpsPara;
} hvcCStruct;

DWORD32 insert_startcode_emulation(BYTE *pData_o, BYTE *pData_i, DWORD32 dwData);
DWORD32	remove_startcode_emulation(BYTE* pOutData, BYTE *pInData, DWORD32 dwData);
DWORD32 make_hevc_annexB_config(BYTE *pConfig, DWORD32 dwConfig, hvcCStruct *hvcc);
DWORD32 make_hvcC_config(BYTE *pConfig, DWORD32 dwConfig, hvcCStruct *hvcc);
DWORD32 make_hevc_annexB_config_from_hvcC(BYTE *pConfig_o, BYTE *pConfig_i, DWORD32 dwConfig);
LRSLT make_hevc_config_data(FrVideoInfo* hInfo, BYTE *pData_o, DWORD32 size);
DWORD32 make_hevc_config_data2(BYTE *pData_o, BYTE *pData_i, DWORD32 size, BOOL *pbAnnexB);
DWORD32 make_annexB_hevc_config_data_from_annexB(BYTE *pData_o, BYTE *pData_i, DWORD32 size);
INT32 GetH265IntraFrame(DWORD32 dwNalType);
BOOL is_a_h265_frame_intra(BYTE* pFrame, int nFrameLen);
DWORD32 MakeH265SDPFormat(CHAR* pszFormat, DWORD32 dwFormat, BYTE *pConfig, DWORD32 dwCfg);
DWORD32 MakeH265DashCodecsFromHvcc(CHAR* pszFormat_o, DWORD32 dwFormat_o, BYTE *pConfig, DWORD32 dwCfg);
BOOL GetHEVCDecInfoFromConfig(BYTE *pConfig, DWORD32 dwCfg, BYTE* pVPS, DWORD32 *pdwVPS, BYTE* pSPS, DWORD32 *pdwSPS, BYTE* pPPS, DWORD32 *pdwPPS);
BOOL make_h265_annexB_config_from_rtp(BYTE *pConfig, DWORD32 *pdwConfig, BYTE *pData, DWORD32 dwData);

LRSLT make_h264_config_data(FrVideoInfo* hInfo, BYTE *pData, DWORD32 size);
DWORD32 convert_startcode2nalsize_in_h264stream(BYTE *pInData, DWORD32 in_size);
DWORD32 convert_nalsize2startcode_in_h264stream(BYTE *pInData, DWORD32 in_size);
BOOL get_h264_picture_type(BYTE* pData, DWORD32 dwSize);

// for FLAC meta removal 
LRSLT RemoveTagInfoFromFLACFile(TCHAR* pszSrcFileName, TCHAR* pszDstFileName, BOOL bFlag);

// VP9 SuperFrame
typedef struct {
	BYTE*			m_pFrame;
	DWORD32			m_dwFrame;
} vp9frameStruct;

typedef struct {
	DWORD32			m_dwFrameNum;
	DWORD32			m_dwCurFrame;
	DWORD32			m_dwCTS;
	DWORD32			m_dwDTS;
	FrTimeStamp		m_eTimeStamp;
	vp9frameStruct	m_Frame[MAX_FRAMES];
} vp9superframeStruct;

BOOL  parse_vp9_superframe(vp9superframeStruct *hSuperframe, BYTE* pBuffer, DWORD32 dwBuffer);
BOOL traceH2645NAL(BYTE *pData, DWORD32 dwData);
DWORD32 check_h2645_rtp_nal_stream(BYTE *pFrame, DWORD32 dwFrame);

// opus
DWORD32 remove_opus_control(FrMediaStream *pData, BYTE *pFrame, DWORD32 dwFrame);
DWORD32	add_opus_control_header(BYTE *pBuffer, BYTE *pOpusFrame, DWORD32 dwOpusFrame);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _STREAMUTIL_H_
