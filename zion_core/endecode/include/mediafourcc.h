#ifndef _MEDIAFOURCC_H_
#define _MEDIAFOURCC_H_

#include "SystemAPI.h"
#include "UtilAPI.h"

/////////////////////////////////////
// Video FourCC Type
/////////////////////////////////////
#ifdef	WIN32
#define FOURCC_RGB			BI_RGB
#else
#define FOURCC_RGB			0
#endif
#define FOURCC_NO_FORMAT		FOURCC('N','F','M','T')		// Not defined color format

#define	AUDIOFOURCC(a)			((a) & (0xFFFF))			// Audio FourCC - 2 Bytes

#define	FOURCC_VOID				FOURCC('V','O','I','D')		// Unsupported codec
#define FOURCC_H263				FOURCC('H','2','6','3')		// H263
#define FOURCC_S263				FOURCC('s','2','6','3')		// H263
#define FOURCC_H264				FOURCC('H','2','6','4')		// H264 by FFMPEG
#define FOURCC_h264				FOURCC('h','2','6','4')		// H264 by FFMPEG
#define FOURCC_H265				FOURCC('H','2','6','5')		// H265
#define	FOURCC_HEV1				FOURCC('H','E','V','1')		// H265
#define FOURCC_AVC1				FOURCC('A','V','C','1')		// H264 by McubeWorks
#define FOURCC_avc1				FOURCC('a','v','c','1')		// H264 by McubeWorks  // hoperok 2007.11.02
#define FOURCC_X264				FOURCC('X','2','6','4')		// FFMPEG
#define FOURCC_HEVC				FOURCC('H','E','V','C')		// HEVC
#define FOURCC_CUDA				FOURCC('C','U','D','A')		// CUDA inner fourcc
#define FOURCC_INTEL			FOURCC('I','Q','S','V')		// Intel Media SDK

#define FOURCC_MP43				FOURCC('M','P','4','3')		// msmp4v3
#define FOURCC_DIV3				FOURCC('D','I','V','3')		// msmp4v3
#define FOURCC_div3				FOURCC('d','i','v','3')		// msmp4v3
#define FOURCC_DIV4				FOURCC('D','I','V','4')		// msmp4v3
#define FOURCC_DIV5				FOURCC('D','I','V','5')		// msmp4v3
#define FOURCC_DIV6				FOURCC('D','I','V','6')		// msmp4v3
#define	FOURCC_AP41				FOURCC('A','P','4','1')		// msmp4v3
#define FOURCC_MP42				FOURCC('M','P','4','2')		// msmp4v2
#define FOURCC_DIV2				FOURCC('D','I','V','2')		// msmp4v2
#define FOURCC_MP41				FOURCC('M','P','4','1')		// msmp4v1
#define FOURCC_DIV1				FOURCC('D','I','V','1')		// msmp4v2
#define FOURCC_DIVX				FOURCC('D','I','V','X')		// mp4v  -- MPEG4
#define FOURCC_divx				FOURCC('d','i','v','x')		// mp4v
#define FOURCC_DIVXFF			FOURCC('D','I','F','F')		// mp4v  -- MPEG4 : FF
#define FOURCC_DVX3				FOURCC('D','V','X','3')		// mp4v
#define FOURCC_DX50				FOURCC('D','X','5','0')		// mp4v
#define FOURCC_MP4S				FOURCC('M','P','4','S')		// mp4v
#define FOURCC_MP4V				FOURCC('M','P','4','V')		// mp4v
#define	FOURCC_MP4F				FOURCC('M','P','4','F')		// mp4v without decoder init info
#define	FOURCC_MP4VFF			FOURCC('M','4','F','F')		// FFMPEG : MPEG4 Video
#define FOURCC_M4S2				FOURCC('M','4','S','2')		// mp4v
#define FOURCC_m4s2				FOURCC('m','4','s','2')		// mp4v
#define FOURCC_XVID				FOURCC('X','V','I','D')		// mp4v
#define FOURCC_XVIDFF			FOURCC('X','V','F','F')		// ffmpeg xvid decoder
#define FOURCC_MPEG				FOURCC('M','P','E','G')		// FFMPEG
#define	FOURCC_MPG1				FOURCC('M','P','G','1')		// FFMPEG : MPEG1
#define	FOURCC_MPG2				FOURCC('M','P','G','2')		// FFMPEG : MPEG2
#define	FOURCC_MPG4				FOURCC('M','P','G','4')		// FFMPEG : MSMPEG4v1
#define FOURCC_WMV1				FOURCC('W','M','V','1')		// wmv
#define FOURCC_WMV2				FOURCC('W','M','V','2')		// wmv
#define FOURCC_WMV3				FOURCC('W','M','V','3')		// wmv
#define FOURCC_WMV3F			FOURCC('W','V','3','F')		// wmv3 : FFmpeg
#define	FOURCC_WMVA				FOURCC('W','M','V','A')		// wmva
#define	FOURCC_WVC1				FOURCC('W','V','C','1')		// vc-1
#define	FOURCC_WVP2				FOURCC('W','V','P','2')		// vc-1 image
#define FOURCC_MSS1				FOURCC('M','S','S','1')		// Windows Screen Video 7
#define FOURCC_MSS2				FOURCC('M','S','S','2')		// Windoes Screen Video 9
#define FOURCC_FLV1				FOURCC('F','L','V','1')
#define FOURCC_FLV4				FOURCC('F','L','V','4')		// On VP6F
#define	FOURCC_VP60				FOURCC('V','P','6','0')		// On VP6
#define	FOURCC_VP61				FOURCC('V','P','6','1')
#define	FOURCC_VP62				FOURCC('V','P','6','2')
#define FOURCC_VP8				FOURCC('V','P','8',' ')		// On VP8
#define FOURCC_VP9				FOURCC('V','P','9',' ')		// On VP9
#define FOURCC_AV1				FOURCC('A','V','1',' ')		// OMA AV1
#define	FOURCC_SVC1				FOURCC('S','V','C','1')		// SVC
#define	FOURCC_MSS2				FOURCC('M','S','S','2')		// Aiden MSS 2
#define FOURCC_DVSD				FOURCC('C','D','V','C')		// CDVC (Canopus DV Codec) FOURCC('D','V','S','D')
#define	FOURCC_RLE8				FOURCC('R','L','E','8')		// RunLength encoding 8
#define	FOURCC_RV20				FOURCC('R','V','2','0')		// RealVideo G2
#define	FOURCC_RV30				FOURCC('R','V','3','0')		// RealVideo 8
#define FOURCC_RV40				FOURCC('R','V','4','0')		// RealVideo 9
#define	FOURCC_RV10				FOURCC('R','V','1','0')		// RealVideo 10

#define FOURCC_RV4F				FOURCC('R','V','4','F')		// RealVideo 9		// FFMPEG AVI
#define	FOURCC_RV1F				FOURCC('R','V','1','F')		// RealVideo 10		// FFMPEG AVI

#define	FOURCC_SVQ1				FOURCC('S','V','Q','1')
#define	FOURCC_SVQ3				FOURCC('S','V','Q','3')
#define	FOURCC_WVC1				FOURCC('W','V','C','1')
#define	FOURCC_CRAM				FOURCC('C','R','A','M')		// Microsoft Video 1 (CRAM)
#define	FOURCC_TSCC				FOURCC('T','S','C','C')		// TechSmith Screen Capture Codec
#define	FOURCC_CVID				FOURCC('C','V','I','D')		// Cinepak
#define	FOURCC_CSCD				FOURCC('C','S','C','D')		// CamStudio
#define	FOURCC_IV30				FOURCC('I','V','3','0')		// Indeo video 3.0
#define	FOURCC_IV40				FOURCC('I','V','4','0')		// Indeo video 4.0
#define	FOURCC_IV50				FOURCC('I','V','5','0')		// Indeo video 5.0
#define	FOURCC_QTRLE			FOURCC('r','l','e',' ')		// QuickTime RunLength
#define	FOURCC_APCN				FOURCC('A','P','C','N')		// PNG
#define	FOURCC_SP54				FOURCC('S','P','5','4')		// Sunplus JPEG
#define	FOURCC_FPS1				FOURCC('F','P','S','1')		// Lossless Fraps
#define	FOURCC_RPZA				FOURCC('r','p','z','a')		// Apple video rpza
#define	FOURCC_VCO1				FOURCC('V','C','O','1')		// NHN on-air
#define	FOURCC_HFYU				FOURCC('H','F','Y','U')		// HUFF YUV
#define	FOURCC_FLASHVIDEO1		FOURCC('F','S','V','1')		// Flash screen video 1
#define	FOURCC_FLASHVIDEO2		FOURCC('F','S','V','2')		// Flash screen video 2
#define	FOURCC_UTVIDEO			FOURCC('U','L','R','G')		// UtVideo
#define	FOURCC_OMX264			FOURCC('O','M','X','A')		// OMX codec
#define	FOURCC_OMXMP4V			FOURCC('O','M','X','V')		// OMX codec
#define	FOURCC_OMX265			FOURCC('O','M','X','H')		// OMX codec
#define	FOURCC_IMX264			FOURCC('I','M','X','A')		// IMX codec




/////////////////////////////////////
// Image FourCC Type
/////////////////////////////////////
#define	FOURCC_MJPB				FOURCC('M','J','P','B')		// MJPEG B
#define	FOURCC_JPEG				FOURCC('J','P','E','G')		// JPEG (when you want to use mcw_jpegenc.lib to encoder still pictures)
#define	FOURCC_JPG				FOURCC('J','P','G',' ')		// JPEG (when you want to use LibImageCodec to encoder still pictures)
#define	FOURCC_GIF				FOURCC('G','I','F',' ')		// GIF
#define	FOURCC_PNG				FOURCC('P','N','G',' ')		// PNG
#define	FOURCC_BMP				FOURCC('B','M','P',' ')		// BMP
#define	FOURCC_WBMP				FOURCC('W','B','M','P')		// WBMP

#define FOURCC_I420				FOURCC('I','4','2','0')		// YUV  420 Planary
#define FOURCC_IYUV				FOURCC('I','4','2','0')		// YUV  420 Planary
#define FOURCC_YV12				FOURCC('Y','V','1','2')		// YVU  420 Planary
#define	FOURCC_YUV9				FOURCC('Y','U','V','9')		// YUV  410 Planary (Y:320x240 U:80x60 V:80x60 Planar)
#define FOURCC_YCbCr422			FOURCC('Y','C','b','2')		// YCbCr 422 Planary
#define FOURCC_YUV422P			FOURCC('4','2','2','P')		// YUV  422 Planary
#define FOURCC_YUV411P			FOURCC('4','1','1','P')		// YUV  411 Planary
#define FOURCC_I420P10			FOURCC('4','2','0','H')		// YUV  420 Planary (10 bit)
#define FOURCC_I422P10			FOURCC('4','2','2','H')		// YUV  422 Planary (10 bit)
#define FOURCC_YUY2				FOURCC('Y','U','Y','2')		// YUV  422 Packed
#define FOURCC_YUYV				FOURCC('Y','U','Y','V')		// YUYV 422 Packed
#define FOURCC_YVYU				FOURCC('Y','V','Y','U')		// YVYU 422 Packed
#define FOURCC_UYVY				FOURCC('U','Y','V','Y')		// UYVY 422 Packed
#define FOURCC_NV12				FOURCC('N','V','1','2')		// UYVY 422 Packed
#define FOURCC_PIX_QSV			FOURCC('P','Q','S','V')		// Intel QSV : mfxFrameSurface1 structure
#define	FOURCC_RGB16			FOURCC('R','G','B','2')		// RGB 16 Bit
#define	FOURCC_XVFF				FOURCC('X','V','F','F')
/////////////////////////////////////
// Audio FourCC Type
/////////////////////////////////////
#define FOURCC_NONE				0xFFFFFFFF  // None
#define FOURCC_PCM				0x00000001	// PCM Format
#define FOURCC_ADPCM_MS			0x00000002	// Microsoft ADPCM
#define FOURCC_PCM_FLOAT		0x00000003	// PCM Float 32 Bit
#define	FOURCC_MS_ALAW			0x00000006	// Microsoft A Law
#define	FOURCC_MS_MULAW			0x00000007	// Microsoft Mu Law
#define FOURCC_LPCM				0x00000008  // DVD (LPCM)
//#define FOURCC_ADPCM_OKI		0x00000010
#define FOURCC_ADPCM_DVI		0x00000011
//#define FOURCC_ADPCM_MEDIA	0x00000012
//#define FOURCC_ADPCM_SIERRA	0x00000013
//#define FOURCC_ADPCM_G723		0x00000014
//#define FOURCC_ADPCM_OKI2		0x00000017
//#define FOURCC_ADPCM_MEDIA2	0x00000018
//#define FOURCC_ADPCM_YAMAHA	0x00000020
#define FOURCC_TRUE_SPEECH		0x00000022
//#define FOURCC_ADPCM_ANTEX	0x00000033
//#define FOURCC_ADPCM_ROCKWELL	0x0000003B
//#define FOURCC_ADPCM_G726		0x00000064
//#define FOURCC_ADPCM_G722		0x00000065
//#define FOURCC_ADPCM_ZYZEL	0x00000097
#define	FOURCC_GSM_MS			0x00000031	// GSM Microsoft variant
#define	FOURCC_G726				0x00000045	// g726 adpcm
#define	FOURCC_G711A			0x00000048	// g711 a-law
#define	FOURCC_G711U			0x00000049	// g711 u-law
#define FOURCC_MP3_1			0x00000050	// MP3 Audio Layer
#define FOURCC_MP3_2			0x00000055	// MPEG Layer 3
#define	FOURCC_AVI_AMR			0x00000057	// AMR in AVI ?
#define	FOURCC_METASOUND		0x00000075	// Voxware MetaSound
#define FOURCC_AAC				0x000000FF	// AAC
#define	FOURCC_AAC_ADTS			FOURCC(0x00, 0x16, 0, 0)	// AAC ADTS
#define FOURCC_AAC_PLUS			0x000000FE	// AAC+
#define FOURCC_AAC_PS			0x000000FD	// AACPS
#define	FOURCC_AACM				FOURCC('A','A','C','M')	// AAC Main
#define	FOURCC_CRBT				0x000000F1	// temporarily fourcc
#define FOURCC_ADPCM_TOREX		0x00000100
//#define FOURCC_ADPCM_CREAT	0x00000200
//#define FOURCC_ADPCM_OLI		0x00001001
#define FOURCC_AC3_1			0x00000092	// Dolby AC3 SPDIF
#define	FOURCC_AC3_2			0x00002000	// AC3 Audio
#define	FOURCC_DTS				0x00002001	// DTS
#define FOURCC_G723				0x00000123	// Digital G.723
#define	FOURCC_ACELP			0x00000130
#define	FOURCC_WMA1				0x00000160	// WMA V1 	:: MSAUDIO1
#define FOURCC_WMA2				0x00000161	// WMA V2	:: WMAUDIO2
#define FOURCC_WMA3				0x00000162	// WMA Pro	:: WMAUDIO3
#define FOURCC_WMA_LOSSLESS		0x00000163	// WMA Lossless	:: WMAAUDIO_LOSSLESS
#define	FOURCC_WMASPDIF			0x00000164	// :: WMASPDIF	(? yet to be verified ?)
#define FOURCC_OGG1				0x0000674F	// Ogg Vorbis (mode1)
#define	FOURCC_OGG2				0x00006750	// Ogg Vorbis (mode2)
#define FOURCC_OGG3				0x00006751	// Ogg Vorbis (mode3)
#define	FOURCC_OGG1P			0x0000676F	// Ogg Vorbis (mode1+)
#define	FOURCC_OGG2P			0x00006770	// Ogg Vorbis (mode2+)
#define	FOURCC_OGG3P			0x00006771	// Ogg Vorbis (mode3+)
#define FOURCC_OGG1FF			FOURCC('O','G','1','F')		// Ogg Vorbis (mode1) :: FFMPEG
#define	FOURCC_WMAV				0x0000000a	// WMA Voice 9
#define	FOURCC_ADPCM_SWF		0x0000F17B	// SWF
#define	FOURCC_ADPCM_SWFN		0x0000F17C	// SWF
#define	FOURCC_FLAC				0x0000F1AC	// Flac

#define FOURCC_MPEG2_AUDIO		0xE06D802B	// MPEG2 AUDIO
#define	FOURCC_AMR				0x726D6173	// Amr
#define FOURCC_AMRWB			0x726D6177	// Amrw
#define FOURCC_EVRC				0x43525645	// Evrc
#define	FOURCC_QCELP			0x516B3331	// Qcelp
/*
 * MCW_REALAUDIO_DECODER_CONFIG
 * stucture type for configuration or informative output parameters
 */
//#define FOURCC_SIPR				FOURCC('s','i','p','r') /* 'sipr' for RealAudio Voice */
//#define FOURCC_COOK				FOURCC('c','o','o','k') /* 'cook' for RealAudio 8 LBR */
//#define FOURCC_ATRC				FOURCC('a','t','r','c') /* 'atrc' for RealAudio 8 HBR */
//#define FOURCC_RAAC				FOURCC('r','a','a','c') /* 'raac' for RealAudio 10 */
//#define FOURCC_RACP				FOURCC('r','a','c','p') /* 'racp' for RealAudio 10 */
//#define FOURCC_RALF				FOURCC('r','a','l','f') /* 'ralf' for RealAudio 10 */
#define FOURCC_SIPR				FOURCC('r','p','i','s') /* 'sipr' for RealAudio Voice */
#define FOURCC_COOK				FOURCC('k','o','o','c') /* 'cook' for RealAudio 8 LBR */
#define FOURCC_ATRC				FOURCC('c','r','t','a') /* 'atrc' for RealAudio 8 HBR */
#define FOURCC_RAAC				FOURCC('c','a','a','r') /* 'raac' for RealAudio 10 */
#define FOURCC_RACP				FOURCC('p','c','a','r') /* 'racp' for RealAudio 10 */
#define FOURCC_RALF				FOURCC('f','l','a','r') /* 'ralf' for RealAudio 10 */

#define	FOURCC_QDM2				FOURCC('Q','D','M','2') // QuickTime Low-Rate audio codec
#define	FOURCC_ADPCM_QT			FOURCC('i','m','a','4')	// Adpcm QucikTime
#define	FOURCC_ACO1				FOURCC('A','C','O','1')	// NHN on-air
#define FOURCC_ALAC				FOURCC('A','L','A','c') // QuickTime Lossless audio codec
#define FOURCC_EAC3				FOURCC('e','a','c','3') // Dolby digital plus - enhance ac-3
#define	FOURCC_NELLYMOSER		FOURCC('n','m','o','s')	// Nellymoser audio codec
#define	FOURCC_OPUS				FOURCC('O','P','U','S')	// OPUS

//#define FOURCC_SIPR			0x73697072 /* 'sipr' for RealAudio Voice */
//#define FOURCC_COOK			0x636F6F6B /* 'cook' for RealAudio 8 LBR */
//#define FOURCC_ATRC			0x61747263 /* 'atrc' for RealAudio 8 HBR */
//#define FOURCC_RAAC			0x72616163 /* 'raac' for RealAudio 10 */
//#define FOURCC_RACP			0x72616370 /* 'racp' for RealAudio 10 */
//#define FOURCC_RALF			0x72616C66 /* 'ralf' for RealAudio 10 */

/////////////////////////////////////
// Text FourCC Type
/////////////////////////////////////
#define FOURCC_T140				FOURCC('T','1','4','0')
#define FOURCC_SAMI				FOURCC('S','A','M','I')
#define	FOURCC_UTF8				FOURCC('U','T','F','8')
#define	FOURCC_UCS2				FOURCC('U','C','S','2')
#define	FOURCC_UTF16			FOURCC_UCS2
#define FOURCC_WVTT				FOURCC('W','V','T','T')

#endif // _MEDIAFOURCC_H_

