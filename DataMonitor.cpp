// DataMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "MainFrm.h"

#include "DataMonitorDoc.h"
#include "LeftView.h"
#include "DataMonitorView.h"
#include "Monitor.h"
#include ".\datamonitor.h"
#include "ProcessDlg.h"
#include "ollyport.h"
#include <io.h>
#include "XTraceView.h"
#include "BinFindDlg.h"
#include <atlbase.h>
#include "MemRangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DbgEng* g_dbgEng = NULL;

// CDataMonitorApp

BEGIN_MESSAGE_MAP(CDataMonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_MONITOR_ATTACH, OnMonitorAttach)
	ON_COMMAND(ID_MONITOR_DETACH, OnMonitorDetach)
	ON_COMMAND(ID_MONITOR_ENABLEALL, OnMonitorEnableall)
	ON_COMMAND(ID_MONITOR_DISABLEALL, OnMonitorDisableall)
	ON_COMMAND(ID_MONITOR_SELECTMEMORY, OnMonitorSelectmemory)
	ON_COMMAND(ID_MONITOR_WRITEONLY, OnMonitorWriteonly)
	ON_UPDATE_COMMAND_UI(ID_MONITOR_WRITEONLY, OnUpdateMonitorWriteonly)
	ON_COMMAND(ID_MONITOR_CLEARALL, OnMonitorClearall)
	ON_COMMAND(ID_MONITOR_FIND, OnMonitorFind)
	ON_COMMAND(ID_MONITOR_AUTOSELECTMEMORY, OnMonitorAutoselectmemory)
	ON_COMMAND(ID_MONITOR_FINDALLMEMORY, OnMonitorFindallmemory)
	ON_COMMAND(ID_MONITOR_COMPARESEARCHRESULT, OnMonitorComparesearchresult)
	ON_COMMAND(ID_MONITOR_FINDCURRENTPAGE, OnMonitorFindcurrentpage)
	ON_COMMAND(ID_HIT_HITONCE, OnHitHitonce)
	ON_COMMAND(ID_HIT_RESET, OnHitReset)
	ON_UPDATE_COMMAND_UI(ID_HIT_HITONCE, OnUpdateHitHitonce)
	ON_COMMAND(ID_MONITOR_ENABLEDEBUGSTRING, OnMonitorEnabledebugstring)
	ON_UPDATE_COMMAND_UI(ID_MONITOR_ENABLEDEBUGSTRING, OnUpdateMonitorEnabledebugstring)
	ON_COMMAND(ID_FIND_FINDINRESULT, OnFindFindinresult)
	ON_COMMAND(ID_MONITOR_INPUTMEMORYRANGE, OnMonitorInputmemoryrange)
	ON_COMMAND(ID_OPTION_ENABLELOGUNHANDLEDEXECPTION, OnOptionEnablelogunhandledexecption)
	ON_UPDATE_COMMAND_UI(ID_OPTION_ENABLELOGUNHANDLEDEXECPTION, OnUpdateOptionEnablelogunhandledexecption)
	ON_COMMAND(ID_MONITOR_TRACEMALLOC, OnMonitorTracemalloc)
	ON_COMMAND(ID_VIEW_CMDDLG, OnViewCmddlg)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CMDDLG, OnUpdateViewCmddlg)
	ON_COMMAND(ID_MONITOR_INCHITNUMBER, &CDataMonitorApp::OnMonitorInchitnumber)
END_MESSAGE_MAP()


// CDataMonitorApp 构造

CDataMonitorApp::CDataMonitorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	_monitorWriteOnly = true;
	_saveResult = true;
}


inline void WINAPI XTraceLog2View(LPCTSTR str)
{
	static CMainFrame* mainFrm = (CMainFrame* )AfxGetMainWnd();
	if (mainFrm == NULL) {

		mainFrm = (CMainFrame* )AfxGetMainWnd();

		if (mainFrm == NULL)
			return;
	}

	static CXTraceView* view = (CXTraceView* )mainFrm->GetLogPane();

	if (view == NULL) {

		view = (CXTraceView* )mainFrm->GetLogPane();

		if (view == NULL)
			return;
	}

	view->Output(str);	
}

// 唯一的一个 CDataMonitorApp 对象

CDataMonitorApp theApp;

// CDataMonitorApp 初始化

BOOL CDataMonitorApp::InitInstance()
{
	g_dbgEng = &_dbgEng;
	

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// command dialog
	_cmdDlg.Create(IDD_CMD);
	// _cmdDlg.ShowWindow(SW_SHOW);

	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生

	g_dbgEng->m_logProc = XTraceLog2View;
	// g_dbgEng->m_logUnhandledExcept = false;

	XTraceLog2View("DataMonitor 1.0");

	if (_taccess(_T(".\\plugin"), 0) == 0) {

		OllyPort::instance().InitOllyPort();		
	}

	Monitor::InitMonitor();

	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CDataMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CDataMonitorApp 消息处理程序


void CDataMonitorApp::OnMonitorAttach()
{
	ProcDataProv procDataProv;
	CProcessDlg	dlg(&procDataProv);	
	dlg.m_title = _T("Process");

	if (dlg.DoModal() == IDOK) {
		_dbgEng.Attach(dlg.m_dwSel);
		OnMonitorSelectmemory();
	}	
}

void CDataMonitorApp::RefreshView()
{
	CMainFrame* frm = (CMainFrame* )AfxGetMainWnd();
	frm->GetLeftPane()->Invalidate();
	frm->GetRightPane()->Invalidate();
}

void CDataMonitorApp::OnMonitorDetach()
{
	_dbgEng.Stop();
	Monitor::instance().Reset();
	RefreshView();
}

void CDataMonitorApp::OnMonitorSelectmemory()
{
	if (_dbgEng.GetProcessHandle() == NULL)
		return;

	MemDataProv prov(_dbgEng.GetProcessHandle());
	CProcessDlg dlg(&prov);
	dlg.m_title = _T("Memory");
	dlg.m_multisel = true;
	while (dlg.DoModal() == IDOK) {

		MemDataProv::MemInfo& memInfo = prov.m_memInfoVec[dlg.m_dwSel - 1];

		Monitor::instance().AddMemorBlock((ULONG_PTR )memInfo.BaseAddress, 
			memInfo.RegionSize, _monitorWriteOnly);		
	}

	Monitor::instance().SortPages();

	CMainFrame* frm = (CMainFrame* )AfxGetMainWnd();
	frm->GetLeftPane()->ResetScrollBar();
}

void CDataMonitorApp::OnMonitorEnableall()
{
	Monitor::instance().EnableAll(true);
}

void CDataMonitorApp::OnMonitorDisableall()
{
	Monitor::instance().EnableAll(false);
}

void CDataMonitorApp::OnMonitorWriteonly()
{
	_monitorWriteOnly = !_monitorWriteOnly;
}

void CDataMonitorApp::OnUpdateMonitorWriteonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_monitorWriteOnly);
}

void CDataMonitorApp::OnMonitorClearall()
{
	Monitor::instance().ClearAll();
	RefreshView();
}

void CDataMonitorApp::OnMonitorFind()
{
	USES_CONVERSION;

	CBinFindDlg dlg;
	dlg.m_saveResult = _saveResult;
	if (dlg.DoModal() != IDOK)
		return;

	_saveResult = dlg.m_saveResult == TRUE;

	g_dbgEng->Log(L_INFO, "Begin search: %s", (LPCTSTR )dlg.m_strWhat);

	if (_saveResult) {
		_searchResult2 = _searchResult1;
		_searchResult1.clear();
	}

	DWORD n;
	WCHAR* wstr;
	size_t len;

	switch(dlg.m_type) {
	case 0: // BYTE
	case 1: // WORD
	case 2:	// DWORD
		if (dlg.m_hex)
			_stscanf((LPCTSTR )dlg.m_strWhat, "%x", &n);
		else
			_stscanf((LPCTSTR )dlg.m_strWhat, "%d", &n);

		FindBin((BYTE* )&n, dlg.m_type == 0 ? 1 : dlg.m_type * 2, 
			dlg.m_freezeThreads == TRUE);
		break;

	case 3: // ASCII
		FindBin((BYTE* )(LPCSTR )dlg.m_strWhat, dlg.m_strWhat.GetLength(), 
			dlg.m_freezeThreads == TRUE);
		break;

	case 4: // UNICODE		

		wstr = T2W((LPCSTR )dlg.m_strWhat);
		len = wcslen(wstr) * 2;

		FindBin((BYTE* )wstr, len, dlg.m_freezeThreads == TRUE);
		break;
	}

	g_dbgEng->Log(L_INFO, "End search: %d", _foundCount);
}

ULONG_PTR MatchBin(BYTE buf[], size_t buflen, ULONG_PTR start, BYTE pat[], size_t patlen)
{
	size_t hint = 0;
	for (size_t i = start; i < buflen; i ++) {
		if (buf[i] == pat[hint])
			hint ++;
		else
			hint = 0;

		if (hint == patlen)
			return (ULONG_PTR)(i - patlen + 1);
	}

	return -1;
}

void CDataMonitorApp::FindPage(ULONG_PTR pageBound, BYTE pat[], size_t patlen)
{
	static BYTE data[PAGE_SIZE];
	size_t len = g_dbgEng->ReadMemory((void *)pageBound, data, sizeof(data));
	if (len != sizeof(data)) {

		g_dbgEng->Log(L_WARNING, __FUNCTION__ ": ReadMemory() failed. len = %d", len);
	}

	ULONG_PTR pos = 0;

	while ((pos = MatchBin(data, sizeof(data), pos, pat, patlen)) != -1) {

		g_dbgEng->Log(L_INFO, "%d: %p Found", _foundCount, pageBound + pos);
		_foundCount ++;

		if (_saveResult)
			_searchResult1.insert(pageBound + pos);

		pos += patlen;
	}
}

void CDataMonitorApp::FindBin(BYTE pat[], size_t patlen, bool freeze)
{
	PageMgr& pageMgr = Monitor::instance().GetPageMgr();

	PageMgr::PageList& pageList = pageMgr.GetPages();
	size_t pageCount = pageList.size();

	_foundCount = 0;

	if (freeze)
		g_dbgEng->FreezeThreads();

	for (size_t i = 0; i < pageCount; i ++) {

		PageMgr::PageAttr& pageAttr = pageList[i];
		FindPage(pageAttr.pageBound, pat, patlen);
	}

	if (freeze)
		g_dbgEng->UnfreezeThreads();
}

void CDataMonitorApp::OnMonitorAutoselectmemory()
{
	HANDLE hProcess = g_dbgEng->GetProcessHandle();
	if (hProcess == NULL)
		return;

	ULONG_PTR base = 0;

	MEMORY_BASIC_INFORMATION memInfo;

	Monitor& monitor = Monitor::instance();

	while (base < 0x80000000) {

		SIZE_T r = VirtualQueryEx(hProcess, (PVOID )base, &memInfo, 
			sizeof(MEMORY_BASIC_INFORMATION));

		if (r != sizeof(MEMORY_BASIC_INFORMATION)) {
			break;
		}

		if (memInfo.State != MEM_COMMIT || memInfo.Protect != PAGE_READWRITE || 
			Monitor::IsThreadStack((ULONG_PTR )memInfo.BaseAddress, memInfo.RegionSize)) 
		{
			base += memInfo.RegionSize;
			continue;
		}

		monitor.AddMemorBlock((ULONG_PTR )memInfo.BaseAddress, 
			memInfo.RegionSize, _monitorWriteOnly);		

		base += memInfo.RegionSize;
	}

	monitor.SortPages();
	CMainFrame* frm = (CMainFrame* )AfxGetMainWnd();
	frm->GetLeftPane()->ResetScrollBar();
}

void CDataMonitorApp::FindRange(ULONG_PTR addr, ULONG_PTR len, BYTE pat[], size_t patlen)
{
	size_t pageCount = (len + (PAGE_SIZE - 1)) / PAGE_SIZE;

	ULONG_PTR pageBound = PAGE_BOUND(addr);

	for (size_t i = 0; i < pageCount; i ++) {

		FindPage(pageBound, pat, patlen);
		pageBound += PAGE_SIZE;
	}
}

void CDataMonitorApp::FindAllRange(BYTE pat[], size_t patlen, bool freeze, bool searchCode)
{
	ULONG_PTR base = 0;

	MEMORY_BASIC_INFORMATION memInfo;

	Monitor& monitor = Monitor::instance();

	HANDLE hProcess = g_dbgEng->GetProcessHandle();
	if (hProcess == NULL)
		return;

	_foundCount = 0;

	if (freeze)
		g_dbgEng->FreezeThreads();

	while (base < 0x80000000) {

		SIZE_T r = VirtualQueryEx(hProcess, (PVOID )base, &memInfo, 
			sizeof(MEMORY_BASIC_INFORMATION));

		if (r != sizeof(MEMORY_BASIC_INFORMATION)) {
			return;
		}

		/*
		if (memInfo.State != MEM_COMMIT || (memInfo.Protect & (PAGE_GUARD | PAGE_NOACCESS)) != 0) {
			base += memInfo.RegionSize;
			continue;
		}
		*/

		if (memInfo.State != MEM_COMMIT || 
			(_skipStack && Monitor::IsThreadStack((ULONG_PTR )memInfo.BaseAddress, memInfo.RegionSize))) 
		{
			base += memInfo.RegionSize;
			continue;
		}

		if (searchCode) {
			if (memInfo.Protect == PAGE_NOACCESS) {

				base += memInfo.RegionSize;
				continue;
			}

		} else {

			if (memInfo.Protect != PAGE_READWRITE ) {

				base += memInfo.RegionSize;
				continue;
			}
		}
		
		FindRange((ULONG_PTR )memInfo.BaseAddress, memInfo.RegionSize, pat, patlen);

		base += memInfo.RegionSize;
	}

	if (freeze)
		g_dbgEng->UnfreezeThreads();
}

void CDataMonitorApp::OnMonitorFindallmemory()
{
	HANDLE hProcess = g_dbgEng->GetProcessHandle();
	if (hProcess == NULL)
		return;

	USES_CONVERSION;

	CBinFindDlg dlg;
	dlg.m_saveResult = _saveResult;
	if (dlg.DoModal() != IDOK)
		return;

	_saveResult = dlg.m_saveResult == TRUE;

	g_dbgEng->Log(L_INFO, "Begin search: %s", (LPCTSTR )dlg.m_strWhat);

	DWORD n;
	WCHAR* wstr;
	size_t len;

	if (_saveResult) {
		_searchResult2 = _searchResult1;
		_searchResult1.clear();
	}

	_skipStack = dlg.m_skipStack == TRUE;

	switch(dlg.m_type) {
	case 0: // BYTE
	case 1: // WORD
	case 2:	// DWORD
		if (dlg.m_hex)
			_stscanf((LPCTSTR )dlg.m_strWhat, "%x", &n);
		else
			_stscanf((LPCTSTR )dlg.m_strWhat, "%d", &n);

		FindAllRange((BYTE* )&n, dlg.m_type == 0 ? 1 : dlg.m_type * 2, 
			dlg.m_freezeThreads == TRUE, dlg.m_code == TRUE);
		break;

	case 3: // ASCII
		FindAllRange((BYTE* )(LPCSTR )dlg.m_strWhat, dlg.m_strWhat.GetLength(), 
			dlg.m_freezeThreads == TRUE, dlg.m_code == TRUE);
		break;

	case 4: // UNICODE
		wstr = T2W((LPCSTR )dlg.m_strWhat);
		len = wcslen(wstr) * 2;

		FindAllRange((BYTE* )wstr, len, dlg.m_freezeThreads == TRUE, 
			dlg.m_code == TRUE);
		break;
	}

	g_dbgEng->Log(L_INFO, "End search: %d", _foundCount);
}

void CDataMonitorApp::OnMonitorComparesearchresult()
{
	g_dbgEng->Log(L_INFO, "Begin compare search result");

	BYTE data[10] = {0};

	SearchResult::iterator it;

	for (it = _searchResult1.begin(); it != _searchResult1.end(); it ++) {
		if (_searchResult2.find(*it) == _searchResult2.end()) {

			g_dbgEng->ReadMemory((PVOID )*it, data, sizeof(data));
			g_dbgEng->Log(L_INFO, "removed addr: %p, current: %04X(%hd) %08X(%d) %c%c%c%c%c%c%c%c%c%c", 
				*it, *(WORD* )data, *(WORD* )data, *(DWORD* )data, *(DWORD* )data, data[0], data[1], 
				data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
		}
	}

	for (it = _searchResult2.begin(); it != _searchResult2.end(); it ++) {
		if (_searchResult1.find(*it) == _searchResult1.end()) {

			g_dbgEng->ReadMemory((PVOID )*it, data, sizeof(data));
			g_dbgEng->Log(L_INFO, "removed addr: %p, current: %04X(%hd) %08X(%d) %c%c%c%c%c%c%c%c%c%c", 
				*it, *(WORD* )data, *(WORD* )data, *(DWORD* )data, *(DWORD* )data, data[0], data[1], 
				data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
		}
	}

	g_dbgEng->Log(L_INFO, "End compare search result");
}

void CDataMonitorApp::OnMonitorFindcurrentpage()
{
	HANDLE hProcess = g_dbgEng->GetProcessHandle();
	if (hProcess == NULL)
		return;

	ULONG_PTR curPage = Monitor::instance().GetCurrentPage();
	if (curPage == NULL)
		return;

	USES_CONVERSION;

	CBinFindDlg dlg;
	dlg.m_saveResult = _saveResult;
	if (dlg.DoModal() != IDOK)
		return;

	_foundCount = 0;
	_saveResult = dlg.m_saveResult == TRUE;
	
	g_dbgEng->Log(L_INFO, "Begin search: %s", (LPCTSTR )dlg.m_strWhat);

	DWORD n;
	WCHAR* wstr;
	size_t len;

	if (_saveResult) {
		_searchResult2 = _searchResult1;
		_searchResult1.clear();
	}

	if (dlg.m_freezeThreads)
		g_dbgEng->FreezeThreads();

	switch(dlg.m_type) {
	case 0: // BYTE
	case 1: // WORD
	case 2:	// DWORD
		if (dlg.m_hex)
			_stscanf((LPCTSTR )dlg.m_strWhat, "%x", &n);
		else
			_stscanf((LPCTSTR )dlg.m_strWhat, "%d", &n);

		FindPage(curPage, (BYTE* )&n, dlg.m_type == 0 ? 1 : dlg.m_type * 2);
		break;

	case 3: // ASCII
		FindPage(curPage, (BYTE* )(LPCSTR )dlg.m_strWhat, dlg.m_strWhat.GetLength());
		break;

	case 4: // UNICODE
		wstr = T2W((LPCSTR )dlg.m_strWhat);
		len = wcslen(wstr) * 2;

		FindPage(curPage, (BYTE* )wstr, len);
		break;
	}

	if (dlg.m_freezeThreads)
		g_dbgEng->UnfreezeThreads();

	g_dbgEng->Log(L_INFO, "End search: %d", _foundCount);
}

void CDataMonitorApp::OnFindFindinresult()
{

	HANDLE hProcess = g_dbgEng->GetProcessHandle();
	if (hProcess == NULL)
		return;

	USES_CONVERSION;

	CBinFindDlg dlg;
	dlg.m_saveResult = _saveResult;
	if (dlg.DoModal() != IDOK)
		return;

	PBYTE data;
	size_t len;
	DWORD n;
	WCHAR* wstr;

	_saveResult = dlg.m_saveResult == TRUE;
	SearchResult* searchResult;

	if (_saveResult) {
		_searchResult2 = _searchResult1;
		_searchResult1.clear();
		searchResult = &_searchResult2;
	} else 
		searchResult = &_searchResult1;

	switch(dlg.m_type) {
	case 0: // BYTE
	case 1: // WORD
	case 2:	// DWORD
		if (dlg.m_hex)
			_stscanf((LPCTSTR )dlg.m_strWhat, "%x", &n);
		else
			_stscanf((LPCTSTR )dlg.m_strWhat, "%d", &n);

		data = (PBYTE )&n;
		len = dlg.m_type * 2;

		break;

	case 3: // ASCII

		data = (PBYTE )(LPCSTR )dlg.m_strWhat;
		len = dlg.m_strWhat.GetLength();
		break;

	case 4: // UNICODE
		wstr = T2W((LPCSTR )dlg.m_strWhat);		
		len = wcslen(wstr) * 2;
		data = (PBYTE )wstr;
		break;
	}

	g_dbgEng->Log(L_INFO, "Begin search: %s", (LPCTSTR )dlg.m_strWhat);

	BYTE buf[PAGE_SIZE];

	if (dlg.m_freezeThreads)
		g_dbgEng->FreezeThreads();

	int foundCount = 0;

	SearchResult::iterator it;
	for (it = searchResult->begin(); it != searchResult->end(); it ++) {

		ULONG_PTR addr = *it;
		if (g_dbgEng->ReadCommand((PVOID )addr, buf, len) != len) {
			g_dbgEng->Log(L_INFO, __FUNCTION__ ": g_dbgEng->ReadCommand() failed");
			continue;
		}

		if (MatchBin(buf, len, 0, data, len) != -1) {

			if (_saveResult) {
				_searchResult1.insert(addr);
			}

			g_dbgEng->Log(L_INFO, "%d: %p Found", ++ foundCount, (PVOID )addr);
		}
	}

	if (dlg.m_freezeThreads)
		g_dbgEng->UnfreezeThreads();

	g_dbgEng->Log(L_INFO, "End search: %s", (LPCTSTR )dlg.m_strWhat);
}

void CDataMonitorApp::OnHitHitonce()
{
	Monitor::instance().SetHitOnce(!Monitor::instance().GetHitOnce());
}

void CDataMonitorApp::OnHitReset()
{
	Monitor::instance().Reset();
}

void CDataMonitorApp::OnUpdateHitHitonce(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Monitor::instance().GetHitOnce());
}

void CDataMonitorApp::OnMonitorEnabledebugstring()
{
	Monitor::instance().EnableDbgStr(!Monitor::instance().IsEnableDbgStr());
}

void CDataMonitorApp::OnUpdateMonitorEnabledebugstring(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Monitor::instance().IsEnableDbgStr());
}


void CDataMonitorApp::OnMonitorInputmemoryrange()
{
	CMemRangeDlg dlg;
	if (dlg.DoModal() == IDOK) {

		if (dlg.m_end < dlg.m_start)
			dlg.m_end = dlg.m_start;

		Monitor::instance().AddMemorBlock((ULONG_PTR )dlg.m_start, 
			dlg.m_end - dlg.m_start + 1, _monitorWriteOnly);
	}
}

void CDataMonitorApp::OnOptionEnablelogunhandledexecption()
{
	// g_dbgEng->m_logUnhandledExcept = !g_dbgEng->m_logUnhandledExcept;
}

void CDataMonitorApp::OnUpdateOptionEnablelogunhandledexecption(CCmdUI *pCmdUI)
{
	// pCmdUI->SetCheck(g_dbgEng->m_logUnhandledExcept);
}

CHAR dllPath[MAX_PATH];

DWORD WINAPI TraceMallocThread(PVOID)
{
	if (!DetourContinueProcessWithDll(g_dbgEng->GetProcessHandle(), dllPath)) {
		g_dbgEng->Log(L_ERROR, "DetourContinueProcessWithDll(%s) failed: %d\n",
			dllPath, GetLastError());

		return -1;
	}

	return 0;
}

void CDataMonitorApp::OnMonitorTracemalloc()
{
	DWORD tid;

	CFileDialog dlg(TRUE, 0, 0, OFN_OVERWRITEPROMPT, "*.dll|*.dll||");
	if (dlg.DoModal() != IDOK)
		return;

	lstrcpy(dllPath, (LPCTSTR )dlg.GetPathName());
	HANDLE hThread = ::CreateThread(NULL, 0, TraceMallocThread, NULL, 0, &tid);
	CloseHandle(hThread);
}

void CDataMonitorApp::OnViewCmddlg()
{
	_cmdDlg.ShowWindow(_cmdDlg.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}

void CDataMonitorApp::OnUpdateViewCmddlg(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_cmdDlg.IsWindowVisible());
}

void CDataMonitorApp::OnMonitorInchitnumber()
{
	Monitor::instance().IncNum();
}
