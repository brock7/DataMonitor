// MainFrm.h : CMainFrame 类的接口
//


#pragma once

class CDataMonitorView;
class CLeftView;
class CXTraceView;

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;
public:

// 操作
public:

	void UpdatePageInfo(LPCTSTR str);
	void UpdateDataInfo(LPCTSTR str);

// 重写
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
	CDataMonitorView* GetRightPane();
	CLeftView* GetLeftPane();
	CXTraceView* GetLogPane();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


