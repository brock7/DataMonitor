// DataMonitorView.cpp : CDataMonitorView 类的实现
//

#include "stdafx.h"
#include "DataMonitor.h"

#include "DataMonitorDoc.h"
#include "DataMonitorView.h"
#include ".\datamonitorview.h"
#include "MainFrm.h"
#include "Monitor.h"
#include "MemDC.h"
#include "BinModifyDlg.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataMonitorView

IMPLEMENT_DYNCREATE(CDataMonitorView, CScrollView)

BEGIN_MESSAGE_MAP(CDataMonitorView, CScrollView)
	ON_MESSAGE(WM_DATAUPDATE, OnDataUpdate)
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_MONITOR_SHOWCHARACTER, OnMonitorShowcharacter)
	ON_UPDATE_COMMAND_UI(ID_MONITOR_SHOWCHARACTER, OnUpdateMonitorShowcharacter)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CDataMonitorView 构造/析构

CDataMonitorView::CDataMonitorView()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));  
	lstrcpy(lf.lfFaceName, _T("宋体"));
	lf.lfHeight = 12;
	VERIFY(m_font.CreateFontIndirect(&lf)); 
	m_showChar = false;
	_selChar = -1;
}

CDataMonitorView::~CDataMonitorView()
{
}

BOOL CDataMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CDataMonitorView::ResetScrollBar()
{
	SIZE size;
	size.cx = OffsetWidth + Column * ColWidth;
	if (m_showChar) {
		size.cx += ChaSpace + Column * ChaWidth;
	}

	size.cy = RowHight * PAGE_SIZE / Column;
	SetScrollSizes(MM_TEXT, size);
}

void CDataMonitorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	ResetScrollBar();
	SetTimer(1, 500, NULL);

	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	// 从而可以用项填充 ListView。
}


// CDataMonitorView 诊断

#ifdef _DEBUG
void CDataMonitorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDataMonitorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDataMonitorDoc* CDataMonitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataMonitorDoc)));
	return (CDataMonitorDoc*)m_pDocument;
}
#endif //_DEBUG

void CDataMonitorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{	
	CScrollView::OnPrepareDC(pDC, pInfo);
}

inline void CDataMonitorView::DrawData(CDC* pDC)
{
	CString str;

	DWORD now = GetTickCount();
	BYTE data;

	const COLORREF FlashColor = RGB(255, 0, 0);
	const COLORREF NormalColor  = RGB(0, 0, 0);

	CFont* oldfont = pDC->SelectObject(&m_font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(127, 127, 127));

	pDC->SetTextColor(NormalColor);
	bool selFlash = false;

	for (ULONG i = 0; i < PAGE_SIZE; i ++) {

		ULONG line = i / Column;
		ULONG col = i % Column;

		if (col == 0) {

			str.Format("%03X", i);
			pDC->SetTextColor(NormalColor);
			selFlash = false;

			pDC->TextOut(3, line * RowHight, str);
		}

		bool flash = _pageData.GetByte(i, now, data);

		if (flash && !selFlash)
			pDC->SetTextColor(FlashColor);
		else if (!flash && selFlash)
			pDC->SetTextColor(NormalColor);

		selFlash = flash;

		if (i == _selChar)
			pDC->SetBkMode(OPAQUE);

		str.Format(_T("%02X"), data);
		ULONG linePos = line * RowHight;
		pDC->TextOut(OffsetWidth + col * ColWidth, linePos, str);
		if (i == _selChar)
			pDC->SetBkMode(TRANSPARENT);

		if (m_showChar) {
			str.Format(_T("%c"), data);
			pDC->TextOut(StringLeft + OffsetWidth + col * ChaWidth, linePos, str);
		}
	}

	pDC->SelectObject(oldfont);
}

void CDataMonitorView::OnDraw(CDC* pDC)
{
	ULONG_PTR sel = Monitor::instance().GetSelected();
	if (sel == 0)
		return;

	CMemDC dc(pDC);
	DrawData(&dc);
}

LRESULT CDataMonitorView::OnDataUpdate(WPARAM wParam, LPARAM lParam)
{
	_pageData.UpdateBytes(0, (PBYTE)lParam, PAGE_SIZE, wParam);
	Invalidate(FALSE);
	return 0;
}

void CDataMonitorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Invalidate(FALSE);

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDataMonitorView::OnMouseMove(UINT nFlags, CPoint point)
{
	ULONG offset = GetOffsetFromPoint(point.x, point.y);
	if (offset != -1) {

		BYTE data[10] = {0};
		_pageData.GetData(offset, data, sizeof(data));
        
		CString str;
		str.Format("%03X %04X(%hd) %08X(%d) %c%c%c%c%c%c%c%c%c%c", offset, *(WORD* )data, 
			*(WORD* )data, *(DWORD* )data, *(DWORD* )data, data[0], data[1], data[2], data[3], 
			data[4], data[5], data[6], data[7], data[8], data[9]);

		CMainFrame* mainFrame = (CMainFrame* )GetParentFrame();
		mainFrame->UpdateDataInfo(str);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CDataMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}

void CDataMonitorView::OnTimer(UINT nIDEvent)
{
	Invalidate(FALSE);
	CScrollView::OnTimer(nIDEvent);
}

void CDataMonitorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_F5) {
		static BYTE data[PAGE_SIZE];
		
		ULONG_PTR sel = Monitor::instance().GetSelected();
		if (sel == NULL)
			return;

		g_dbgEng->FreezeThreads();
		size_t len = g_dbgEng->ReadMemory((void *)sel, data, sizeof(data));
		g_dbgEng->UnfreezeThreads();
		_pageData.UpdateBytes(0, data, sizeof(data), GetTickCount());
		Invalidate(FALSE);
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDataMonitorView::OnMonitorShowcharacter()
{
	m_showChar = !m_showChar;
	ResetScrollBar();
}

void CDataMonitorView::OnUpdateMonitorShowcharacter(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_showChar);
}

bool CDataMonitorView::ModifyMem(ULONG offset, BYTE data[], size_t len)
{
	ULONG_PTR sel = Monitor::instance().GetSelected();
	if (sel == NULL)
		return false;

	if (offset + len > PAGE_SIZE)
		return false;

	return g_dbgEng->WriteMemory(PVOID(sel + offset), data, len) == len;
}

void CDataMonitorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ULONG offset = GetOffsetFromPoint(point.x, point.y);
	if (offset != -1) {

		CBinModifyDlg dlg;
		DWORD n;
		WCHAR* wstr;
		size_t len;

		bool result = false;
		if (dlg.DoModal() == IDOK) {

			USES_CONVERSION;

			switch(dlg.m_type) {
			case 0: // BYTE
			case 1: // WORD
			case 2:	// DWORD
				if (dlg.m_hex)
					_stscanf((LPCTSTR )dlg.m_strWhat, "%x", &n);
				else
					_stscanf((LPCTSTR )dlg.m_strWhat, "%d", &n);

				result = ModifyMem(offset, (BYTE* )&n, dlg.m_type == 0 ? 1 : dlg.m_type * 2);
				break;

			case 3: // ASCII
				result = ModifyMem(offset, (BYTE* )(LPCSTR )dlg.m_strWhat, dlg.m_strWhat.GetLength());
				break;

			case 4: // UNICODE
				wstr = T2W((LPCSTR )dlg.m_strWhat);
				len = wcslen(wstr) * 2;

				result = ModifyMem(offset, (BYTE* )wstr, len);
				break;
			}

			if (result)
				OnKeyDown(VK_F5, 0, 0);				
			else
				MessageBox(_T("modification failed."), NULL);
				
		}
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CDataMonitorView::OnLButtonDown(UINT nFlags, CPoint point)
{	
	_selChar = GetOffsetFromPoint(point.x, point.y);
	Invalidate(FALSE);

	CScrollView::OnLButtonDown(nFlags, point);
}
