
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <conio.h>
#endif

#ifndef _LOGIFACE_H_
#define _LOGIFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BUF_LENG		10240			/// < Max size for log buffer

typedef signed int	LOG4RESULT;
typedef	signed int	LOG4LONG;
//typedef signed int	INT32;

/**
@remarks logger handler 타입
*/
typedef void* FrLogHandle;

enum {
	e_LOG_FATAL = 0x01,	
	e_LOG_ASSERT = 0x02,
	e_LOG_ERROR = 0x04,	
	e_LOG_WARN = 0x08,	
	e_LOG_INFO = 0x10,	
	e_LOG_DEBUG = 0x20,	
	e_LOG_TRACE = 0x40
};


/**
@remarks global variable for log handle
*/
extern FrLogHandle	gLogHandle;


/**
@remarks 시스템 에러를 출력하는 레벨
*/
#define FDR_LOG_FATAL	(char*)__FILE__, __LINE__, e_LOG_FATAL

/**
@remarks Application 에러를 출력하는 레벨
*/
#define FDR_LOG_ERROR	(char*)__FILE__, __LINE__, e_LOG_ERROR

/**
@remarks Application 운영상 경고할 내용을 출력하는 레벨
*/
#define FDR_LOG_WARN	(char*)__FILE__, __LINE__, e_LOG_WARN

/**
@remarks Application의 실행 상태를 간단히 출력하는 레벨, 운영자 및 고객이 상태를 조회할 때 사용
*/
#define FDR_LOG_INFO	(char*)__FILE__, __LINE__, e_LOG_INFO

/**
@remarks Application 디버깅 용도로 사용되는 레벨, 최소한의 디버깅 내용을 출력할 때 사용
*/
#define FDR_LOG_DEBUG	(char*)__FILE__, __LINE__, e_LOG_DEBUG

/**
@remarks Application의 상세한 동작 내용을 디버깅 할 때 사용되는 레벨, 매우 많은 내용의 출력이거나 고객에게 공개하기 어려운 내용일 경우 사용
*/
#define FDR_LOG_TRACE	(char*)__FILE__, __LINE__, e_LOG_TRACE

/**
######################################################################
#%p  debug, info, warn, error, fatal 등의 priority 가 출력된다.
#%m  로그내용이 출력됩니다
#%d  로깅 이벤트가 발생한 시간을 기록합니다.
#  포맷은 %d{HH:mm:ss, SSS}, %d{yyyy MMM dd HH:mm:ss, SSS}같은 형태로 사용하며 SimpleDateFormat에 따른 포맷팅을 하면 된다
#%t  로그이벤트가 발생된 쓰레드의 이름을 출력합니다.
#%%  % 표시를 출력하기 위해 사용한다.
#%n  플랫폼 종속적인 개행문자가 출력된다. \r\n 또는 \n 일것이다.
#%c  카테고리를 표시합니다
#  예) 카테고리가 a.b.c 처럼 되어있다면 %c{2}는 b.c가 출력됩니다.
#%C  클래스명을 포시합니다.
#  예)클래스구조가 org.apache.xyz.SomeClass 처럼 되어있다면 %C{2}는 xyz.SomeClass 가 출력됩니다
#%F  로깅이 발생한 프로그램 파일명을 나타냅니다.
#%l  로깅이 발생한 caller의 정보를 나타냅니다
#%L  로깅이 발생한 caller의 라인수를 나타냅니다
#%M  로깅이 발생한 method 이름을 나타냅니다.
#%r  어플리케이션 시작 이후 부터 로깅이 발생한 시점의 시간(milliseconds)
#%x  로깅이 발생한 thread와 관련된 NDC(nested diagnostic context)를 출력합니다.
#%X  로깅이 발생한 thread와 관련된 MDC(mapped diagnostic context)를 출력합니다.
#######################################################################

"[%d{yyyy.MM.dd HH:mm:ss,SSS}](%-14F,%4L)[%p] %m%n";  ///< Basic log pattern
**/


/**
@brief Open the console logger
@remarks 출력으로 console을 사용하는 logger handler를 생성한다.
@param phHandle logger handler
@param szUnitName logger 식별자 (각 로그 별로 Unique)
@param szPattern Pattern string (만약 NULL이면 default pattern 사용)
@return 처리 결과 코드
*/
LOG4RESULT FrLogConsoleOpen(FrLogHandle* phHandle, char *szUnitName, char *szPattern);

/**
@brief Open the file logger
@remarks 출력으로 파일을 사용하는 logger handler를 생성한다.
@param phHandle logger handler
@param szUnitName logger 식별자 (각 로그 별로 Unique)
@param szLogFilename 로그 파일 이름 (full path)
@param szPattern Pattern string (만약 NULL이면 default pattern 사용)
@return 처리 결과 코드
*/
LOG4RESULT FrLogFileOpen(FrLogHandle* phHandle, char *szUnitName, char *szLogFilename, char *szPattern, LOG4LONG nAppend);

/**
@brief Open the rolling file logger
@remarks 출력으로 파일을 사용하는 logger handler를 생성한다. 이때 파일은 지정된 조건에 따라 백업 생성하여 rolling된다.
@param phHandle logger handler
@param szUnitName logger 식별자 (각 로그 별로 Unique)
@param szLogFilename 로그 파일 이름 (full path)
@param szPattern Pattern string (만약 NULL이면 default pattern 사용)
@param nMaxFileSize 백업파일로 roll over 되는 파일 사이즈
@param nIndexCnt wraparound 되는 백업 파일의 최대 개수
@return 처리 결과 코드
*/
LOG4RESULT FrLogRollingFileOpen(FrLogHandle* phHandle, char *szUnitName, char *szLogFilename, char *szPattern, LOG4LONG nMaxFileSize, LOG4LONG nIndexCnt);

/**
@brief Open the daily rolling file logger
@remarks 출력으로 파일을 사용하는 logger handler를 생성한다. 이때 daily로 백업 생성하여 rolling된다.
@param phHandle logger handler
@param szUnitName logger 식별자 (각 로그 별로 Unique)
@param szLogFilename 로그 파일 이름 (full path)
@param szPattern Pattern string (만약 NULL이면 default pattern 사용)
@return 처리 결과 코드
*/
LOG4RESULT FrLogDailyRollingFileOpen(FrLogHandle* phHandle, char *szUnitName, char *szLogFilename, char *szPattern);

/**
@brief Open the external configured logger
@remarks 외부 설정파일로 부터 구성 정보를 참조하는 logger handler를 생성한다.
@param phHandle logger handler
@param szUnitName logger 식별자 (각 로그 별로 Unique)
@param szConfigFile 구성 정보 파일 이름
@return 처리 결과 코드
*/
LOG4RESULT FrLogOpenFromCFGFile(FrLogHandle* phHandle, char *szUnitName, char *szConfigFile);

/**
@brief Close the logger
@remarks logger handle를 정리한다.
@param phHandle logger handle
*/
void FrLogClose(FrLogHandle phHandle);

/**
@brief Select the log level
@remarks 로그 레벨을 설정한다.
@param phHandle logger handle
@param log_level 출력될 로그의 최소 레벨 (지정된 레벨 이상의 레벨은 모두 출력)
*/
LOG4RESULT FrLogSetLevel(FrLogHandle phHandle, int log_level);

/**
@brief Get the logger
@remarks 지정된 Unit Name의 logger 개체를 얻어온다.
@param phHandle logger handler
@param szUnitName logger 식별자 (각 로그 별로 Unique)
@return 처리 결과 코드
*/
LOG4RESULT FrGetLogger(FrLogHandle* phHandle, char *szUnitName);

/**
@brief Print the log message
@remarks 지정된 포맷의 메시지를 지정된 레벨의 로그로 출력한다. 출력 대상은 Open함수로 결정되거나 설정파일로 결정된다.
@param phHandle logger handle
@param file_name 로그 출력 요청한 소스 파일 이름 (full path)
@param file_line 로그 출력 요청한 소스 파일의 라인
@param log_level log level (로그 레벨의 순서 : LOG_FATAL, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_TRACE, LOG_DEBUG)
@param format 출력될 로그 패턴 (printf의 사용법과 동일)
*/
void FrLogPrn(FrLogHandle phHandle, char* file_name, int file_line, int log_level , const char *format, ... );

#ifdef __cplusplus
}
#endif

#endif //_LOGIFACE_H_
