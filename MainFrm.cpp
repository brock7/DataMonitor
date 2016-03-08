// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "DataMonitor.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "DataMonitorView.h"
#include "XTraceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_PAGEINFO, 
	ID_INDICATOR_DATAINFO, 
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	UpdatePageInfo(_T(""));
	UpdateDataInfo(_T(""));

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// �������������
	if (!m_wndSplitter2.CreateStatic(this, 2, 1))
		return FALSE;

	if (!m_wndSplitter.CreateStatic(&m_wndSplitter2, 1, 2, WS_CHILD | WS_VISIBLE, 
		m_wndSplitter2.IdFromRowCol(0, 0)))
	{
		return FALSE;
	}

	// m_wndSplitter.
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(200, 600), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CDataMonitorView), CSize(100, 600), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CXTraceView), CSize(10, 100), pContext);
	m_wndSplitter2.SetRowInfo(0, 350, 10);
	m_wndSplitter2.SetRowInfo(1, 100, 10);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

CDataMonitorView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CDataMonitorView* pView = DYNAMIC_DOWNCAST(CDataMonitorView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}

CXTraceView* CMainFrame::GetLogPane()
{
	CWnd* pWnd = m_wndSplitter2.GetPane(1, 0);
	CXTraceView* pView = DYNAMIC_DOWNCAST(CXTraceView, pWnd);
	return pView;
}

void CMainFrame::UpdatePageInfo(LPCTSTR str)
{
	m_wndStatusBar.SetPaneText(4, str);
}

void CMainFrame::UpdateDataInfo(LPCTSTR str)
{
	m_wndStatusBar.SetPaneText(5, str);
}
