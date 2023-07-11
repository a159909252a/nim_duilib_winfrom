// Copyright (c) 2010-2011, duilib develop team(www.duilib.com).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted provided that the 
// following conditions are met.
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above 
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials 
// provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DUILIB_DEFS_H_
#define DUILIB_DEFS_H_ 1

#pragma once

#include "duilib/duilib_config.h"
#include "duilib/duilib_types.h"

#define DUI_NOSET_VALUE		-1
#define DUI_LENGTH_STRETCH  -1
#define DUI_LENGTH_AUTO		-2

namespace ui
{
	//定义所有控件类型
	#define  DUI_CTR_CONTROL                         (L"Control")
	#define  DUI_CTR_LABEL                           (L"Label")
	#define  DUI_CTR_RICHTEXT                        (L"RichText")
	#define  DUI_CTR_BUTTON                          (L"Button")
	#define  DUI_CTR_OPTION                          (L"Option")
	#define  DUI_CTR_CHECKBOX                        (L"CheckBox")

	#define  DUI_CTR_LABELBOX                        (L"LabelBox")
	#define  DUI_CTR_BUTTONBOX                       (L"ButtonBox")
	#define  DUI_CTR_OPTIONBOX                       (L"OptionBox")
	#define  DUI_CTR_CHECKBOXBOX                     (L"CheckBoxBox")

	#define  DUI_CTR_BOX							 (L"Box")
	#define  DUI_CTR_SCROLLBOX						 (L"ScrollBox")
	#define  DUI_CTR_HBOX							 (L"HBox")
	#define  DUI_CTR_VBOX                            (L"VBox")
	#define  DUI_CTR_TABBOX                          (L"TabBox")
	#define  DUI_CTR_TILEBOX						 (L"TileBox")
	#define  DUI_CTR_CHILDBOX                        (L"ChildBox")

	#define  DUI_CTR_LISTBOX_ELEMENT				 (L"ListBoxElement")
	#define  DUI_CTR_LISTBOX						 (L"ListBox")
	#define  DUI_CTR_HLISTBOX						 (L"HListBox")
	#define  DUI_CTR_VLISTBOX                        (L"VListBox")
	#define  DUI_CTR_VIRTUALLISTBOX                  (L"VirtualListBox")
	#define  DUI_CTR_TILELISTBOX                     (L"TileListBox")
	#define  DUI_CTR_VIRTUALTILEBOX                  (L"VirtualTileBox")

	#define  DUI_CTR_TREENODE                        (L"TreeNode")
	#define  DUI_CTR_TREEVIEW                        (L"TreeView")

	#define  DUI_CTR_RICHEDIT                        (L"RichEdit")
	#define  DUI_CTR_COMBO                           (L"Combo")
	#define  DUI_CTR_SLIDER                          (L"Slider")
	#define  DUI_CTR_PROGRESS                        (L"Progress")
	#define  DUI_CTR_CIRCLEPROGRESS                  (L"CircleProgress")
	#define  DUI_CTR_SCROLLBAR                       (L"ScrollBar")

	#define  DUI_CTR_MENU							 (L"Menu")
	#define  DUI_CTR_SUB_MENU				   		 (L"SubMenu")
	#define  DUI_CTR_MENUELEMENT					 (L"MenuElement")

	#define  DUI_CTR_DATETIME                        (L"DateTime")

	// 窗口标题栏按钮：最大化、最小化、关闭、还原窗口的名字，代码中写死的
	#define DUI_CTR_BUTTON_CLOSE					 (L"closebtn")
	#define DUI_CTR_BUTTON_MIN						 (L"minbtn")
	#define DUI_CTR_BUTTON_MAX						 (L"maxbtn")
	#define DUI_CTR_BUTTON_RESTORE					 (L"restorebtn")

	//EventType事件类型对应的字符串定义
	#define	EVENTSTR_ALL				(L"all")
	#define	EVENTSTR_KEYDOWN			(L"keydown")
	#define	EVENTSTR_KEYUP				(L"keyup")
	#define	EVENTSTR_CHAR				(L"char")
	
	#define EVENTSTR_SETFOCUS			(L"setfocus")
	#define EVENTSTR_KILLFOCUS			(L"killfocus")
	#define	EVENTSTR_SETCURSOR			(L"setcursor")

	#define EVENTSTR_MOUSEMOVE			(L"mousemove")
	#define	EVENTSTR_MOUSEENTER			(L"mouseenter")
	#define	EVENTSTR_MOUSELEAVE			(L"mouseleave")
	#define	EVENTSTR_MOUSEHOVER			(L"mousehover")

	#define	EVENTSTR_BUTTONDOWN			(L"buttondown")
	#define	EVENTSTR_BUTTONUP			(L"buttonup")
	#define	EVENTSTR_DOUBLECLICK		(L"doubleclick")
	#define	EVENTSTR_RBUTTONDOWN		(L"rbuttondown")
	#define	EVENTSTR_RBUTTONUP			(L"rbuttonup")
	#define	EVENTSTR_RDOUBLECLICK		(L"rdoubleclick")

	#define EVENTSTR_SELECT				(L"select")
	#define EVENTSTR_UNSELECT			(L"unselect")
	#define	EVENTSTR_MENU				(L"menu")

	#define EVENTSTR_EXPAND				(L"expand")
	#define EVENTSTR_UNEXPAND			(L"unexpand")

	#define	EVENTSTR_SCROLLWHEEL		(L"scrollwheel")
	#define EVENTSTR_SCROLLCHANGE		(L"scrollchange")

	#define EVENTSTR_VALUECHANGE		(L"valuechange")
	#define EVENTSTR_RETURN				(L"return")
	#define EVENTSTR_TAB				(L"tab")
	#define EVENTSTR_WINDOWCLOSE		(L"windowclose")

	//水平对齐方式
	enum HorAlignType
	{
		kHorAlignLeft,		// 靠左对齐
		kHorAlignCenter,	// 水平居中
		kHorAlignRight,		// 靠右对齐
	};

	//垂直对齐方式
	enum VerAlignType
	{
		kVerAlignTop,		// 顶端对齐
		kVerAlignCenter,	// 垂直居中
		kVerAlignBottom,	// 底部对齐
	};

	//控件图片类型
	enum StateImageType
	{
		kStateImageBk,
		kStateImageFore,
		kStateImageSelectedBk,
		kStateImageSelectedFore,
	};

	//控件状态
	enum ControlStateType
	{
		kControlStateNormal,	// 普通状态
		kControlStateHot,		// 焦点状态
		kControlStatePushed,	// 按下状态
		kControlStateDisabled	// 禁用状态
	};

	//控件动画类型
	enum AnimationType
	{
		kAnimationNone,
		kAnimationAlpha,
		kAnimationHeight,
		kAnimationWidth,
		kAnimationHot,
		kAnimationInoutXFromLeft,
		kAnimationInoutXFromRight,
		kAnimationInoutYFromTop,
		kAnimationInoutYFromBottom,
	};

	//GIF图片停止类型
	enum GifStopType
	{
		kGifStopCurrent = -1,	// 当前帧
		kGifStopFirst = 0,		// GIF 停止时定位到第一帧	
		kGifStopLast = 0xFFFF,	// 最后一帧
	};

	//光标
	enum CursorType
	{
		kCursorArrow,		// 箭头
		kCursorHand,		// 手型
		kCursorHandIbeam	// “I”形状
	};

	//定义所有消息类型
	enum EventType
	{
		kEventNone,
		kEventFirst,				//第一个消息（暂时没用到）
		kEventAll,					//代表所有消息

		kEventKeyBegin,
		kEventKeyDown,				//Window类：当收到WM_KEYDOWN消息时触发，发送给Focus控件
		kEventKeyUp,				//Window类：当收到WM_KEYUP消息时触发，发送给WM_KEYDOWN事件中的那个Focus控件
		kEventChar,					//Window类：当收到WM_CHAR消息时触发，发送给WM_KEYDOWN事件中的那个Focus控件
		kEventKeyEnd,

		kEventMouseBegin,		
		kEventMouseEnter,			//Window类：当收到WM_MOUSEMOVE消息时触发，发送给MouseMove时，鼠标进入的那个控件，表明鼠标进入到这个控件内
		kEventMouseLeave,			//Window类：当收到WM_MOUSEMOVE消息时触发，发送给MouseMove时，鼠标进入的那个控件，表明鼠标已经离开这个控件
		kEventMouseMove,			//Window类：当收到WM_MOUSEMOVE消息时触发，发送给鼠标左键、右键、双击时记录的那个控件
		kEventMouseHover,			//Window类：当收到WM_MOUSEHOVER消息时触发，发送给MouseMove时，鼠标进入的那个控件，表明鼠标在这个控件内悬停
		kEventMouseWheel,			//Window类：当收到WM_MOUSEWHEEL消息时触发，发送给当前鼠标所在位置对应的控件
		kEventMouseButtonDown,		//Window类：当收到WM_LBUTTONDOWN消息时触发，发送给鼠标左键按下时对应的控件
		kEventMouseButtonUp,		//Window类：当收到WM_LBUTTONUP消息时触发，发送给鼠标左键按下时对应的控件
		kEventMouseDoubleClick,		//Window类：当收到WM_LBUTTONDBLCLK消息时触发，发送给当前鼠标所在位置对应的控件		
		kEventMouseRightButtonDown,	//Window类：当收到WM_RBUTTONDOWN消息时触发，发送给鼠标右键按下时对应的控件
		kEventMouseRightButtonUp,	//Window类：当收到WM_RBUTTONUP消息时触发，发送给鼠标右键按下时对应的控件
		kEventMouseRightDoubleClick,//Window类：当收到WM_RBUTTONDBLCLK消息时触发，发送给当前鼠标所在位置对应的控件	
		kEventMouseMenu,			//Window类：当收到WM_CONTEXTMENU消息时触发，发送给所有注册回调函数的控件
		kEventMouseEnd,

		kEventSetFocus,				//Window类：发送给Focus控件，当控件获得焦点时触发事件（控件焦点不是窗口焦点，两者完全不同）
		kEventKillFocus,			//Window类：发送给Focus控件，当控件失去焦点时触发事件（控件焦点不是窗口焦点，两者完全不同）		
		kEventSetCursor,			//Window类：发送给当前鼠标所在控件，设置光标
		kEventImeStartComposition,	//Window类：发送给Focus控件，当收到系统WM_IME_STARTCOMPOSITION消息时触发
		kEventImeEndComposition,	//Window类：发送给Focus控件，当收到系统WM_IME_ENDCOMPOSITION消息时触发

		kEventWindowKillFocus,		//Window类：发送给鼠标左键、右键按下时记录的控件，当窗口失去焦点时触发事件（主要用于恢复一些内部状态）
		kEventWindowSize,			//Window类：发送给Focus控件，当窗口大小发生变化时触发事件
		kEventWindowClose,			//Window类，Combo控件：当窗口关闭（或者Combo的下拉框窗口关闭）时触发

		kEventClick,				//Button类、ListBoxElement、Option、CheckBox等：当点击按钮（或者键盘回车）时触发

		kEventSelect,				//CheckBox类、Option类、ListBox类、Combo类：当变成选中状态时触发
		kEventUnSelect,			    //CheckBox类、Option类、ListBox类、Combo类：当变成非选中状态时触发

		kEventExpand,				//TreeNode类：当树节点展开时触发
		kEventUnExpand,				//TreeNode类：当树节点收起时触发

		kEventTextChange,			//RichEdit类：当文本内容发生变化时触发
		kEventReturn,				//ListBoxElement、RichEdit类：当收到回车键时触发
		kEventTab,					//RichEdit类：在WantTab为false时，当收到TAB键时触发
		kEventCustomLinkClick,		//RichEdit类：当点击到自定义link的数据上时触发		

		kEventScrollChange,			//ScrollBox类：当滚动条位置发生变化时触发
		kEventValueChange,			//DateTime、Slider类：当值发生变化时触发
		kEventResize,				//Control类：当控件的大小和位置发生变化时触发
		kEventVisibleChange,		//Control类：当控件的Visible属性发生变化时触发

		kEventLast					//Control类：该控件的最后一个消息，当这个控件对象销毁时触发
	};

}// namespace ui

#endif //DUILIB_DEFS_H_


