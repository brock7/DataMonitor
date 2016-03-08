// LeftView.cpp : CLeftView 类的实现
//

#include "stdafx.h"
#include "DataMonitor.h"

#include "DataMonitorDoc.h"
#include "LeftView.h"
#include ".\leftview.h"
#include "MainFrm.h"
#include ".\monitor.h"
#include "MemDC.h"
#include "FindPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CScrollView)

BEGIN_MESSAGE_MAP(CLeftView, CScrollView)
	ON_MESSAGE(WM_PAGEUPAGE, OnPageUpdate)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLeftView 构造/析构

CLeftView::CLeftView()
{
	const COLORREF FlashColor = RGB(255, 150, 150);
	const COLORREF HighlightColor = RGB(255, 0, 0);
	const COLORREF NormalColor  = RGB(255, 255, 255);
	const COLORREF SelColor  = RGB(0, 0, 255);

	_normalBursh.CreateSolidBrush(NormalColor);
	_flashBursh.CreateSolidBrush(FlashColor);
	_hightBursh.CreateSolidBrush(HighlightColor);
	_selPen.CreatePen(PS_SOLID, 3, SelColor);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));  
	lstrcpy(lf.lfFaceName, _T("宋体"));
	lf.lfHeight = 12;
	VERIFY(m_font.CreateFontIndirect(&lf)); 

	_sel = -1;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetTimer(1, 500, NULL);

	/*
	PageMgr::PageList& pageList = _pageMgr.GetPages();
	
	for (int i = 0; i < 231; i ++) {
		PageMgr::PageAttr pageAttr(i * PAGE_SIZE);
		pageList.push_back(pageAttr);
	}
	*/
}


// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDataMonitorDoc* CLeftView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataMonitorDoc)));
	return (CDataMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


void CLeftView::DrawPages(CDC* pDC)
{
	PageMgr::PageList& pageList = _pageMgr.GetPages();
	size_t n;

	ULONG line = BoxSpace;
	ULONG col = BoxSpace;
	ULONG now = GetTickCount();

	CPen* oldpen = NULL;

	size_t pageCount = pageList.size();

	pDC->SetBkMode(TRANSPARENT);

	CFont* oldfont = pDC->SelectObject(&m_font);

	int selBrush = PageMgr::PageAttr::Normal;
	CBrush* oldbrush = pDC->SelectObject(&_normalBursh);

	for (n = 0; n < pageCount; ) {

		for (ULONG i = 0; i < _lineBoxCount && n < pageCount; i ++) {

			PageMgr::PageAttr& pageAttr = pageList[n];

			int pageHit = pageAttr.TestPageHit(now);

			if (pageHit == PageMgr::PageAttr::Flash && selBrush != PageMgr::PageAttr::Flash)
				oldbrush = pDC->SelectObject(&_flashBursh);
			else if (pageHit == PageMgr::PageAttr::Highlight && selBrush != PageMgr::PageAttr::Highlight)
				oldbrush = pDC->SelectObject(&_hightBursh);
			else if (selBrush != PageMgr::PageAttr::Normal)
				oldbrush = pDC->SelectObject(&_normalBursh);

			selBrush = pageHit;

			if (n == _sel) {

				oldpen = pDC->SelectObject(&_selPen);
				pDC->Rectangle(col, line, col + BoxWidth, line + BoxHight);
				pDC->SelectObject(oldpen);

			} else {

				pDC->Rectangle(col, line, col + BoxWidth, line + BoxHight);
			}

			CString str;
			str.Format(_T("%c %05X"), pageAttr.enabled ? _T('E') : _T('D'), 
				pageAttr.pageBound >> PAGE_SHIFT);
			pDC->TextOut(col + 2, line + 2, str);

			col += BoxWidth + BoxSpace;

			n ++;
		}

		line += BoxHight + BoxSpace;
		col = BoxSpace;
	}

	if (oldbrush)
		pDC->SelectObject(oldbrush);

	pDC->SelectObject(oldfont);
}

// CLeftView 消息处理程序
void CLeftView::OnDraw(CDC* pDC)
{
	CMemDC dc(pDC);
	DrawPages(&dc);
}

LRESULT CLeftView::OnPageUpdate(WPARAM wParam, LPARAM lParam)
{
#pragma warning(push)
#pragma warning(disable: 4800)
    _pageMgr.HitPage((LPARAM )lParam, GetTickCount(), (bool )wParam);
#pragma warning(pop)

	CString str;
	str.Format(_T("%08X"), lParam);
	CMainFrame* mainFrame = (CMainFrame* )GetParentFrame();
	mainFrame->UpdatePageInfo(str);

	return 0;
}

void CLeftView::ResetScrollBar()
{
	CRect rc;
	GetClientRect(&rc);

	_lineBoxCount = rc.Width() / (BoxWidth + BoxSpace);
	if (_lineBoxCount == 0)
		_lineBoxCount = 1;

	SIZE size;
	size.cx = (BoxWidth + BoxSpace) * _lineBoxCount;
	size.cy = (BoxHight + BoxSpace) * ((_pageMgr.GetPageCount() + _lineBoxCount - 1) / _lineBoxCount);
	SetScrollSizes(MM_TEXT, size);
}

void CLeftView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	ResetScrollBar();
}

void CLeftView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Invalidate(FALSE);

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	CScrollView::OnLButtonDblClk(nFlags, point);
}

BOOL CLeftView::PreTranslateMessage(MSG* pMsg)
{
	return CScrollView::PreTranslateMessage(pMsg);
}

void CLeftView::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	ULONG num = GetPageFromPoint(point.x, point.y);
	if (num != -1) {

		PageMgr::PageList& pageList = _pageMgr.GetPages();

		CString str;
		str.Format("%#08x, %d", pageList[num].pageBound, pageList[num].hitTimes);
		CMainFrame* mainFrame = (CMainFrame* )GetParentFrame();
		mainFrame->UpdatePageInfo(str);
	}
	*/

	CScrollView::OnMouseMove(nFlags, point);
}

void CLeftView::OnLButtonDown(UINT nFlags, CPoint point)
{
	ULONG num = GetPageFromPoint(point.x, point.y);
	if (num != -1) {
		_sel = num;
		
		Monitor::instance().SelectPage(num);

		Invalidate(TRUE);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CLeftView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (_sel == -1)	
		return;

	if (nChar == VK_F2) {

		PageMgr::PageAttr* pageAttr = _pageMgr.GetPage(_sel);
		ASSERT(pageAttr);

		Monitor::instance().Enable(_sel, !pageAttr->enabled);
		Invalidate(FALSE);

	} else if (nChar == VK_DELETE) {
		Monitor::instance().Delete(_sel);
		_sel = -1;
		Invalidate(TRUE);
	} else if (nChar == 'G') {
		CFindPage dlg;
		if (dlg.DoModal() == IDOK) {
			ULONG pageNum = 0;
			ULONG_PTR pageBound;
			_stscanf((LPCTSTR )dlg.m_pageNum, _T("%x"), &pageBound);
			pageBound <<= PAGE_SHIFT;
			pageNum = _pageMgr.GetPageNum(pageBound);
			_sel = pageNum;
			POINT pt;
			pt.x = 0;
			pt.y = (BoxHight + BoxSpace) * ((pageNum + _lineBoxCount - 1) / _lineBoxCount - 1);
			ScrollToPosition(pt);
		}
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLeftView::OnTimer(UINT nIDEvent)
{
	Invalidate(FALSE);

	CScrollView::OnTimer(nIDEvent);
}
