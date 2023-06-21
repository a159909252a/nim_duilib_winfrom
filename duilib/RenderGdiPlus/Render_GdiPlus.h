#ifndef UI_RENDER_GDIPLUS_RENDER_H_
#define UI_RENDER_GDIPLUS_RENDER_H_

#pragma once

#include "duilib/Render/IRender.h"

namespace ui 
{

class UILIB_API Render_GdiPlus : public IRender
{
public:
	Render_GdiPlus();
	virtual ~Render_GdiPlus();

	virtual bool Resize(int width, int height) override;
	virtual void Clear() override;
	virtual std::unique_ptr<IRender> Clone() override;

	/** 分离位图
	*@return 返回位图接口，返回后由调用方管理资源（包括释放资源等）
	*/
	virtual IBitmap* DetachBitmap();

	virtual int	GetWidth() override;
	virtual int GetHeight() override;
	virtual void ClearAlpha(const UiRect& rcDirty, int alpha = 0) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding, int alpha) override;
	virtual void RestoreAlpha(const UiRect& rcDirty, const UiRect& rcShadowPadding = UiRect()) override;

	virtual bool IsRenderTransparent() const override;
	virtual bool SetRenderTransparent(bool bTransparent) override;

	virtual UiPoint OffsetWindowOrg(UiPoint ptOffset) override;
	virtual UiPoint SetWindowOrg(UiPoint ptOffset) override;
	virtual UiPoint GetWindowOrg() const override;

	virtual void SaveClip(int& nState) override;
	virtual void RestoreClip(int nState) override;
	virtual void SetClip(const UiRect& rc) override;
	virtual void SetRoundClip(const UiRect& rc, int width, int height) override;
	virtual void ClearClip() override;

	virtual bool BitBlt(int x, int y, int cx, int cy, IBitmap* pSrcBitmap, int xSrc, int ySrc, RopMode rop) override;
	virtual bool BitBlt(int x, int y, int cx, int cy, IRender* pSrcRender, int xSrc, int ySrc, RopMode rop) override;
	virtual bool StretchBlt(int xDest, int yDest, int widthDest, int heightDest, IRender* pSrcRender, int xSrc, int ySrc, int widthSrc, int heightSrc, RopMode rop) override;
	virtual bool AlphaBlend(int xDest, int yDest, int widthDest, int heightDest, IRender* pSrcRender, int xSrc, int ySrc, int widthSrc, int heightSrc, uint8_t alpha = 255) override;

	/** 绘制图片（采用九宫格方式绘制图片）
	* @param [in] rcPaint 当前全部可绘制区域（用于避免非可绘制区域的绘制，以提高绘制性能）
	* @param [in] pBitmap 用于绘制的位图接口
	* @param [in] rcImageDest 绘制的目标区域
	* @param [in] rcImageSource 绘制的源图片区域
	* @param [in] rcImageCorners 绘制源图片的边角信息，用于九宫格绘制
	* @param [in] bBitmapDpiScaled 位图尺寸是否已经做过DPI适应
	* @param [in] uFade 透明度（0 - 255）
	* @param [in] xtiled 横向平铺
	* @param [in] ytiled 纵向平铺
	* @param [in] fullxtiled 如果为true，横向平铺绘制时，确保是完整绘制图片，该参数仅当xtiled为true时有效
	* @param [in] fullytiled 如果为true，纵向平铺绘制时，确保是完整绘制图片，该参数仅当ytiled为true时有效
	* @param [in] nTiledMargin 平铺绘制时，图片的横向、纵向间隔，该参数仅当xtiled为true或者ytiled为true时有效
	*/
	virtual void DrawImage(const UiRect& rcPaint, 
						   IBitmap* pBitmap, 
						   const UiRect& rcImageDest, 
						   UiRect rcImageSource, 
						   UiRect rcImageCorners,
						   bool bBitmapDpiScaled = false,
						   uint8_t uFade = 255,
						   bool xtiled = false, 
						   bool ytiled = false, 
						   bool fullxtiled = true, 
						   bool fullytiled = true, 
						   int nTiledMargin = 0) override;

	virtual void DrawLine(const UiPoint& pt1, const UiPoint& pt2, UiColor penColor, int nWidth) override;

	virtual void DrawRect(const UiRect& rc, UiColor penColor, int nWidth) override;
	virtual void FillRect(const UiRect& rc, UiColor dwColor, uint8_t uFade = 255) override;

	virtual void DrawRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor penColor, int nWidth) override;
	virtual void FillRoundRect(const UiRect& rc, const UiSize& roundSize, UiColor dwColor, uint8_t uFade = 255) override;

	virtual void DrawPath(const IPath* path, const IPen* pen) override;
	virtual void FillPath(const IPath* path, const IBrush* brush) override;

	virtual UiRect MeasureString(const std::wstring& strText,
		                         const std::wstring& strFontId, 
								 uint32_t uFormat, int width = DUI_NOSET_VALUE) override;


	virtual void DrawString(const UiRect& rc, const std::wstring& strText,
		                    UiColor dwTextColor, const std::wstring& strFontId, 
							uint32_t uFormat, uint8_t uFade = 255) override;

	void DrawBoxShadow(const UiRect& rc, const UiSize& roundSize, const UiPoint& cpOffset, int nBlurRadius, int nSpreadRadius, UiColor dwColor, bool bExclude) override;

#ifdef UILIB_IMPL_WINSDK
	/** 获取DC句柄，当不使用后，需要调用ReleaseDC接口释放资源
	*/
	virtual HDC GetDC() override;

	/** 释放DC资源
	* @param [in] hdc 需要释放的DC句柄
	*/
	virtual void ReleaseDC(HDC hdc) override;
#endif

private:
	/** 获取GDI的光栅操作代码
	*/
	DWORD GetRopMode(RopMode rop) const;

private:
	/** DC句柄
	*/
	HDC m_hDC;

	/** 保存的DC的状态
	*/
	int m_saveDC;

	/** 是否指定为支持透明
	*/
	bool m_bTransparent;

	/** DC原来关联的位图句柄
	*/
	HBITMAP m_hOldBitmap;

	/** Render所用的位图
	*/
	HBITMAP	m_hBitmap;

	/** Render所用的位图数据
	*/
	BYTE* m_pPiexl;

	/** Render所用的位图宽度
	*/
	int m_nWidth;

	/** Render所用的位图高度
	*/
	int m_nHeight;
};

} // namespace ui

#endif // UI_RENDER_GDIPLUS_RENDER_H_
