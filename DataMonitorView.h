// DataMonitorView.h : CDataMonitorView 类的接口
//

#pragma once

#include "PageData.h"

#define WM_DATAUPDATE		(WM_USER + 102)

class CDataMonitorView : public CScrollView
{
protected: // 仅从序列化创建
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

	static const ULONG Column = 32;
	static const ULONG RowHight = 16;
	static const ULONG ColWidth = 20;
	static const ULONG OffsetWidth = 50;
	static const ULONG ChaWidth = 15;
	static const ULONG ChaSpace = 20;
	static const ULONG StringLeft = ChaSpace + ColWidth * Column;

// 属性
public:
	CDataMonitorDoc* GetDocument() const;

	void DrawData(CDC* pDC);
	virtual void OnDraw(CDC* pDC);

// 操作
public:

	void QueueDataUpdate(BYTE data[PAGE_SIZE], ULONG hit)
	{
		PostMessage(WM_DATAUPDATE, (WPARAM )hit, (LPARAM )data);
	}

	PageData& GetPageData()
	{
		return _pageData;
	}

	void ResetScrollBar();

// 重写
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CDataMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	ULONG GetOffsetFromPoint(int x, int y)
	{
		CPaintDC dc(this);
		CScrollView::OnPrepareDC(&dc);
		POINT pt;
		pt.x = x;
		pt.y = y;

		dc.DPtoLP(&pt);

		ULONG line = pt.y / RowHight;
		ULONG col = (pt.x - OffsetWidth) / ColWidth;
		if (col >= Column)
			return -1;

		ULONG offset = line * Column + col;
		if (offset >= PAGE_SIZE)
			return -1;

		return offset;
	}

	bool ModifyMem(ULONG offset, BYTE data[], size_t len);

protected:

	PageData			_pageData;
	CFont				m_font;
	bool				m_showChar;

	struct DataUpdateItem {
		ULONG			offset;
		BYTE			data[PAGE_SIZE];
		ULONG			len;
		ULONG			hit;
	};

	ULONG				_selChar;

// 生成的消息映射函数
protected:
	afx_msg LRESULT OnDataUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMonitorShowcharacter();
	afx_msg void OnUpdateMonitorShowcharacter(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // DataMonitorView.cpp 的调试版本
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

