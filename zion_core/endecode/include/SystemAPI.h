/*****************************************************************************
*                                                                            *
*                            System Library									 *
*                                                                            *
******************************************************************************

    File Name       : SystemAPI.h
    Author(s)       : mwkang
    Created         : 2021.05.13

    Description     : System API
    Notes           :

==============================================================================
                      Modification History (Reverse Order)
==============================================================================
    Date        Author      Location (variables) / Description
------------------------------------------------------------------------------

------------------------------------------------------------------------------*/

#ifndef _SYSTEMAPI_H_
#define _SYSTEMAPI_H_

/****************************************/
/*		Default Header Files			*/
/****************************************/
#include "mediaerror.h"

/****************/
/*	   WIN32	*/
/****************/
#if defined(WIN32)

// 아래 지정된 플랫폼에 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER						// Windows Server 2008 이상에서만 기능을 사용할 수 있습니다.
#define WINVER _WIN32_WINNT_WS08	// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT					// Windows Server 2008 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT _WIN32_WINNT_WS08	// 다른 버전의 Windows에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINDOWS						// Windows 98 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS _WIN32_WINNT_WS08	// Windows Me 이상에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 6.0 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0600	// 다른 버전의 IE에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <locale.h>
#include <assert.h>
#include <io.h>
#include <process.h>
#include <tchar.h>			// unicode macro _tfopen, _T()
#include <stdint.h>

//#ifdef UNICODE
//typedef WCHAR TCHAR;
//#else
//typedef CHAR TCHAR;
//#endif

//#ifdef USING_OPNENCV
//#include <opencv2/opencv.hpp>
//#if defined(_DEBUG)
//#pragma comment(lib, "opencv_core440d.lib")
//#pragma comment(lib, "opencv_highgui440d.lib")
//#pragma comment(lib, "opencv_imgproc440d.lib")
//#pragma comment(lib, "opencv_imgcodecs440d.lib")
//#pragma comment(lib, "opencv_calib3d440d.lib")
//#pragma comment(lib, "opencv_cudaimgproc440d.lib")
//#pragma comment(lib, "opencv_cudawarping440d.lib")
//#pragma comment(lib, "opencv_cudaarithm440d.lib")
//#else
//#pragma comment(lib, "opencv_core440.lib")
//#pragma comment(lib, "opencv_highgui440.lib")
//#pragma comment(lib, "opencv_imgproc440.lib")
//#pragma comment(lib, "opencv_imgcodecs440.lib")
//#pragma comment(lib, "opencv_calib3d440.lib")
//#pragma comment(lib, "opencv_cudaimgproc440.lib")
//#pragma comment(lib, "opencv_cudawarping440.lib")
//#pragma comment(lib, "opencv_cudaarithm440.lib")
//#endif
//#endif

#if _DEBUG
#include<crtdbg.h>
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif


#endif


/****************/
/*	   Unix	*/
/****************/

#if defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <errno.h>
#include <wchar.h>  // support wcsxxx
#include <semaphore.h>	// posix sema

#define	CP_ACP  0
#define	WCHAR	wchar_t
#define CHAR	char

#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */

#define TCHAR   char
#define _T(A)   A

#define _tcscpy(A, B)                   strcpy(A, B)
#define _tcsncpy(A,B,C)                 strncpy(A,B,C)
#define _tcscmp(A, B)                   strcmp(A, B)
#define _tcsncmp(A,B,C)                 strncmp(A,B,C)
#define _tcslen(A)                      strlen(A)
#define _tcsnlen(A,B)                   strnlen(A,B)
#define _tcschr(A, B)                   strchr(A, B)
#define _tcsrchr(A, B)                  strrchr(A, B)
#define _tcsstr(A, B)                   strstr(A, B)
#define _tcscat(A,B)                    strcat(A,B)
#define _stprintf(A,B, ...)             sprintf(A,B, __VA_ARGS__)
#define	_tcstok(A, B)					strtok(A,B)
#define sprintf_s(A,B, ...)             snprintf(A,B, __VA_ARGS__)


#define stricmp(A,B)		strcasecmp((A),(B))
#define strnicmp(A,B,C)		strncasecmp((A),(B),(C))
#define _strnicmp(A,B,C)		strncasecmp((A),(B),(C))
#define	_atoi64(A)			atoll((CHAR*)(A))

#endif // defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

/****************************************/
/*		Type Definition					*/
/****************************************/
#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE						0

#ifdef	TRUE
#undef	TRUE
#endif
#define TRUE						1

#ifdef	INFINITE
#undef	INFINITE
#endif
#define	INFINITE					0x7FFFFFFFL

#ifdef	MAX_QWORD
#undef	MAX_QWORD
#endif
#define	MAX_QWORD					0xFFFFFFFFFFFFFFFFLL

#ifdef	MAX_DWORD
#undef	MAX_DWORD
#endif
#define	MAX_DWORD					0xFFFFFFFFL

#ifdef	MAX_WORD
#undef	MAX_WORD
#endif
#define	MAX_WORD					0xFFFF

#ifdef	MAX_BYTE
#undef	MAX_BYTE
#endif
#define	MAX_BYTE					0xFF

#ifdef	MIN_INT64
#undef	MIN_INT64
#endif
#define	MIN_INT64					0x8000000000000000LL

#ifndef M_PI
#define M_PI						3.14159265358979323846
#endif

#ifdef MAX_PATH
#undef MAX_PATH
#endif
#define MAX_PATH					(260 * sizeof(TCHAR))

#define MAX_URL_LENGTH				(1024 * 10)
#define MAX_LONG_PATH_LENGTH		4096
#define MAX_LONG_STRING_LENG		1024
#define	MAX_STRING_LENGTH			512
#define MAX_DEVICE_NUM				10
#define	MAX_IPLIST_NUM				16
#define	MAX_NAS_NUM					32
#define	MAX_L4_NUM					16

#define DEF_LENGTH_20480			20480
#define DEF_LENGTH_10240			10240
#define DEF_LENGTH_8192				8192
#define DEF_LENGTH_4096				4096
#define DEF_LENGTH_2048				2048
#define DEF_LENGTH_1024				1024
#define DEF_LENGTH_512				512
#define DEF_LENGTH_256				256
#define DEF_LENGTH_128				128
#define DEF_LENGTH_64				64
#define DEF_LENGTH_32				32
#define DEF_LENGTH_16				16

#ifndef MIN
#define MIN(a,b)					((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)					((a) > (b) ? (a) : (b))
#endif

#ifndef	MC_T
#define	MC_T(a)						(a)
#endif

#ifndef	WIN32
#define	HANDLE						DWORD
#endif

#if !defined(BYTE) && !defined(_BYTE_DEFINED)
#define _BYTE_DEFINED
typedef unsigned char				BYTE;
#endif

#if !defined(WORD) && !defined(_WORD_DEFINED)
#define _WORD_DEFINED
typedef unsigned short				WORD;
#endif

#if !defined(DWORD) && !defined(_DWORD_DEFINED)
#define _DWORD_DEFINED
typedef unsigned long				DWORD;
#endif

#if !defined(DWORD32) && !defined(_DWORD32_DEFINED)
#define _DWORD32_DEFINED
typedef unsigned int				DWORD32;
#endif

#if !defined(LONG32) && !defined(_LONG32_DEFINED)
#define _LONG32_DEFINED
typedef signed int				    LONG32;
#endif

#if !defined(QWORD) && !defined(_QWORD_DEFINED)
#define _QWORD_DEFINED
#ifdef  WIN32
typedef unsigned __int64			QWORD;
#else
typedef unsigned long long int		QWORD;
#endif
#endif

#if !defined(LPPTR) && !defined(_LPPTR_DEFINED)
#define _LPPTR_DEFINED
typedef unsigned long				LPPTR;
#endif

#if defined(SUPPORT_IOS)
#define BOOL int
#endif

#if !defined(BOOL) && !defined(_BOOL_DEFINED)
#define _BOOL_DEFINED
#ifndef OBJC_BOOL_DEFINED
typedef int							BOOL;
#endif // OBJC_BOOL_DEFINED
#endif
      
#if _MSC_VER < 1400
#if !defined(WCHAR) && !defined(_WCHAR_DEFINED)
#define _WCHAR_DEFINED
typedef unsigned short				WCHAR;
#endif
#endif

#if !defined(UCS4) && !defined(_UCS4_DEFINED)
#define _UCS4_DEFINED
typedef unsigned int				UCS4;
#endif

#if !defined(UCS2) && !defined(_UCS2_DEFINED)
#define _UCS2_DEFINED
typedef unsigned short				UCS2;
#endif

#if !defined(UTF32) && !defined(_UTF32_DEFINED)
#define _UTF32_DEFINED
typedef unsigned int				UTF32;
#endif

#if !defined(UTF16) && !defined(_UTF16_DEFINED)
#define _UTF16_DEFINED
typedef unsigned short				UTF16;
#endif

#if !defined(UTF8) && !defined(_UTF8_DEFINED)
#define _UTF8_DEFINED
typedef unsigned char				UTF8;
#endif

#if !defined(UINT) && !defined(_UINT_DEFINED)
#define _UINT_DEFINED
typedef unsigned int				UINT;
#endif

#if !defined(INT) && !defined(_INT_DEFINED)
#define _INT_DEFINED
typedef signed int				    INT;
#endif

#if !defined(LRSLT) && !defined(_LRSLT_DEFINED)
#define _LRSLT_DEFINED
typedef signed int					LRSLT;
#endif

#if !defined(LONG) && !defined(_LONG_DEFINED)
#define _LONG_DEFINED
typedef signed long					LONG;
#endif

#if !defined(ULONG) && !defined(_ULONG_DEFINED)
#define _ULONG_DEFINED
typedef unsigned long				ULONG;
#endif

#if !defined(INT32) && !defined(_INT32_DEFINED)
#define _INT32_DEFINED
#ifdef  WIN32
typedef __int32						INT32;
#elif defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
typedef int32_t						INT32;
#else
typedef long         				INT32;
#endif
#endif

#if !defined(INT64) && !defined(_INT64_DEFINED)
#define _INT64_DEFINED
#ifdef  WIN32
typedef __int64						INT64;
#elif defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
typedef int64_t						INT64;
#else
typedef long long int				INT64;
#endif
#endif

#if !defined(UINT32) && !defined(_UINT32_DEFINED)
#define _UINT32_DEFINED
#ifdef  WIN32
typedef unsigned __int32			UINT32;
#elif defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
typedef uint32_t					UINT32;
#else
typedef unsigned long  				UINT32;
#endif
#endif

#if !defined(UINT64) && !defined(_UINT64_DEFINED)
#define _UINT64_DEFINED
#ifdef  WIN32
typedef unsigned __int64			UINT64;
#elif defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
typedef uint64_t					UINT64;
#else
typedef unsigned long long 			UINT64;
#endif
#endif

#if !defined(VOID) && !defined(_VOID_DEFINED)
#define _VOID_DEFINED
typedef	void	VOID;
#endif

#if !defined(CHAR) && !defined(_CHAR_DEFINED)
#define _CHAR_DEFINED
typedef	char	CHAR;
#endif

#if !defined(UCHAR) && !defined(_UCHAR_DEFINED)
#define _UCHAR_DEFINED
typedef	unsigned char	UCHAR;
#endif

#if !defined(SHORT) && !defined(_SHORT_DEFINED)
#define _SHORT_DEFINED
typedef	short	SHORT;
#endif

#if !defined(USHORT) && !defined(_USHORT_DEFINED)
#define _USHORT_DEFINED
typedef	unsigned short	USHORT;
#endif

#if !defined(DOUBLE) && !defined(_DOUBLE_DEFINED)
#define _DOUBLE_DEFINED
typedef	double	DOUBLE;
#endif

#if !defined(FLOAT) && !defined(_FLOAT_DEFINED)
#define _FLOAT_DEFINED
typedef	float	FLOAT;
#endif

#if !defined(VOID) && !defined(_VOID_DEFINED)
#define _VOID_DEFINED
typedef	void	VOID;
#endif

#if !defined(INLINE) && !defined(_INLINE_DEFINED)
#define _INLINE_DEFINED
#ifdef  WIN32
#define	 _INLINE_		_inline
#elif defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
#define	 _INLINE_		inline
#else
#define	 _INLINE_		inline
#endif
#endif

// x264
#if !defined(uint8_t) && !defined(_UINT8_DEFINED)
#define _UINT8_DEFINED
typedef unsigned char  uint8_t;
#endif

#if defined(Win32)
#ifdef _INT64_DEFINED
typedef INT64	int64_t;
#endif
#endif

#if !defined(GUID_TYPE) && !defined(_GUID_TYPE_DEFINED)
#define _GUID_TYPE_DEFINED
typedef struct {DWORD32 v1; WORD v2; WORD v3; BYTE v4[8];}	GUID_TYPE;
#endif

#if !defined(WIN32) && !defined(_GUID_DEFINED)
#define _GUID_DEFINED
typedef struct {DWORD32 Data1; WORD Data2; WORD Data3; BYTE Data4[8];}	GUID;
#endif


typedef	LPPTR	TASK_HANDLE, *PTASK_HANDLE;
typedef	LPPTR	EVENT_HANDLE, *PEVENT_HANDLE;
#ifdef  WIN32
typedef LPPTR	SEMA_HANDLE, *PSEMA_HANDLE;
#elif defined(Linux) || defined(SunOS) || defined(HPUX) || defined(MINGW) || defined(SUPPORT_ANDROID) || defined(SUPPORT_IOS)
typedef sem_t	SEMA_HANDLE, *PSEMA_HANDLE;
#else
typedef LPPTR	SEMA_HANDLE, *PSEMA_HANDLE;
#endif



typedef	struct {
	void*		m_pFile;
    int         m_nFd;
	BYTE*		m_pBuffer;
	BYTE*		m_pPos;
	BYTE*		m_pFileEnd;
	BOOL		m_bMem;
	DWORD32		m_dwLen;
	DWORD32		m_dwBuffLen;
	BOOL		m_bHTTPSource;


	BOOL		m_bUsedFdOpen;

	QWORD		m_qwFileSize;
	QWORD		m_qwTrueFilePos;
	QWORD		m_qwPhyFilePos;

	BOOL		m_bBufferIsValid;
	QWORD		m_qwBufFilePosStart;
	QWORD		m_qwBufFilePosEnd;
	BYTE*		m_pCurBufferPos;
	
	BYTE*		m_pReadBuffer;
	DWORD32     m_dwReadBufferSize;
	DWORD32     m_dwMaxReadBufferSize;

} FILE_STRUCT, *FILE_HANDLE;

#ifdef	WIN32
typedef	CRITICAL_SECTION		MUTEX_HANDLE, *PMUTEX_HANDLE;
#else
typedef	LPPTR					MUTEX_HANDLE, *PMUTEX_HANDLE;
#define	CALLBACK
#endif

typedef void *(*PTASK_START_ROUTINE)(void*);
typedef void *(CALLBACK *PCALLBACK_ROUTINE)(void*);
typedef void *(CALLBACK *PCALLBACK_ROUTINE_EX)(void *, INT32);
typedef void *(CALLBACK *PCALLBACK_ROUTINE_EX2)(void *, void*);
typedef BOOL (CALLBACK *PCALLBACK_ROUTINE_RET)(void*);

typedef void (CALLBACK *CALLBACK_ROUTINE)(void*);
typedef void (CALLBACK *CALLBACK_ROUTINE_EX)(void *, void *);
typedef BOOL (CALLBACK *CALLBACK_ROUTINE_RET)(void *);
typedef BOOL (CALLBACK *CALLBACK_ROUTINE_RET2)(void *, void *);

typedef void (CALLBACK *SA_PCALLBACK_ROUTINE)(void *);
typedef void (CALLBACK *SA_PCALLBACK_ROUTINE_EX)(void *, INT32);
typedef void (CALLBACK *SA_PCALLBACK_ROUTINE_EX2)(void *, char *);

#define	INVALID_TASK			(TASK_HANDLE)-1
#define	INVALID_HANDLE			(EVENT_HANDLE)-1

/****************************************/
/*		Func Proto Type Definition		*/
/****************************************/

#define ISSPACE(c)		( (unsigned) ((c) == _T(' ')) )
#define ISRETURN(c)		( (unsigned) ((c) == _T('\r')) )
#define	ISLINEFEED(c)	( (unsigned) ((c) == _T('\n')) )
#define	ISWSPACE(c)		( (unsigned) ((c) == _T('\t')) || ((c) == _T(' ')) || ((c) == _T('\r')) || ((c) == _T('\n')) )
#define ISDIGIT(c)		( (unsigned) ((c) - _T('0')) < 10 )
#define ISHEXLOW(c)		( (unsigned) ( ((c)|32) - _T('a')) < 6 )
#define ISHEXHIGH(c)	( (unsigned) ( ((c)|32) - _T('A')) < 6 )
#define ISALPHALOW(c)	( (unsigned) ( ((c)|32) - _T('a')) < 26 )
#define ISALPHAHIGH(c)	( (unsigned) ( ((c)|32) - _T('A')) < 26 )
#define ISALPHA(c)		( ISALPHALOW(c) || ISALPHAHIGH(c) )
#define ISDOT(c)		( (unsigned) ((c) == _T('.')) )
#define ISCOLON(c)		( (unsigned) ((c) == _T(':')) )
#define	ISCOMMA(c)		( (unsigned) ((c) == _T(',')) )
#define	ISSEMICOLON(c)	( (unsigned) ((c) == _T(';')) )
#define ISEQUAL(c)		( (unsigned) ((c) == _T('=')) )
#define ISHIPON(c)		( (unsigned) ((c) == _T('-')) )
#define	ISNULL(c)		( (unsigned) ((c) == _T('\0')) )
#define	ISCRLF(c)		( ISRETURN(c) || ISLINEFEED(c) )
#define	ISENDLINE(c)	( ISRETURN(c) || ISLINEFEED(c) || ISNULL(c))
#define	ISADDR(c)		( ISALNUM(c) || ISDOT(c) || ISHIPON(c) )
#define	ISFLOAT(c)		( ISDIGIT(c) || ISDOT(c) )
#define	ISTIME(c)		( ISDIGIT(c) || ISDOT(c) || ISCOLON(c) )
#define ISALNUM(c)		( ISDIGIT(c) || ISALPHA(c) )
#define	ISHEXDIGIT(c)	( ISDIGIT(c) || ISHEXLOW(c) || ISHEXHIGH(c) )
#define	ISCHAR(c)		( ISALNUM(c) || ISDOT(c) || ((c) == _T('-')) || ((c) == _T('_')) || ((c) == _T('+')) || ((c) == _T('$')) ) 
#define ISDBLQUOTES(c)	((c) == _T('\"'))
#define	ISBASE64(c)		( ISALNUM(c) || ((c) == _T('/')) || ((c) == _T('+')) || ISEQUAL(c) || ISCOMMA(c) )
#define	ISSAFE(c)		( ISALNUM(c) || ((c) == _T('\'')) || ((c) == _T('`')) || ((c) == _T('-')) || ((c) == _T('.')) || ((c) == _T('/')) \
									 || ((c) == _T(':')) || ((c) == _T('?')) || ((c) == _T('\"')) || ((c) == _T('#')) || ((c) == _T('$')) \
									 || ((c) == _T('&')) || ((c) == _T('*')) || ((c) == _T(','))  || ((c) == _T('=')) || ((c) == _T('@')) \
									 || ((c) == _T('[')) || ((c) == _T(']')) || ((c) == _T('^')) || ((c) == _T('_')) || ((c) == _T('{')) \
									 || ((c) == _T('|')) || ((c) == _T('}')) || ((c) == _T('+')) || ((c) == _T('~')) )
#define ISSAFEFIRSTCHAR(c) ( ISALNUM(c) || ISDOT(c) || ((c) == _T('_')) )

#define ISWILDCARD(c)	( (unsigned) ((c) == _T('*')) )
#define ISQUESTION(c)	( (unsigned) ((c) == _T('?')) )
#define ISANGLE(c)		( (unsigned) ((c) == _T('<')) || ((c) == _T('>')))
#define ISBACKSLASH(c)	( (unsigned) ((c) == _T('\\')) )
#define ISNOFILENAME(c)	( ISWILDCARD(c) || ISQUESTION(c) || ISANGLE(c) \
										|| ISBACKSLASH(c)|| ISNULL(c) || ISCRLF(c) || ISENDLINE(c))

//////////////////
// Memory
//////////////////
void* MALLOC(DWORD32 dwSize);
void* MALLOCZ(DWORD32 dwSize);
void* REALLOC(void* pMem, DWORD32 dwSize);
void* CALLOC(DWORD32 dwNum, DWORD32 dwSize);
void FREE(void* pMem);

//////////////////
// File
//////////////////
#define	FILE_READ				1
#define	FILE_WRITE				2
#define	FILE_READ_WRITE			3
#define	FILE_CREATE				4
#define	FILE_MEM				8
#define FILE_TEXT				16
#define FILE_BINARY				32
#define FILE_OVERWRITE			64

#define	FDR_SEEK_SET				0
#define	FDR_SEEK_CUR				1
#define	FDR_SEEK_END				2

FILE_HANDLE	FrOpenFile(const CHAR* strFilename, INT32 eMode);
void FrCloseFile(FILE_HANDLE hFile);
INT32 FrReadFile(FILE_HANDLE hFile, BYTE* pBuf, DWORD32 dwSize);
INT32 FrRead8File(FILE_HANDLE hFile);
INT32 FrReadBE16File(FILE_HANDLE hFile);
INT32 FrReadLE16File(FILE_HANDLE hFile);
INT32 FrReadBE24File(FILE_HANDLE hFile);
INT32 FrReadLE24File(FILE_HANDLE hFile);
INT32 FrReadBE32File(FILE_HANDLE hFile, INT32* iRet);
INT32 FrReadLE32File(FILE_HANDLE hFile, INT32* iRet);
QWORD FrReadBE64File(FILE_HANDLE hFile, INT32* iRet);
QWORD FrReadLE64File(FILE_HANDLE hFile, INT32* iRet);
INT32 FrWriteFile(FILE_HANDLE hFile, char* pBuf, DWORD32 dwSize);
BOOL FrCheckFile(CHAR* pFilename);
INT32 FrAccessFile(const CHAR* pFilename, INT32 nMode);
INT32 FrSeekFile(FILE_HANDLE hFile, INT64 i64OffSet, INT32 iFromWhere);
QWORD FrSizeOfFile(FILE_HANDLE hFile);
QWORD FrTellFile(FILE_HANDLE hFile);
INT32 FrFlushFile(FILE_HANDLE hFile);


//////////////////
// Task
//////////////////
#ifndef	WIN32

#define THREAD_BASE_PRIORITY_LOWRT	15
#define THREAD_BASE_PRIORITY_MAX	2
#define THREAD_BASE_PRIORITY_MIN	-2
#define THREAD_BASE_PRIORITY_IDLE	-15

#define THREAD_PRIORITY_LOWEST			THREAD_BASE_PRIORITY_MIN
#define THREAD_PRIORITY_BELOW_NORMAL	(THREAD_PRIORITY_LOWEST+1)
#define THREAD_PRIORITY_NORMAL			0
#define THREAD_PRIORITY_HIGHEST			THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL	(THREAD_PRIORITY_HIGHEST-1)

#define THREAD_PRIORITY_TIME_CRITICAL	THREAD_BASE_PRIORITY_LOWRT
#define THREAD_PRIORITY_IDLE			THREAD_BASE_PRIORITY_IDLE

#endif
TASK_HANDLE McCreateTask(
			char*				strName,
			PTASK_START_ROUTINE pStartAddress,
			void*				lpParam,
			INT32				nPriority,
			DWORD32				dwStackSize,
			DWORD32				dwCreateFlag
			);
DWORD32 McResumeTask(TASK_HANDLE hTask);
DWORD32 McSuspendTask(TASK_HANDLE hTask);
void McCloseTask(TASK_HANDLE hTask);
void McExitTask(void);
BOOL McDeleteTask(TASK_HANDLE hTask);

//////////////////
// IPC
//////////////////
EVENT_HANDLE FrCreateEvent(char* strName);
BOOL	FrDeleteEvent(EVENT_HANDLE hEvent);
BOOL	FrResetEvent(EVENT_HANDLE hEvent);
BOOL	FrSendEvent(EVENT_HANDLE hEvent);
BOOL	FrWaitEvent(EVENT_HANDLE hEvent, DWORD32 dwMSec);

BOOL FrCreateSema(PSEMA_HANDLE phSema, UINT32 lInitCount, UINT32 lMaxCount, char* strName);
BOOL	FrDeleteSema(PSEMA_HANDLE phSema);
BOOL	Fr_P_Sema(PSEMA_HANDLE phSema, DWORD32 dwMSec);
BOOL	Fr_V_Sema(PSEMA_HANDLE phSema);

BOOL	FrCreateMutex(PMUTEX_HANDLE phMutex);
BOOL	FrDeleteMutex(PMUTEX_HANDLE phMutex);
BOOL	FrEnterMutex(PMUTEX_HANDLE phMutex);
BOOL	FrLeaveMutex(PMUTEX_HANDLE phMutex);

//////////////////
// System
//////////////////

void FrSleep(DWORD32 dwMilliseconds);
DWORD32 FrGetTickCount(void);
INT64 FrGetQueryFreq(void);






BOOL FrSysInit();
BOOL FrSysClose();


BOOL McSystemCOMInit();
void McSystemCOMClose();
DWORD32 RAND(void);

void McMicroSleep(DWORD32 dwMicroSeconds);

INT64 McGetMicroTickCount();
void McGetTime(WORD* pwHour, WORD* pwMin, WORD* pwSec, WORD *pwMil);
void McGetTimeByString(char *szTime);
void McGetDateByString(char *szDate);
void McGetDateFromTime(DWORD32 dwTime, WORD* pwHour, WORD* pwMin, WORD* pwSec, WORD *pwMil);
DWORD32 McGetProcessNum(void);
DWORD32 McGetPid(void);
DWORD32 McGetHostName(char* pchHostName, DWORD32 dwBufLeng);
DWORD32 McGetSubtract(DWORD32 dwBig, DWORD32 dwSmall);
INT64 McGetMicroSubtract(INT64 dwBig, INT64 dwSmall);
BOOL McGetDate(char* pchInFormat, char* pchOutFormat, INT32 nLengthOfOutFormat);
BOOL McCheckExecDup(char* pchBinId);
BOOL McGetFileVersion(char* pchBinName, char* pchVerInfo);
#if defined(WIN32) && !defined(_WIN32_WCE)
INT32 McGetDiffSecond(SYSTEMTIME sTm1, SYSTEMTIME sTm2);
#endif

INT64 McGetQueryFreq();
INT64 McGetQueryCounter();
DWORD32 McGetQueryDuringTime(INT64 nFreq, INT64 nEnd, INT64 nBegin);
INT64 McGetQueryDiffTime(INT64 nFreq, INT64 nEnd, INT64 nBegin);


#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _SYSTEMAPI_H_

