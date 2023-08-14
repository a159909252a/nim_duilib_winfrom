#ifndef UI_CONTROL_RICHEDIT_HOST_H_
#define UI_CONTROL_RICHEDIT_HOST_H_

#pragma once

#include "duilib/Core/UiTypes.h"
//ʹ��Windows��ITextHostʵ��
#include <Richedit.h>
#include <TextServ.h>
#include <RichOle.h>

namespace ui
{

class RichEdit;
class UILIB_API RichEditHost : public ITextHost
{
public:
	RichEditHost();
	RichEditHost(const RichEditHost& r) = delete;
	RichEditHost& operator=(const RichEditHost& r) = delete;
	virtual ~RichEditHost();

public:

    BOOL Init(RichEdit* re, const CREATESTRUCT* pcs);
    ITextServices* GetTextServices(void) { return pserv; }
    void SetClientRect(UiRect* prc);
    UiRect* GetClientRect() { return &rcClient; }
    BOOL GetWordWrap(void) { return fWordWrap; }
    void SetWordWrap(BOOL bWordWrap);
    BOOL GetReadOnly();
    void SetReadOnly(BOOL fReadOnly);
    BOOL IsPassword();
    void SetPassword(BOOL bPassword);
    void SetFontId(const std::wstring& fontId);
    void SetColor(DWORD dwColor);
    SIZEL* GetExtent();
    void SetExtent(SIZEL* psizelExtent);
    void LimitText(LONG nChars);
    BOOL IsCaptured();

    BOOL GetAllowBeep();
    void SetAllowBeep(BOOL bAllowBeep);
    WORD GetDefaultAlign();
    void SetDefaultAlign(WORD wNewAlign);
    BOOL GetRichTextFlag();
    void SetRichTextFlag(BOOL fNew);
    LONG GetDefaultLeftIndent();
    void SetDefaultLeftIndent(LONG lNewIndent);
    BOOL SetSaveSelection(BOOL fSaveSelection);
    HRESULT OnTxInPlaceDeactivate();
    HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
    BOOL GetActiveState(void) { return fInplaceActive; }
    BOOL DoSetCursor(const UiRect* prc, const UiPoint* pt);
    void SetTransparent(BOOL fTransparent);
    void GetControlRect(UiRect* prc);
    LONG SetAccelPos(LONG laccelpos);
    WCHAR SetPasswordChar(WCHAR chPasswordChar);
    void SetDisabled(BOOL fOn);
    LONG SetSelBarWidth(LONG lSelBarWidth);
    BOOL GetTimerState();

    void SetCharFormat(const CHARFORMAT2W& c);
    void SetParaFormat(const PARAFORMAT2& p);

	ITextHost* GetTextHost();
    ITextServices* GetTextServices2();
    BOOL SetOleCallback(IRichEditOleCallback* pCallback);
    BOOL CanPaste(UINT nFormat = 0);
    void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0);
    UiRect MakeUiRect(const RECT& rc);

public:

    static HRESULT CreateHost(RichEdit* re, const CREATESTRUCT* pcs, RichEditHost** pptec);
    static HRESULT InitDefaultCharFormat(RichEdit* re, CHARFORMAT2W* pcf);
    static HRESULT InitDefaultParaFormat(RichEdit* re, PARAFORMAT2* ppf);
    static void GetLogFont(const std::wstring& fontId, LOGFONT& lf);
    // Convert Pixels on the X axis to Himetric
    static LONG DXtoHimetricX(LONG dx, LONG xPerInch);
    // Convert Pixels on the Y axis to Himetric
    static LONG DYtoHimetricY(LONG dy, LONG yPerInch);

    /** ��ȡĬ�ϵ�����ַ�����
    */
    static int32_t GetDefaultMaxText();

public:
    // -----------------------------
    //	IUnknown interface
    // -----------------------------
    virtual HRESULT _stdcall QueryInterface(REFIID riid, void** ppvObject) override;
    virtual ULONG _stdcall AddRef(void) override;
    virtual ULONG _stdcall Release(void) override;

    // -----------------------------
    //	ITextHost interface
    // -----------------------------
    virtual HDC TxGetDC() override;
    virtual INT TxReleaseDC(HDC hdc) override;
    virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow) override;
    virtual BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags) override;
    virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw) override;
    virtual BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw) override;
    virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode) override;
    virtual void TxViewChange(BOOL fUpdate) override;
    virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight) override;
    virtual BOOL TxShowCaret(BOOL fShow) override;
    virtual BOOL TxSetCaretPos(INT x, INT y) override;
    virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout) override;
    virtual void TxKillTimer(UINT idTimer) override;
    virtual void TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll) override;
    virtual void TxSetCapture(BOOL fCapture) override;
    virtual void TxSetFocus() override;
    virtual void TxSetCursor(HCURSOR hcur, BOOL fText) override;
    virtual BOOL TxScreenToClient(LPPOINT lppt) override;
    virtual BOOL TxClientToScreen(LPPOINT lppt) override;
    virtual HRESULT TxActivate(LONG* plOldState) override;
    virtual HRESULT TxDeactivate(LONG lNewState) override;
    virtual HRESULT TxGetClientRect(LPRECT prc) override;
    virtual HRESULT TxGetViewInset(LPRECT prc) override;
    virtual HRESULT TxGetCharFormat(const CHARFORMATW** ppCF) override;
    virtual HRESULT TxGetParaFormat(const PARAFORMAT** ppPF) override;
    virtual COLORREF TxGetSysColor(int nIndex) override;
    virtual HRESULT TxGetBackStyle(TXTBACKSTYLE* pstyle) override;
    virtual HRESULT TxGetMaxLength(DWORD* plength) override;
    virtual HRESULT TxGetScrollBars(DWORD* pdwScrollBar) override;
    virtual HRESULT TxGetPasswordChar(TCHAR* pch) override;
    virtual HRESULT TxGetAcceleratorPos(LONG* pcp) override;
    virtual HRESULT TxGetExtent(LPSIZEL lpExtent) override;
    virtual HRESULT OnTxCharFormatChange(const CHARFORMATW* pcf) override;
    virtual HRESULT OnTxParaFormatChange(const PARAFORMAT* ppf) override;
    virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits) override;
    virtual HRESULT TxNotify(DWORD iNotify, void* pv) override;
    virtual HIMC TxImmGetContext(void) override;
    virtual void TxImmReleaseContext(HIMC himc) override;
    virtual HRESULT TxGetSelectionBarWidth(LONG* lSelBarWidth) override;

private:
    RichEdit* m_re;
    ULONG cRefs;					// Reference Count
    ITextServices* pserv;		    // pointer to Text Services object

    DWORD dwStyle;				// style bits

    unsigned	fEnableAutoWordSel : 1;	// enable Word style auto word selection?
    unsigned	fWordWrap : 1;	// Whether control should word wrap
    unsigned	fAllowBeep : 1;	// Whether beep is allowed
    unsigned	fRich : 1;	// Whether control is rich text
    unsigned	fSaveSelection : 1;	// Whether to save the selection when inactive
    unsigned	fInplaceActive : 1; // Whether control is inplace active
    unsigned	fTransparent : 1; // Whether control is transparent
    unsigned	fTimer : 1;	// A timer is set
    unsigned    fCaptured : 1;

    LONG		lSelBarWidth;			// Width of the selection bar
    LONG  		cchTextMost;			// maximum text size
    DWORD		dwEventMask;			// HandleMessage mask to pass on to parent window
    LONG		icf;
    LONG		ipf;
    UiRect		rcClient;				// Client Rect for this control
    SIZEL		sizelExtent;			// Extent array
    CHARFORMAT2W cf;					// Default character format
    PARAFORMAT2	pf;					    // Default paragraph format
    LONG		laccelpos;				// Accelerator position
    WCHAR		chPasswordChar;		    // Password character
};


}//name space ui

#endif // UI_CONTROL_RICHEDIT_HOST_H_