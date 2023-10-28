#ifndef UI_CONTROL_LIST_CTRL_H_
#define UI_CONTROL_LIST_CTRL_H_

#pragma once

#include "duilib/Box/VirtualListBox.h"
#include "duilib/Control/Button.h"
#include "duilib/Control/CheckBox.h"
#include "duilib/Control/Split.h"
#include "duilib/Box/ListBoxItem.h"
#include "duilib/Image/Image.h"

namespace ui
{

/** 列表数据项的表头UI控件
*/
class ListCtrlHeader;

/** 列表数据项的表头列数据项UI控件（列）
*/
class ListCtrlHeaderItem;

/** 列表数据项UI控件（行）
*/
class ListCtrlItem;

/** 列表数据显示UI控件
*/
class ListCtrlDataView;

/** 列表数据管理容器
*/
class ListCtrlDataProvider;

/** 列的基本信息（用于添加列）
*/
struct ListCtrlColumn
{
    std::wstring text;              //表头的文本
    int32_t nColumnWidth = 100;     //列宽，如果bNeedDpiScale为true，则执行DPI自适应处理
    int32_t nColumnWidthMin = 0;    //列宽最小值，0表示用默认设置，如果bNeedDpiScale为true，则执行DPI自适应处理
    int32_t nColumnWidthMax = 0;    //列宽最大值，0表示用默认设置，如果bNeedDpiScale为true，则执行DPI自适应处理
    int32_t nTextFormat = -1;       //文本对齐方式等属性, 该属性仅应用于Header, 取值可参考：IRender.h中的DrawStringFormat，如果为-1，表示按默认配置的对齐方式
    bool bSortable = true;          //是否支持排序
    bool bResizeable = true;        //是否支持通过拖动调整列宽
    bool bShowCheckBox = true;      //是否显示CheckBox（支持在表头和数据列显示CheckBox）
    int32_t nCheckBoxWidth = 24;    //CheckBox控件所占的宽度，用于设置文本偏移量，避免显示重叠，仅当bShowCheckBox为true时有效, 如果bNeedDpiScale为true，则执行DPI自适应处理
    bool bNeedDpiScale = true;      //是否对数值做DPI自适应
};

/** 列表数据项的基本信息（用于添加数据）
*/
struct ListCtrlDataItem
{
    size_t nColumnIndex = 0;        //【必填】第几列，有效范围：[0, GetColumnCount())
    std::wstring text;              //文本内容
    int32_t nTextFormat = -1;       //文本对齐方式等属性, 该属性仅应用于Header, 取值可参考：IRender.h中的DrawStringFormat，如果为-1，表示按默认配置的对齐方式
    int32_t nImageIndex = -1;       //图标资源索引号，在图片列表里面的下标值，如果为-1表示不显示图标
    UiColor textColor;              //文本颜色
    UiColor bkColor;                //背景颜色
    bool bShowCheckBox = true;      //是否显示CheckBox
    int32_t nCheckBoxWidth = 24;    //CheckBox控件所占的宽度，仅当bShowCheckBox为true时有效, 如果bNeedDpiScale为true，则执行DPI自适应处理
    bool bNeedDpiScale = true;      //是否对数值做DPI自适应
};

/** 用于存储的数据结构
*/
struct ListCtrlData
{
    UiString text;                  //文本内容
    uint16_t nTextFormat = 0;       //文本对齐方式等属性, 该属性仅应用于Header, 取值可参考：IRender.h中的DrawStringFormat，如果为-1，表示按默认配置的对齐方式
    int32_t nImageIndex = -1;       //图标资源索引号，在图片列表里面的下标值，如果为-1表示不显示图标
    UiColor textColor;              //文本颜色
    UiColor bkColor;                //背景颜色
    bool bShowCheckBox = true;      //是否显示CheckBox
    uint8_t nCheckBoxWidth = 0;     //CheckBox控件所占的宽度，仅当bShowCheckBox为true时有效
    bool bSelected = false;         //是否处于选择状态（ListBoxItem按整行选中）
    bool bChecked = false;          //是否处于勾选状态（CheckBox勾选状态）
    size_t nItemData = 0;           //用户自定义数据

    //TODO: 待实现功能列表
    //bool bVisible;                  //是否可见
    //bool bAlwaysAtTop;              //置顶显示
    //int32_t nBkImageIndex = -1;     //背景图片资源索引号
    //Header是可以隐藏的，可以通过配置或者接口改变这个行为
    //Item的文本可以编辑
    //选择：单选，多选，整行选中，提供接口
    //CheckBox: 提供接口
    //滚动条：水平滚动条，垂直滚动条
    //事件响应：点击，右键等
    //设置行高（最好支持每行的行高不同，Header的行高单独设置）
    //多视图的支持：Report，Icon等，类似与Windows资源管理器
    //数据类型的支持：比如整型，日期型，下拉表，字符串类型等
    //关联图片列表，图片列表需要单独实现
};

/** 比较数据的附加信息
*/
struct ListCtrlCompareParam
{
    size_t nColumnIndex; //数据关联第几列，有效范围：[0, GetColumnCount())
    size_t nColumnId;    //数据关联列的ID
    void* pUserData;     //用户自定义数据，设置比较函数的时候一同传入
};

/** 存储数据的比较函数的原型, 实现升序的比较(a < b)
* @param [in] a 第一个比较数据
* @param [in] b 第二个比较数据
* @param [in] param 数据关联的参数
* @return 如果 (a < b)，返回true，否则返回false
*/
typedef std::function<bool(const ListCtrlData& a, const ListCtrlData& b, const ListCtrlCompareParam& param)> 
        ListCtrlDataCompareFunc;

/** ListCtrl控件
*/
class ListCtrl: public VBox
{
    friend class ListCtrlHeader;
    friend class ListCtrlDataProvider;
    friend class ListCtrlDataView;
public:
	ListCtrl();
	virtual ~ListCtrl();

	/** 获取控件类型
	*/
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

public:
    /** 在指定位置添加一列
    * @param [in] columnIndex 在第几列以后插入该列，如果是-1，表示在最后追加一列
    * @param [in] columnInfo 列的基本属性
    * @return 返回这一列的表头控件接口
    */
    ListCtrlHeaderItem* InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo);

    /** 获取列的个数
    */
    size_t GetColumnCount() const;

    /** 获取列宽度
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    int32_t GetColumnWidth(size_t columnIndex) const;

    /** 获取列表头的控件接口
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    ListCtrlHeaderItem* GetColumn(size_t columnIndex) const;

    /** 获取列表头的控件接口
    * @param [in] columnId 列的ID值，通过ListCtrlHeaderItem::GetColomnId()函数获取
    */
    ListCtrlHeaderItem* GetColumnById(size_t columnId) const;

    /** 获取列的索引序号
    * @param [in] columnId 列的ID值，通过ListCtrlHeaderItem::GetColomnId()函数获取
    * @return 列的序号：[0, GetColumnCount())，代表第几列
    */
    size_t GetColumnIndex(size_t columnId) const;

    /** 获取列的ID
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    size_t GetColumnId(size_t columnIndex) const;

    /** 删除一列
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    bool DeleteColumn(size_t columnIndex);

    /** 获取表头控件接口, 在控件初始化以后才有值
    */
    ListCtrlHeader* GetListCtrlHeader() const;

    /** 设置是否支持列表头拖动改变列的顺序
    */
    void SetEnableHeaderDragOrder(bool bEnable);

    /** 是否支持列表头拖动改变列的顺序
    */
    bool IsEnableHeaderDragOrder() const;

    /** 设置是否显示表头控件
    */
    void SetHeaderVisible(bool bVisible);

    /** 当前是否显示表头控件
    */
    bool IsHeaderVisible() const;

    /** 设置表头控件的高度
    * @param[in] bNeedDpiScale 如果为true表示需要对宽度进行DPI自适应
    */
    void SetHeaderHeight(int32_t nHeaderHeight, bool bNeedDpiScale);

    /** 获取表头控件的高度
    */
    int32_t GetHeaderHeight() const;

public:
    /** 设置数据项的高度
    * @param[in] bNeedDpiScale 如果为true表示需要对宽度进行DPI自适应
    */
    void SetDataItemHeight(int32_t nItemHeight, bool bNeedDpiScale);

    /** 获取数据项的高度
    */
    int32_t GetDataItemHeight() const;

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
    * @param [in] columnIndex 列的索引号，有效范围：[0, GetColumnCount())
    * @param [in] bSortedUp true表示升序，false表示降序
    * @param [in] pfnCompareFunc 自定义的比较函数，如果为nullptr, 则使用默认的比较函数
    * @param [in] pUserData 用户自定义数据，调用比较函数的时候，通过参数传回给比较函数
    */
    bool SortDataItems(size_t columnIndex, bool bSortedUp, 
                       ListCtrlDataCompareFunc pfnCompareFunc = nullptr,
                       void* pUserData = nullptr);

    /** 设置外部自定义的排序函数, 替换默认的排序函数
    * @param [in] pfnCompareFunc 数据比较函数
    * @param [in] pUserData 用户自定义数据，调用比较函数的时候，通过参数传回给比较函数
    */
    void SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData);

protected:
    /** 控件初始化
    */
    virtual void DoInit() override;

    /** 设置ListCtrlHeader的属性Class
    */
    void SetHeaderClass(const std::wstring& className);

    /** ListCtrlHeaderItem的属性Class
    */
    void SetHeaderItemClass(const std::wstring& className);
    std::wstring GetHeaderItemClass() const;

    /** ListCtrlHeader/SplitBox的属性Class
    */
    void SetHeaderSplitBoxClass(const std::wstring& className);
    std::wstring GetHeaderSplitBoxClass() const;

    /** ListCtrlHeader/SplitBox/Control的属性Class
    */
    void SetHeaderSplitControlClass(const std::wstring& className);
    std::wstring GetHeaderSplitControlClass() const;

    /** CheckBox的Class属性(应用于Header和ListCtrl数据)
    */
    void SetCheckBoxClass(const std::wstring& className);
    std::wstring GetCheckBoxClass() const;

    /** 数据视图中的ListBox的Class属性
    */
    void SetDataViewClass(const std::wstring& className);
    std::wstring GetDataViewClass() const;

    /** ListCtrlItem的Class属性
    */
    void SetDataItemClass(const std::wstring& className);
    std::wstring GetDataItemClass() const;

    /** ListCtrlItem/LabelBox的Class属性
    */
    void SetDataItemLabelClass(const std::wstring& className);
    std::wstring GetDataItemLabelClass() const;

    /** 横向网格线的宽度
    * @param [in] nLineWidth 网格线的宽度，如果为0表示不显示横向网格线
    * @param [in] bNeedDpiScale 如果为true表示需要对宽度进行DPI自适应
    */
    void SetRowGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale);
    int32_t GetRowGridLineWidth() const;

    /** 纵向网格线的宽度
    * @param [in] nLineWidth 网格线的宽度，如果为0表示不显示纵向网格线
    * @param [in] bNeedDpiScale 如果为true表示需要对宽度进行DPI自适应
    */
    void SetColumnGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale);
    int32_t GetColumnGridLineWidth() const;

protected:
    /** 增加一列
    * @param [in] nColumnId 列的ID
    */
    void OnHeaderColumnAdded(size_t nColumnId);

    /** 删除一列
    * @param [in] nColumnId 列的ID
    */
    void OnHeaderColumnRemoved(size_t nColumnId);

    /** 调整列的宽度（拖动列宽调整，每次调整两个列的宽度）
    * @param [in] nColumnId1 第一列的ID
    * @param [in] nColumnId2 第二列的ID
    */
    void OnColumnWidthChanged(size_t nColumnId1, size_t nColumnId2);

    /** 对列排序
    * @param [in] nColumnId 列的ID
    * @param [in] bSortedUp 如果为true表示升序，如果为false表示降序
    */
    void OnColumnSorted(size_t nColumnId, bool bSortedUp);

    /** 通过拖动列表头，调整了列的顺序
    */
    void OnHeaderColumnOrderChanged();

    /** 表头的CheckBox勾选操作
    * @param [in] nColumnId 列的ID
    * @param [in] bChecked true表示勾选（Checked状态），false表示取消勾选（UnChecked状态）
    */
    void OnHeaderColumnCheckStateChanged(size_t nColumnId, bool bChecked);

    /** 同步UI的Check状态
    */
    void UpdateControlCheckStatus(size_t nColumnId);

private:
	/** 初始化标志
	*/
	bool m_bInited;

	/** 表头控件
	*/
	ListCtrlHeader* m_pHeaderCtrl;

	/** 列表数据展示
	*/
    ListCtrlDataView* m_pDataView;

	/** 列表数据管理
	*/
    ListCtrlDataProvider* m_pDataProvider;

    /** ListCtrlHeader的属性Class
    */
    UiString m_headerClass;

    /** ListCtrlHeaderItem的属性Class
    */
    UiString m_headerItemClass;

    /** ListCtrlHeader/SplitBox的属性Class
    */
    UiString m_headerSplitBoxClass;

    /** ListCtrlHeader/SplitBox/Control的属性Class
    */
    UiString m_headerSplitControlClass;

    /** CheckBox的Class
    */
    UiString m_checkBoxClass;

    /** 是否支持拖动改变列的顺序
    */
    bool m_bEnableHeaderDragOrder;

    /** ListCtrlItem的Class属性
    */
    UiString m_dataItemClass;

    /** ListCtrlItem/LabelBox的Class属性
    */
    UiString m_dataItemLabelClass;

    /** 当前是否可以更新Header的Check状态
    */
    bool m_bCanUpdateHeaderCheckStatus;

    /** 横向网格线的宽度
    */
    int32_t m_nRowGridLineWidth;

    /** 纵向网格线的宽度
    */
    int32_t m_nColumnGridLineWidth;

    /** ListBox的Class属性
    */
    UiString m_dataViewClass;

    /** 表头的高度
    */
    int32_t m_nHeaderHeight;

    /** 是否显示表头控件
    */
    bool m_bShowHeaderCtrl;

    /** 数据项的高度
    */
    int32_t m_nItemHeight;
};

/** ListCtrl子项控件
*/
class ListCtrlItem : public ListBoxItemH
{
public:
    /** 获取控件类型
    */
    virtual std::wstring GetType() const override { return L"ListCtrlItem"; }

};

/** ListCtrl的表头控件的显示项
*/
class ListCtrlHeaderItem:
    public ButtonBox
{
public:
    ListCtrlHeaderItem();
    virtual ~ListCtrlHeaderItem();

    /** 获取控件类型
    */
    virtual std::wstring GetType() const override;

    /** 设置属性
    */
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

    /** 绘制文字
    */
    virtual void PaintText(IRender* pRender) override;

    /** 按钮点击事件
    */
    virtual void Activate() override;

public:
    /** 排序方式
    */
    enum class SortMode
    {
        kNone, //不支持排序
        kUp,   //升序
        kDown  //降序
    };

    /** 设置排序方式
    */
    void SetSortMode(SortMode sortMode);

    /** 获取排序方式
    */
    SortMode GetSortMode() const;

    /** 设置排序图标：降序
    */
    void SetSortedDownImage(const std::wstring& sImageString);

    /** 设置排序图标：升序
    */
    void SetSortedUpImage(const std::wstring& sImageString);

    /** 获取唯一ID
    */
    size_t GetColomnId() const;

    /** 设置关联的Split控件接口
    */
    void SetSplitBox(SplitBox* pSplitBox);

    /** 获取关联的Split控件接口
    */
    SplitBox* GetSplitBox() const;

    /** 设置是否允许调整列宽
    */
    void SetColumnResizeable(bool bResizeable);

    /** 获取是否允许调整列宽
    */
    bool IsColumnResizeable() const;

    /** 设置列宽
    * @param [in] nWidth 列宽值
    * @param [in] bNeedDpiScale 是否需要对列宽值进行DPI自适应
    */
    void SetColumnWidth(int32_t nWidth, bool bNeedDpiScale);

    /** 获取列宽值
    */
    int32_t GetColumnWidth() const;

    /** 设置文字与图标之间的间隔（像素）
    */
    void SetIconSpacing(int32_t nIconSpacing, bool bNeedDpiScale);

    /** 获取文字与图标之间的间隔（像素）
    */
    int32_t GetIconSpacing() const;

    /** 设置图标显示在文字上方，居中显示
    */
    void SetShowIconAtTop(bool bShowIconAtTop);

    /** 获取图标是否显示在文字上方
    */
    bool IsShowIconAtTop() const;

public:
    /** 设置是否显示CheckBox
    * @param [in] bVisible true表示显示，false表示隐藏
    */
    bool SetCheckBoxVisible(bool bVisible);

    /** 判断当前CheckBox是否处于显示状态
    @return 返回true表示CheckBox存在，并且可见； 如果不含CheckBox，返回false
    */
    bool IsCheckBoxVisible() const;

    /** 是否有CheckBox
    */
    bool HasCheckBox() const;

    /** 设置CheckBox的勾选状态
    * @param [in] bSelected true表示勾选，false表示不勾选
    * @param [in] 如果bSelected和bPartSelect同时为true，表示部分选择
    */
    bool SetCheckBoxSelect(bool bSelected, bool bPartSelect);

private:
    /** 同步列宽与UI控件宽度
    */
    void CheckColumnWidth();

protected:
    /** @name 拖动相关的成员函数
    * @{ */

    virtual bool ButtonDown(const EventArgs& msg) override;
    virtual bool ButtonUp(const EventArgs& msg) override;
    virtual bool MouseMove(const EventArgs& msg) override;
    virtual bool OnWindowKillFocus(const EventArgs& msg) override;//控件所属的窗口失去焦点

    /** 根据鼠标位置调整各个控件的位置(拖动操作的一部分)
    */
    void AdjustHeaderItemPos(const UiPoint& mousePt);

    /** 清除拖动状态
    */
    void ClearDragStatus();

    /** @} */

private:
    /** 排序图标：降序
    */
    Image* m_pSortedDownImage;

    /** 排序图标：升序
    */
    Image* m_pSortedUpImage;

    /** 关联的Split控件接口
    */
    SplitBox* m_pSplitBox;

    /** 排序方式
    */
    SortMode m_sortMode;

    /** 列宽
    */
    int32_t m_nColumnWidth;

    /** 是否允许改变列宽
    */
    bool m_bColumnResizeable;

    /** 文字与图标之间的间隔(图标显示在文字的右侧或者左侧时)
    */
    int32_t m_nIconSpacing;

    /** 图标显示在文字上方，居中显示
    */
    bool m_bShowIconAtTop;

private:
    /** @name 拖动相关的成员变量
    * @{ */

    /** 是否鼠标左键按下
    */
    bool m_bMouseDown;

    /** 是否处于拖拽操作中
    */
    bool m_bInDragging;

    /** 原来的透明度
    */
    uint8_t m_nOldAlpha;

    /** 鼠标按下时的鼠标位置
    */
    UiPoint m_ptMouseDown;

    /** 鼠标按下时的控件矩形区域
    */
    UiRect m_rcMouseDown;

    struct ItemStatus
    {
        Control* m_pItem = nullptr;
        UiRect m_rcPos;
        size_t m_index = Box::InvalidIndex;
    };

    /** 鼠标按下时，父容器中，每个控件的位置
    */
    std::vector<ItemStatus> m_rcItemList;

    /** @} */
};

/** ListCtrl的表头控件
*/
class ListCtrlHeader : public ListBoxItemH
{
    friend class ListCtrlHeaderItem;
public:
    ListCtrlHeader();

    /** 获取控件类型
    */
    virtual std::wstring GetType() const override;

public:
    /** 在指定位置添加一列
    *  表头控件的基本结构如下：
    * 
    *   <ListCtrlHeader>
    *       <ListCtrlHeaderItem> CheckBox[可选] </ListCtrlHeaderItem>
    *       <SplitBox> <Control/> </SplitBox>
    *       ..
    *       <ListCtrlHeaderItem> CheckBox[可选] </ListCtrlHeaderItem>
    *       <SplitBox> <Control/> </SplitBox>
    *   </ListCtrlHeader>
    * 
    * @param [in] columnIndex 在第几列以后插入该列，如果是-1，表示在最后追加一列
    * @param [in] columnInfo 列的基本属性
    * @return 返回这一列的表头控件接口
    */
    ListCtrlHeaderItem* InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo);

    /** 获取列的个数
    */
    size_t GetColumnCount() const;

    /** 获取列宽度
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    int32_t GetColumnWidth(size_t columnIndex) const;

    /** 获取列表头的控件接口
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    ListCtrlHeaderItem* GetColumn(size_t columnIndex) const;

    /** 获取列表头的控件接口
    * @param [in] columnId 列的ID值，通过ListCtrlHeaderItem::GetColomnId()函数获取
    */
    ListCtrlHeaderItem* GetColumnById(size_t columnId) const;

    /** 获取列宽度和列索引序号
    * @param [in] columnId 列的ID值，通过ListCtrlHeaderItem::GetColomnId()函数获取
    * @param [out] columnIndex 列的序号：[0, GetColumnCount())，代表第几列
    * @param [out] nColumnWidth 列的宽度值
    */
    bool GetColumnInfo(size_t columnId, size_t& columnIndex, int32_t& nColumnWidth) const;

    /** 获取列的索引序号
    * @param [in] columnId 列的ID值，通过ListCtrlHeaderItem::GetColomnId()函数获取
    * @return 列的序号：[0, GetColumnCount())，代表第几列
    */
    size_t GetColumnIndex(size_t columnId) const;

    /** 获取列的索引序号
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    * @return 列的ID，如果匹配不到，则返回Box::InvalidIndex
    */
    size_t GetColumnId(size_t columnIndex) const;

    /** 删除一列
    * @param [in] columnIndex 列索引序号：[0, GetColumnCount())
    */
    bool DeleteColumn(size_t columnIndex);

public:

    /** 设置关联的ListCtrl接口
    */
    void SetListCtrl(ListCtrl* pListCtrl);

protected:
    /** 是否支持列表头拖动改变列的顺序
    */
    bool IsEnableHeaderDragOrder() const;

    /** 拖动列表头改变列宽的事件响应函数
    * @param [in] pLeftHeaderItem 左侧的列表头控件接口
    * @param [in] pRightHeaderItem 右侧的列表头控件接口
    */
    void OnHeaderColumnResized(Control* pLeftHeaderItem, Control* pRightHeaderItem);

    /** 点击列表头触发排序的事件响应函数
    * @param [in] pHeaderItem 列表头控件接口
    */
    void OnHeaderColumnSorted(ListCtrlHeaderItem* pHeaderItem);

    /** 通过拖动列表头，调整了列的顺序
    */
    void OnHeaderColumnOrderChanged();

    /** CheckBox的勾选项操作
    * @param [in] pHeaderItem 列表头控件接口
    * @param [in] bChecked true表示勾选（Checked状态），false表示取消勾选（UnChecked状态）
    */
    void OnHeaderColumnCheckStateChanged(ListCtrlHeaderItem* pHeaderItem, bool bChecked);

private:
    /** 关联的ListCtrl接口
    */
    ListCtrl* m_pListCtrl;
};

}//namespace ui

#endif //UI_CONTROL_LIST_CTRL_H_
