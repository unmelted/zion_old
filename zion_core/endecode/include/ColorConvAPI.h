#ifndef	__COLORCON_API__
#define	__COLORCON_API__

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

////////////////////////
// Color Conv
////////////////////////
VOID RGB32_to_IYUV(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 bAlphaIsFront);
VOID RGB24_to_IYUV(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB);
VOID RGB16_to_IYUV(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 b555, INT32 bLE);
VOID RGB16_to_BGR24(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 b555, INT32 bLE);
VOID RGB32_to_BGR24(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 bAlpahIsFront);
VOID RGB24_to_RGB555LE(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB);
VOID RGB32_to_RGB555LE(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 bAlphaIsFront);

//VOID YVU9_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
VOID YUV9_to_IYUV(BYTE * out, INT32 width, INT32 height, INT32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
VOID UYVY_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
VOID YVYU_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
VOID YUY2_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
VOID YV12_to_IYUV(BYTE* out, BYTE * in, INT32 w, INT32 h);
VOID YCbCr422_to_IYUV(BYTE *out, BYTE * in, INT32 w, INT32 h);

VOID IYUV_to_YUV9(BYTE* out, BYTE * in, INT32 w, INT32 h);
VOID IYUV_to_YUY2(BYTE* out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_UYVY(BYTE* out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_YVYU(BYTE* out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
VOID YUV422P_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
VOID YUV411P_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
VOID YUV420P10_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
VOID YUV422P10_to_IYUV(BYTE* out, DWORD32 width, DWORD32 height, DWORD32 pitch, BYTE* py, BYTE* pu, BYTE* pv, DWORD32 ypitch);
VOID PLANE_to_PLANE(BYTE* out, DWORD32 o_pitch, BYTE* in, DWORD32 width, DWORD32 height, DWORD32 i_pitch);

VOID IYUV_to_RGB565LE(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_RGB555LE(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_RGB16_555(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_BGR24(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);
VOID IYUV_to_BGR24_BITMAP(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv);
VOID IYUV_to_BGRA(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *pu, BYTE *pv, DWORD32 ypitch);

VOID NV12_to_BGR24(BYTE *out, INT32 width, INT32 height, INT32 pitch, BYTE *py, BYTE *puv, DWORD32 ypitch);

VOID RGB16_to_BGRA(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB, INT32 b555, INT32 bLE);
VOID BGR24_to_BGRA(BYTE *out, INT32 out_pitch, BYTE *input, INT32 in_pitch, INT32 width, INT32 height, INT32 bRGB);

// Morrir
VOID Mirror_PLANE(BYTE *src, DWORD32 i_pitch, DWORD32 width, DWORD32 height, DWORD32 mirror_mode);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif
