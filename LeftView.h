// LeftView.h : CLeftView ��Ľӿ�
//

#pragma once

#include "PageData.h"

#define	WM_PAGEUPAGE		(WM_USER + 101)

class CDataMonitorDoc;

class CLeftView : public CScrollView
{
protected: // �������л�����
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

	static const ULONG BoxWidth = 55;
	static const ULONG BoxHight = 20;
	static const ULONG BoxSpace = 5;

// ����
public:
	CDataMonitorDoc* GetDocument();

// ����
public:

	void QueuePageUpdate(ULONG_PTR pageBound, bool hitwrite)
	{
		PostMessage(WM_PAGEUPAGE, (WPARAM )hitwrite, (LPARAM ) pageBound);
	}

	void ResetScrollBar();

// ��д
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

	void DrawPages(CDC* pDC);
	virtual void OnDraw(CDC* pDC);

// ʵ��
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	ULONG_PTR GetPageFromPoint(int x, int y)
	{
		CPaintDC dc(this);
		CScrollView::OnPrepareDC(&dc);
		POINT pt;
		pt.x = x;
		pt.y = y;

		dc.DPtoLP(&pt);
		ULONG line = pt.y / (BoxHight + BoxSpace);
		ULONG col = pt.x / (BoxWidth + BoxSpace);

		ULONG num = line * _lineBoxCount + col;
		if (num >= _pageMgr.GetPageCount())
			return -1;

		return num;
	}

	PageMgr& GetPageMgr()
	{
		return _pageMgr;
	}

protected:
	PageMgr			_pageMgr;

	struct PageUpdateItem {
		ULONG_PTR		pageBound;
		ULONG			hit;
		bool			hitwrite;
	};

	ULONG			_lineBoxCount;
	ULONG			_sel;

	CBrush			_normalBursh;
	CBrush			_flashBursh;
	CBrush			_hightBursh;
	CPen			_selPen;
	CFont			m_font;

// ���ɵ���Ϣӳ�亯��
protected:

	afx_msg LRESULT OnPageUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
};

#ifndef _DEBUG  // LeftView.cpp �ĵ��԰汾
inline CDataMonitorDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

