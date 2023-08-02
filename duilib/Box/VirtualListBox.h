#ifndef UI_BOX_VIRTUAL_LISTBOX_H_
#define UI_BOX_VIRTUAL_LISTBOX_H_

#pragma once

#include "duilib/Box/ListBox.h"
#include "duilib/Box/VirtualTileLayout.h"
#include "duilib/Box/VirtualHLayout.h"
#include "duilib/Box/VirtualVLayout.h"
#include "base/callback/callback.h"
#include <functional>

namespace ui {

typedef std::function<void(size_t nStartIndex, size_t nEndIndex)> DataChangedNotify;
typedef std::function<void()> CountChangedNotify;

class UILIB_API VirtualListBoxElement : public virtual nbase::SupportWeakCallback
{
public:
    VirtualListBoxElement();

    /** 创建一个数据项
    * @return 返回创建后的数据项指针
    */
    virtual Control* CreateElement() = 0;

    /** 填充指定数据项
    * @param[in] control 数据项控件指针
    * @param[in] index 索引
    */
    virtual void FillElement(Control* control, size_t index) = 0;

    /** 获取数据项总数
    * @return 返回数据项总数
    */
    virtual size_t GetElementCount() = 0;

    /** 注册事件通知回调
    * @param [in] dcNotify 数据内容变化通知接口
    * @param [in] ccNotify 数据项个数变化通知接口
    */
    void RegNotifys(const DataChangedNotify& dcNotify, const CountChangedNotify& ccNotify);

protected:

    /** 发送通知：数据内容发生变化
    * @param [in] nStartIndex 数据的开始下标
    * @param [in] nEndIndex 数据的结束下标
    */
    void EmitDataChanged(size_t nStartIndex, size_t nEndIndex);

    /** 发送通知：数据项个数发生变化
    */
    void EmitCountChanged();

private:

    /** 数据内容发生变化的响应函数
    */
    DataChangedNotify m_pfnDataChangedNotify;

    /** 数据个数发生变化的响应函数
    */
    CountChangedNotify m_pfnCountChangedNotify;
};

/** 虚表实现的ListBox，支持大数据量，只支持纵向滚动条
*/
class UILIB_API VirtualListBox : public ListBox
{
    friend class VirtualTileLayout;
    friend class VirtualVLayout;
    friend class VirtualHLayout;
public:
    explicit VirtualListBox(Layout* pLayout);

    /**
    * @brief 设置数据代理对象
    * @param[in] pProvider 开发者需要重写 VirtualListBoxElement 的接口来作为数据代理对象
    */
    void SetDataProvider(VirtualListBoxElement* pProvider);

    /** 获取数据代理对象
    */
    VirtualListBoxElement* GetDataProvider();

    /** 刷新列表
    */
    void Refresh();

    /** 获取当前所有可见控件的数据元素索引
    * @param[out] collection 索引列表，范围是：[0, GetElementCount())
    */
    void GetDisplayElements(std::vector<size_t>& collection);

    /** 让控件在可见范围内
    * @param[in] iIndex 元素索引号，范围是：[0, GetElementCount())
    * @param[in] bToTop 是否在最上方
    */
    void EnsureVisible(size_t iIndex, bool bToTop);

protected:
    /// 重写父类接口，提供个性化功能
    virtual void SetScrollPos(UiSize64 szPos) override;
    virtual void SetPos(UiRect rc) override;
    virtual void PaintChild(IRender* pRender, const UiRect& rcPaint) override;

    //重载删除函数，禁止外部调用
    virtual bool RemoveItem(Control* pControl) override;
    virtual bool RemoveItemAt(size_t iIndex) override;
    virtual void RemoveAllItems() override;

    virtual void HandleEvent(const EventArgs& event) override;

protected:
    /** 键盘按下通知
    * @param[in] ch 按键
    */
    virtual void OnKeyDown(TCHAR ch);

    /** 键盘弹起通知
    * @param[in] ch 按键
    */
    virtual void OnKeyUp(TCHAR ch);

    /** 设置虚表布局接口
    */
    void SetVirtualLayout(VirtualLayout* pVirtualLayout);

private:
    /** 是否含有数据代理对象接口
    */
    bool HasDataProvider() const;

    /** 创建一个子项
    * @return 返回创建后的子项指针
    */
    Control* CreateElement();

    /** 填充指定子项
    * @param[in] control 子项控件指针
    * @param[in] index 索引
    * @return 返回创建后的子项指针
    */
    void FillElement(Control* pControl, size_t iIndex);

    /** 获取元素总数
    * @return 返回元素总指数
    */
    size_t GetElementCount();

    /** 数据内容发生变化，在事件中需要重新加载展示数据
    */
    void OnModelDataChanged(size_t nStartIndex, size_t nEndIndex);

    /** 数据个数发生变化，在事件中需要重新加载展示数据
    */
    void OnModelCountChanged();

    /** 重新布局子项
    * @param[in] bForce 是否强制重新布局
    */
    void ReArrangeChild(bool bForce);

    /** 将数据元素的索引号转变为展示内容的索引号
    * @param [in] nElementIndex 数据元素的索引号
    * @return 如果nElementIndex是显示范围内的数据，返回对应的Item索引号
    *         如果nElementIndex不是显示范围内的数据，返回Box::InvalidIndex，表示无对应的Item索引号
    */
    size_t ElementIndexToItemIndex(size_t nElementIndex) const;

private:
    /** 数据代理对象接口，提供展示数据
    */
    VirtualListBoxElement* m_pDataProvider;

    /** 虚表布局接口
    */
    VirtualLayout* m_pVirtualLayout;
};

/** 瓦片布局的虚表ListBox
*/
class UILIB_API VirtualTileListBox : public VirtualListBox
{
public:
    VirtualTileListBox() :
        VirtualListBox(new VirtualTileLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualTileLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_TILELISTBOX; }
};

/** 纵向布局的虚表ListBox
*/
class UILIB_API VirtualVListBox : public VirtualListBox
{
public:
    VirtualVListBox() :
        VirtualListBox(new VirtualVLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualVLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_VLISTBOX; }
};

/** 横向布局的虚表ListBox
*/
class UILIB_API VirtualHListBox : public VirtualListBox
{
public:
    VirtualHListBox() :
        VirtualListBox(new VirtualHLayout)
    {
        VirtualLayout* pVirtualLayout = dynamic_cast<VirtualHLayout*>(GetLayout());
        SetVirtualLayout(pVirtualLayout);
    }

    virtual std::wstring GetType() const override { return DUI_CTR_VIRTUAL_HLISTBOX; }
};

}

#endif //UI_BOX_VIRTUAL_LISTBOX_H_
