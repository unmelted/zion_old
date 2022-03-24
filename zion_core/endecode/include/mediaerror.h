
#ifndef _MEDIAERROR_H_
#define _MEDIAERROR_H_

#define FRSUCCEEDED(x)					(!((x)&0x80000000))
#define FRFAILED(x)						((x)&0x80000000)

#define FR_OK							0x00000000
#define FR_FAIL							0x80000000

///////////////////////////////////////////////////////////////////////////
// Base error code
///////////////////////////////////////////////////////////////////////////
#define	COMMON_ERR_BASE					(FR_FAIL + 0x00000000)			// common

#define	HTTP_ERR_BASE					(FR_FAIL + 0x01000000)			// HTTP Base Error

#define FILTERGRAPH_ERR_BASE			(FR_FAIL + 0x00600000)			// filter graph
#define	SRCFILTER_ERR_BASE				(FR_FAIL + 0x00610000)			// source filter
#define	ADECFILTER_ERR_BASE				(FR_FAIL + 0x00620000)			// audio decoder filter
#define	VDECFILTER_ERR_BASE				(FR_FAIL + 0x00630000)			// video decoder filter
#define	AENCFILTER_ERR_BASE				(FR_FAIL + 0x00640000)			// audio encoder filter
#define	VENCFILTER_ERR_BASE				(FR_FAIL + 0x00650000)			// video encoder filter
#define	ARENFILTER_ERR_BASE				(FR_FAIL + 0x00660000)			// audio render filter
#define	VRENFILTER_ERR_BASE				(FR_FAIL + 0x00670000)			// video render filter

///////////////////////////////////////////////////////////////////////////
// common error code
///////////////////////////////////////////////////////////////////////////
#define	COMMON_ERR_MEM					(COMMON_ERR_BASE + 0x01)		// 메모리가 부족합니다.
#define COMMON_ERR_THREAD				(COMMON_ERR_BASE + 0x02)		// 스레드를 생성할 수 없습니다.
#define	COMMON_ERR_SYSTEM				(COMMON_ERR_BASE + 0x03)		// 시스템 초기화에 실패했습니다.
#define	COMMON_ERR_NETWORK				(COMMON_ERR_BASE + 0x04)		// 네트워크 System이 Fail
#define COMMON_ERR_CANCEL				(COMMON_ERR_BASE + 0x05)		// 외부에서 인코딩 취소 요청
#define COMMON_ERR_NULLPOINTER			(COMMON_ERR_BASE + 0x06)		// NULL 포인터 입력
#define	COMMON_ERR_INVALIDINPUT			(COMMON_ERR_BASE + 0x07)		// 지원하지 않는 입력값 범위

#define	COMMON_ERR_FOPEN				(COMMON_ERR_BASE + 0x10)		// 파일을 찾을 수가 없습니다.
#define	COMMON_ERR_FCREATE				(COMMON_ERR_BASE + 0x11)		// 파일을 생성 할 수가 없습니다.
#define	COMMON_ERR_URL					(COMMON_ERR_BASE + 0x12)		// 지원하지 않는 URL 형식입니다.
#define COMMON_ERR_ENCRYPTION			(COMMON_ERR_BASE + 0x13)		// Encryption된 데이터입니다.
#define COMMON_ERR_EXPIRED_DATA			(COMMON_ERR_BASE + 0x14)		// 유효기간이 만료되었습니다.
#define	COMMON_ERR_NODATA				(COMMON_ERR_BASE + 0x15)		// 데이터가 없다.
#define	COMMON_ERR_ENDOFDATA			(COMMON_ERR_BASE + 0x16)		// 데이터의 끝이다.
#define	COMMON_ERR_NEEDDATA				(COMMON_ERR_BASE + 0x17)		// 데이터가 더 필요하다.
#define	COMMON_ERR_NONEEDDATA			(COMMON_ERR_BASE + 0x18)		// 데이터가 더 필요없다.
#define	COMMON_ERR_WAITDATA				(COMMON_ERR_BASE + 0x19)		// 데이터를 기다려야 한다.
#define	COMMON_ERR_NOMEDIA				(COMMON_ERR_BASE + 0x1A)		// 해당 미디어가 없다.
#define	COMMON_ERR_FUNCTION				(COMMON_ERR_BASE + 0x1B)		// 지원하지 않는 기능입니다.
#define	COMMON_ERR_INVALID_RANGE		(COMMON_ERR_BASE + 0x1C)		// 랜덤 액세스에 실패했습니다.
#define	COMMON_ERR_FREAD				(COMMON_ERR_BASE + 0x1D)		// 파일읽기를 실패하였습니다.
#define COMMON_ERR_LIMITDATA            (COMMON_ERR_BASE + 0x1E)        // 제한된 사이즈에 도달하였습니다.
#define	COMMON_ERR_MUSTNOTSUPPORT		(COMMON_ERR_BASE + 0x101)		// 지원해서는 안되는 포멧
#define COMMON_ERR_NOTCAPABLE			(COMMON_ERR_BASE + 0x102)		// 지원하지 못하는 파일 (파일 크기나 길이 등의 이유)
#define	COMMON_ERR_PAUSEDATA			(COMMON_ERR_BASE + 0x103)		// 지원하지 못하는 파일 (파일 크기나 길이 등의 이유)

#define	COMMON_ERR_FILETYPE				(COMMON_ERR_BASE + 0x200)		// 지원하지 않는 파일 형식입니다.
#define	COMMON_ERR_STREAMSYNTAX			(COMMON_ERR_BASE + 0x201)		// 스트림 에러가 발생하였습니다.
#define	COMMON_ERR_MEDIAPARAM			(COMMON_ERR_BASE + 0x202)		// 잘못된 미디어 파라미터 형식입니다.
#define	COMMON_ERR_AUDIOCODEC			(COMMON_ERR_BASE + 0x203)		// 지원하지 않는 오디오 형식입니다.
#define	COMMON_ERR_VIDEOCODEC			(COMMON_ERR_BASE + 0x204)		// 지원하지 않는 비디오 형식입니다.
#define	COMMON_ERR_TEXTCODEC			(COMMON_ERR_BASE + 0x205)		// 지원하지 않는 텍스트 형식입니다.
#define	COMMON_ERR_AUDIODEC				(COMMON_ERR_BASE + 0x206)		// 오디오 디코더를 초기화하기 못했습니다.
#define	COMMON_ERR_VIDEODEC				(COMMON_ERR_BASE + 0x207)		// 비디오 디코더를 초기화하기 못했습니다.
#define	COMMON_ERR_TEXTDEC				(COMMON_ERR_BASE + 0x208)		// 텍스트 디코더를 초기화하기 못했습니다.
#define	COMMON_ERR_AUDIOENC				(COMMON_ERR_BASE + 0x209)		// 오디오 인코더를 초기화하기 못했습니다.
#define	COMMON_ERR_VIDEOENC				(COMMON_ERR_BASE + 0x20A)		// 비디오 인코더를 초기화하기 못했습니다.
#define	COMMON_ERR_TEXTENC				(COMMON_ERR_BASE + 0x20B)		// 텍스트 인코더를 초기화하기 못했습니다.
#define	COMMON_ERR_AUDIODEVIN			(COMMON_ERR_BASE + 0x20C)		// 오디오 입력장치를 초기화하기 못했습니다.
#define	COMMON_ERR_VIDEODEVIN			(COMMON_ERR_BASE + 0x20D)		// 비디오 입력장치를 초기화하기 못했습니다.
#define	COMMON_ERR_AUDIODEVOUT			(COMMON_ERR_BASE + 0x20E)		// 오디오 출력장치를 초기화하기 못했습니다.
#define	COMMON_ERR_VIDEODEVOUT			(COMMON_ERR_BASE + 0x20F)		// 비디오 출력장치를 초기화하기 못했습니다.
#define	COMMON_ERR_VIDEOOUTFORMAT		(COMMON_ERR_BASE + 0x210)		// 지원하지 않는 비디오 출력 포맷입니다.	
#define	COMMON_ERR_AUDIODEC_FRAME		(COMMON_ERR_BASE + 0x211)		// 오디오 프레임 디코딩 에러
#define	COMMON_ERR_VIDEODEC_FRAME		(COMMON_ERR_BASE + 0x212)		// 비디오 프레임 디코딩 에러
#define	COMMON_ERR_DECODE_BLOCKING		(COMMON_ERR_BASE + 0x213)		// 디코드 실패 혹은 디코더 블러킹
#define	COMMON_ERR_ENCODE_BLOCKING		(COMMON_ERR_BASE + 0x214)		// 인코드 실패 혹은 인코더 블러킹
#define	COMMON_ERR_TEXTDEVOUT			(COMMON_ERR_BASE + 0x215)		// 텍스트 출력장치를 초기화하기 못했습니다.

#define	COMMON_ERR_SOCKET				(COMMON_ERR_BASE + 0x30)		// 소켓 에러가 발생했습니다.
#define	COMMON_ERR_INVALID_PROTOCOL		(COMMON_ERR_BASE + 0x31)		// 프로토콜 에러가 발생하였습니다.
#define	COMMON_ERR_SEND					(COMMON_ERR_BASE + 0x32)		// Send 실패
#define	COMMON_ERR_RECV					(COMMON_ERR_BASE + 0x33)		// Recv 실패
#define	COMMON_ERR_TIMEOUT				(COMMON_ERR_BASE + 0x34)		// 시간을 초과하였습니다.
#define	COMMON_ERR_SERVER				(COMMON_ERR_BASE + 0x35)		// 서버에서 에러가 발생했습니다.
#define	COMMON_ERR_SVR_CONNECT			(COMMON_ERR_BASE + 0x36)		// 서버와 연결할 수가 없습니다.
#define	COMMON_ERR_SVR_DISCONNECT		(COMMON_ERR_BASE + 0x37)		// 서버와의 연결이 끊어졌습니다.
#define	COMMON_ERR_SVR_NORESPONSE		(COMMON_ERR_BASE + 0x38)		// 서버로부터 응답이 없습니다.
#define	COMMON_ERR_SVR_NODATA			(COMMON_ERR_BASE + 0x39)		// 서버로부터 전송되는 데이터가 없습니다.
#define	COMMON_ERR_CLIENT				(COMMON_ERR_BASE + 0x3A)		// 잘못된 요청을 시도하였습니다.
#define	COMMON_ERR_CLI_ACCEPT			(COMMON_ERR_BASE + 0x3B)		// 단말의 연결 요청이 거부되었습니다.
#define	COMMON_ERR_CLI_DISCONNECT		(COMMON_ERR_BASE + 0x3C)		// 단말과의 연결이 끊어졌습니다.
#define	COMMON_ERR_CLI_NOREQUEST		(COMMON_ERR_BASE + 0x3D)		// 단말로부터 요청이 없습니다.
#define	COMMON_ERR_NOTMATCH_SSRC		(COMMON_ERR_BASE + 0x3E)		// SSRC가 일치하지 않는다.
#define	COMMON_ERR_NOTMATCH_SCID		(COMMON_ERR_BASE + 0x3F)		// SID CID가 일치하지 않는다.
#define	COMMON_ERR_UNSUPPORT_TRASNPORT	(COMMON_ERR_BASE + 0x40)		// Unsupported transport.
#define	COMMON_ERR_RECV_ZERO			(COMMON_ERR_BASE + 0x41)		// recv size is Zero..

//HTTP
#define COMMON_ERR_REDIRECT				(COMMON_ERR_BASE + 0x4E)		// 신규 URL을 반환합니다. - Naver UCC Real URL을 구했을 때 사용
#define COMMON_ERR_NOTSENDGET			(COMMON_ERR_BASE + 0x4F)		// GET Method를 전송하지 말라는 의미의 반환코드

#define COMMON_ERR_BADREQUEST			(HTTP_ERR_BASE	 + 0x400)		// 잘못된 요구입니다.
#define COMMON_ERR_UNAUTHORIZED			(HTTP_ERR_BASE   + 0x401)		// 인증되지 않았습니다.
#define COMMON_ERR_FORBIDDEN			(HTTP_ERR_BASE   + 0x403)		// 금지되었습니다.
#define COMMON_ERR_NOTFOUND				(HTTP_ERR_BASE	 + 0x404)		// 사용자가 존재 하지 않습니다.
#define COMMON_ERR_NOTALLOWED			(HTTP_ERR_BASE   + 0x405)		// Method를 사용할 수 없습니다.
#define COMMON_ERR_NOTACCEPT			(HTTP_ERR_BASE   + 0x406)		// 접속할 수 없습니다.
#define COMMON_ERR_NEEDPROXYAUTH		(HTTP_ERR_BASE   + 0x407)		// Proxy 인증이 필요합니다.
#define COMMON_ERR_HTTPTIMEOUT			(HTTP_ERR_BASE   + 0x408)		// TimeOut.
#define COMMON_ERR_CONFLICT				(HTTP_ERR_BASE   + 0x409)		// 충돌이 발생하였습니다.
#define COMMON_ERR_GONE					(HTTP_ERR_BASE   + 0x410)		// 내용물이 사라졌습니다.
#define COMMON_ERR_NEEDLENGTH			(HTTP_ERR_BASE   + 0x411)		// 길이가 필요합니다.
#define COMMON_ERR_PRECONDITION			(HTTP_ERR_BASE   + 0x412)		// 사전 조건이 충족 되지 못했습니다.
#define COMMON_ERR_ENTITYBIG			(HTTP_ERR_BASE   + 0x413)		// Request Entity가 너무 큽니다.
#define COMMON_ERR_URLLONG				(HTTP_ERR_BASE   + 0x414)		// Request-URI가 너무깁니다.
#define COMMON_ERR_MEDIATYPE			(HTTP_ERR_BASE   + 0x415)		// 지원하지 않는 Media Type입니다.
#define COMMON_ERR_BADRESPONSE			(HTTP_ERR_BASE	 + 0x415)		// 잘못된 응답입니다.
#define COMMON_ERR_INTERNALSVR          (HTTP_ERR_BASE   + 0x500)		// Internal Server Error

#define COMMON_ERR_HTTP_REQUEST_400     (HTTP_ERR_BASE	 + 0x400)		// This value is only HTTP Request Error Range
#define	COMMON_ERR_HTTP_REQUEST_499     (HTTP_ERR_BASE	 + 0x499)		// This value is only HTTP Request Error Range

//NAS Control
#define COMMON_ERR_NAS_PERFORMANCE		COMMON_ERR_BASE + 0x50			// Performance Query 실패
#define COMMON_ERR_NAS_ALIVE			COMMON_ERR_BASE + 0x51			// Alive Check 실패
#define COMMON_ERR_NAS_DISCONNECT		COMMON_ERR_BASE + 0x52			// NAS Drive Disconnect 실패
#define COMMON_ERR_NAS_CONNECT			COMMON_ERR_BASE + 0x53			// NAS Drive Connect 실패

// DLL linking
#define COMMON_ERR_DLLOPEN				COMMON_ERR_BASE + 0x60			// open dll dynamically 실패

#endif	// _MEDIAERROR_H_

