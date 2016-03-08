#include "StdAfx.h"
#include ".\monitor.h"
#include "DataMonitor.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "DataMonitorView.h"

Monitor::Monitor(void)
{
	_selected = 0;
	_hitonce = true;
	_enableDbgStr = true;
	_hitNum = 1;
}

Monitor::~Monitor(void)
{
}

bool Monitor::InitMonitor()
{
	if (!g_dbgEng->RegisterEventHandle(instance()))
		return false;

	Monitor& monitor = instance();

	CMainFrame* mainFrame = (CMainFrame* )AfxGetMainWnd();
	monitor._leftView = mainFrame->GetLeftPane();
	monitor._rightView = mainFrame->GetRightPane();

	monitor._pageMgr = &monitor._leftView->GetPageMgr();
	monitor._pageData = &monitor._rightView->GetPageData();

	return true;
}

//////////////////////////////////////////////////////////////////////////

// EventHResult Monitor::OnException(DWORD tid, EXCEPTION_DEBUG_INFO& info)
EventHResult Monitor::OnBreakPoint(DWORD tid, BreakPoint& bp, EXCEPTION_DEBUG_INFO& info)
{
	EXCEPTION_RECORD& excepRec = info.ExceptionRecord;

	bool wrtten;

	/* if (excepRec.ExceptionCode == STATUS_ACCESS_VIOLATION && g_dbgEng->IsBreakPoint(info)) */ {
		
		ULONG_PTR pageBound;
		ULONG_PTR addr;
		ULONG_PTR code_addr = (ULONG_PTR )excepRec.ExceptionAddress;
		if (excepRec.ExceptionInformation[1] != NULL) {
			
			addr = excepRec.ExceptionInformation[1];
			pageBound = ULONG_PTR(addr) & (MAXULONG_PTR << 	PAGE_SHIFT);

			if (excepRec.ExceptionInformation[0] != 0) { // 写数据， 需要更新

				if (pageBound == _selected) {
					_updateDataFlag = true;					
				}

				wrtten = true;
			} else
				wrtten = false;

		} else {
            addr = (ULONG_PTR )excepRec.ExceptionAddress;
			pageBound = ULONG_PTR(addr) & (MAXULONG_PTR << 	PAGE_SHIFT);
		}

		if (_hitonce) {

			if (_hits.find(addr) == _hits.end()) {
				_hits.insert(addr);
				g_dbgEng->Log(L_INFO, "[%d]hit: %p, addr: %p, prog: %p", Monitor::instance().GetHitNum(), 
					pageBound, addr, code_addr);
			}

		} else {

			g_dbgEng->Log(L_INFO, "[%d]hit: %p, addr: %p, prog: %p", Monitor::instance().GetHitNum(), 
				pageBound, addr, code_addr);
		}

		_leftView->QueuePageUpdate(pageBound, wrtten);

	} /* else {

		if (excepRec.ExceptionCode != STATUS_BREAKPOINT && excepRec.ExceptionCode != STATUS_SINGLE_STEP) {

			ExcepHandleMap::iterator it;
			it = _excepHandles.find(excepRec.ExceptionCode);
			if (it == _excepHandles.end()) {
				TCHAR buf[1024];
				wsprintf(buf, _T("Unknown exception: %#08x, addr:%p, continue?"), 
					excepRec.ExceptionCode, excepRec.ExceptionAddress);

				if (AfxMessageBox(buf, MB_YESNO) == IDYES) {
					
					_excepHandles[excepRec.ExceptionCode] = DbgContinue;
					return DbgContinue;
				}
				else 
					_excepHandles[excepRec.ExceptionCode] = DbgNext;
			}
		}

		// return DbgContinue;
	} */

	return DbgNext;
}

EventHResult Monitor::OnCreateThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info)
{	
	return DbgNext;
}

EventHResult Monitor::OnCreateProcess(DWORD tid, CREATE_PROCESS_DEBUG_INFO& info)
{
	CDataMonitorApp* app = (CDataMonitorApp* )AfxGetApp();
	app->m_pMainWnd->PostMessage(WM_COMMAND, MAKELONG(ID_MONITOR_SELECTMEMORY, 0));
	return DbgNext;
}

EventHResult Monitor::OnExitThread(DWORD tid, EXIT_THREAD_DEBUG_INFO& info)
{
	return DbgNext;
}

EventHResult Monitor::OnExitProcess(DWORD tid, EXIT_PROCESS_DEBUG_INFO& info)
{
	return DbgNext;
}

EventHResult Monitor::OnLoadDll(DWORD tid, LOAD_DLL_DEBUG_INFO& info)
{
	return DbgNext;
}

EventHResult Monitor::OnUnloadDll(DWORD tid, UNLOAD_DLL_DEBUG_INFO & info)
{
	return DbgNext;
}

EventHResult Monitor::OnDbgStr(DWORD tid, OUTPUT_DEBUG_STRING_INFO& info)
{
	if (_enableDbgStr)
		return DbgNext;
	else
		return DbgContinue;
}

EventHResult Monitor::OnRipEvent(DWORD tid, RIP_INFO& info)
{
	return DbgNext;
}

EventHResult Monitor::OnBreakPointContiune(DWORD tid, BreakPoint* bp, ULONG_PTR brkAddr)
{

	/*
	ULONG_PTR pageBound;
	ULONG_PTR addr;
	bool updateDataFlag = false;

	bool written = bp->type == BreakPoint::Write;

	ULONG_PTR code_addr = (ULONG_PTR )info.ExceptionRecord.ExceptionAddress;
	addr = (ULONG_PTR )bp->addr;
	pageBound = ULONG_PTR(addr) & (MAXULONG_PTR << 	PAGE_SHIFT);

	if (written) {

		if (pageBound == _selected) {
			updateDataFlag = true;					
		}
	}

	if (_hitonce) {

		if (_hits.find(addr) == _hits.end()) {
			_hits.insert(addr);
			g_dbgEng->Log(L_INFO, "hit: %p, addr: %p, prog: %p", pageBound, 
				addr, code_addr);
		}

	} else {

		g_dbgEng->Log(L_INFO, "hit: %p, addr: %p, prog: %p", pageBound, 
			addr, code_addr);
	}
	*/

	if (_updateDataFlag) {

		static BYTE data[PAGE_SIZE];
        size_t len = g_dbgEng->ReadMemory((void *)_selected, data, sizeof(data));
		if (len != sizeof(data)) {

			g_dbgEng->Log(L_WARNING, __FUNCTION__ ": ReadMemory() failed. len = %d", len);
		}

		_rightView->QueueDataUpdate(data, GetTickCount());

		_updateDataFlag = false;
	}

	return DbgNext;
}

//////////////////////////////////////////////////////////////////////////

void Monitor::AddMemorBlock(ULONG_PTR addr, ULONG_PTR size, bool monitorWrite)
{
	ULONG_PTR firstPage = ULONG_PTR(addr) & (MAXULONG_PTR << PAGE_SHIFT);
	ULONG_PTR lastPage = (ULONG_PTR(addr) + size - 1) & (MAXULONG_PTR << PAGE_SHIFT);

	ULONG_PTR page = firstPage;
	BreakPoint::BreakPointType type;
	if (monitorWrite)
		type = BreakPoint::Write;
	else
		type = BreakPoint::Access;

	do {

		ULONG bp = g_dbgEng->SetBreakPoint((PVOID )page, type, PAGE_SIZE, false, false, false);
		_pageMgr->InsertPage(page, bp);
		page += PAGE_SIZE;		

	} while (page <= lastPage);
}

void Monitor::SortPages()
{
	_pageMgr->SortPages();
}

bool Monitor::SelectPage(ULONG num)
{
	PageMgr::PageList& pageList = _pageMgr->GetPages();
	_selected = pageList[num].pageBound;
	static BYTE data[PAGE_SIZE];
	size_t len = g_dbgEng->ReadMemory((void *)_selected, data, sizeof(data));
	if (len != sizeof(data)) {

		g_dbgEng->Log(L_WARNING, __FUNCTION__ ": ReadMemory() failed. len = %d", len);
	}

	_rightView->QueueDataUpdate(data, 0);

	return true;
}

bool Monitor::Enable(ULONG num, bool enabled)
{
	PageMgr::PageList& pageList = _pageMgr->GetPages();

	if (num >= pageList.size()) {
		assert(false);
		return false;
	}

	PageMgr::PageAttr& pageAttr = pageList[num];
	if (pageAttr.enabled == enabled)
		return true;

	pageAttr.enabled = enabled;

	if (enabled) {

		g_dbgEng->EnableBreakPoint(pageAttr.bpnum);

	} else {
		g_dbgEng->DisableBreakPoint(pageAttr.bpnum);

	}

	return true;
}

bool Monitor::EnableAll(bool enabled)
{
	PageMgr::PageList& pageList = _pageMgr->GetPages();
	size_t pageCount = pageList.size();

	g_dbgEng->FreezeThreads();

	for (size_t i = 0; i < pageCount; i ++) {

		PageMgr::PageAttr& pageAttr = pageList[i];
		if (pageAttr.enabled == enabled)
			continue;

		pageAttr.enabled = enabled;

		if (enabled) {

			g_dbgEng->EnableBreakPoint(pageAttr.bpnum, false);

		} else {
			g_dbgEng->DisableBreakPoint(pageAttr.bpnum, false);

		}
	}

	g_dbgEng->UnfreezeThreads();

	return true;	
}

void Monitor::Delete(ULONG num)
{
	PageMgr::PageList& pageList = _pageMgr->GetPages();
	if (num >= pageList.size()) {
		assert(false);
		return;
	}

	PageMgr::PageAttr& pageAttr = pageList[num];

	g_dbgEng->FreezeThreads();
	g_dbgEng->EnableBreakPoint(pageAttr.bpnum, false);
	_pageMgr->DeletePage(num);
	g_dbgEng->UnfreezeThreads();

	_selected = 0;
}

void Monitor::ClearAll()
{
	PageMgr::PageList& pageList = _pageMgr->GetPages();
	g_dbgEng->ClearBreakPoints();
	_pageMgr->Reset();
	_selected = 0;
	_updateDataFlag = false;
}

void Monitor::Reset()
{
	_pageMgr->Reset();
	_pageData->Reset();
}

class _IsTheStack {
public:
	_IsTheStack()
	{
		_result = false;
	}

	bool operator() (ThreadInfo& thread, void* param)
	{
		if (_addr >= (ULONG_PTR )thread.initTeb.StackLimit && 
			(_addr + _size) <= (ULONG_PTR )thread.initTeb.StackBase)
		{
			_result = true;
			return false;
		} else
			return true;
	}

	ULONG_PTR		_addr;
	ULONG_PTR		_size;
	bool			_result;
};

bool Monitor::IsThreadStack(ULONG_PTR addr, ULONG_PTR len)
{
	_IsTheStack isTheStack;
	isTheStack._addr = addr;
	isTheStack._size = len;

	g_dbgEng->ForeachThread(isTheStack, NULL);
	return isTheStack._result;
}

void Monitor::ResetHit()
{
	g_dbgEng->FreezeThreads();
	_hits.clear();
	g_dbgEng->UnfreezeThreads();
}
