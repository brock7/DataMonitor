#pragma once

#define _PURE						= 0
#define EVENT_UNHANDLED				{ return DbgNext; }

#define L_DEBUG						0
#define L_INFO						1
#define L_NOTICE					2
#define L_WARNING					3
#define L_ERROR						4

enum EventHResult {

	DbgNext, 
	DbgBreak, 
	DbgContinue = DBG_CONTINUE,  // 0x00010002L
	DbgExceptionNotHandled = DBG_EXCEPTION_NOT_HANDLED, // 0x80010001L
};

class DbgEvent {
public:

	virtual EventHResult OnException(DWORD tid, EXCEPTION_DEBUG_INFO& info) 
		EVENT_UNHANDLED;

	virtual EventHResult OnCreateThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info) 
		EVENT_UNHANDLED;

	virtual EventHResult OnCreateProcess(DWORD tid, CREATE_PROCESS_DEBUG_INFO& info) 
		EVENT_UNHANDLED;

	virtual EventHResult OnExitThread(DWORD tid, EXIT_THREAD_DEBUG_INFO& info) 
		EVENT_UNHANDLED;

	virtual EventHResult OnExitProcess(DWORD tid, EXIT_PROCESS_DEBUG_INFO& info) 
		EVENT_UNHANDLED;
	virtual EventHResult OnLoadDll(DWORD tid, LOAD_DLL_DEBUG_INFO& info) 
		EVENT_UNHANDLED;

	virtual EventHResult OnUnloadDll(DWORD tid, UNLOAD_DLL_DEBUG_INFO & info) 
		EVENT_UNHANDLED;

	virtual EventHResult OnDbgStr(DWORD tid, OUTPUT_DEBUG_STRING_INFO& info) 
		EVENT_UNHANDLED;
	
	virtual EventHResult OnRipEvent(DWORD tid, RIP_INFO& info) EVENT_UNHANDLED;

	virtual EventHResult OnBreakPointContiune(DWORD tid, struct BreakPoint* bp, 
		ULONG_PTR brkAddr) EVENT_UNHANDLED;
};

enum DbgState {
	StateNone, 
	StateRunning, 
	StateStopped, 
	StateBreakPoint, 
	StateExcption, 
};

class XTraceEng {
public:
	bool IsDebugging()
	{
		return GetState() > StateNone;
	}

	virtual bool AttachProcess(DWORD id) _PURE;
	virtual bool DetachProcess() _PURE;
	virtual bool RegisterEventHandle(DbgEvent& event) _PURE;
	virtual bool UnregisterEventHandle(DbgEvent& event) _PURE;
	virtual size_t WriteMemory(void* addr, const BYTE* buf, size_t len) _PURE;
	virtual size_t ReadMemory(void* addr, BYTE* buf, size_t len) _PURE;

	virtual HANDLE GetProcessHandle() _PURE;
	virtual DWORD GetProcessId() _PURE;
	
	virtual DbgState GetState() _PURE;
	virtual int Log(int level, LPCTSTR fmt, ...) _PURE;
};

