
#ifndef _DEVICEOUTPUTAPI_H_
#define _DEVICEOUTPUTAPI_H_

#include "SystemAPI.h"
#include "mediainfo.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

/////////////////////////////////////////////////////
// VideoOut
typedef	void*	McVideoOutHandle;


LRSLT McVideoOutOpen(McVideoOutHandle* phDevice, FrVideoInfo* hInfo, HANDLE hWnd, BOOL bFull);
void McVideoOutClose(McVideoOutHandle hDevice);


BOOL McVideoOutSetWndHandle(McVideoOutHandle hDevice, HANDLE hWnd);
BOOL McVideoOutSetTextWndHandle(McVideoOutHandle hHandle, HANDLE hTextWnd);
BOOL McVideoOutConversion(McVideoOutHandle hDevice, FrRawVideo* pMedia);
BOOL McVideoOutClear(McVideoOutHandle hDevice);
BOOL McVideoOutReset(McVideoOutHandle hHandle);
BOOL McVideoOutPlay(McVideoOutHandle hDevice);
BOOL McVideoOutPlayWaitForVSync(McVideoOutHandle hHandle);
//BOOL McVideoOutPutString(McVideoOutHandle hDevice, McMediaText* strString);
BOOL McVideoOutPlayString(McVideoOutHandle hDevice);
BOOL McVideoOutSetVSync(McVideoOutHandle hHandle, BOOL bVSync);
BOOL McVideoOutSetDisplayType(McVideoOutHandle hDevice, DWORD dwType, DWORD dwRatioX, DWORD dwRatioY);
BOOL McVideoOutSetTextDisplay(McVideoOutHandle hDevice, BOOL bTextShowHide, DWORD dwType,DWORD dwUpPos,DWORD dwDownPos,DWORD dwLeftPos,DWORD dwRightPos);
BOOL McVideoOutSetTextFont(McVideoOutHandle hDevice, DWORD dwSize,BOOL bBold, BOOL bUserDefineColor, DWORD dwUserDefineColor , CHAR *szFontName);
BOOL McVideoOutSetStatusString(McVideoOutHandle hDevice, CHAR *szStatusString,INT32 nColor, INT32 nFontSize);
BOOL McVideoOutSetPerfInfoString(McVideoOutHandle hDevice, CHAR *szStatusString, INT32 nColor, INT32 nFontSize);

BOOL McVideoOutSetLogo(McVideoOutHandle hHandle, CHAR* pszLogoFile, DWORD32 dwInterval, DWORD32 dwDuration, FLOAT fStartW, FLOAT fSizeW, FLOAT fStartH, FLOAT fSizeH);


#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _DEVICEOUTPUTAPI_H_
