#pragma once

#include "duilib/Utils/WinImplBase.h"
#include "duilib/Control/Button.h"
#include "duilib/Control/RichEdit.h"
#include "duilib/Control/Label.h"
#include <functional>

namespace nim_comp {

enum MsgBoxRet
{
    MB_YES,
    MB_NO
};

typedef std::function<void(MsgBoxRet)> MsgboxCallback;

void ShowMsgBox(HWND hwnd, MsgboxCallback cb,
    const DString &content = _T(""), bool content_is_id = true,
    const DString &title = _T("STRING_TIPS"), bool title_is_id = true,
    const DString &yes = _T("STRING_OK"), bool btn_yes_is_id = true,
    const DString &no = _T(""), bool btn_no_is_id = false);

class MsgBox : public ui::WindowImplBase
{
public:
    friend void ShowMsgBox(HWND hwnd, MsgboxCallback cb,
        const DString &content, bool content_is_id,
        const DString &title, bool title_is_id,
        const DString &yes, bool btn_yes_is_id,
        const DString &no, bool btn_no_is_id);
public:
    MsgBox();
    virtual ~MsgBox();

    virtual DString GetSkinFolder() override;
    virtual DString GetSkinFile() override;
    virtual DString GetZIPFileName() const;
    virtual void OnEsc(BOOL &bHandled);
    virtual void CloseWnd(UINT nRet = IDOK) override;

    virtual DString GetWindowClassName() const override;
    virtual DString GetWindowId() const /*override*/;
    virtual UINT GetClassStyle() const override;
    virtual void OnInitWindow() override;
private:
    bool OnClicked(const ui::EventArgs& msg);

    void SetTitle(const DString &str);
    void SetContent(const DString &str);
    void SetButton(const DString &yes, const DString &no);
    void Show(HWND hwnd, MsgboxCallback cb);

    void EndMsgBox(MsgBoxRet ret);
public:
    static const LPCTSTR kClassName;
private:
    ui::Label*        title_   = nullptr;
    ui::RichEdit*    content_ = nullptr;
    ui::Button*        btn_yes_ = nullptr;
    ui::Button*        btn_no_  = nullptr;

    MsgboxCallback     msgbox_callback_;
};

}
