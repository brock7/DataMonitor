#pragma once

#include "DbgEng.h"

class CDataMonitorView;
class CLeftView;

class PageMgr;
class PageData;

class Monitor: public DbgEvent {
	Monitor(void);

public:

	~Monitor(void);

	static Monitor& instance()
	{
		static Monitor inst;
		return inst;
	}

	static bool InitMonitor();
	
	void AddMemorBlock(ULONG_PTR addr, ULONG_PTR size, bool monitorWrite);
	void SortPages();

	bool SelectPage(ULONG num);
	bool Enable(ULONG num, bool enabled);
	bool EnableAll(bool enabled);
	void Reset();

	ULONG_PTR GetSelected()
	{
		return _selected;
	}

	void ClearAll();
	void Delete(ULONG num);
	PageMgr& GetPageMgr()
	{
		return *_pageMgr;
	}

	ULONG_PTR GetCurrentPage()
	{
		return _selected;
	}

	static bool IsThreadStack(ULONG_PTR addr, ULONG_PTR len);

	void SetHitOnce(bool once)
	{
		_hitonce = once;
	}

	bool GetHitOnce()
	{
		return _hitonce;
	}

	void ResetHit();

	bool IsEnableDbgStr()
	{
		return _enableDbgStr;
	}

	bool EnableDbgStr(bool enable)
	{
        return _enableDbgStr = enable;
	}

	void IncNum() 
	{
		_hitNum ++;
	}

	int GetHitNum() const 
	{
		return _hitNum;
	}

protected:
	// EventHResult OnException(DWORD tid, EXCEPTION_DEBUG_INFO& info);
	EventHResult OnBreakPoint(DWORD tid, BreakPoint& bp, EXCEPTION_DEBUG_INFO& info);
	EventHResult OnCreateThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info);
	EventHResult OnCreateProcess(DWORD tid, CREATE_PROCESS_DEBUG_INFO& info);
	EventHResult OnExitThread(DWORD tid, EXIT_THREAD_DEBUG_INFO& info);
	EventHResult OnExitProcess(DWORD tid, EXIT_PROCESS_DEBUG_INFO& info);
	EventHResult OnLoadDll(DWORD tid, LOAD_DLL_DEBUG_INFO& info);
	EventHResult OnUnloadDll(DWORD tid, UNLOAD_DLL_DEBUG_INFO & info);
	EventHResult OnDbgStr(DWORD tid, OUTPUT_DEBUG_STRING_INFO& info);	
	EventHResult OnRipEvent(DWORD tid, RIP_INFO& info);
	EventHResult OnBreakPointContiune(DWORD tid, BreakPoint* bp, ULONG_PTR brkAddr);

protected:

	CDataMonitorView*	_rightView;
	CLeftView*			_leftView;
	PageMgr*			_pageMgr;
	PageData*			_pageData;
	ULONG_PTR			_selected;
	bool				_updateDataFlag;
	typedef std::map<ULONG, ULONG> ExcepHandleMap;
	ExcepHandleMap		_excepHandles;

	typedef std::set<ULONG_PTR> HitSet;
	HitSet				_hits;
	bool				_hitonce;

	bool				_enableDbgStr;

	int					_hitNum;
};
