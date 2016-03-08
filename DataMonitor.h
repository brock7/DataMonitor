// DataMonitor.h : DataMonitor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号

#include "DbgEng.h"
#include "CmdDlg.h"

extern DbgEng* g_dbgEng;

// CDataMonitorApp:
// 有关此类的实现，请参阅 DataMonitor.cpp
//

class CDataMonitorApp : public CWinApp
{
public:
	CDataMonitorApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();

	void RefreshView();

	void FindPage(ULONG_PTR pageBound, BYTE pat[], size_t patlen);
	
	void FindBin(BYTE pat[], size_t patlen, bool freeze);

	void FindRange(ULONG_PTR addr, ULONG_PTR len, BYTE pat[], size_t patlen);
	void FindAllRange(BYTE pat[], size_t patlen, bool freeze, bool searchCode);

	DECLARE_MESSAGE_MAP()

protected:
	DbgEng		_dbgEng;
	bool		_monitorWriteOnly;
	ULONG		_foundCount;

	typedef std::set<ULONG_PTR> SearchResult;
	SearchResult	_searchResult1;
	SearchResult	_searchResult2;
	bool			_saveResult;
	bool			_skipStack;

	CCmdDlg			_cmdDlg;

public:
	afx_msg void OnMonitorAttach();
	afx_msg void OnMonitorDetach();
	afx_msg void OnMonitorEnableall();
	afx_msg void OnMonitorDisableall();
	afx_msg void OnMonitorSelectmemory();
	afx_msg void OnMonitorWriteonly();
	afx_msg void OnUpdateMonitorWriteonly(CCmdUI *pCmdUI);

	afx_msg void OnMonitorClearall();
	afx_msg void OnMonitorFind();
	afx_msg void OnMonitorAutoselectmemory();
	afx_msg void OnMonitorFindallmemory();
	afx_msg void OnMonitorComparesearchresult();
	afx_msg void OnMonitorFindcurrentpage();
	afx_msg void OnHitHitonce();
	afx_msg void OnHitReset();
	afx_msg void OnUpdateHitHitonce(CCmdUI *pCmdUI);
	afx_msg void OnMonitorEnabledebugstring();
	afx_msg void OnUpdateMonitorEnabledebugstring(CCmdUI *pCmdUI);
	afx_msg void OnFindFindinresult();
	afx_msg void OnMonitorInputmemoryrange();
	afx_msg void OnOptionEnablelogunhandledexecption();
	afx_msg void OnUpdateOptionEnablelogunhandledexecption(CCmdUI *pCmdUI);
	afx_msg void OnMonitorTracemalloc();
	afx_msg void OnViewCmddlg();
	afx_msg void OnUpdateViewCmddlg(CCmdUI *pCmdUI);
	afx_msg void OnMonitorInchitnumber();
};

extern CDataMonitorApp theApp;
