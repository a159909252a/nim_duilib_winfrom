#ifndef UI_CONTROL_RICHTEXT_H_
#define UI_CONTROL_RICHTEXT_H_

#pragma once

#include "duilib/Core/Control.h"

namespace ui 
{
class RichTextSlice
{
public:
    RichTextSlice();
    
    /** �ڵ�����
    */
    std::wstring m_nodeName;

    /** ��������
    */
    std::wstring m_text;

    /** �����ӵ�URL: "href"
    */
    std::wstring m_linkUrl;

    /** ������ɫ: "color"
    */
    std::wstring m_textColor;

    /** ������ɫ: "color"
    */
    std::wstring m_bgColor;

    /** ������Ϣ
    */
    UiFont m_fontInfo;

    /** �����������
    */
    //UiRect m_textRect;

public:
    /** �ӽڵ�
    */
    std::vector<RichTextSlice> m_childs;
};

class UILIB_API RichText : public Control
{
public:
    RichText();
    RichText(const RichText& r) = delete;
    RichText& operator=(const RichText& r) = delete;
    virtual ~RichText();

    /// ��д���෽�����ṩ���Ի����ܣ���ο���������
    virtual std::wstring GetType() const override;
    virtual UiSize EstimateText(UiSize szAvailable, bool& bReEstimateSize) override;
    virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
    virtual void PaintText(IRender* pRender) override;

public:
    /** ��ȡ�����ڱ߾�
     */
    const UiRect& GetTextPadding() const;

    /** ���������ڱ߾���Ϣ, �����ڲ������DPI����Ӧ����
     * @param [in] padding ���ε��ĸ���ֵ�ֱ������Ӧ���ĸ��ڱ߾�ֵ
     */
    void SetTextPadding(const UiRect& padding);

    /** ���ø�ʽ���ı�
    * @param [in] richText ���и�ʽ���ı�����
    */
    bool SetRichText(const std::wstring& richText);

    /** ׷��һ���ı�Ƭ��
    */
    void AppendTextSlice(const RichTextSlice&& textSlice);

    /** ׷��һ���ı�Ƭ��
    */
    void AppendTextSlice(const RichTextSlice& textSlice);

public:
    /** �������ʽ���ı�
    */
    std::wstring ToString() const;

private:
    /** �������ʽ���ı�
    */
    std::wstring ToString(const RichTextSlice& textSlice, const std::wstring& indent) const;

private:
    /** �ı����Ƶ��ڱ߾�(�ֱ��Ӧ�ĸ��ߵ��ڱ߾��С)
    */
    UiRect m_rcTextPadding;

    /** ���Ƶ��ı�����
    */
    std::vector<RichTextSlice> m_textSlice;
};

} // namespace ui

#endif // UI_CONTROL_RICHTEXT_H_