// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

class CDataMonitorView;
class CLeftView;
class CXTraceView;

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;
public:

// ����
public:

	void UpdatePageInfo(LPCTSTR str);
	void UpdateDataInfo(LPCTSTR str);

// ��д
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
	CDataMonitorView* GetRightPane();
	CLeftView* GetLeftPane();
	CXTraceView* GetLogPane();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


