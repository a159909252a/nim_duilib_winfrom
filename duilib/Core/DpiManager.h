#ifndef UI_CORE_DPI_MANAGER_H_
#define UI_CORE_DPI_MANAGER_H_

#pragma once

#include "duilib/Core/UiTypes.h"
#include "duilib/Core/DpiAwareness.h"

namespace ui
{
/** @class DpiManager
* @brief DPI适配管理类
* @copyright (c) 2016, NetEase Inc. All rights reserved
* @author Redrain
* @date 2016/10/10
*/
class Window;

/** DPI感知功能的接口
*/
class UILIB_API DpiManager
{
public:
	DpiManager();
	~DpiManager();
	DpiManager(const DpiManager&) = delete;
	DpiManager& operator = (const DpiManager&) = delete;

public:
	/** 初始化DPI感知模式和DPI值（该函数只可调用一次，后续调用被忽略）
	* @param [in] dpiInitParam 初始化参数，详见参数说明
	*/
	void InitDpiAwareness(const DpiInitParam& dpiInitParam);

	/** DPI值是否为用户自定义(调用InitDpiAwareness函数时，dpiInitParam.m_dpiFlag == DpiInitParam::DpiFlag::kFromUserDefine)
	*/
	bool IsUserDefineDpi() const;

	/** 获取进程的DPI感知模式
	*/
	DpiAwarenessMode GetDpiAwareness() const;

	/** 是否DPI为每显示器模式
	*/
	bool IsPerMonitorDpiAware() const;

	/** 当DPI发生变化时，按配置检测更新DPI值为新值
	* @param [out] nOldDPI 如果有更新，返回原来的DPI值
	* @return 如果更新过，返回true，否则返回false
	*/
	bool UpdateDPI(uint32_t& nOldDPI);

	/** 根据窗口的DPI值，初始化DPI管理器的DPI值
	    (Win10 以后，每个显示器可以有不同的DPI值，所以不同的窗口可能有不同的DPI值)
	 @param [in] pWindow 窗口的接口，如果为nullptr，则读取系统配置的DPI值
	*/
	void SetDpiByWindow(Window* pWindow);

	/** 设置DPI值，并更新界面缩放比
	*   DPI值会影响界面缩放比，常用的值关系如下：
	*   DPI 值    缩放百分比(Scale值)
	* -------------------------------
	*   96        100%
	*   120       125%
	*   144       150%
	*   168       175%
	*   192       200%
	* @param [in] uDPI DPI值，如果设置为96，则代表界面无缩放
	*/
	void SetDPI(uint32_t uDPI);

public:
	/** 获取DPI值
	*/
	uint32_t GetDPI() const;

	/** 获取当前界面缩放比（100代表无缩放）
	* @return 缩放比，比如：如果返回缩放比为125，则对应DPI值是120
	*/
	uint32_t GetScale() const;

	/** 当前界面是否有DPI缩放
	* @return 如果当前界面缩放比为100返回false，否则返回true
	*/
	bool IsScaled() const;

public:
	/** 根据界面缩放比来缩放整数
	* @param[in] iValue 整数
	* @return int 缩放后的值
	*/
	int32_t ScaleInt(int32_t& iValue) const;
	int32_t GetScaleInt(int32_t iValue) const;
	uint32_t GetScaleInt(uint32_t iValue) const;

	/** 根据界面缩放比来缩放SIZE
	* @param[in] size 需要缩放的Size引用
	*/
	void ScaleSize(SIZE& size) const;
	void ScaleSize(UiSize& size) const;
	UiSize GetScaleSize(UiSize size) const;

	/** 根据界面缩放比来缩放POINT
	* @param[in] point 需要缩放的point引用
	*/
	void ScalePoint(POINT& point) const;
	void ScalePoint(UiPoint& point) const;

	/** 根据界面缩放比来缩放RECT
	* @param[in] rect 需要缩放的rect引用
	* @return void	无返回值
	*/
	void ScaleRect(RECT& rect) const;
	void ScaleRect(UiRect& rect) const;
	void ScalePadding(UiPadding& padding) const;
	void ScaleMargin(UiMargin& margin) const;

private:
	/** 获取主显示器DPI，开启DPI感知后有效
	* @return 返回 DPI值
	*/
	uint32_t GetMainMonitorDPI();

	/** MulDiv 函数封装
	*/
	int32_t MulDiv(int32_t nNumber, int32_t nNumerator, int32_t nDenominator) const;

private:
	/** 是否已经初始化过
	*/
	bool m_bDpiInited;

	/** 当前进程的DPI感知模式
	*/
	DpiAwarenessMode m_dpiAwarenessMode;

	/** DPI值是否为用户自定义
	*/
	bool m_bUserDefineDpi;

	/** 当前的DPI值
	*/
	uint32_t m_uDpi;

	/** DPI缩放因子，100表示无缩放
	*/
	uint32_t m_nScaleFactor;
};
}
#endif //UI_CORE_DPI_MANAGER_H_