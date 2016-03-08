// XTraceView.h : interface of the CXTraceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTRACEVIEW_H__B2C8AB95_DCE2_441B_B4EA_25535C851D44__INCLUDED_)
#define AFX_XTRACEVIEW_H__B2C8AB95_DCE2_441B_B4EA_25535C851D44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataMonitorDoc;

class CXTraceView : public CListView
{
protected: // create from serialization only
	CXTraceView();
	DECLARE_DYNCREATE(CXTraceView)

// Attributes
public:
	CDataMonitorDoc* GetDocument();

	bool		m_enableLog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTraceView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXTraceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void Output(LPCTSTR str);
protected:	
	void InitList();

	BOOL Save(FILE* fp);

// Generated message map functions
protected:
	//{{AFX_MSG(CXTraceView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSave();

	ULONG	m_LineNum;
	afx_msg void OnMonitorEnablelog();
	afx_msg void OnUpdateMonitorEnablelog(CCmdUI *pCmdUI);
	afx_msg void OnMonitorClearlog();
};

#ifndef _DEBUG  // debug version in XTraceView.cpp
inline CDataMonitorDoc* CXTraceView::GetDocument()
   { return (CDataMonitorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XTRACEVIEW_H__B2C8AB95_DCE2_441B_B4EA_25535C851D44__INCLUDED_)
