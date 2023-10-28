#include "ListCtrl.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Render/IRender.h"
#include <unordered_map>

namespace ui
{
/** 列表数据显示控件
*/
class ListCtrlDataView : public VirtualListBox
{
    friend class ListCtrlDataLayout;
public:
    ListCtrlDataView();
    virtual ~ListCtrlDataView();

    /** 设置ListCtrl控件接口
    */
    void SetListCtrl(ListCtrl* pListCtrl);

    /** 获取列表控件的宽度（Header的各个列总宽度之和）
    */
    int32_t GetListCtrlWidth() const;

    /** 设置顶部元素的索引号
    */
    void SetTopElementIndex(size_t nTopElementIndex);

    /** 获取顶部元素的索引号
    */
    size_t GetTopElementIndex() const;

protected:
    /** 执行了刷新操作, 界面的UI控件个数可能会发生变化
    */
    virtual void OnRefresh() override;

    /** 执行了重排操作，界面的UI控件进行了重新数据填充（通过FillElement函数）
    */
    virtual void OnArrangeChild() override;

private:
    /** ListCtrl 控件接口
    */
    ListCtrl* m_pListCtrl;

    /** 顶部元素的索引号(用于画网格线)
    */
    size_t m_nTopElementIndex;
};

/** 列表数据显示控件的布局管理接口
*/
class ListCtrlDataLayout : public Layout, public VirtualLayout
{
public:
    /** 调整内部所有控件的位置信息
        * @param [in] items 控件列表
        * @param[in] rc 当前容器位置信息, 包含内边距，但不包含外边距
        * @return 返回排列后最终盒子的宽度和高度信息
        */
    virtual UiSize64 ArrangeChild(const std::vector<Control*>& items, UiRect rc) override;

    /** 根据内部子控件大小估算容器自身大小，拉伸类型的子控件被忽略，不计入大小估算
        * @param[in] items 子控件列表
        * @param [in] szAvailable 可用大小，包含分配给该控件的内边距，但不包含分配给控件的外边距
        * @return 返回排列后最终布局的大小信息（宽度和高度）；
                包含items中子控件的外边距，包含items中子控件的内边距；
                包含Box控件本身的内边距；
                不包含Box控件本身的外边距；
                返回值中不包含拉伸类型的子控件大小。
        */
    virtual UiSize EstimateSizeByChild(const std::vector<Control*>& items, UiSize szAvailable) override;

public:
    /** 延迟加载展示数据
        * @param [in] rc 当前容器大小信息, 外部调用时，需要先剪去内边距
        */
    virtual void LazyArrangeChild(UiRect rc) const override;

    /** 获取需要展示的真实数据项最大个数（即有Control对象对应的真实数据项）
    * @param [in] rc 当前容器大小信息, 外部调用时，需要先剪去内边距
    */
    virtual size_t AjustMaxItem(UiRect rc) const override;

    /** 得到可见范围内第一个元素的前一个元素索引
    * @param [in] rc 当前显示区域的矩形，不包含内边距
    * @return 返回元素的索引
    */
    virtual size_t GetTopElementIndex(UiRect rc) const override;

    /** 判断某个元素是否在可见范围内
    * @param[in] iIndex 元素索引
    * @param [in] rc 当前显示区域的矩形，不包含内边距
    * @return 返回 true 表示可见，否则为不可见
    */
    virtual bool IsElementDisplay(UiRect rc, size_t iIndex) const override;

    /** 判断是否要重新布局
    */
    virtual bool NeedReArrange() const override;

    /** 获取当前所有可见控件的数据元素索引
    * @param [in] rc 当前显示区域的矩形，不包含内边距
    * @param[out] collection 索引列表，范围是：[0, GetElementCount())
    */
    virtual void GetDisplayElements(UiRect rc, std::vector<size_t>& collection) const override;

    /** 让控件在可见范围内
    * @param [in] rc 当前显示区域的矩形，不包含内边距
    * @param[in] iIndex 元素索引号，范围是：[0, GetElementCount())
    * @param[in] bToTop 是否在最上方
    */
    virtual void EnsureVisible(UiRect rc, size_t iIndex, bool bToTop) const override;

private:
    /** 获取关联的Box接口
    */
    ListCtrlDataView* GetOwnerBox() const;

    /** 获取数据项的高度
    * @param [in] nCount 数据项个数，如果为Box::InvalidIndex，则获取所有数据项的高度总和
    * @param [in] rc 当前容器大小信息, 外部调用时，需要先剪去内边距
    * @return 返回 nCount 个数据项的高度总和
    */
    int64_t GetElementsHeight(UiRect rc, size_t nCount) const;

    /** 获取数据项的高度和宽度
    * @param [in] rc 当前容器大小信息, 外部调用时，需要先剪去内边距
    * @param [in] nElementIndex 数据元素的索引号
    * @return 返回数据元素的高度和宽度
    */
    UiSize GetElementSize(UiRect rc, size_t nElementIndex) const;

    /** 计算列数
    *@param [in] rcWidth 可用区域宽度
    *@return 计算得到的列数, 大于或等于1
    */
    int32_t CalcTileColumns(int32_t rcWidth) const;

    /** 获取行宽
    */
    int32_t GetItemWidth() const;

    /** 获取行高(目前仅支持所有行的行高都相等的情况)
    */
    int32_t GetItemHeight() const;

    /** 获取表头控件的高度 
    */
    int32_t GetHeaderHeight() const;
};

////////////////////////////////////////////////////////
//ListCtrlHeaderItem

ListCtrlHeaderItem::ListCtrlHeaderItem() :
    m_pSortedDownImage(nullptr),
    m_pSortedUpImage(nullptr),
    m_sortMode(SortMode::kDown),
    m_pSplitBox(nullptr),
    m_bColumnResizeable(true),
    m_nColumnWidth(0),
    m_bMouseDown(false),
    m_bInDragging(false),
    m_nOldAlpha(255),
    m_bShowIconAtTop(true)
{
    m_nIconSpacing = GlobalManager::Instance().Dpi().GetScaleInt(6);
}

ListCtrlHeaderItem::~ListCtrlHeaderItem()
{
    if (m_pSortedDownImage != nullptr) {
        delete m_pSortedDownImage;
        m_pSortedDownImage = nullptr;
    }
    if (m_pSortedUpImage != nullptr) {
        delete m_pSortedUpImage;
        m_pSortedUpImage = nullptr;
    }
}
 
std::wstring ListCtrlHeaderItem::GetType() const { return L"ListCtrlHeaderItem"; }

void ListCtrlHeaderItem::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"sorted_up_image") {
        SetSortedUpImage(strValue);
    }
    else if (strName == L"sorted_down_image") {
        SetSortedDownImage(strValue);
    }
    else if (strName == L"icon_spacing") {
        SetIconSpacing(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"show_icon_at_top") {
        SetShowIconAtTop(strValue == L"true");
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrlHeaderItem::PaintText(IRender* pRender)
{
    __super::PaintText(pRender);
    if (pRender == nullptr) {
        return;
    }

    Image* pImage = nullptr;
    if (m_sortMode == SortMode::kUp) {
        //升序
        pImage = m_pSortedUpImage;
    }
    else if (m_sortMode == SortMode::kDown) {
        //降序
        pImage = m_pSortedDownImage;
    }
    if (pImage == nullptr) {
        return;
    }
    
    //确定排序图标的位置
    UiRect rc = GetRect();
    UiPadding rcPadding = GetControlPadding();
    rc.Deflate(rcPadding);
    if (IsShowIconAtTop()) {
        //图标显示在文字的上方，居中显示
        int32_t nImageWidth = 0;
        int32_t nImageHeight = 0;
        if (pImage != nullptr) {
            if (pImage->GetImageCache() == nullptr) {
                LoadImageData(*pImage);
            }
            if (pImage->GetImageCache() != nullptr) {
                nImageWidth = pImage->GetImageCache()->GetWidth();
                nImageHeight = pImage->GetImageCache()->GetHeight();
            }
        }
        if (nImageWidth > 0) {
            rc.left = rc.CenterX() - nImageWidth / 2;
            rc.right = rc.left + nImageWidth;
        }
        if (nImageHeight > 0) {
            rc.bottom = rc.top + nImageHeight;
        }
    }
    else {
        //图标显示在文字的后面（文字左对齐，居中对齐），或者前面（文字靠右对齐）
        rc.Deflate(GetTextPadding());
        int32_t nIconTextSpacing = GetIconSpacing();
        uint32_t textStyle = GetTextStyle();
        std::wstring textValue = GetText();
        UiRect textRect = pRender->MeasureString(textValue, GetFontId(), textStyle);
        if (textStyle & TEXT_CENTER) {
            rc.left = rc.CenterX() + textRect.Width() / 2;
            rc.left += nIconTextSpacing;
        }
        else if (textStyle & TEXT_RIGHT) {
            rc.left = rc.right - textRect.Width() - nIconTextSpacing;
            if (pImage != nullptr) {
                if (pImage->GetImageCache() == nullptr) {
                    LoadImageData(*pImage);
                }
                if (pImage->GetImageCache() != nullptr) {
                    rc.left -= pImage->GetImageCache()->GetWidth();
                }
            }
        }
        else {
            rc.left += textRect.Width();
            rc.left += nIconTextSpacing;
        }
    }
    rc.Validate();

    //绘制排序图标
    PaintImage(pRender, pImage, L"", -1, nullptr, &rc, nullptr);
}

void ListCtrlHeaderItem::Activate()
{
    if (m_bInDragging) {
        //处于拖动改变列顺序的状态
        return;
    }
    if (!this->IsActivatable()) {
        return;
    }
    if (m_sortMode == SortMode::kUp) {
        m_sortMode = SortMode::kDown;
        Invalidate();
    }
    else if (m_sortMode == SortMode::kDown) {
        m_sortMode = SortMode::kUp;
        Invalidate();
    }
    __super::Activate();
}

void ListCtrlHeaderItem::SetSortMode(SortMode sortMode)
{
    if (m_sortMode != sortMode) {
        m_sortMode = sortMode;
        Invalidate();
    }
}

ListCtrlHeaderItem::SortMode ListCtrlHeaderItem::GetSortMode() const
{
    return m_sortMode;
}

void ListCtrlHeaderItem::SetSortedDownImage(const std::wstring& sImageString)
{
    if (m_pSortedDownImage == nullptr) {
        m_pSortedDownImage = new Image;
    }
    m_pSortedDownImage->SetImageString(sImageString);
    Invalidate();
}

void ListCtrlHeaderItem::SetSortedUpImage(const std::wstring& sImageString)
{
    if (m_pSortedUpImage == nullptr) {
        m_pSortedUpImage = new Image;
    }
    m_pSortedUpImage->SetImageString(sImageString);
    Invalidate();
}

size_t ListCtrlHeaderItem::GetColomnId() const
{
    return (size_t)this;
}

void ListCtrlHeaderItem::SetSplitBox(SplitBox* pSplitBox)
{
    m_pSplitBox = pSplitBox;
    if (pSplitBox != nullptr) {
        ASSERT(pSplitBox->GetFixedWidth().IsInt32());
        pSplitBox->SetEnabled(IsColumnResizeable() ? true : false);
    }
    if (GetColumnWidth() > 0) {
        CheckColumnWidth();
    }
}

SplitBox* ListCtrlHeaderItem::GetSplitBox() const
{
    return m_pSplitBox;
}

void ListCtrlHeaderItem::SetColumnResizeable(bool bResizeable)
{
    m_bColumnResizeable = bResizeable;
    if (m_pSplitBox != nullptr) {
        m_pSplitBox->SetEnabled(IsColumnResizeable() ? true : false);
    }
}

bool ListCtrlHeaderItem::IsColumnResizeable() const
{
    return m_bColumnResizeable;
}

void ListCtrlHeaderItem::SetColumnWidth(int32_t nWidth, bool bNeedDpiScale)
{
    if (nWidth < 0) {
        nWidth = 0;
    }
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nWidth);
    }
    m_nColumnWidth = nWidth;
    CheckColumnWidth();
}

int32_t ListCtrlHeaderItem::GetColumnWidth() const
{
    return m_nColumnWidth;
}

void ListCtrlHeaderItem::CheckColumnWidth()
{
    int32_t nSplitWidth = 0;
    if (m_pSplitBox != nullptr) {
        ASSERT(m_pSplitBox->GetFixedWidth().IsInt32());
        nSplitWidth = m_pSplitBox->GetFixedWidth().GetInt32();
    }
    int32_t nWidth = GetFixedWidth().GetInt32();
    if ((nWidth + nSplitWidth) != GetColumnWidth()) {
        nWidth = GetColumnWidth() - nSplitWidth;
        if (nWidth < 0) {
            nWidth = 0;
        }
        SetFixedWidth(UiFixedInt(nWidth), true, false);
    }
}

void ListCtrlHeaderItem::SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nIconSpacing);
    }
    m_nIconSpacing = nIconSpacing;
    if (m_nIconSpacing < 0) {
        m_nIconSpacing = 0;
    }
}

int32_t ListCtrlHeaderItem::GetIconSpacing() const
{
    return m_nIconSpacing;
}

void ListCtrlHeaderItem::SetShowIconAtTop(bool bShowIconAtTop)
{
    m_bShowIconAtTop = bShowIconAtTop;
}

bool ListCtrlHeaderItem::IsShowIconAtTop() const
{
    return m_bShowIconAtTop;
}

bool ListCtrlHeaderItem::SetCheckBoxVisible(bool bVisible)
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            pCheckBox->SetVisible(bVisible);
            return true;
        }
    }
    return false;
}

bool ListCtrlHeaderItem::IsCheckBoxVisible() const
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            return pCheckBox->IsVisible();
        }
    }
    return false;
}

bool ListCtrlHeaderItem::HasCheckBox() const
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            return true;
        }
    }
    return false;
}

bool ListCtrlHeaderItem::SetCheckBoxSelect(bool bSelected, bool bPartSelect)
{
    if (GetItemCount() > 0) {
        CheckBox* pCheckBox = dynamic_cast<CheckBox*>(GetItemAt(0));
        if (pCheckBox != nullptr) {
            pCheckBox->SetSelected(bSelected);
            if (bSelected) {
                pCheckBox->SetPartSelected(bPartSelect);
            }
            pCheckBox->Invalidate();
            return true;
        }
    }
    return false;
}

bool ListCtrlHeaderItem::ButtonDown(const EventArgs& msg)
{
    bool bRet = __super::ButtonDown(msg);
    ListCtrlHeader* pParentBox = dynamic_cast<ListCtrlHeader*>(GetParent());
    ASSERT(pParentBox != nullptr);
    if (pParentBox == nullptr) {
        return bRet;
    }
    if (!pParentBox->IsEnableHeaderDragOrder()) {
        //不支持拖动调整顺序
        return bRet;
    }
    m_bMouseDown = true;
    m_ptMouseDown = msg.ptMouse;
    m_rcMouseDown = GetRect();
    
    m_rcItemList.clear();
    size_t nItemCount = pParentBox->GetItemCount();
    for (size_t index = 0; index < nItemCount; ++index) {
        ItemStatus itemStatus;
        itemStatus.m_index = index;
        itemStatus.m_pItem = pParentBox->GetItemAt(index);
        if (itemStatus.m_pItem != nullptr) {
            itemStatus.m_rcPos = itemStatus.m_pItem->GetRect();
            m_rcItemList.push_back(itemStatus);
        }
        if (itemStatus.m_pItem == this) {
            ASSERT(itemStatus.m_rcPos.ContainsPt(m_ptMouseDown));
        }
    }
    return bRet;
}

bool ListCtrlHeaderItem::ButtonUp(const EventArgs& msg)
{
    bool bRet = __super::ButtonUp(msg);
    Box* pParentBox = dynamic_cast<Box*>(GetParent());
    if (pParentBox == nullptr) {
        ClearDragStatus();
        return bRet;
    }

    bool bOrderChanged = false;
    const size_t itemCount = pParentBox->GetItemCount();
    size_t nMouseItemIndex = Box::InvalidIndex;
    size_t nCurrentItemIndex = Box::InvalidIndex;
    for (const ItemStatus& itemStatus : m_rcItemList) {
        if (itemStatus.m_rcPos.ContainsPt(msg.ptMouse)) {
            nMouseItemIndex = itemStatus.m_index;
        }
        if ((itemStatus.m_pItem == this) && !itemStatus.m_rcPos.ContainsPt(msg.ptMouse)){
            nCurrentItemIndex = itemStatus.m_index;
        }
    }
    if ((nMouseItemIndex != Box::InvalidIndex) && 
        (nCurrentItemIndex != Box::InvalidIndex) &&
        (nMouseItemIndex < itemCount) &&
        (nCurrentItemIndex < itemCount)) {
        //交换控件的位置
        if (nMouseItemIndex < nCurrentItemIndex) {
            //向左侧交换
            pParentBox->SetItemIndex(this, nMouseItemIndex);
            if (m_pSplitBox != nullptr) {
                size_t nNewIndex = pParentBox->GetItemIndex(this);
                ASSERT(nNewIndex < itemCount);
                ASSERT((nNewIndex + 1) < itemCount);
                if ((nNewIndex + 1) < itemCount) {
                    pParentBox->SetItemIndex(m_pSplitBox, nNewIndex + 1);
                }
            }
        }
        else {
            //向右侧交换
            nMouseItemIndex += 1;
            ASSERT(nMouseItemIndex < itemCount);
            if (nMouseItemIndex < itemCount) {
                pParentBox->SetItemIndex(this, nMouseItemIndex);
                if (m_pSplitBox != nullptr) {
                    size_t nNewIndex = pParentBox->GetItemIndex(this);
                    ASSERT(nNewIndex < itemCount);
                    pParentBox->SetItemIndex(m_pSplitBox, nNewIndex);
                }
            }
        }
        bOrderChanged = true;
        ASSERT(pParentBox->GetItemIndex(this) == (pParentBox->GetItemIndex(m_pSplitBox) - 1));

        //交换后，对所有的项进行校验
        for (size_t index = 0; index < itemCount; index += 2) {
            ASSERT(dynamic_cast<ListCtrlHeaderItem*>(pParentBox->GetItemAt(index)) != nullptr);
            ASSERT((index + 1) < itemCount);
            if ((index + 1) >= itemCount) {
                break;
            }
            ASSERT(dynamic_cast<SplitBox*>(pParentBox->GetItemAt(index + 1)) != nullptr);
            ASSERT(dynamic_cast<SplitBox*>(pParentBox->GetItemAt(index + 1)) == 
                   dynamic_cast<ListCtrlHeaderItem*>(pParentBox->GetItemAt(index))->m_pSplitBox);
        }
    }
    ClearDragStatus();

    if (bOrderChanged) {
        //触发列交换事件
        ListCtrlHeader* pHeader = dynamic_cast<ListCtrlHeader*>(GetParent());
        ASSERT(pHeader != nullptr);
        if (pHeader != nullptr) {
            pHeader->OnHeaderColumnOrderChanged();
        }
    }
    return bRet;
}

bool ListCtrlHeaderItem::MouseMove(const EventArgs& msg)
{
    bool bRet = __super::MouseMove(msg);
    if (!m_bMouseDown) {
        return bRet;
    }
    Control* pParent = GetParent();
    if (pParent == nullptr) {
        return bRet;
    }
    UiPoint pt = msg.ptMouse;
    int32_t xOffset = pt.x - m_ptMouseDown.x;
    if (std::abs(xOffset) < GlobalManager::Instance().Dpi().GetScaleInt(2)) {
        return bRet;
    }

    UiRect boxRect = pParent->GetRect();
    if ((pt.x >= boxRect.left) && (pt.x < boxRect.right)) {
        UiRect rect = m_rcMouseDown;
        rect.left += xOffset;
        rect.right += xOffset;
        SetPos(rect);

        if (!m_bInDragging) {
            m_bInDragging = true;
            m_nOldAlpha = (uint8_t)GetAlpha();
            //设置为半透明的效果
            SetAlpha(216);
        }

        AdjustHeaderItemPos(pt);
    }
    return bRet;
}

bool ListCtrlHeaderItem::OnWindowKillFocus(const EventArgs& msg)
{
    bool bRet = __super::OnWindowKillFocus(msg);
    ClearDragStatus();
    return bRet;
}

void ListCtrlHeaderItem::AdjustHeaderItemPos(const UiPoint& mousePt)
{
    Control* pMouseItem = nullptr;
    size_t nMouseItemIndex = Box::InvalidIndex;
    size_t nMouseDownItemIndex = Box::InvalidIndex;
    for (const ItemStatus& itemStatus : m_rcItemList) {
        if (itemStatus.m_rcPos.ContainsPt(mousePt)) {
            pMouseItem = itemStatus.m_pItem;
            nMouseItemIndex = itemStatus.m_index;
        }
        if (itemStatus.m_pItem == this) {
            nMouseDownItemIndex = itemStatus.m_index;
        }
    }
    if ((pMouseItem == nullptr) ||
        (nMouseItemIndex == Box::InvalidIndex) || 
        (nMouseDownItemIndex == Box::InvalidIndex)) {
        return;
    }
    if (dynamic_cast<ListCtrlHeaderItem*>(pMouseItem) == nullptr) {
        //鼠标不在表头控件上
        return;
    }

    const size_t itemCount = m_rcItemList.size();
    int32_t xOffset = mousePt.x - m_ptMouseDown.x;
    if (pMouseItem == this) {
        //当前鼠标位置：在自身的位置，恢复各个控件的实际位置
        for (ItemStatus& item : m_rcItemList) {
            if (item.m_pItem == this) {
                continue;
            }
            if (item.m_pItem->GetRect() != item.m_rcPos) {
                item.m_pItem->SetPos(item.m_rcPos);
            }
        }
    }
    else if (xOffset < 0) {
        //当前鼠标位置：在按下点的左侧，向右侧移动控件
        for (size_t index = 0; index < itemCount; ++index) {
            ItemStatus& item = m_rcItemList[index];
            if (item.m_pItem == this) {
                //恢复关联的Split控件位置
                if ((index + 1) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index + 1];
                    if ((nextItem.m_pItem->GetRect() != nextItem.m_rcPos)) {
                        nextItem.m_pItem->SetPos(nextItem.m_rcPos);
                    }
                }
                continue;
            }
            else if ((item.m_index >= nMouseItemIndex) && (item.m_index < nMouseDownItemIndex)) {
                //向右侧移动
                if ((index + 2) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index + 2];
                    item.m_pItem->SetPos(nextItem.m_rcPos);
                }
                else {
                    if (item.m_pItem->GetRect() != item.m_rcPos) {
                        item.m_pItem->SetPos(item.m_rcPos);
                    }
                }
            }
            else {
                //恢复原位置
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
    else {
        //当前鼠标位置：在按下点的右侧，向左侧移动控件
        for (size_t index = 0; index < itemCount; ++index) {
            ItemStatus& item = m_rcItemList[index];
            if (item.m_pItem == this) {
                //恢复关联的Split控件位置
                if ((index + 1) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index + 1];
                    if ((nextItem.m_pItem->GetRect() != nextItem.m_rcPos)) {
                        nextItem.m_pItem->SetPos(nextItem.m_rcPos);
                    }
                }
                continue;
            }
            else if ((item.m_index > nMouseDownItemIndex) && (item.m_index <= nMouseItemIndex)) {
                //向左侧移动
                if ((index - 2) < itemCount) {
                    const ItemStatus& nextItem = m_rcItemList[index - 2];
                    item.m_pItem->SetPos(nextItem.m_rcPos);
                }
                else {
                    if (item.m_pItem->GetRect() != item.m_rcPos) {
                        item.m_pItem->SetPos(item.m_rcPos);
                    }
                }
            }
            else {
                //恢复原位置
                if (item.m_pItem->GetRect() != item.m_rcPos) {
                    item.m_pItem->SetPos(item.m_rcPos);
                }
            }
        }
    }
}

void ListCtrlHeaderItem::ClearDragStatus()
{
    if (m_bInDragging) {
        SetAlpha(m_nOldAlpha);
        m_nOldAlpha = 255;
        m_bInDragging = false;
    }
    m_bMouseDown = false;
    if (!m_rcItemList.empty()) {
        m_rcItemList.clear();
        if (GetParent() != nullptr) {
            GetParent()->Invalidate();
            GetParent()->SetPos(GetParent()->GetPos());
        }
    }
}

////////////////////////////////////////////////////////////////
/** ListCtrl的表头控件
*/
ListCtrlHeader::ListCtrlHeader() :
    m_pListCtrl(nullptr)
{
}

std::wstring ListCtrlHeader::GetType() const { return L"ListCtrlHeader"; }

ListCtrlHeaderItem* ListCtrlHeader::InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo)
{
    int32_t nColumnWidth = columnInfo.nColumnWidth;
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nullptr;
    }
    if (columnInfo.bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nColumnWidth);
    }
    if (nColumnWidth < 0) {
        nColumnWidth = 0;
    }

    ListCtrlHeaderItem* pHeaderItem = new ListCtrlHeaderItem;
    SplitBox* pHeaderSplit = new SplitBox;
    size_t nColumnCount = GetColumnCount();
    if ((size_t)columnIndex >= nColumnCount) {
        //放在最后
        AddItem(pHeaderItem);
        AddItem(pHeaderSplit);
    }
    else {
        //插入在中间位置        
        AddItemAt(pHeaderSplit, columnIndex);
        AddItemAt(pHeaderItem, columnIndex);
    }

    //设置属性
    if (!m_pListCtrl->GetHeaderSplitBoxClass().empty()) {
        pHeaderSplit->SetClass(m_pListCtrl->GetHeaderSplitBoxClass());
    }

    Control* pSplitCtrl = new Control;
    pSplitCtrl->SetMouseEnabled(false);
    pSplitCtrl->SetMouseFocused(false);
    pSplitCtrl->SetNoFocus();
    if (!m_pListCtrl->GetHeaderSplitControlClass().empty()) {
        pSplitCtrl->SetClass(m_pListCtrl->GetHeaderSplitControlClass());
    }
    pHeaderSplit->AddItem(pSplitCtrl);

    if (!m_pListCtrl->GetHeaderItemClass().empty()) {
        pHeaderItem->SetClass(m_pListCtrl->GetHeaderItemClass());
    }
    pHeaderItem->SetText(columnInfo.text);

    //保存关联的Split控件接口
    pHeaderItem->SetSplitBox(pHeaderSplit);
    pHeaderItem->SetColumnWidth(nColumnWidth, false);
    if (columnInfo.nColumnWidthMin > 0) {
        //列宽最小值
        pHeaderItem->SetMinWidth(columnInfo.nColumnWidthMin, columnInfo.bNeedDpiScale);
    }
    if (columnInfo.nColumnWidthMax > 0) {
        //列宽最大值
        pHeaderItem->SetMaxWidth(columnInfo.nColumnWidthMax, columnInfo.bNeedDpiScale);
    }

    if (columnInfo.bSortable) {
        pHeaderItem->SetSortMode(ListCtrlHeaderItem::SortMode::kUp);
    }
    else {
        pHeaderItem->SetSortMode(ListCtrlHeaderItem::SortMode::kNone);
    }
    pHeaderItem->SetColumnResizeable(columnInfo.bResizeable);

    if (columnInfo.nTextFormat >= 0) {
        uint32_t textStyle = columnInfo.nTextFormat;
        //水平对齐
        if (columnInfo.nTextFormat & TEXT_CENTER) {
            //文本：居中对齐
            textStyle &= ~(TEXT_LEFT | TEXT_RIGHT);
            textStyle |= TEXT_CENTER;            
        }
        else if (columnInfo.nTextFormat & TEXT_RIGHT) {
            //文本：右对齐
            textStyle &= ~(TEXT_LEFT | TEXT_CENTER);
            textStyle |= TEXT_RIGHT;
        }
        else {
            //文本：左对齐
            textStyle &= ~(TEXT_CENTER | TEXT_RIGHT);
            textStyle |= TEXT_LEFT;
        }

        //垂直对齐
        if (columnInfo.nTextFormat & TEXT_VCENTER) {
            //文本：居中对齐
            textStyle &= ~(TEXT_TOP | TEXT_BOTTOM);
            textStyle |= TEXT_VCENTER;
        }
        else if (columnInfo.nTextFormat & TEXT_BOTTOM) {
            //文本：底部对齐
            textStyle &= ~(TEXT_TOP | TEXT_VCENTER);
            textStyle |= TEXT_BOTTOM;
        }
        else {
            //文本：顶部对齐
            textStyle &= ~(TEXT_BOTTOM | TEXT_VCENTER);
            textStyle |= TEXT_TOP;
        }
        pHeaderItem->SetTextStyle(textStyle, true);
    }

    //CheckBox属性
    if (columnInfo.bShowCheckBox) {
        std::wstring checkBoxClass = m_pListCtrl->GetCheckBoxClass();
        ASSERT(!checkBoxClass.empty());
        CheckBox* pCheckBox = new CheckBox;
        pHeaderItem->AddItem(pCheckBox);
        if (!checkBoxClass.empty()) {
            pCheckBox->SetClass(checkBoxClass);
        }
        UiPadding textPadding = pHeaderItem->GetTextPadding();
        int32_t nCheckBoxWidth = columnInfo.nCheckBoxWidth;
        if (columnInfo.bNeedDpiScale) {
            GlobalManager::Instance().Dpi().ScaleInt(nCheckBoxWidth);
        }
        if (textPadding.left < nCheckBoxWidth) {
            textPadding.left = nCheckBoxWidth;
            pHeaderItem->SetTextPadding(textPadding, false);
        }

        //挂载CheckBox的事件处理
        pCheckBox->AttachSelect([this, pHeaderItem](const EventArgs& /*args*/) {
            OnHeaderColumnCheckStateChanged(pHeaderItem, true);
            return true;
            });
        pCheckBox->AttachUnSelect([this, pHeaderItem](const EventArgs& /*args*/) {
            OnHeaderColumnCheckStateChanged(pHeaderItem, false);
            return true;
            });
    }

    //挂载排序事件
    pHeaderItem->AttachClick([this, pHeaderItem](const EventArgs& /*args*/) {
        OnHeaderColumnSorted(pHeaderItem);
        return true;
        });

        //挂载拖动响应事件
    pHeaderSplit->AttachSplitDraged([this](const EventArgs& args) {
        OnHeaderColumnResized((Control*)args.wParam, (Control*)args.lParam);
        return true;
        });

    m_pListCtrl->OnHeaderColumnAdded(pHeaderItem->GetColomnId());
    return pHeaderItem;
}

size_t ListCtrlHeader::GetColumnCount() const
{
    size_t nItemCount = GetItemCount();
    if (nItemCount == 0) {
        return 0;
    }
    ASSERT((nItemCount % 2) == 0);
    if ((nItemCount % 2) != 0) {
        return 0;
    }
    const size_t nColumnCount = nItemCount / 2;
#ifdef _DEBUG
    //校验结构是否符合预期    
    for (size_t index = 0; index < nColumnCount; ++index) {
        ASSERT(dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2)) != nullptr);
        ASSERT(dynamic_cast<SplitBox*>(GetItemAt(index * 2 + 1)) != nullptr);
    }
#endif // _DEBUG  
    
    return nColumnCount;
}

int32_t ListCtrlHeader::GetColumnWidth(size_t columnIndex) const
{
    int32_t nColumnWidth = 0;
    size_t nColumnCount = GetColumnCount();
    ASSERT(columnIndex < nColumnCount);
    if (columnIndex >= nColumnCount) {
        return nColumnWidth;
    }
    ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(columnIndex * 2));
    ASSERT(pHeaderItem != nullptr);
    if (pHeaderItem != nullptr) {
        nColumnWidth = pHeaderItem->GetColumnWidth();
    }
    return nColumnWidth;
}

ListCtrlHeaderItem* ListCtrlHeader::GetColumn(size_t columnIndex) const
{
    size_t nColumnCount = GetColumnCount();
    ASSERT(columnIndex < nColumnCount);
    if (columnIndex >= nColumnCount) {
        return nullptr;
    }
    ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(columnIndex * 2));
    ASSERT(pHeaderItem != nullptr);
    return pHeaderItem;
}

ListCtrlHeaderItem* ListCtrlHeader::GetColumnById(size_t columnId) const
{
    ListCtrlHeaderItem* pFoundHeaderItem = nullptr;
    size_t nColumnCount = GetColumnCount();
    for (size_t index = 0; index < nColumnCount; ++index) {
        ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2));
        ASSERT(pHeaderItem != nullptr);
        if (pHeaderItem != nullptr) {
            if (pHeaderItem->GetColomnId() == columnId) {
                pFoundHeaderItem = pHeaderItem;
                break;
            }
        }
    }
    return pFoundHeaderItem;
}

bool ListCtrlHeader::GetColumnInfo(size_t columnId, size_t& columnIndex, int32_t& nColumnWidth) const
{
    bool bRet = false;
    columnIndex = Box::InvalidIndex;
    nColumnWidth = -1;
    size_t nColumnCount = GetColumnCount();
    for (size_t index = 0; index < nColumnCount; ++index) {
        ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(index * 2));
        ASSERT(pHeaderItem != nullptr);
        if (pHeaderItem != nullptr) {
            if (pHeaderItem->GetColomnId() == columnId) {
                nColumnWidth = pHeaderItem->GetColumnWidth();
                columnIndex = index;
                bRet = true;
                break;
            }
        }
    }
    return bRet;
}

size_t ListCtrlHeader::GetColumnIndex(size_t columnId) const
{
    size_t columnIndex = Box::InvalidIndex;
    int32_t nColumnWidth = -1;
    GetColumnInfo(columnId, columnIndex, nColumnWidth);
    return columnIndex;
}

size_t ListCtrlHeader::GetColumnId(size_t columnIndex) const
{
    size_t columnId = Box::InvalidIndex;
    ListCtrlHeaderItem* pHeaderItem = GetColumn(columnIndex);
    if (pHeaderItem != nullptr) {
        columnId = pHeaderItem->GetColomnId();
    }
    return columnId;
}

bool ListCtrlHeader::DeleteColumn(size_t columnIndex)
{
    bool bRet = false;
    size_t columnId = Box::InvalidIndex;
    size_t nColumnCount = GetColumnCount();
    if (columnIndex < nColumnCount) {
        ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(GetItemAt(columnIndex * 2));
        ASSERT(pHeaderItem != nullptr);
        if (pHeaderItem != nullptr) {
            columnId = pHeaderItem->GetColomnId();
            if (pHeaderItem->GetSplitBox() != nullptr) {
                ASSERT(dynamic_cast<SplitBox*>(GetItemAt(columnIndex * 2 + 1)) == pHeaderItem->GetSplitBox());
                RemoveItem(pHeaderItem->GetSplitBox());
            }
            RemoveItem(pHeaderItem);
            bRet = true;
        }
    }
    if (bRet && (m_pListCtrl != nullptr)) {
        m_pListCtrl->OnHeaderColumnRemoved(columnId);
    }
    return bRet;
}

void ListCtrlHeader::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

bool ListCtrlHeader::IsEnableHeaderDragOrder() const
{
    if (m_pListCtrl != nullptr) {
        return m_pListCtrl->IsEnableHeaderDragOrder();
    }
    return false;
}

void ListCtrlHeader::OnHeaderColumnResized(Control* pLeftHeaderItem, Control* pRightHeaderItem)
{
    size_t nColumnId1 = Box::InvalidIndex;
    size_t nColumnId2 = Box::InvalidIndex;
    ListCtrlHeaderItem* pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(pLeftHeaderItem);
    if (pHeaderItem != nullptr) {
        int32_t nSplitWidth = 0;
        if (pHeaderItem->GetSplitBox() != nullptr) {
            nSplitWidth = pHeaderItem->GetSplitBox()->GetFixedWidth().GetInt32();
        }
        int32_t nItemWidth = pHeaderItem->GetFixedWidth().GetInt32();
        int32_t nColumnWidth = nItemWidth + nSplitWidth;
        pHeaderItem->SetColumnWidth(nColumnWidth, false);
        nColumnId1 = pHeaderItem->GetColomnId();
    }
    pHeaderItem = dynamic_cast<ListCtrlHeaderItem*>(pRightHeaderItem);
    if (pHeaderItem != nullptr) {
        int32_t nSplitWidth = 0;
        if (pHeaderItem->GetSplitBox() != nullptr) {
            nSplitWidth = pHeaderItem->GetSplitBox()->GetFixedWidth().GetInt32();
        }
        int32_t nItemWidth = pHeaderItem->GetFixedWidth().GetInt32();
        int32_t nColumnWidth = nItemWidth + nSplitWidth;
        pHeaderItem->SetColumnWidth(nColumnWidth, false);
        nColumnId2 = pHeaderItem->GetColomnId();
    } 

    if ((nColumnId1 != Box::InvalidIndex) || (nColumnId2 != Box::InvalidIndex)) {
        if (m_pListCtrl != nullptr) {
            m_pListCtrl->OnColumnWidthChanged(nColumnId1, nColumnId2);
        }
    }
}

void ListCtrlHeader::OnHeaderColumnSorted(ListCtrlHeaderItem* pHeaderItem)
{
    if (pHeaderItem == nullptr) {
        return;
    }
    size_t nColumnId = pHeaderItem->GetColomnId();
    ListCtrlHeaderItem::SortMode sortMode = pHeaderItem->GetSortMode();
    ASSERT(sortMode != ListCtrlHeaderItem::SortMode::kNone);
    if (sortMode == ListCtrlHeaderItem::SortMode::kNone) {
        return;
    }
    if (m_pListCtrl != nullptr) {
        bool bSortedUp = (sortMode == ListCtrlHeaderItem::SortMode::kUp) ? true : false;
        m_pListCtrl->OnColumnSorted(nColumnId, bSortedUp);
    }
}

void ListCtrlHeader::OnHeaderColumnOrderChanged()
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->OnHeaderColumnOrderChanged();
    }
}

void ListCtrlHeader::OnHeaderColumnCheckStateChanged(ListCtrlHeaderItem* pHeaderItem, bool bChecked)
{
    if (pHeaderItem == nullptr) {
        return;
    }
    size_t nColumnId = pHeaderItem->GetColomnId();
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->OnHeaderColumnCheckStateChanged(nColumnId, bChecked);
    }
}

/////////////////////////////////////////////////////////////////
/** 列表项的数据管理器
*/
class ListCtrlDataProvider : public ui::VirtualListBoxElement
{
public:
    //用于存储的数据结构
    typedef ListCtrlData Storage;
    typedef std::shared_ptr<Storage> StoragePtr;
    typedef std::vector<StoragePtr> StoragePtrList;
    typedef std::unordered_map<size_t, StoragePtrList> StorageMap;

public:
    ListCtrlDataProvider();

    /** 创建一个数据项
    * @return 返回创建后的数据项指针
    */
    virtual Control* CreateElement() override;

    /** 填充指定数据项
    * @param [in] pControl 数据项控件指针
    * @param [in] nElementIndex 数据元素的索引ID，范围：[0, GetElementCount())
    */
    virtual bool FillElement(ui::Control* pControl, size_t nElementIndex) override;

    /** 获取数据项总数
    * @return 返回数据项总数
    */
    virtual size_t GetElementCount() override;

    /** 设置选择状态
    * @param [in] nElementIndex 数据元素的索引ID，范围：[0, GetElementCount())
    * @param [in] bSelected true表示选择状态，false表示非选择状态
    */
    virtual void SetElementSelected(size_t nElementIndex, bool bSelected) override;

    /** 获取选择状态
    * @param [in] nElementIndex 数据元素的索引ID，范围：[0, GetElementCount())
    * @return true表示选择状态，false表示非选择状态
    */
    virtual bool IsElementSelected(size_t nElementIndex) override;

public:
    /** 设置表头接口
    */
    void SetListCtrl(ListCtrl* pListCtrl);

    /** 增加一列
    * @param [in] columnId 列的ID
    */
    bool AddColumn(size_t columnId);

    /** 删除一列
    * @param [in] columnId 列的ID
    */
    bool RemoveColumn(size_t columnId);

    /** 设置一列的勾选状态（Checked或者UnChecked）
    * @param [in] columnId 列的ID
    * @param [in] bChecked true表示选择，false表示取消选择
    */
    bool SetColumnCheck(size_t columnId, bool bChecked);

    /** 获取数据项总个数
    */
    size_t GetDataItemCount() const;

    /** 设置数据项总个数
    * @param [in] itemCount 数据项的总数，具体每个数据项的数据，通过回调的方式进行填充（内部为虚表实现）
    */
    bool SetDataItemCount(size_t itemCount);

    /** 在最后添加一个数据项
    * @param [in] dataItem 数据项的内容
    * @return 成功返回数据项的行索引号(rowIndex)，失败则返回Box::InvalidIndex
    */
    size_t AddDataItem(const ListCtrlDataItem& dataItem);

    /** 在指定行位置添加一个数据项
    * @param [in] itemIndex 数据项的索引号
    * @param [in] dataItem 数据项的内容
    */
    bool InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** 设置指定行的数据项
    * @param [in] itemIndex 数据项的索引号
    * @param [in] dataItem 数据项的内容
    */
    bool SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem);

    /** 删除指定行的数据项
    * @param [in] itemIndex 数据项的索引号
    */
    bool DeleteDataItem(size_t itemIndex);

    /** 删除所有行的数据项
    */
    bool DeleteAllDataItems();

    /** 设置数据项的自定义数据
    * @param [in] itemIndex 数据项的索引号
    * @param [in] itemData 数据项关联的自定义数据
    */
    bool SetDataItemData(size_t itemIndex, size_t itemData);

    /** 获取数据项的自定义数据
    * @param [in] itemIndex 数据项的索引号
    * @return 返回数据项关联的自定义数据
    */
    size_t GetDataItemData(size_t itemIndex) const;

    /** 设置指定数据项的文本
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @param [in] text 需要设置的文本内容
    */
    bool SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text);

    /** 获取指定数据项的文本
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @return 数据项关联的文本内容
    */
    std::wstring GetDataItemText(size_t itemIndex, size_t columnIndex) const;

    /** 设置指定数据项的文本颜色
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @param [in] textColor 需要设置的文本颜色
    */
    bool SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor);

    /** 获取指定数据项的文本颜色
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @param [out] textColor 数据项关联的文本颜色
    */
    bool GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const;

    /** 设置指定数据项的背景颜色
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @param [in] bkColor 需要设置的背景颜色
    */
    bool SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor);

    /** 获取指定数据项的背景颜色
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @param [out] bkColor 数据项关联的背景颜色
    */
    bool GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const;

    /** 对数据排序
    * @param [in] columnId 列的ID
    * @param [in] bSortedUp true表示升序，false表示降序
    * @param [in] pfnCompareFunc 数据比较函数
    * @param [in] pUserData 用户自定义数据，调用比较函数的时候，通过参数传回给比较函数
    */
    bool SortDataItems(size_t nColumnId, bool bSortedUp, 
                       ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData);

    /** 设置外部自定义的排序函数, 替换默认的排序函数
    * @param [in] pfnCompareFunc 数据比较函数
    * @param [in] pUserData 用户自定义数据，调用比较函数的时候，通过参数传回给比较函数
    */
    void SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData);

private:
    /** 数据转换为存储数据结构
    */
    void DataItemToStorage(Storage& storage, const ListCtrlDataItem& item) const;

    /** 根据列序号查找列ID
    * @return 返回列ID，如果匹配不到，则返回Box::InvalidIndex
    */
    size_t GetColumnId(size_t nColumnIndex) const;

    /** 根据列ID查找列序号
    * @param [in] nColumnId 列ID
    * @return 返回列序号，如果匹配不到，则返回Box::InvalidIndex
    */
    size_t GetColumnIndex(size_t nColumnId) const;

    /** 判断一个数据项索引是否有效
    */
    bool IsValidDataItemIndex(size_t itemIndex) const;

    /** 判断一个列ID在数据存储中是否有效
    */
    bool IsValidDataColumnId(size_t nColumnId) const;

    /** 获取指定数据项的数据, 读取
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @return 如果失败则返回nullptr
    */
    StoragePtr GetDataItemStorage(size_t itemIndex, size_t columnIndex) const;

    /** 获取指定数据项的数据, 写入
    * @param [in] itemIndex 数据项的索引号
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @return 如果失败则返回nullptr
    */
    StoragePtr GetDataItemStorageForWrite(size_t itemIndex, size_t columnIndex);

    /** 获取各个列的数据，用于UI展示
    * @param [in] nDataItemIndex 数据Item的下标，代表行
    * @param [in] columnIdList 列ID列表
    * @param [out] storageList 返回数据列表
    */
    bool GetDataItemStorageList(size_t nDataItemIndex,
                                std::vector<size_t>& columnIdList,
                                StoragePtrList& storageList) const;

    /** 某个数据项的Check勾选状态变化    
    * @param [in] itemIndex 数据Item的下标，代表行
    * @param [in] nColumnId 列ID
    * @param [in] bChecked 是否勾选
    */
    void OnDataItemChecked(size_t itemIndex, size_t nColumnId, bool bChecked);

    /** 同步UI的Check状态
    */
    void UpdateControlCheckStatus(size_t nColumnId);

private:
    /** 排序数据
    */
    struct StorageData
    {
        size_t index;       //原来的数据索引号
        StoragePtr pStorage;
    };

    /** 对数据排序
    * @param [in] dataList 待排序的数据
    * @param [in] nColumnId 列的ID
    * @param [in] bSortedUp true表示升序，false表示降序
    * @param [in] pfnCompareFunc 数据比较函数
    * @param [in] pUserData 用户自定义数据，调用比较函数的时候，通过参数传回给比较函数
    */
    bool SortStorageData(std::vector<StorageData>& dataList, 
                         size_t nColumnId, bool bSortedUp, 
                         ListCtrlDataCompareFunc pfnCompareFunc,
                         void* pUserData);

    /** 默认的数据比较函数
    * @param [in] a 第一个比较数据
    * @param [in] b 第二个比较数据
    * @return 如果 (a < b)，返回true，否则返回false
    */
    bool SortDataCompareFunc(const ListCtrlData& a, const ListCtrlData& b) const;

private:
    /** 表头控件
    */
    ListCtrl* m_pListCtrl;

    /** 数据，按列保存，每个列一个数组
    */
    StorageMap m_dataMap;

    /** 外部设置的排序函数
    */
    ListCtrlDataCompareFunc m_pfnCompareFunc;

    /** 外部设置的排序函数附加数据
    */
    void* m_pUserData;
};

ListCtrlDataProvider::ListCtrlDataProvider() :
    m_pListCtrl(nullptr),
    m_pfnCompareFunc(nullptr),
    m_pUserData(nullptr)
{
}

Control* ListCtrlDataProvider::CreateElement()
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nullptr;
    }
    std::wstring dataItemClass = m_pListCtrl->GetDataItemClass();
    ASSERT(!dataItemClass.empty());
    if (dataItemClass.empty()) {
        return nullptr;
    }
    ListCtrlItem* pItem = new ListCtrlItem;
    pItem->SetClass(dataItemClass);
    return pItem;
}

bool ListCtrlDataProvider::FillElement(ui::Control* pControl, size_t nElementIndex)
{
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return false;
    }
    ListCtrlHeader* pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return false;
    }
    ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(pControl);
    ASSERT(pItem != nullptr);
    if (pItem == nullptr) {
        return false;
    }
    // 基本结构: <ListCtrlItem> <LabelBox/> ... <LabelBox/>  </ListCtrlItem>
    // 附加说明: 1. ListCtrlItem 是 HBox的子类;   
    //          2. 每一列，放置一个LabelBox控件

    //获取需要显示的各个列的属性
    struct ElementData
    {
        size_t nColumnId = Box::InvalidIndex;
        int32_t nColumnWidth = 0;
        StoragePtr pStorage;
    };
    std::vector<ElementData> elementDataList;
    const size_t nColumnCount = pHeaderCtrl->GetColumnCount();
    for (size_t nColumnIndex = 0; nColumnIndex < nColumnCount; ++nColumnIndex) {
        ListCtrlHeaderItem* pHeaderItem = pHeaderCtrl->GetColumn(nColumnIndex);
        if ((pHeaderItem == nullptr) || !pHeaderItem->IsVisible()){
            continue;
        }
        int32_t nColumnWidth = pHeaderCtrl->GetColumnWidth(nColumnIndex);
        if (nColumnWidth < 0) {
            nColumnWidth = 0;
        }
        ElementData data;
        data.nColumnId = pHeaderCtrl->GetColumnId(nColumnIndex);
        data.nColumnWidth = nColumnWidth;
        elementDataList.push_back(data);
    }
    std::vector<size_t> columnIdList;
    for (auto data : elementDataList) {
        columnIdList.push_back(data.nColumnId);
    }
    StoragePtrList storageList;
    if (!GetDataItemStorageList(nElementIndex, columnIdList, storageList)) {
        return false;
    }
    ASSERT(storageList.size() == elementDataList.size());
    for (size_t index = 0; index < storageList.size(); ++index) {
        elementDataList[index].pStorage = storageList[index];
    }
    storageList.clear();
    ASSERT(!elementDataList.empty());
    if (elementDataList.empty()) {
        return false;
    }

    const size_t showColumnCount = elementDataList.size(); //显示的列数
    while (pItem->GetItemCount() > showColumnCount) {
        //移除多余的列
        if (pItem->RemoveItemAt(pItem->GetItemCount() - 1)) {
            ASSERT(!"RemoveItemAt failed!");
            return false;
        }
    }

    //默认属性
    std::wstring defaultLabelBoxClass = m_pListCtrl->GetDataItemLabelClass();
    LabelBox defaultLabelBox;
    if (!defaultLabelBoxClass.empty()) {
        defaultLabelBox.SetClass(defaultLabelBoxClass);
    }
    bool bFirstLine = false;//第一个数据行
    if (m_pListCtrl->m_pDataView != nullptr) {
        bFirstLine = m_pListCtrl->m_pDataView->GetTopElementIndex() == nElementIndex;
    }    
    for (size_t nColumn = 0; nColumn < showColumnCount; ++nColumn) {
        const ElementData& elementData = elementDataList[nColumn];
        LabelBox* pLabelBox = nullptr;
        if (nColumn < pItem->GetItemCount()) {
            pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(nColumn));
            ASSERT(pLabelBox != nullptr);
            if (pLabelBox == nullptr) {
                return false;
            }
        }
        else {
            pLabelBox = new LabelBox;
            pItem->AddItem(pLabelBox);            
            if (!defaultLabelBoxClass.empty()) {
                pLabelBox->SetClass(defaultLabelBoxClass);
            }
            pLabelBox->SetMouseEnabled(false);
        }

        //填充数据，设置属性        
        pLabelBox->SetFixedWidth(UiFixedInt(elementData.nColumnWidth), true, false);
        const StoragePtr& pStorage = elementData.pStorage;
        if (pStorage != nullptr) {
            pLabelBox->SetText(pStorage->text.c_str());
            if (pStorage->nTextFormat != 0) {
                pLabelBox->SetTextStyle(pStorage->nTextFormat, false);
            }
            else {
                pLabelBox->SetTextStyle(defaultLabelBox.GetTextStyle(), false);
            }
            pLabelBox->SetTextPadding(defaultLabelBox.GetTextPadding(), false);
            if (!pStorage->textColor.IsEmpty()) {
                pLabelBox->SetStateTextColor(kControlStateNormal, pLabelBox->GetColorString(pStorage->textColor));
            }
            else {
                pLabelBox->SetStateTextColor(kControlStateNormal, defaultLabelBox.GetStateTextColor(kControlStateNormal));
            }
            if (!pStorage->bkColor.IsEmpty()) {
                pLabelBox->SetBkColor(pStorage->bkColor);
            }
            else {
                pLabelBox->SetBkColor(defaultLabelBox.GetBkColor());
            }
            if (pStorage->bShowCheckBox) {
                //添加CheckBox
                CheckBox* pCheckBox = nullptr;
                if (pLabelBox->GetItemCount() > 0) {
                    pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                }
                if (pCheckBox == nullptr) {
                    pCheckBox = new CheckBox;
                    std::wstring checkBoxClass = m_pListCtrl->GetCheckBoxClass();
                    ASSERT(!checkBoxClass.empty());
                    pLabelBox->AddItem(pCheckBox);
                    if (!checkBoxClass.empty()) {
                        pCheckBox->SetClass(checkBoxClass);
                    }
                }
                UiPadding textPadding = pLabelBox->GetTextPadding();
                int32_t nCheckBoxWidth = pStorage->nCheckBoxWidth;
                if (textPadding.left < nCheckBoxWidth) {
                    textPadding.left = nCheckBoxWidth;
                    pLabelBox->SetTextPadding(textPadding, false);
                }

                //挂载CheckBox的事件处理
                pCheckBox->DetachEvent(kEventSelect);
                pCheckBox->DetachEvent(kEventUnSelect);
                pCheckBox->SetSelected(pStorage->bChecked);
                size_t nColumnId = elementData.nColumnId;
                pCheckBox->AttachSelect([this, nColumnId, nElementIndex](const EventArgs& /*args*/) {
                    OnDataItemChecked(nElementIndex, nColumnId, true);
                    return true;
                    });
                pCheckBox->AttachUnSelect([this, nColumnId, nElementIndex](const EventArgs& /*args*/) {
                    OnDataItemChecked(nElementIndex, nColumnId, false);
                    return true;
                    });
            }
            else {
                if (pLabelBox->GetItemCount() > 0) {
                    CheckBox* pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                    if (pCheckBox != nullptr) {
                        pLabelBox->RemoveItemAt(0);
                    }
                }
            }
        }
        else {
            pLabelBox->SetTextStyle(defaultLabelBox.GetTextStyle(), false);
            pLabelBox->SetText(defaultLabelBox.GetText());
            pLabelBox->SetTextPadding(defaultLabelBox.GetTextPadding(), false);
            pLabelBox->SetStateTextColor(kControlStateNormal, defaultLabelBox.GetStateTextColor(kControlStateNormal));
            pLabelBox->SetBkColor(defaultLabelBox.GetBkColor());
            if (pLabelBox->GetItemCount() > 0) {
                CheckBox* pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                if (pCheckBox != nullptr) {
                    pLabelBox->RemoveItemAt(0);
                }
            }
        }

        //绘制边线
        int32_t mColumnLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);//纵向边线宽度        
        int32_t mRowLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);   //横向边线宽度
        if (m_pListCtrl != nullptr) {
            mColumnLineWidth = m_pListCtrl->GetColumnGridLineWidth();
            mRowLineWidth = m_pListCtrl->GetRowGridLineWidth();
        }
        if (bFirstLine) {
            //第一行
            if (nColumn == 0) {
                //第一列
                UiRect rc(mColumnLineWidth, mRowLineWidth, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
            else {
                //非第一列
                UiRect rc(0, mRowLineWidth, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
        }
        else {
            //非第一行
            if (nColumn == 0) {
                //第一列
                UiRect rc(mColumnLineWidth, 0, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
            else {
                //非第一列
                UiRect rc(0, 0, mColumnLineWidth, mRowLineWidth);
                pLabelBox->SetBorderSize(rc, false);
            }
        }
    }
    return true;
}

size_t ListCtrlDataProvider::GetElementCount()
{
    return GetDataItemCount();
}

void ListCtrlDataProvider::SetElementSelected(size_t nElementIndex, bool bSelected)
{
    StoragePtrList& storageList = m_dataMap[0];
    ASSERT(nElementIndex < storageList.size());
    if (nElementIndex < storageList.size()) {
        StoragePtr pStorage = storageList.at(nElementIndex);
        if (pStorage != nullptr) {
            pStorage->bSelected = bSelected;
        }
        else {
            pStorage = std::make_shared<Storage>();
            pStorage->bSelected = bSelected;
            storageList[nElementIndex] = pStorage;
        }
    }
}

bool ListCtrlDataProvider::IsElementSelected(size_t nElementIndex)
{
    bool bSelected = false;
    const StoragePtrList& storageList = m_dataMap[0];
    ASSERT(nElementIndex < storageList.size());
    if (nElementIndex < storageList.size()) {
        const StoragePtr pStorage = storageList.at(nElementIndex);
        if (pStorage != nullptr) {
            bSelected = pStorage->bSelected;
        }        
    }
    return bSelected;
}

void ListCtrlDataProvider::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

void ListCtrlDataProvider::DataItemToStorage(Storage& storage, const ListCtrlDataItem& item) const
{
    storage.text = item.text;
    if (item.nTextFormat >= 0) {
        storage.nTextFormat = TruncateToUInt16(item.nTextFormat);
    }
    else {
        storage.nTextFormat = 0;
    }
    int32_t nCheckBoxWidth = item.nCheckBoxWidth;
    if (item.bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nCheckBoxWidth);
    }
    storage.nImageIndex = item.nImageIndex;
    storage.textColor = item.textColor;
    storage.bkColor = item.bkColor;
    storage.bShowCheckBox = item.bShowCheckBox;
    storage.nCheckBoxWidth = TruncateToUInt8(nCheckBoxWidth);

    storage.bSelected = false;
    storage.bChecked = false;
    storage.nItemData = 0;
}

size_t ListCtrlDataProvider::GetColumnId(size_t nColumnIndex) const
{
    ListCtrlHeader* pHeaderCtrl = nullptr;
    if (m_pListCtrl != nullptr) {
        pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    }
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    size_t columnId = pHeaderCtrl->GetColumnId(nColumnIndex);
    return columnId;
}

size_t ListCtrlDataProvider::GetColumnIndex(size_t nColumnId) const
{
    ListCtrlHeader* pHeaderCtrl = nullptr;
    if (m_pListCtrl != nullptr) {
        pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    }
    ASSERT(pHeaderCtrl != nullptr);
    if (pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    size_t columnIndex = pHeaderCtrl->GetColumnIndex(nColumnId);
    return columnIndex;
}

bool ListCtrlDataProvider::IsValidDataItemIndex(size_t itemIndex) const
{
    if (itemIndex == Box::InvalidIndex) {
        return false;
    }
    if (m_dataMap.empty()) {
        return false;
    }
    bool bValidItemIndex = true;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        const StoragePtrList& storageList = iter->second;
        if (itemIndex >= storageList.size()) {
            bValidItemIndex = false;
            break;
        }
    }
    return bValidItemIndex;
}

bool ListCtrlDataProvider::IsValidDataColumnId(size_t nColumnId) const
{
    bool bValidColumnId = false;
    if (nColumnId == Box::InvalidIndex) {
        return false;
    }
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        if (id == nColumnId) {
            bValidColumnId = true;
            break;
        }
    }
    return bValidColumnId;
}

bool ListCtrlDataProvider::AddColumn(size_t columnId)
{
    ASSERT((columnId != Box::InvalidIndex) && (columnId != 0));
    if ((columnId == Box::InvalidIndex) || (columnId == 0)) {
        return false;
    }
    StoragePtrList& storageList = m_dataMap[columnId];
    auto iter = m_dataMap.find(0);
    if (iter != m_dataMap.end()) {
        //保持数据行数相同
        storageList.resize(iter->second.size());
    }
    else {
        m_dataMap[0].resize(storageList.size());
    }
    EmitCountChanged();
    return true;
}

bool ListCtrlDataProvider::RemoveColumn(size_t columnId)
{
    ASSERT((columnId != Box::InvalidIndex) && (columnId != 0));
    if ((columnId == Box::InvalidIndex) || (columnId == 0)) {
        return false;
    }
    auto iter = m_dataMap.find(columnId);
    if (iter != m_dataMap.end()) {
        m_dataMap.erase(iter);
        if (m_dataMap.size() == 1) {
            iter = m_dataMap.find(0);
            if (iter != m_dataMap.end()) {
                m_dataMap.erase(iter);
            }
        }
        EmitCountChanged();
        return true;
    }
    return false;
}

bool ListCtrlDataProvider::SetColumnCheck(size_t columnId, bool bChecked)
{
    ASSERT((columnId != Box::InvalidIndex) && (columnId != 0));
    if ((columnId == Box::InvalidIndex) || (columnId == 0)) {
        return false;
    }
    auto iter = m_dataMap.find(columnId);
    if (iter != m_dataMap.end()) {
        StoragePtrList& storageList = iter->second;
        for (StoragePtr pStorage : storageList) {
            if (pStorage != nullptr) {
                pStorage->bChecked = bChecked;
            }
        }
    }
    return true;
}

ListCtrlDataProvider::StoragePtr ListCtrlDataProvider::GetDataItemStorage(
    size_t itemIndex, size_t columnIndex) const
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //索引号无效
        return nullptr;
    }
    size_t columnId = GetColumnId(columnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return nullptr;
    }
    StoragePtr pStorage;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        if (id == columnId) {
            const StoragePtrList& storageList = iter->second;
            if (itemIndex < storageList.size()) {
                //关联列：获取数据
                pStorage = storageList.at(itemIndex);
            }
            break;
        }
    }
    return pStorage;
}

ListCtrlDataProvider::StoragePtr ListCtrlDataProvider::GetDataItemStorageForWrite(
    size_t itemIndex, size_t columnIndex)
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //索引号无效
        return nullptr;
    }
    size_t columnId = GetColumnId(columnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return nullptr;
    }

    StoragePtr pStorage;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        if (id == columnId) {
            StoragePtrList& storageList = iter->second;
            if (itemIndex < storageList.size()) {
                //关联列：获取可更新数据
                pStorage = storageList.at(itemIndex);
                if (pStorage == nullptr) {
                    pStorage = std::make_shared<Storage>();
                    storageList[itemIndex] = pStorage;
                }
            }
            break;
        }
    }
    return pStorage;
}

bool ListCtrlDataProvider::GetDataItemStorageList(size_t nDataItemIndex, std::vector<size_t>& columnIdList,
                                                  StoragePtrList& storageList) const
{
    storageList.clear();
    ASSERT(nDataItemIndex != Box::InvalidIndex);
    if (nDataItemIndex == Box::InvalidIndex) {
        return false;
    }
    for (size_t nColumnId : columnIdList) {
        auto iter = m_dataMap.find(nColumnId);
        StoragePtr pStorage;
        if (iter != m_dataMap.end()) {
            const StoragePtrList& dataList = iter->second;
            if (nDataItemIndex < dataList.size()) {
                pStorage = dataList.at(nDataItemIndex);
            }
        }
        storageList.push_back(pStorage);
    }
    return storageList.size() == columnIdList.size();
}

void ListCtrlDataProvider::OnDataItemChecked(size_t itemIndex, size_t nColumnId, bool bChecked)
{
    auto iter = m_dataMap.find(nColumnId);
    StoragePtr pStorage;
    if (iter != m_dataMap.end()) {
        const StoragePtrList& dataList = iter->second;
        if (itemIndex < dataList.size()) {
            pStorage = dataList.at(itemIndex);
        }
    }
    if (pStorage != nullptr) {
        //更新存储状态
        pStorage->bChecked = bChecked;
    }

    //更新header的勾选状态（三态），仅仅同步UI的状态
    UpdateControlCheckStatus(nColumnId);
}

void ListCtrlDataProvider::UpdateControlCheckStatus(size_t nColumnId)
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->UpdateControlCheckStatus(nColumnId);
    }
}

size_t ListCtrlDataProvider::GetDataItemCount() const
{
    size_t nDataCount = 0;
    if (!m_dataMap.empty()) {        
        auto iter = m_dataMap.begin();
        for (; iter != m_dataMap.end(); ++iter) {
            if (!iter->second.empty()) {
                ASSERT((nDataCount == 0) || (nDataCount == iter->second.size()));
                nDataCount = std::max(nDataCount, iter->second.size());
            }
        }
    }
    return nDataCount;
}

bool ListCtrlDataProvider::SetDataItemCount(size_t itemCount)
{
    ASSERT(itemCount != Box::InvalidIndex);
    if (itemCount == Box::InvalidIndex) {
        return false;
    }
    if (m_dataMap.empty()) {
        m_dataMap[0].resize(itemCount);
    }
    else {
        m_dataMap[0].resize(itemCount);
        auto iter = m_dataMap.begin();
        for (; iter != m_dataMap.end(); ++iter) {
            iter->second.resize(itemCount);
        }
    }
    EmitCountChanged();
    return true;
}

size_t ListCtrlDataProvider::AddDataItem(const ListCtrlDataItem& dataItem)
{
    size_t columnId = GetColumnId(dataItem.nColumnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return Box::InvalidIndex;
    }
    ASSERT(!m_dataMap.empty());
    ASSERT(m_dataMap.find(0) != m_dataMap.end());
    ASSERT(m_dataMap.find(columnId) != m_dataMap.end());

    Storage storage;
    DataItemToStorage(storage, dataItem);

    size_t nDataItemIndex = Box::InvalidIndex;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        StoragePtrList& storageList = iter->second;
        if ((id == 0) || (id == columnId)) {
            //关联列：保存数据
            storageList.push_back(std::make_shared<Storage>(storage));
            if (id == columnId) {
                nDataItemIndex = storageList.size();
            }
        }
        else {
            //其他列：插入空数据
            storageList.push_back(nullptr);
        }
    }

    EmitCountChanged();
    return nDataItemIndex;
}

bool ListCtrlDataProvider::InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    size_t columnId = GetColumnId(dataItem.nColumnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return false;
    }

    if (!IsValidDataItemIndex(itemIndex)) {
        //如果索引号无效，则按追加的方式添加数据
        return (AddDataItem(dataItem) != Box::InvalidIndex);
    }

    Storage storage;
    DataItemToStorage(storage, dataItem);

    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;
        StoragePtrList& storageList = iter->second;
        if ((id == 0) || (id == columnId)) {
            //关联列：保存数据
            storageList.insert(storageList.begin() + itemIndex, std::make_shared<Storage>(storage));
        }
        else {
            //其他列：插入空数据
            storageList.insert(storageList.begin() + itemIndex, nullptr);
        }
    }

    EmitCountChanged();
    return true;
}

bool ListCtrlDataProvider::SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    size_t columnId = GetColumnId(dataItem.nColumnIndex);
    ASSERT(IsValidDataColumnId(columnId));
    if (!IsValidDataColumnId(columnId)) {
        return false;
    }

    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //索引号无效
        return false;
    }

    Storage storage;
    DataItemToStorage(storage, dataItem);

    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        size_t id = iter->first;        
        if (id == columnId) {
            StoragePtrList& storageList = iter->second;
            //关联列：更新数据
            StoragePtr pStorage = storageList[itemIndex];
            if (pStorage == nullptr) {
                storageList[itemIndex] = std::make_shared<Storage>(storage);
            }
            else {
                *pStorage = storage;
            }
            break;
        }
    }

    EmitDataChanged(itemIndex, itemIndex);
    return true;
}

bool ListCtrlDataProvider::DeleteDataItem(size_t itemIndex)
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //索引号无效
        return false;
    }

    bool bDeleted = false;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        StoragePtrList& storageList = iter->second;
        if (itemIndex < storageList.size()) {
            storageList.erase(storageList.begin() + itemIndex);
            bDeleted = true;
        }
    }

    if (bDeleted) {
        EmitCountChanged();
    }    
    return bDeleted;
}

bool ListCtrlDataProvider::DeleteAllDataItems()
{
    bool bDeleted = false;
    for (auto iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        StoragePtrList& storageList = iter->second;
        if (!storageList.empty()) {
            bDeleted = true;
        }
        StoragePtrList emptyList;
        storageList.swap(emptyList);
    }
    if (bDeleted) {
        EmitCountChanged();
    }
    return bDeleted;
}

bool ListCtrlDataProvider::SetDataItemData(size_t itemIndex, size_t itemData)
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //索引号无效
        return false;
    }

    StoragePtrList& storageList = m_dataMap[0];
    if (itemIndex < storageList.size()) {
        StoragePtr pStorage = storageList[itemIndex];
        if (pStorage == nullptr) {
            pStorage = std::make_shared<Storage>();
            storageList[itemIndex] = pStorage;
        }
        pStorage->nItemData = itemData;
        return true;
    }
    return false;
}

size_t ListCtrlDataProvider::GetDataItemData(size_t itemIndex) const
{
    ASSERT(IsValidDataItemIndex(itemIndex));
    if (!IsValidDataItemIndex(itemIndex)) {
        //索引号无效
        return 0;
    }
    size_t nItemData = 0;
    auto iter = m_dataMap.find(0);
    if (iter != m_dataMap.end()) {
        const StoragePtrList& storageList = iter->second;
        if (itemIndex < storageList.size()) {
            StoragePtr pStorage = storageList[itemIndex];
            if (pStorage != nullptr) {
                nItemData = pStorage->nItemData;
            }
        }
    }
    return nItemData;
}

bool ListCtrlDataProvider::SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text)
{
    StoragePtr pStorage = GetDataItemStorageForWrite(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //索引号无效
        return false;
    }
    pStorage->text = text;
    EmitDataChanged(itemIndex, itemIndex);
    return true;
}

std::wstring ListCtrlDataProvider::GetDataItemText(size_t itemIndex, size_t columnIndex) const
{
    StoragePtr pStorage = GetDataItemStorage(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //索引号无效
        return std::wstring();
    }
    return pStorage->text.c_str();
}

bool ListCtrlDataProvider::SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor)
{
    StoragePtr pStorage = GetDataItemStorageForWrite(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //索引号无效
        return false;
    }
    pStorage->textColor = textColor;
    EmitDataChanged(itemIndex, itemIndex);
    return true;
}

bool ListCtrlDataProvider::GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const
{
    StoragePtr pStorage = GetDataItemStorage(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //索引号无效
        return false;
    }
    textColor = pStorage->textColor;
    return true;
}

bool ListCtrlDataProvider::SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor)
{
    StoragePtr pStorage = GetDataItemStorageForWrite(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //索引号无效
        return false;
    }
    pStorage->bkColor = bkColor;
    EmitDataChanged(itemIndex, itemIndex);
    return true;
}

bool ListCtrlDataProvider::GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const
{
    StoragePtr pStorage = GetDataItemStorage(itemIndex, columnIndex);
    ASSERT(pStorage != nullptr);
    if (pStorage == nullptr) {
        //索引号无效
        return false;
    }
    bkColor = pStorage->bkColor;
    return true;
}

bool ListCtrlDataProvider::SortDataItems(size_t nColumnId, bool bSortedUp, 
                                         ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData)
{
    StorageMap::iterator iter = m_dataMap.find(nColumnId);
    ASSERT(iter != m_dataMap.end());
    if (iter == m_dataMap.end()) {
        return false;
    }
    StoragePtrList sortStorageList = iter->second;
    if (sortStorageList.empty()) {
        return false;
    }
    std::vector<StorageData> sortedDataList;
    const size_t dataCount = sortStorageList.size();
    for (size_t index = 0; index < dataCount; ++index) {
        sortedDataList.push_back({index, sortStorageList[index] });
    }    
    SortStorageData(sortedDataList, nColumnId, bSortedUp, pfnCompareFunc, pUserData);

    //对原数据进行顺序调整
    const size_t sortedDataCount = sortedDataList.size();
    StoragePtrList orgStorageList; //副本数据
    for (iter = m_dataMap.begin(); iter != m_dataMap.end(); ++iter) {
        orgStorageList = iter->second; 
        StoragePtrList& storageList = iter->second;   //修改目标
        ASSERT(storageList.size() == sortedDataList.size());
        if (storageList.size() != sortedDataList.size()) {
            return false;
        }
        for (size_t index = 0; index < sortedDataCount; ++index) {
            const StorageData& sortedData = sortedDataList[index];
            storageList[index] = orgStorageList[sortedData.index]; //赋值原数据
        }
    }
    return true;
}

bool ListCtrlDataProvider::SortStorageData(std::vector<StorageData>& dataList,                                            
                                           size_t nColumnId, bool bSortedUp,
                                           ListCtrlDataCompareFunc pfnCompareFunc,
                                           void* pUserData)
{
    if (dataList.empty()) {
        return false;
    }

    if (pfnCompareFunc == nullptr) {
        //如果无有效参数，则使用外部设置的排序函数
        pfnCompareFunc = m_pfnCompareFunc;
        pUserData = m_pUserData;
    }

    if (pfnCompareFunc != nullptr) {
        //使用自定义的比较函数排序
        ListCtrlCompareParam param;
        param.nColumnId = nColumnId;
        param.nColumnIndex = Box::InvalidIndex;
        param.pUserData = pUserData;
        if (m_pListCtrl != nullptr) {
            param.nColumnIndex = m_pListCtrl->GetColumnIndex(nColumnId);
        }
        std::sort(dataList.begin(), dataList.end(), [this, pfnCompareFunc, &param](const StorageData& a, const StorageData& b) {
                //实现(a < b)的比较逻辑
                if (b.pStorage == nullptr) {
                    return false;
                }
                if (a.pStorage == nullptr) {
                    return true;
                }
                const Storage& storageA = *a.pStorage;
                const Storage& storageB = *b.pStorage;
                return pfnCompareFunc(storageA, storageB, param);
            });
    }
    else {
        //排序：升序，使用默认的排序函数
        std::sort(dataList.begin(), dataList.end(), [this](const StorageData& a, const StorageData& b) {
                //实现(a < b)的比较逻辑
                if (b.pStorage == nullptr) {
                    return false;
                }
                if (a.pStorage == nullptr) {
                    return true;
                }
                const Storage& storageA = *a.pStorage;
                const Storage& storageB = *b.pStorage;
                return SortDataCompareFunc(storageA, storageB);
            });
    }
    if (!bSortedUp) {
        //降序
        std::reverse(dataList.begin(), dataList.end());
    }
    return true;
}

bool ListCtrlDataProvider::SortDataCompareFunc(const ListCtrlData& a, const ListCtrlData& b) const
{
    //默认按字符串比较, 区分大小写
    return ::wcscmp(a.text.c_str(), b.text.c_str()) < 0;
}

void ListCtrlDataProvider::SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData)
{
    m_pfnCompareFunc = pfnCompareFunc;
    m_pUserData = pUserData;
}

UiSize64 ListCtrlDataLayout::ArrangeChild(const std::vector<ui::Control*>& /*items*/, ui::UiRect rc)
{
    ListCtrlDataView* pList = dynamic_cast<ListCtrlDataView*>(m_pOwner);
    if ((pList == nullptr) || !pList->HasDataProvider()) {
        ASSERT(FALSE);
        return UiSize64();
    }
    DeflatePadding(rc);
    int64_t nTotalHeight = GetElementsHeight(rc, Box::InvalidIndex);
    UiSize64 sz(rc.Width(), rc.Height());
    sz.cy = std::max(nTotalHeight, sz.cy);
    sz.cx = std::max(GetItemWidth(), rc.Width()); //允许出现横向滚动条
    LazyArrangeChild(rc);
    return sz;
}

UiSize ListCtrlDataLayout::EstimateSizeByChild(const std::vector<Control*>& /*items*/, ui::UiSize szAvailable)
{
    ListCtrlDataView* pList = dynamic_cast<ListCtrlDataView*>(m_pOwner);
    if ((pList == nullptr) || !pList->HasDataProvider()) {
        ASSERT(FALSE);
        return UiSize();
    }
    UiRect rc(0, 0, szAvailable.cx, szAvailable.cy);
    UiSize szItem = GetElementSize(rc, 0);
    ASSERT((szItem.cx > 0) || (szItem.cy > 0));
    if ((szItem.cx <= 0) || (szItem.cy <= 0)) {
        return UiSize();
    }
    szAvailable.Validate();
    int32_t nColumns = CalcTileColumns(szAvailable.cx);
    UiEstSize estSize;
    if (m_pOwner != nullptr) {
        estSize = m_pOwner->Control::EstimateSize(szAvailable);
    }
    UiSize size(estSize.cx.GetInt32(), estSize.cy.GetInt32());
    if (estSize.cx.IsStretch()) {
        size.cx = CalcStretchValue(estSize.cx, szAvailable.cx);
    }
    if (estSize.cy.IsStretch()) {
        size.cy = CalcStretchValue(estSize.cy, szAvailable.cy);
    }
    if (size.cx == 0) {
        size.cx = szItem.cx * nColumns + GetChildMarginX() * (nColumns - 1);
    }
    size.Validate();
    return size;
}

void ListCtrlDataLayout::LazyArrangeChild(UiRect rc) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return;
    }
    if (!pOwnerBox->HasDataProvider()) {
        return;
    }

    const size_t nItemCount = pOwnerBox->GetItemCount();
    if (nItemCount > 0) {
        //第一个元素是表头控件，设置其位置大小
        Control* pHeaderCtrl = pOwnerBox->GetItemAt(0);
        if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()){
            int32_t nHeaderHeight = pHeaderCtrl->GetFixedHeight().GetInt32();
            if (nHeaderHeight > 0) {
                int32_t nHeaderWidth = GetElementSize(rc, 0).cx;
                if (nHeaderWidth <= 0) {
                    nHeaderWidth = rc.Width();
                }
                ui::UiRect rcTile(rc.left, rc.top, rc.left + nHeaderWidth, rc.top + nHeaderHeight);
                pHeaderCtrl->SetPos(rcTile);
                rc.top += nHeaderHeight;
            }
        }
    }

    // 顶部元素的索引号
    const size_t nTopElementIndex = GetTopElementIndex(rc);

    //列数
    int32_t nColumns = CalcTileColumns(rc.Width());

    //子项的左边起始位置 
    int32_t iPosLeft = rc.left;

    //Y轴坐标的偏移，需要保持，避免滚动位置变动后，重新刷新界面出现偏差
    int32_t yOffset = 0;
    UiSize szTopItem = GetElementSize(rc, nTopElementIndex);
    if (szTopItem.cy > 0) {
        yOffset = TruncateToInt32(pOwnerBox->GetScrollPos().cy % szTopItem.cy);
    }

    //子项的顶部起始位置
    int32_t iPosTop = rc.top - yOffset;

    //设置虚拟偏移，否则当数据量较大时，rc这个32位的矩形的高度会越界，需要64位整型才能容纳
    pOwnerBox->SetScrollVirtualOffsetY(pOwnerBox->GetScrollPos().cy);

    //控件的左上角坐标值
    ui::UiPoint ptTile(iPosLeft, iPosTop);

    UiSize szItem;
    size_t iCount = 0;    
    for (size_t index = 1; index < nItemCount; ++index) {
        //第一个元素是表头控件，跳过填充数据
        Control* pControl = pOwnerBox->GetItemAt(index);
        if (pControl == nullptr) {
            continue;
        }
        //当前数据元素的索引号
        size_t nElementIndex = nTopElementIndex + iCount;
        szItem = GetElementSize(rc, nElementIndex);

        //设置当前控件的大小和位置
        ui::UiRect rcTile(ptTile.x, ptTile.y, ptTile.x + szItem.cx, ptTile.y + szItem.cy);
        pControl->SetPos(rcTile);

        // 填充数据        
        if (nElementIndex < pOwnerBox->GetElementCount()) {
            if (!pControl->IsVisible()) {
                pControl->SetVisible(true);
            }
            pOwnerBox->FillElement(pControl, nElementIndex);
        }
        else {
            if (pControl->IsVisible()) {
                pControl->SetVisible(false);
            }
        }

        if ((++iCount % nColumns) == 0) {
            ptTile.x = iPosLeft;
            ptTile.y += szItem.cy + GetChildMarginY();
        }
        else {
            ptTile.x += rcTile.Width() + GetChildMarginX();
        }
    }
    pOwnerBox->OnArrangeChild();
}

size_t ListCtrlDataLayout::AjustMaxItem(UiRect rc) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return 0;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 0;
    }
    if (rc.IsEmpty()) {
        return 0;
    }
    int32_t nColumns = CalcTileColumns(rc.Width());
    int32_t nRows = rc.Height() / (nItemHeight + GetChildMarginY() / 2);
    //验证并修正
    if (nRows > 1) {
        int32_t calcHeight = nRows * nItemHeight + (nRows - 1) * GetChildMarginY();
        if (calcHeight < rc.Height()) {
            nRows += 1;
        }
    }
    //额外增加1行，确保真实控件填充满整个可显示区域
    nRows += 1;
    return nRows * nColumns;
}

size_t ListCtrlDataLayout::GetTopElementIndex(UiRect rc) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return 0;
    }
    int64_t nPos = pOwnerBox->GetScrollPos().cy;
    if (nPos < 0) {
        nPos = 0;
    }

    int64_t nColumns = (int64_t)CalcTileColumns(rc.Width());
    if (nColumns < 0) {
        nColumns = 0;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 0;
    }
    int64_t iIndex = (nPos / nItemHeight) * nColumns;
    return static_cast<size_t>(iIndex);
}

bool ListCtrlDataLayout::IsElementDisplay(UiRect rc, size_t iIndex) const
{
    if (!Box::IsValidItemIndex(iIndex)) {
        return false;
    }
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return false;
    }

    int64_t nPos = pOwnerBox->GetScrollPos().cy;
    int64_t nElementPos = GetElementsHeight(rc, iIndex);
    if (nElementPos >= nPos) {
        int64_t nHeight = pOwnerBox->GetHeight();
        if (nElementPos <= nPos + nHeight) {
            return true;
        }
    }
    return false;
}

bool ListCtrlDataLayout::NeedReArrange() const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return false;
    }
    if (!pOwnerBox->HasDataProvider()) {
        return false;
    }
    size_t nCount = pOwnerBox->GetItemCount();
    if (nCount == 0) {
        return false;
    }

    if (pOwnerBox->GetElementCount() <= nCount) {
        return false;
    }

    ui::UiRect rcThis = pOwnerBox->GetPos();
    if (rcThis.IsEmpty()) {
        return false;
    }

    int64_t nScrollPosY = pOwnerBox->GetScrollPos().cy;
    int64_t nVirtualOffsetY = pOwnerBox->GetScrollVirtualOffset().cy;

    if (nScrollPosY >= nVirtualOffsetY) {
        //向下滚动
        ui::UiRect rcItem = pOwnerBox->GetItemAt(nCount - 1)->GetPos();
        if ((rcItem.bottom + nVirtualOffsetY) < (nScrollPosY + rcThis.bottom)) {
            return true;
        }
    }
    else {
        //向上滚动
        ui::UiRect rcItem = pOwnerBox->GetItemAt(0)->GetPos();
        if ((rcItem.top + nVirtualOffsetY) > (nScrollPosY + rcThis.top)) {
            return true;
        }
    }
    return false;
}

void ListCtrlDataLayout::GetDisplayElements(UiRect rc, std::vector<size_t>& collection) const
{
    collection.clear();
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return;
    }

    if (pOwnerBox->GetItemCount() == 0) {
        return;
    }

    size_t nEleHeight = GetElementsHeight(rc, 1);
    if (nEleHeight == 0) {
        return;
    }

    int32_t nColumns = CalcTileColumns(rc.Width());
    size_t min = ((size_t)pOwnerBox->GetScrollPos().cy / nEleHeight) * nColumns;
    size_t max = min + ((size_t)rc.Height() / nEleHeight) * nColumns;

    size_t nCount = pOwnerBox->GetElementCount();
    if (nCount > 0) {
        if (max >= nCount) {
            max = nCount - 1;
        }
    }
    else {
        return;
    }

    for (size_t i = min; i <= max; ++i) {
        collection.push_back(i);
    }
}

void ListCtrlDataLayout::EnsureVisible(UiRect rc, size_t iIndex, bool bToTop) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return;
    }
    if (!Box::IsValidItemIndex(iIndex) || iIndex >= pOwnerBox->GetElementCount()) {
        return;
    }
    if (pOwnerBox->m_pVScrollBar == nullptr) {
        return;
    }
    int64_t nPos = pOwnerBox->GetScrollPos().cy;
    int64_t elementHeight = GetElementsHeight(rc, 1);
    if (elementHeight == 0) {
        return;
    }
    int32_t nColumns = CalcTileColumns(rc.Width());
    int64_t nTopIndex = 0;
    if (elementHeight > 0) {
        nTopIndex = (nPos / elementHeight) * nColumns;
    }
    int64_t nNewPos = 0;

    if (bToTop)
    {
        nNewPos = GetElementsHeight(rc, iIndex);
        if (nNewPos > elementHeight) {
            nNewPos -= elementHeight;
        }
    }
    else {
        if (IsElementDisplay(rc, iIndex)) {
            return;
        }

        if ((int64_t)iIndex > nTopIndex) {
            // 向下
            int64_t height = GetElementsHeight(rc, iIndex + 1);
            nNewPos = height - pOwnerBox->GetRect().Height();
        }
        else {
            // 向上
            nNewPos = GetElementsHeight(rc, iIndex);
            if (nNewPos > elementHeight) {
                nNewPos -= elementHeight;
            }
        }
    }
    if (nNewPos < 0) {
        nNewPos = 0;
    }
    if (nNewPos > pOwnerBox->m_pVScrollBar->GetScrollRange()) {
        nNewPos = pOwnerBox->m_pVScrollBar->GetScrollRange();
    }
    ui::UiSize64 sz(0, nNewPos);
    pOwnerBox->SetScrollPos(sz);
}

ListCtrlDataView* ListCtrlDataLayout::GetOwnerBox() const
{
    ListCtrlDataView* pList = dynamic_cast<ListCtrlDataView*>(m_pOwner);
    ASSERT(pList != nullptr);
    return pList;
}

int64_t ListCtrlDataLayout::GetElementsHeight(UiRect rc, size_t nCount) const
{
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox == nullptr) {
        return 0;
    }
    int32_t nItemHeight = GetItemHeight();
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return 0;
    }
    int32_t nColumns = CalcTileColumns(rc.Width());
    if (nCount <= nColumns && nCount != Box::InvalidIndex) {
        //不到1行，或者刚好1行
        return nItemHeight + GetChildMarginY();
    }
    if (!Box::IsValidItemIndex(nCount)) {
        nCount = pOwnerBox->GetElementCount();
    }
    if (!Box::IsValidItemIndex(nCount)) {
        ASSERT(FALSE);
        return nItemHeight + GetChildMarginY();
    }

    int64_t rows = nCount / nColumns;
    if (nCount % nColumns != 0) {
        rows += 1;
    }
    int64_t iChildMargin = 0;
    if (GetChildMarginY() > 0) {
        iChildMargin = GetChildMarginY();
    }
    if (nCount > 0) {
        int64_t childMarginTotal = 0;
        if (nCount % nColumns == 0) {
            childMarginTotal = ((int64_t)nCount / nColumns - 1) * iChildMargin;
        }
        else {
            childMarginTotal = ((int64_t)nCount / nColumns) * iChildMargin;
        }
        return nItemHeight * (rows + 1) + childMarginTotal;
    }
    return 0;
}

UiSize ListCtrlDataLayout::GetElementSize(UiRect rc, size_t /*nElementIndex*/) const
{
    UiSize szElementSize;
    szElementSize.cx = std::max(GetItemWidth(), rc.Width());
    szElementSize.cy = GetItemHeight();
    return szElementSize;
}

int32_t ListCtrlDataLayout::CalcTileColumns(int32_t /*rcWidth*/) const
{
    //固定，只有一列
    return 1;
}

int32_t ListCtrlDataLayout::GetItemWidth() const
{
    int32_t nItemWidth = 0;
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox != nullptr) {
        nItemWidth = pOwnerBox->GetListCtrlWidth();
    }
    return nItemWidth;
}

int32_t ListCtrlDataLayout::GetItemHeight() const
{
    //////////////////////////////////////////
    //TODO:
    return 64;
}

int32_t ListCtrlDataLayout::GetHeaderHeight() const
{
    int32_t nHeaderHeight = 0;
    size_t nItemCount = 0;
    ListCtrlDataView* pOwnerBox = GetOwnerBox();
    if (pOwnerBox != nullptr) {
        nItemCount = pOwnerBox->GetItemCount();
    }
    if (nItemCount > 0) {
        //第一个元素是表头控件，设置其位置大小
        Control* pHeaderCtrl = pOwnerBox->GetItemAt(0);
        if ((pHeaderCtrl != nullptr) && pHeaderCtrl->IsVisible()) {
            nHeaderHeight = pHeaderCtrl->GetFixedHeight().GetInt32();            
        }
    }
    return nHeaderHeight;
}

/** 列表数据显示控件
*/
ListCtrlDataView::ListCtrlDataView() :
    VirtualListBox(new ListCtrlDataLayout),
    m_pListCtrl(nullptr),
    m_nTopElementIndex(0)
{
    VirtualLayout* pVirtualLayout = dynamic_cast<VirtualLayout*>(GetLayout());
    SetVirtualLayout(pVirtualLayout);
}

ListCtrlDataView::~ListCtrlDataView() 
{
}

void ListCtrlDataView::SetListCtrl(ListCtrl* pListCtrl)
{
    m_pListCtrl = pListCtrl;
}

int32_t ListCtrlDataView::GetListCtrlWidth() const
{
    int32_t nToltalWidth = 0;
    ASSERT(m_pListCtrl != nullptr);
    if (m_pListCtrl == nullptr) {
        return nToltalWidth;
    }
    ListCtrlHeader* pHeaderCtrl = m_pListCtrl->GetListCtrlHeader();
    if (pHeaderCtrl == nullptr) {
        return nToltalWidth;
    }
    size_t nColumnCount = pHeaderCtrl->GetColumnCount();
    for (size_t index = 0; index < nColumnCount; ++index) {
        ListCtrlHeaderItem* pHeaderItem = pHeaderCtrl->GetColumn(index);
        if ((pHeaderItem != nullptr) && pHeaderItem->IsVisible()) {
            nToltalWidth += pHeaderItem->GetColumnWidth();
        }
    }
    return nToltalWidth;
}

void ListCtrlDataView::SetTopElementIndex(size_t nTopElementIndex)
{
    m_nTopElementIndex = nTopElementIndex;
}

size_t ListCtrlDataView::GetTopElementIndex() const
{
    return m_nTopElementIndex;
}

void ListCtrlDataView::OnRefresh()
{
}

void ListCtrlDataView::OnArrangeChild()
{
    if (m_pListCtrl != nullptr) {
        m_pListCtrl->UpdateControlCheckStatus(Box::InvalidIndex);
    }
}

ListCtrl::ListCtrl():
    m_bInited(false),
    m_pHeaderCtrl(nullptr),
    m_pDataView(nullptr),
    m_bEnableHeaderDragOrder(true),
    m_bCanUpdateHeaderCheckStatus(true),
    m_bShowHeaderCtrl(true)
{
    m_pDataProvider = new ListCtrlDataProvider;
    m_nRowGridLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);
    m_nColumnGridLineWidth = GlobalManager::Instance().Dpi().GetScaleInt(1);
    m_nItemHeight = GlobalManager::Instance().Dpi().GetScaleInt(32);
    m_nHeaderHeight = m_nItemHeight;
}

ListCtrl::~ListCtrl()
{
    if (m_pDataProvider != nullptr) {
        delete m_pDataProvider;
        m_pDataProvider = nullptr;
    }
}

std::wstring ListCtrl::GetType() const { return DUI_CTR_LISTCTRL; }

void ListCtrl::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
    if (strName == L"header_class") {
        SetHeaderClass(strValue);
    }
    else if (strName == L"header_item_class") {
        SetHeaderItemClass(strValue);
    }
    else if (strName == L"header_split_box_class") {
        SetHeaderSplitBoxClass(strValue);
    }
    else if (strName == L"header_split_control_class") {
        SetHeaderSplitControlClass(strValue);
    }
    else if (strName == L"enable_header_drag_order") {
        SetEnableHeaderDragOrder(strValue == L"true");
    }
    else if (strName == L"check_box_class") {
        SetCheckBoxClass(strValue);
    }
    else if (strName == L"data_item_class") {
        SetDataItemClass(strValue);
    }
    else if (strName == L"data_item_label_class") {
        SetDataItemLabelClass(strValue);
    }
    else if (strName == L"row_grid_line_width") {
        SetRowGridLineWidth(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"column_grid_line_width") {
        SetColumnGridLineWidth(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"data_view_class") {
        SetDataViewClass(strValue);
    }
    else if (strName == L"header_height") {
        SetHeaderHeight(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"data_item_height") {
        SetDataItemHeight(_wtoi(strValue.c_str()), true);
    }
    else if (strName == L"show_header") {
        SetHeaderVisible(strValue == L"true");
    }
    else {
        __super::SetAttribute(strName, strValue);
    }
}

void ListCtrl::SetHeaderClass(const std::wstring& className)
{
    m_headerClass = className;
    if (m_pHeaderCtrl != nullptr) {
        m_pHeaderCtrl->SetClass(className);
    }
}

void ListCtrl::SetHeaderItemClass(const std::wstring& className)
{
    m_headerItemClass = className;
}

std::wstring ListCtrl::GetHeaderItemClass() const
{
    return m_headerItemClass.c_str();
}

void ListCtrl::SetHeaderSplitBoxClass(const std::wstring& className)
{
    m_headerSplitBoxClass = className;
}

std::wstring ListCtrl::GetHeaderSplitBoxClass() const
{
    return m_headerSplitBoxClass.c_str();
}

void ListCtrl::SetHeaderSplitControlClass(const std::wstring& className)
{
    m_headerSplitControlClass = className;
}

std::wstring ListCtrl::GetHeaderSplitControlClass() const
{
    return m_headerSplitControlClass.c_str();
}

void ListCtrl::SetCheckBoxClass(const std::wstring& className)
{
    m_checkBoxClass = className;
}

std::wstring ListCtrl::GetCheckBoxClass() const
{
    return m_checkBoxClass.c_str();
}

void ListCtrl::SetDataViewClass(const std::wstring& className)
{
    m_dataViewClass = className;
}

std::wstring ListCtrl::GetDataViewClass() const
{
    return m_dataViewClass.c_str();
}

void ListCtrl::SetDataItemClass(const std::wstring& className)
{
    m_dataItemClass = className;
}

std::wstring ListCtrl::GetDataItemClass() const
{
    return m_dataItemClass.c_str();
}

void ListCtrl::SetDataItemLabelClass(const std::wstring& className)
{
    m_dataItemLabelClass = className;
}

std::wstring ListCtrl::GetDataItemLabelClass() const
{
    return m_dataItemLabelClass.c_str();
}

void ListCtrl::SetRowGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nLineWidth);
    }
    if (nLineWidth < 0) {
        nLineWidth = 0;
    }
    m_nRowGridLineWidth = nLineWidth;
}

int32_t ListCtrl::GetRowGridLineWidth() const
{
    return m_nRowGridLineWidth;
}

void ListCtrl::SetColumnGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nLineWidth);
    }
    if (nLineWidth < 0) {
        nLineWidth = 0;
    }
    m_nColumnGridLineWidth = nLineWidth;
}

int32_t ListCtrl::GetColumnGridLineWidth() const
{
    return m_nColumnGridLineWidth;
}

void ListCtrl::DoInit()
{
    if (m_bInited) {
        return;
    }
    m_bInited = true;

    //初始化Header
    ASSERT(m_pHeaderCtrl == nullptr);
    if (m_pHeaderCtrl == nullptr) {
        m_pHeaderCtrl = new ListCtrlHeader;
    }
    m_pHeaderCtrl->SetListCtrl(this);

    if (!m_headerClass.empty()) {
        m_pHeaderCtrl->SetClass(m_headerClass.c_str());
    }
    m_pHeaderCtrl->SetFixedHeight(UiFixedInt(m_nHeaderHeight), true, false);
    if (!m_bShowHeaderCtrl) {
        SetHeaderVisible(false);
    }
    m_pDataProvider->SetListCtrl(this);

    //初始化Body
    ASSERT(m_pDataView == nullptr);
    m_pDataView = new ListCtrlDataView;
    m_pDataView->SetListCtrl(this);
    m_pDataView->SetDataProvider(m_pDataProvider);
    if (!m_dataViewClass.empty()) {
        m_pDataView->SetClass(m_dataViewClass.c_str());
    }
    AddItem(m_pDataView);

    // Header添加到数据视图中管理，作为第一个元素，在Layout的实现中控制显示属性
    m_pDataView->AddItem(m_pHeaderCtrl);

    //TEST
    const size_t columnCount = 10;
    const size_t rowCount = 200;
    //添加列
    for (size_t i = 0; i < columnCount; ++i) {
        ListCtrlColumn columnInfo;
        columnInfo.nColumnWidth = 200;
        //columnInfo.nTextFormat = TEXT_LEFT | TEXT_VCENTER;
        columnInfo.text = StringHelper::Printf(L"第 %d 列", i);
        m_pHeaderCtrl->InsertColumn(-1, columnInfo);
    }
    //填充数据
    SetDataItemCount(rowCount);
    for (size_t itemIndex = 0; itemIndex < rowCount; ++itemIndex) {
        for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
            SetDataItem(itemIndex, { columnIndex, StringHelper::Printf(L"第 %02d 行/第 %02d 列", itemIndex, columnIndex), });
        }
    }
    //排序，默认为升序
    SortDataItems(0, true);
    //TESTs
}

ListCtrlHeaderItem* ListCtrl::InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->InsertColumn(columnIndex, columnInfo);
    }
}

size_t ListCtrl::GetColumnCount() const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return 0;
    }
    else {
        return m_pHeaderCtrl->GetColumnCount();
    }
}

int32_t ListCtrl::GetColumnWidth(size_t columnIndex) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return 0;
    }
    else {
        return m_pHeaderCtrl->GetColumnWidth(columnIndex);
    }
}

ListCtrlHeaderItem* ListCtrl::GetColumn(size_t columnIndex) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->GetColumn(columnIndex);
    }
}

ListCtrlHeaderItem* ListCtrl::GetColumnById(size_t columnId) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->GetColumnById(columnId);
    }
}

size_t ListCtrl::GetColumnIndex(size_t columnId) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    else {
        return m_pHeaderCtrl->GetColumnIndex(columnId);
    }
}

size_t ListCtrl::GetColumnId(size_t columnIndex) const
{
    ListCtrlHeaderItem* pHeaderItem = GetColumn(columnIndex);
    if (pHeaderItem != nullptr) {
        return pHeaderItem->GetColomnId();
    }
    return Box::InvalidIndex;
}

bool ListCtrl::DeleteColumn(size_t columnIndex)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return false;
    }
    else {
        return m_pHeaderCtrl->DeleteColumn(columnIndex);
    }
}

ListCtrlHeader* ListCtrl::GetListCtrlHeader() const
{
    return m_pHeaderCtrl;
}

void ListCtrl::SetEnableHeaderDragOrder(bool bEnable)
{
    m_bEnableHeaderDragOrder = bEnable;
}

bool ListCtrl::IsEnableHeaderDragOrder() const
{
    return m_bEnableHeaderDragOrder;
}

void ListCtrl::SetHeaderVisible(bool bVisible)
{
    m_bShowHeaderCtrl = bVisible;
    if (m_pHeaderCtrl != nullptr) {
        m_pHeaderCtrl->SetVisible(bVisible);
    }
}

bool ListCtrl::IsHeaderVisible() const
{
    if (m_pHeaderCtrl != nullptr) {
        return m_pHeaderCtrl->IsVisible();
    }
    else if(!m_bInited){
        return m_bShowHeaderCtrl;
    }
    else {
        return false;
    }
}

void ListCtrl::SetHeaderHeight(int32_t nHeaderHeight, bool bNeedDpiScale)
{
    if (nHeaderHeight < 0) {
        nHeaderHeight = 0;
    }
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nHeaderHeight);
    }
    if (m_pHeaderCtrl != nullptr) {
        m_pHeaderCtrl->SetFixedHeight(UiFixedInt(nHeaderHeight), true, false);
    }
    m_nHeaderHeight = nHeaderHeight;
}

int32_t ListCtrl::GetHeaderHeight() const
{
    int32_t nHeaderHeight = 0;
    if (m_pHeaderCtrl != nullptr) {
        nHeaderHeight = m_pHeaderCtrl->GetFixedHeight().GetInt32();
    }
    else {
        nHeaderHeight = m_nHeaderHeight;
    }
    return nHeaderHeight;
}

void ListCtrl::SetDataItemHeight(int32_t nItemHeight, bool bNeedDpiScale)
{
    if (nItemHeight < 0) {
        nItemHeight = 0;
    }
    if (bNeedDpiScale) {
        GlobalManager::Instance().Dpi().ScaleInt(nItemHeight);
    }
    m_nItemHeight = nItemHeight;
}

int32_t ListCtrl::GetDataItemHeight() const
{
    return m_nItemHeight;
}

void ListCtrl::OnColumnWidthChanged(size_t nColumnId1, size_t nColumnId2)
{
    if ((m_pDataView == nullptr) || (m_pHeaderCtrl == nullptr)){
        return;
    }

    size_t nColumn1 = Box::InvalidIndex;
    size_t nColumn2 = Box::InvalidIndex;
    int32_t nColumnWidth1 = -1;
    int32_t nColumnWidth2 = -1;
    if (!m_pHeaderCtrl->GetColumnInfo(nColumnId1, nColumn1, nColumnWidth1)) {
        nColumnWidth1 = -1;
    }
    if (!m_pHeaderCtrl->GetColumnInfo(nColumnId2, nColumn2, nColumnWidth2)) {
        nColumnWidth2 = -1;
    }

    if ((nColumnWidth1 < 0) && (nColumnWidth2 < 0)) {
        return;
    }

    size_t itemCount = m_pDataView->GetItemCount();
    for (size_t index = 1; index < itemCount; ++index) {
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(m_pDataView->GetItemAt(index));
        if (pItem == nullptr) {
            continue;
        }
        size_t columnCount = pItem->GetItemCount();
        for (size_t nColumn = 0; nColumn < columnCount; ++nColumn) {
            if ((nColumn == nColumn1) && (nColumnWidth1 >= 0)) {
                LabelBox* pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(nColumn));
                if (pLabelBox != nullptr) {
                    pLabelBox->SetFixedWidth(UiFixedInt(nColumnWidth1), true, false);
                }
            }
            if ((nColumn == nColumn2) && (nColumnWidth2 >= 0)) {
                LabelBox* pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(nColumn));
                if (pLabelBox != nullptr) {
                    pLabelBox->SetFixedWidth(UiFixedInt(nColumnWidth2), true, false);
                }
            }
        }
    }
}

void ListCtrl::OnHeaderColumnAdded(size_t nColumnId)
{
    m_pDataProvider->AddColumn(nColumnId);
}

void ListCtrl::OnHeaderColumnRemoved(size_t nColumnId)
{
    m_pDataProvider->RemoveColumn(nColumnId);
}

void ListCtrl::OnColumnSorted(size_t nColumnId, bool bSortedUp)
{
    //对数据排序，然后刷新界面显示
    m_pDataProvider->SortDataItems(nColumnId, bSortedUp, nullptr, nullptr);

    ASSERT(m_pDataView != nullptr);
    if (m_pDataView != nullptr) {
        m_pDataView->Refresh();
    }
}

void ListCtrl::OnHeaderColumnOrderChanged()
{
    ASSERT(m_pDataView != nullptr);
    if (m_pDataView != nullptr) {
        m_pDataView->Refresh();
    }
}

void ListCtrl::OnHeaderColumnCheckStateChanged(size_t nColumnId, bool bChecked)
{
    m_bCanUpdateHeaderCheckStatus = false;
    m_pDataProvider->SetColumnCheck(nColumnId, bChecked);
    ASSERT(m_pDataView != nullptr);
    if (m_pDataView != nullptr) {
        m_pDataView->Refresh();
    }
    m_bCanUpdateHeaderCheckStatus = true;
}

void ListCtrl::UpdateControlCheckStatus(size_t nColumnId)
{
    if (!m_bCanUpdateHeaderCheckStatus) {
        //避免不必要的更新
        return;
    }
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return;
    }
    ASSERT(m_pDataView != nullptr);
    if (m_pDataView == nullptr) {
        return;
    }
    std::vector<size_t> columnIndexList;
    if (nColumnId == Box::InvalidIndex) {
        const size_t columnCount = GetColumnCount();
        for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())) {
                columnIndexList.push_back(columnIndex);
            }
        }
    }
    else {
        size_t columnIndex = GetColumnIndex(nColumnId);
        if (columnIndex != Box::InvalidIndex) {
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())){
                columnIndexList.push_back(columnIndex);
            }
        }
    }
    for (size_t columnIndex : columnIndexList) {        
        std::vector<bool> checkList;
        const size_t itemCount = m_pDataView->GetItemCount();
        for (size_t itemIndex = 1; itemIndex < itemCount; ++itemIndex) {
            ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(m_pDataView->GetItemAt(itemIndex));
            if ((pItem != nullptr) && pItem->IsVisible()) {
                const size_t columnCount = pItem->GetItemCount();
                LabelBox* pLabelBox = nullptr;
                if (columnIndex < columnCount) {
                    pLabelBox = dynamic_cast<LabelBox*>(pItem->GetItemAt(columnIndex));
                }
                CheckBox* pCheckBox = nullptr;
                if (pLabelBox != nullptr) {
                    pCheckBox = dynamic_cast<CheckBox*>(pLabelBox->GetItemAt(0));
                }
                if (pCheckBox != nullptr) {
                    checkList.push_back(pCheckBox->IsSelected());
                }
            }
        }

        if (!checkList.empty()) {
            bool bHasChecked = false;
            bool bHasUnChecked = false;
            for (bool bChecked : checkList) {
                if (bChecked) {
                    bHasChecked = true;
                }
                else {
                    bHasUnChecked = true;
                }
            }

            bool bSelected = bHasChecked;
            bool bPartSelect = bSelected && bHasUnChecked;
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())) {
                pHeaderItem->SetCheckBoxSelect(bSelected, bPartSelect);
            }
        }
    }
}

size_t ListCtrl::GetDataItemCount() const
{
    return m_pDataProvider->GetDataItemCount();
}

bool ListCtrl::SetDataItemCount(size_t itemCount)
{
    return m_pDataProvider->SetDataItemCount(itemCount);
}

size_t ListCtrl::AddDataItem(const ListCtrlDataItem& dataItem)
{
    return m_pDataProvider->AddDataItem(dataItem);
}

bool ListCtrl::InsertDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    return m_pDataProvider->InsertDataItem(itemIndex, dataItem);
}

bool ListCtrl::SetDataItem(size_t itemIndex, const ListCtrlDataItem& dataItem)
{
    return m_pDataProvider->SetDataItem(itemIndex, dataItem);
}

bool ListCtrl::DeleteDataItem(size_t itemIndex)
{
    return m_pDataProvider->DeleteDataItem(itemIndex);
}

bool ListCtrl::DeleteAllDataItems()
{
    return m_pDataProvider->DeleteAllDataItems();
}

bool ListCtrl::SetDataItemData(size_t itemIndex, size_t itemData)
{
    return m_pDataProvider->SetDataItemData(itemIndex, itemData);
}

size_t ListCtrl::GetDataItemData(size_t itemIndex) const
{
    return m_pDataProvider->GetDataItemData(itemIndex);
}

bool ListCtrl::SetDataItemText(size_t itemIndex, size_t columnIndex, const std::wstring& text)
{
    return m_pDataProvider->SetDataItemText(itemIndex, columnIndex, text);
}

std::wstring ListCtrl::GetDataItemText(size_t itemIndex, size_t columnIndex) const
{
    return m_pDataProvider->GetDataItemText(itemIndex, columnIndex);
}

bool ListCtrl::SetDataItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor)
{
    return m_pDataProvider->SetDataItemTextColor(itemIndex, columnIndex, textColor);
}

bool ListCtrl::GetDataItemTextColor(size_t itemIndex, size_t columnIndex, UiColor& textColor) const
{
    return m_pDataProvider->GetDataItemTextColor(itemIndex, columnIndex, textColor);
}

bool ListCtrl::SetDataItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor)
{
    return m_pDataProvider->SetDataItemBkColor(itemIndex, columnIndex, bkColor);
}

bool ListCtrl::GetDataItemBkColor(size_t itemIndex, size_t columnIndex, UiColor& bkColor) const
{
    return m_pDataProvider->GetDataItemBkColor(itemIndex, columnIndex, bkColor);
}

bool ListCtrl::SortDataItems(size_t columnIndex, bool bSortedUp, 
                             ListCtrlDataCompareFunc pfnCompareFunc,
                             void* pUserData)
{
    size_t nColumnId = GetColumnId(columnIndex);
    ASSERT(nColumnId != Box::InvalidIndex);
    if (nColumnId == Box::InvalidIndex) {
        return false;
    }
    return m_pDataProvider->SortDataItems(nColumnId, bSortedUp, pfnCompareFunc, pUserData);
}

void ListCtrl::SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData)
{
    m_pDataProvider->SetSortCompareFunction(pfnCompareFunc, pUserData);
}

}//namespace ui

