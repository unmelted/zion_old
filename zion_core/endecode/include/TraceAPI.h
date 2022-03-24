
#ifndef _TRACEAPI_H_
#define _TRACEAPI_H_


#include "LogIface.h"


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

#define FDR_LOG_LEVEL_FATAL		1
#define	FDR_LOG_LEVEL_ERROR		2
#define	FDR_LOG_LEVEL_WARN		3
#define	FDR_LOG_LEVEL_INFO		4
#define	FDR_LOG_LEVEL_DEBUG		5
#define	FDR_LOG_LEVEL_TRACE		6


/**
@remarks global variable for log handle
*/
extern FrLogHandle         gLogHandle;


//#if !defined(BOOL) && !defined(_BOOL_DEFINED)
//#define _BOOL_DEFINED
//typedef int		BOOL;
//#endif
//
//#if !defined(DWORD32) && !defined(_DWORD32_DEFINED)
//#define _DWORD32_DEFINED
//typedef unsigned int				DWORD32;
//#endif
//
//#if !defined(BYTE) && !defined(_BYTE_DEFINED)
//#define _BYTE_DEFINED
//typedef unsigned char				BYTE;
//#endif

#undef TRACE
#undef TRACEX


#define TRACE(...)      FrLogPrn(gLogHandle, FDR_LOG_INFO, __VA_ARGS__)
#define TRACEX(A, ...)  FrLogPrn(gLogHandle, A, __VA_ARGS__)

#define LOG_I(...)   FrLogPrn(gLogHandle, FDR_LOG_INFO, __VA_ARGS__)
#define LOG_E(...)   FrLogPrn(gLogHandle, FDR_LOG_ERROR, __VA_ARGS__)
#define LOG_W(...)   FrLogPrn(gLogHandle, FDR_LOG_WARN, __VA_ARGS__)
#define LOG_D(...)   FrLogPrn(gLogHandle, FDR_LOG_DEBUG, __VA_ARGS__)
#define LOG_T(...)   FrLogPrn(gLogHandle, FDR_LOG_TRACE, __VA_ARGS__)

void BoxQuestion (const char* msg);
void BoxError(const char* msg);
void BoxWarning(const char* msg);
void BoxInform(const char* msg);

//void SPRINTF(char* pStrBuff, char* lpszFormat, ...);

void* FILEDUMPOPEN(char* pFilename, char* pExt, unsigned int dwHandle);
void FPRINTF(void* hFile, char* lpszFormat, ...);
void FILEDUMP(void* hFile, unsigned char* pData, unsigned int dwSize);

//void DebugPrintf(char* lpszFormat, ...);
//void DebugPrintfEx(char* file_name, INT32 file_line, INT32 log_level, char* lpszFormat, ...);
//void DebugFprintf(char* lpszFormat, ...);
//void DebugFprintfEx(char* file_name, INT32 file_line, INT32 log_level, char* lpszFormat, ...);

int SetTraceLogLevel(int dwLogLevel);
int FrSetTraceFileName(char* lpszFileName);
int SetTraceRollingFileName(char* lpszFileName, int nMaxFileSize, int nIndexCnt);
//BOOL SetTraceFileNameEx(char* lpszFileName, INT32 nMode);
//BOOL RenameTraceFile(char* lpszFileName);
int ReleaseTraceFileName();

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif		// _TRACEAPI_H_
