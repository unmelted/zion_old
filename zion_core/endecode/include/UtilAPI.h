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

#ifndef _UTILAPI_H_
#define _UTILAPI_H_

#include "SystemAPI.h"
#include "ColorConvAPI.h"

#define CNTR_CPU	"\\Processor(_Total)\\% Processor Time" // % of cpu in use

#define	IS_3B_START_CODE(a)	( (a[0]==0) && (a[1]==0) && (a[2]==1) )
#define	IS_4B_START_CODE(a)	( (a[0]==0) && (a[1]==0) && (a[2]==0) && a[3]==1)

#define ConvByteToDWORD(a)					((DWORD32)(((a)[0] << 24) | ((a)[1] << 16) | ((a)[2] << 8) | ((a)[3])))
#define Conv3ByteToDWORD(a)					((DWORD32)(((a)[0] << 16) | ((a)[1] << 8)  | ((a)[2])))
#define ConvByteToWORD(a)					((USHORT)((a)[0] << 8)  | ((a)[1]))
#define ConvLByteToDWORD(a)					((DWORD32)(((a)[0]) | ((a)[1] << 8) | ((a)[2] << 16) | ((a)[3] << 24)))
#define ConvLByteToWORD(a)					((USHORT)((a)[0]) | ((a)[1] << 8))
#define Conv3LByteToDWORD(a)				((USHORT)((a)[0]) | ((a)[1] << 8) | ((a)[2] << 16))

#define	ConvDWORDToBYTE(a, b)				(b)[0] = (BYTE)(((a)&0xff000000)>>24); \
											(b)[1] = (BYTE)(((a)&0x00ff0000)>>16); \
											(b)[2] = (BYTE)(((a)&0x0000ff00)>>8); \
											(b)[3] = (BYTE)(((a)&0x000000ff))

#define ConvWORDToBYTE(a, b)				(b)[0] = (BYTE)(((a)&0xff00)>>8); \
											(b)[1] = (BYTE)(((a)&0x00ff))

#define ConvDWORDToLBYTE(a, b)				(b)[0] = (BYTE)(((a)&0x000000ff)); \
											(b)[1] = (BYTE)(((a)&0x0000ff00)>>8); \
											(b)[2] = (BYTE)(((a)&0x00ff0000)>>16); \
											(b)[3] = (BYTE)(((a)&0xff000000)>>24)

#define ConvWORDToLBYTE(a, b)				(b)[0] = (BYTE)(((a)&0x00ff)); \
											(b)[1] = (BYTE)(((a)&0xff00)>>8)

#define	ConvDWORDTo3BYTE(a, b)				(b)[0] = (BYTE)(((a)&0x00ff0000)>>16); \
											(b)[1] = (BYTE)(((a)&0x0000ff00)>>8); \
											(b)[2] = (BYTE)(((a)&0x000000ff))

#define ConvByteToQWORD(a)					((QWORD)((a)[0] << 56) | ((a)[1] << 48) | ((a)[2] << 40) | ((a)[3] << 32)) \
													((a)[4] << 24) | ((a)[5] << 16) | ((a)[6] <<  8) | ((a)[7]))

#if defined(WIN32) || defined(Linux) || defined(SunOS) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
#define	FOURCC(a,b,c,d)						((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))
#elif defined(HPUX)
#define	FOURCC(a,b,c,d)						((d) | ((c) << 8) | ((b) << 16) | ((a) << 24))
#endif
#define	FOURCCBE(a,b,c,d)					((d) | ((c) << 8) | ((b) << 16) | ((a) << 24))

#define	SCALE(v, n, d)						((QWORD)(v) * (n) / (d))

#define	CalAudioCTSByBitrate(s, b)			((b) ? ((QWORD)(s) * 8 / (b)) : 0)
#define	CalAudioCTSBySampleRate(s, r)		((r) ? ((QWORD)(s) * 1000 / (r)) : 0)
#define	CalAudioByteSize(t, b)				((QWORD)(t) * (b) / 8)
#define	CalAudioSampleBySampleRate(t, r)	((QWORD)(t) * (r) / 1000)
#define	CalVideoCTSByFrameRate(n, f)		((f) ? ((QWORD)(n) * 1000000 / (QWORD)(f)) : 0)

typedef	void*	McBitHandle;
typedef	void*	FrTimeHandle;
typedef	void*	McBufHandle;
typedef	void*	McQueHandle;
typedef void*   McAQueHandle;
typedef void*	McPerfHandle;	// mwkang

// Code page for ANSI characters
typedef	enum {
	DTB_CP_SYS = 0,			// System CP				:: CP_ACP for Windows
	DTB_CP_KOR,				// KOREAN					:: CP949
	DTB_CP_JP,				// Japanese Shift-JIS		:: CP932
	DTB_CP_CHN_GBK,			// Simplified CHinese GBK	:: CP936
	DTB_CP_CHN_BIG5			// Traditional Chinese Big5 :: CP950
} DtbWindowCopePage;

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

void ITOU(UINT32 n ,char s[] );
void ITOX(UINT32 n ,char s[] );
void ITOA(INT32 n ,char s[] );
QWORD ATOQ(char* strString);
DWORD32 ATOI(char* strString);
INT32 ATOI_MS(char* strString);
INT32 ATOI_TS(char* strString);
INT32 ATOI_HEX(char* strString);
double ATOF(char *strString);
//INT32 WTOI(WCHAR* strString);
#ifdef SUPPORT_IOS
#define ABS(a) ABS2(a)
    INT32 ABS2(INT32 a);
#else
    INT32 ABS(INT32 a);
#endif
INT64 ABS64(INT64 a);
void McAddTime(DWORD32* pdwDate, DWORD32* pdwTime, DWORD32 dwTime);
DWORD32 McGetRangeFromTime(DWORD32 dwStartDate, DWORD32 dwStartTime, DWORD32 dwEndDate, DWORD32 dwEndTime);
char* MwGetErrorMessageKor(LRSLT lError);
char* MwGetErrorMessageEng(LRSLT lError);

////////////////////////
// Bit
////////////////////////
McBitHandle UtilBitOpen(BYTE* pBuffer, INT32 iBufferLen);
void UtilBitClose(McBitHandle hHandle);
UINT32 UtilBitShow(McBitHandle hHandle, INT32 n);
void UtilBitFlush(McBitHandle hHandle, INT32 n);
UINT32 UtilBitGetBit(McBitHandle hHandle, INT32 n);
UINT64 UtilBitGetBit64(McBitHandle hHandle, INT32 n);
void UtilBitPutBit(McBitHandle hHandle, INT32 n, UINT32 m);
UINT32 UtilBitGetFast(McBitHandle hHandle, INT32 n);
UINT32 UtilBitGet1Bit(McBitHandle hHandle);
void UtilBitBookmark(McBitHandle hHandle, INT32 state);
INT32 UtilBitGetProcessed(McBitHandle hHandle);
INT32 UtilBitLeftBits(McBitHandle hHandle);
UINT32 UtilBitByteAlign(McBitHandle hHandle);

////////////////////////
// Buffer
////////////////////////
McBufHandle UtilBufOpen(DWORD32 dwRef);
void UtilBufClose(McBufHandle hHandle);
void UtilBufReset(McBufHandle hHandle);
void UtilBufResetRef(McBufHandle hHandle, DWORD32 dwRefID);
void UtilBufSetRef(McBufHandle hHandle, DWORD32 dwRef);
BOOL UtilBufPutData(McBufHandle hHandle, BYTE *pData, DWORD32 dwLen, DWORD32 dwIndex);
BYTE* UtilBufGetData(McBufHandle hHandle, DWORD32* pdwLen, DWORD32 dwRefID);
BYTE* UtilBufShowFirstData(McBufHandle hHandle, DWORD32* pdwLen);
BYTE* UtilBufShowLastData(McBufHandle hHandle, DWORD32* pdwLen);
BYTE* UtilBufShowData(McBufHandle hHandle, DWORD32* pdwLen);
void UtilBufShowReset(McBufHandle hHandle);
DWORD32 UtilBufGetBufferedSize(McBufHandle hHandle);
DWORD32 UtilBufGetBufferedCnt(McBufHandle hHandle);
BOOL UtilBufCheckLostData(McBufHandle hHandle);

////////////////////////
// Queue
////////////////////////
McQueHandle UtilQueOpen(void);
McQueHandle UtilQueCntOpen(int nCnt);
McAQueHandle UtilAQueCntOpen(int nCnt);
void UtilQueClose(McQueHandle hHandle);
void UtilAQueClose(McAQueHandle hHandle);
void UtilQueReset(McQueHandle hHandle);
BOOL UtilQuePutData(McQueHandle hHandle, void *pData, DWORD32 dwLen);
BOOL UtilAQuePutData(McAQueHandle hHandle, void *pData, DWORD32 dwLen);
BOOL UtilQuePutDataX(McQueHandle hHandle, void *pData, DWORD32 dwLen);
void* UtilQueGetData(McQueHandle hHandle, DWORD32* pdwLen);
void* UtilQuePeekData(McQueHandle hHandle, DWORD32* pdwLen);
void* UtilQuePeekDataEx(McQueHandle hHandle, DWORD32* pdwLen, DWORD32 dwIndex);
BOOL UtilAQueGetData(McAQueHandle hHandle, void* pData);
void* UtilQueGetDataX(McQueHandle hHandle, DWORD32* pdwLen);
void* UtilQueSetHeadData(McQueHandle hHandle, DWORD32* pdwLen);
void* UtilQueShowData(McQueHandle hHandle, DWORD32* pdwLen);
int UtilQueGetCount(McQueHandle hHandle);
int UtilAQueGetCount(McAQueHandle hHandle);
int UtilAQueGetSize(McAQueHandle hHandle);

// following functions are added for Live TS
BOOL UtilQueWriteData(McQueHandle hHandle, void *pData, DWORD32 dwLen);
void* UtilQueReadData(McQueHandle hHandle, DWORD32* pdwLen);
void* UtilQueShowHeadData(McQueHandle hHandle, DWORD32* pdwLen);
void UtilQueClear(McQueHandle hHandle);
void* UtilQueSetStart(McQueHandle hHandle);
////////////////////////
// String
////////////////////////
char* UtilStrGetLowerCaseLetter(char *pBuffer, INT32 iLen);
char* UtilStrGetUpperCaseLetter(char *pBuffer, INT32 iLen);
char* UtilStrFindString(char* pBuffer, char* pFindString);
char* UtilStrIFindString(char* pBuffer, char* pFindString);
char* UtilStrReverseIFindString(char* pBuffer, char* pFindString);
char* UtilStrStrdupn(const char *string, INT32 n);
TCHAR* UtilStrStrdup(const TCHAR *string);
TCHAR* UtilStrStrdupEx(const TCHAR *string, DWORD32 dwExtraSize);
char* UtilStrStrcat(const char *string, const char *string2);
INT32 UtilStrStricmp(const char * dst, const char * src);
INT32 UtilStrStrnicmp(const char * first, const char * last, size_t count);
BOOL UtilStrIsDecNumber(char* pBuffer);
char* UtilStrGetLine(char* pBuffer, DWORD32* dwLen);
char* UtilStrGetLineEx(char* pBuffer, DWORD32* dwLen, BOOL bReplace);
char* UtilStrGetDecNumber(char* pBuffer, char* pString, DWORD32* dwNum);
char* UtilStrIGetDecNumber(char* pBuffer, char* pString, DWORD32* dwNum);
char* UtilStrGetIntNumber(char* pBuffer, char* pString, INT32* pnNum);
char* UtilStrGetHexNumber(char* pBuffer, char* pString, DWORD32* dwNum);
char* UtilStrGetFloatNumber(char* pBuffer, char* pString, DWORD32* dwNum);
char* UtilStrGetDoubleNumber(char* pBuffer, char* pString, double* dbNum);
char* UtilStrGetQwordNumber(char* pBuffer, char* pString, QWORD* qwNum);
char* UtilStrGetTimeNumber(char* pBuffer, char* pString, DWORD32* dwNum);
char* UtilStrGetString(char* pBuffer, char** pString, DWORD32* dwLen);
char* UtilStrGetStringEx(char* pBuffer, char** pString, DWORD32* dwLen);
char* UtilStrGetAddress(char* pBuffer, char** pString, DWORD32* dwLen);
char* UtilStrGetHexString(char* pBuffer, char** pString, DWORD32* dwLen);
char* UtilStrGetSafeString(char* pBuffer, char** pString, DWORD32* dwLen);
char* UtilStrGetSafeFileName(char* pBuffer, char** pString, DWORD32* dwLen);	// Add hjchoi - 2007.04.11
char* UtilStrGetContext(char* pBuffer, char** pContext, DWORD32* dwLen);
char* UtilStrGetBase64String(char* pBuffer, char** pString, DWORD32* dwLen);
DWORD32 UtilStrConvStringToBin(char* pInData, DWORD32 dwInData, BYTE* pOutData, DWORD32 dwOutBuf);
char* UtilStrConvBinToString(BYTE* pData, DWORD32 dwData);
BOOL UtilFindStartCode(BYTE** pBuffer, DWORD32* pdwLen, INT32* pnCodeID);
BOOL UtilFindStartCode2(BYTE** pBuffer, DWORD32* pdwLen, DWORD32 dwCodeID);
char* UtilStrGetHTTPHeader(char* pBuffer, DWORD32* pdwLen);
char* UtilStrTrim(char *pBuffer);
char* UtilStrLeftOneTrim(char *pBuffer);
char* UtilStrReplace(char *pBuffer, char chOld, char chNew);
char *UtilStrReplaceAll(char *s, const char *olds, const char *news);


////////////////////////
// String System API
////////////////////////
//
//INT32 McStrcoll( const TCHAR *string1, const TCHAR *string2 );
//INT32 McStrlen( const TCHAR *string );
//INT32 McStrncmp( const TCHAR *first, const TCHAR *last, INT32 count );
//INT32 McSprintf( TCHAR *buffer, const TCHAR *format, ... );
//INT32 McStrcmp( const TCHAR *string1, const TCHAR *string2 );
//INT32 McStrcspn( const TCHAR *string, const TCHAR *strCharSet );
//INT32 McStrftime( TCHAR *strDest, INT32 maxsize, const TCHAR *format, const struct tm *timeptr );
//INT32 McStrncmp( const TCHAR *string1, const TCHAR *string2, INT32 count );
//INT32 McStrspn( const TCHAR *string, const TCHAR *strCharSet );
//INT32 McVSprintf( TCHAR *buffer, const TCHAR *format, va_list argptr );
//INT32 McStrxfrm( TCHAR *strDest, const TCHAR *strSource, INT32 count );
//TCHAR *McStrcat( TCHAR *strDestination, const TCHAR *strSource );
//TCHAR *McStrchr( const TCHAR *string, wint_t c );
//TCHAR *McStrcpy( TCHAR *strDestination, const TCHAR *strSource );
//TCHAR *McStrncat( TCHAR *strDest, const TCHAR *strSource, INT32 count );
//TCHAR *McStrncpy( TCHAR *strDest, const TCHAR *strSource, INT32 count );
//TCHAR *McStrpbrk( const TCHAR *string, const TCHAR *strCharSet );
//TCHAR *McStrrchr( const TCHAR *string, INT32 c );
//TCHAR *McStrstr( const TCHAR *string, const TCHAR *strCharSet );
//TCHAR *McStrtok( TCHAR *strToken, const TCHAR *strDelimit );


////////////////////////
// Base64
////////////////////////
BYTE* UtilBase64Encoding(BYTE* inData, INT32 inSize, INT32 *outSize);
BYTE* UtilBase64Decoding(BYTE* pInData, BYTE* pOutData, DWORD32 inSize, DWORD32* outSize);
BYTE* UtilBase64ConvToBin(BYTE *pData, DWORD32 dwInSize, DWORD32* pdwOutSize);
DWORD32 UtilBase64ConvToBinWLen(BYTE *pInData, DWORD32 dwInSize, BYTE* pOutData, DWORD32 dwOutBuf);
DWORD32 UtilBin2HexString(BYTE *pInData, DWORD32	 dwInSize, BYTE* pOutData, DWORD32 dwOutBuf);

////////////////////////
// Unicode
////////////////////////
// Charater Encoding
// Char : ASCII
// UCS-2 : UCS BMP. Only 2Bytes
// UCS-4 : same as UTF-32
// Check valid UTF-8
//
BOOL  UtilUniIsUTF8(const UTF8* utf8, DWORD32 dwUtf8Len);
DWORD32 UtilUniUTF8Length(const UTF8* utf8);
DWORD32 UtilUniUTF16Length(const UTF16* utf16);
DWORD32 UtilUniUTF32Length(const UTF32* utf32);
//
//// Conversion between Unicodes or Char
DWORD32 UtilUniUCS2toChar(UCS2* ucs2, DWORD32 dwUcs2Len, CHAR* pChar,  DWORD32 dwCharSize);
DWORD32 UtilUniUCS2toUTF8(UCS2 *ucs2, DWORD32 dwUcs2Len, UTF8 *utf8, DWORD32 dwUtf8Size);
DWORD32 UtilUniChartoUCS2(CHAR* pChar, DWORD32 dwCharLen, UCS2* ucs2, DWORD32 dwUcs2Size);
DWORD32 UtilUniChartoUCS4(CHAR* pChar, DWORD32 dwCharLen, UCS4* ucs4, DWORD32 dwUcs4Size);
DWORD32 UtilUniChartoUTF8(CHAR* pChar, DWORD32 dwCharLen, UTF8 *utf8, DWORD32 dwUtf8Size);
DWORD32 UtilUniChartoUTF16(CHAR* pChar, DWORD32 dwCharLen, UTF16 *utf16, DWORD32 dwUtf16Size);
DWORD32 UtilUniChartoUTF32(CHAR* pChar, DWORD32 dwCharLen, UTF32 *utf32, DWORD32 dwUtf32Size);
DWORD32 UtilUniUCS4toChar(UCS4* ucs4, DWORD32 dwUcs4Len, CHAR* pChar,  DWORD32 dwCharSize);
DWORD32 UtilUniUTF8toChar(UTF8 *utf8, DWORD32 dwUtf8Len, CHAR* pChar, DWORD32 dwCharSize);
DWORD32 UtilUniUTF8toUCS2(UTF8 *utf8, DWORD32 dwUtf8Len, WCHAR *ucs2, DWORD32 dwUcs2Size, BOOL bBigEndian);
DWORD32 UtilUniUTF8toUTF16(const UTF8* utf8, DWORD32 dwUtf8Len, UTF16* utf16, DWORD32 dwUtf16Size);
DWORD32 UtilUniUTF8toUTF32(const UTF8* utf8, DWORD32 dwUtf8Len, UTF32* utf32, DWORD32 dwUtf32Size);
DWORD32 UtilUniUTF16toChar(const UTF16* utf16, DWORD32 dwUtf16Len, CHAR* pChar, DWORD32 dwCharSize);
DWORD32 UtilUniUTF16toUTF8(const UTF16* utf16, DWORD32 dwUtf16Len, UTF8* utf8, DWORD32 dwUtf8Size);
DWORD32 UtilUniUTF16toUTF32(const UTF16* utf16, DWORD32 dwUtf16Len, UTF32* utf32, DWORD32 dwUtf32Size);
DWORD32 UtilUniUTF32toChar(const UTF32* utf32, DWORD32 dwUtf32Len, CHAR* pChar, DWORD32 dwCharSize);
DWORD32 UtilUniUTF32toUTF8(const UTF32* utf32, DWORD32 dwUtf32Len, UTF8* utf8, DWORD32 dwUtf8Size);
DWORD32 UtilUniUTF32toUTF16(const UTF32* utf32, DWORD32 dwUtf32Len, UTF16* utf16, DWORD32 dwUtf16Size);
// Conversion between MulitByte and Unicode
DWORD32 UtilUniANSI2UTF8(const CHAR* pstrMultibyte, DWORD32 dwMulitSize,
		UTF8* pstrUtf8, DWORD32 dwUtf8Size, DtbWindowCopePage eCodePage);			//
DWORD32 UtilUniANSI2UTF16(const CHAR* pstrMultibyte, DWORD32 dwMulitSize,
		UTF16* pstrUtf16, DWORD32 dwUtf16Size, DtbWindowCopePage eCodePage);		// Little Endian
DWORD32 UtilUniUTF8toANSI(const UTF8* pstrUtf8, DWORD32 dwUtf8Size,
		CHAR* pstrMultibyte, DWORD32 dwMulitSize, DtbWindowCopePage eCodePage);		//
DWORD32 UtilUniUTF16toANSI(const UTF16* pstrUtf16, DWORD32 dwUtfSize,
		CHAR* pstrMultibyte, DWORD32 dwMulitSize, DtbWindowCopePage eCodePage);		// Little Endian
#if defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
DWORD32 UtilUniIconv(char *inbuf, DWORD32 inlen, char **outbuffer, DWORD32 *outlen, char *to, char *from);
#endif
//obsolete
//DWORD32 UtilUnicode2Ansi(TCHAR* szUnicode, char* szAnsi);
unsigned short* UtilUniWcsrchr(const unsigned short* string, unsigned short ch);
unsigned short* UtilUniWcscpy(unsigned short* s1, const unsigned short * s2);
unsigned short * UtilUniWcscat(unsigned short* s1, const unsigned short* s2);
unsigned short * UtilUniWcsstr(const unsigned short * wcs1, const unsigned short * wcs2);
DWORD32 UtilUniWcslen(const unsigned short* s);
unsigned short *UtilUniWcsncpy(unsigned short * dst, const unsigned short * src, size_t n);



////////////////////////
// Url
////////////////////////
TCHAR* UtilURLGetContentLocation(const TCHAR *url);
TCHAR* UtilURLGetContentBase(const TCHAR *url);
char* UtilURLGetAddr(const char *url);
WORD UtilURLGetPort(const char *addr);
TCHAR* UtilURLGetFilename(const TCHAR *url);
TCHAR* UtilURLGetRandFilename(const TCHAR *url);
TCHAR* UtilURLGetFoldername(const TCHAR *url);
TCHAR* UtilURLGetFileExt(const TCHAR *filename);
void UtilURLConvWin32FilePath(TCHAR *filename);
char* UtilURLEncoding(char *enc, char *src);
char* UtilURLDecoding(char *dec, char *src);

////////////////////////
// Color Conv
////////////////////////
// The functins in UtilColorConv.c are now moved into CodecDecoder project
// However for compatibility with the previous version, the new header file is included here
//#include "ColorConvAPI.h"
/*
void RGB32_to_IYUV(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 bAlphaIsFront);
void RGB24_to_IYUV(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB);
void RGB16_to_IYUV(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 b555, INT32 bLE);
void RGB16_to_RGB24(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 b555, INT32 bLE);

//void YUV9_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
void YUV9_to_IYUV(BYTE * out, INT32 width, INT32 height, INT32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
void UYVY_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
void YVYU_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
void YUY2_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
void YV12_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
void YCbCr422_to_IYUV(BYTE *out, BYTE * in, INT32 w, INT32 h);

void IYUV_to_YUV9(BYTE* out, BYTE * in, INT32 w, INT32 h);
void IYUV_to_YUY2(BYTE* out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
void IYUV_to_UYVY(BYTE* out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
void IYUV_to_YVYU(BYTE* out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
void IYUV_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
void YUV422P_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
void PLANE_to_PLANE(BYTE* out, DWORD32 o_pitch, BYTE* in, DWORD32 width, DWORD32 height, DWORD32 i_pitch);

void IYUV_to_RGB565LE(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
void IYUV_to_RGB16_555(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
void IYUV_to_BGR24(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
void IYUV_to_BGR24_BITMAP(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv);
void IYUV_to_BGRA(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
*/
// Morrir
void Mirror_PLANE(BYTE *src, DWORD32 i_pitch, DWORD32 width, DWORD32 height, DWORD32 mirror_mode);
////////////////////////
// Time
////////////////////////
FrTimeHandle FrTimeOpen(DWORD32 dwStart, float fRatio);
void FrTimeClose(FrTimeHandle hHandle);
BOOL FrTimeStart(FrTimeHandle hHandle);
BOOL FrTimeStop(FrTimeHandle hHandle);
BOOL FrTimeSetTimeRatio(FrTimeHandle hHandle, float fRatio);
BOOL FrTimeSetCurTime(FrTimeHandle hHandle, DWORD32 dwTime);
BOOL FrTimeSetRefTime(FrTimeHandle hHandle, DWORD32 dwTime);
DWORD32 FrTimeGetTime(FrTimeHandle hHandle);
DWORD32	FrTimeGetRefTime(FrTimeHandle hHandle);

////////////////////////
// Performance
////////////////////////
//#ifdef WIN32
//McPerfHandle UtilPerfOpen();
//void UtilPerfClose(McPerfHandle hHandle);
//BOOL UtilPerfInit(McPerfHandle hHandle);
//void UtilPerfUninit(McPerfHandle hHandle);
//INT32 UtilPerfAddCounter(McPerfHandle hHandle, const char *pszCounterName);
//BOOL UtilPerfRemoveCounter(McPerfHandle hHandle, INT32 nIndex);
//BOOL UtilPerfCollectQueryData(McPerfHandle hHandle);
//BOOL UtilPerfGetStatistics(McPerfHandle hHandle, UINT32 *nMin, UINT32 *nMax, UINT32 *nMean, INT32 nIndex);
//LONG UtilPerfGetCounterValue(McPerfHandle hHandle, INT32 nIndex);
//#endif

////////////////////////
// Stream
////////////////////////
DWORD32 UtilBin2HexString(BYTE *pInData, DWORD32	 dwInSize, BYTE* pOutData, DWORD32 dwOutBuf);
DWORD32 UtilMakeSmoothDuration(QWORD *pDuration, DWORD32 dwSize, BYTE* pOutData, DWORD32 dwOutBuf, QWORD qwTotalDuration, CHAR * pIndent1, CHAR* pIndent2);
VOID ConvQWORDTo8BYTE(QWORD a, BYTE* b);
//DWORD32 make_h264stream_rtp_format_from_ts_format(BYTE* pOutData, DWORD32 out_buff_size, BYTE* pInData, DWORD32 in_size);



// color conv with opencv



#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif		// _UTILAPI_H_
