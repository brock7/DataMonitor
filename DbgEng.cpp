#include "stdafx.h"
#include "DbgEng.h"

extern "C" {
#define MAINPROG
#include "disasm/disasm.h"
}


DbgEng::DbgEng(void)
{
	m_logLevel = L_INFO;
	Reset();
}

DbgEng::~DbgEng(void)
{
}

//////////////////////////////////////////////////////////////////////////

typedef ULONG (NTAPI* TNtQueryInformationThread)(
	HANDLE ThreadHandle,
	ULONG ThreadInformationClass,
	PVOID ThreadInformation,
	ULONG ThreadInformationLength,
	PULONG ReturnLength OPTIONAL
	);

TNtQueryInformationThread NtQueryInformationThread = NULL;

//////////////////////////////////////////////////////////////////////////

bool DbgEng::Init()
{
	if (NtQueryInformationThread == NULL) {

		HMODULE hDll = LoadLibrary(_T("ntdll.dll"));
		NtQueryInformationThread = (TNtQueryInformationThread )GetProcAddress(
			hDll, "NtQueryInformationThread");

		if (NtQueryInformationThread == NULL)
			return false;
	}

	return true;
}

void DbgEng::Uninit()
{

}

bool DbgEng::Reset()
{
	m_state = StateNone;
	m_bpnum = 0;
	m_pid = 0;
	m_hProcess = NULL;
	m_mainTid = 0;
	m_hMainThread = NULL;
	m_brktid = 0;

	m_bpTid = 0;
	m_breakpoints.clear();
	m_pageAttrs.clear();
	m_threads.clear();
	m_dlls.clear();
	m_pendingActions.clear();

	return true;
}

bool DbgEng::Debug(LPCTSTR cmdline, bool suspend /* = false */)
{
	// FIXME
	return false;
}

struct DbgThreadParam {

	DbgEng*		eng;
	bool		attachResult;
};

bool DbgEng::_Attach(DWORD id, DbgThreadParam& param)
{
	if (::DebugActiveProcess(id)) {

		param.attachResult = true;

		m_state = StateRunning;
		return true;

	} else {

		param.attachResult = false;
		SetEvent(m_attachEvent);
		return false;
	}
}

bool DbgEng::Attach(DWORD id)
{
	if (m_pid != 0) {

		// already attached
		return false;
	}

	m_pid = id;

	m_attachEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DbgThreadParam param;
	param.eng = this;

	DWORD tid;
	m_hDbgThread = ::CreateThread(NULL, 0, DbgThreadProc, &param, 0, &tid);
	DWORD wr = WaitForSingleObject(m_attachEvent, INFINITE);
	CloseHandle(m_attachEvent);
	m_attachEvent = NULL;

	if (wr != WAIT_OBJECT_0) {		
		return false;
	}

	if (!param.attachResult) {
		
		CloseHandle(m_hDbgThread);
		m_hDbgThread = NULL;
		return false;
	}

	return true;
}

bool DbgEng::Detach()
{
	if (m_pid == 0)
		return false;

	DebugActiveProcessStop(m_pid);

	Reset();

	return true;
}

bool DbgEng::Stop()
{
	m_stoploop = true;
	return true;
}

bool DbgEng::Wake()
{
	// FIXME 
	return false;
}

DWORD WINAPI DbgEng::DbgThreadProc(void* p)
{
	DbgThreadParam* param = (DbgThreadParam* )p;
	DbgEng*	eng = param->eng;

	if (!eng->_Attach(eng->m_pid, *param)) {

		eng->Log(L_ERROR, _T("Attach process failed\n"));
		return -1;
	}

	// don't touch 'param'

	eng->DbgLoop();
	eng->Detach();

	return 0;
}

//////////////////////////////////////////////////////////////////////////

bool DbgEng::RegisterEventHandle(DbgEvent& event)
{
	m_events.push_back(&event);
	return true;
}

bool DbgEng::UnregisterEventHandle(DbgEvent& event)
{
	return false;
}

void DbgEng::ClearEventHandles()
{
	m_events.clear();
}

//////////////////////////////////////////////////////////////////////////

size_t DbgEng::WriteMemory(void* addr, const BYTE* buf, size_t len)
{
	size_t actlen;

	if (!WriteProcessMemory(m_hProcess, addr, buf, len, (SIZE_T* )&actlen)) {

		DWORD prot;

		if (!VirtualProtectEx(m_hProcess, addr, len, PAGE_READWRITE, &prot)) {

			Log(L_WARNING, "DbgEng::WriteMemory(Addr: %x, Size: %x) prot failed. err: %x\n", 
				addr, len, GetLastError());
			return 0;
		}

		if (!WriteProcessMemory(m_hProcess, addr, buf, len, (SIZE_T* )&actlen)) {

			if (GetLastError() != ERROR_PARTIAL_COPY) {

				Log(L_WARNING, "DbgEng::WriteMemory(Addr: %x, Size: %x) read failed. err: %x\n", 
					addr, len, GetLastError());
			}
		}

		if (!VirtualProtectEx(m_hProcess, addr, len, prot, &prot)) {

			Log(L_WARNING, "DbgEng::WriteMemory(Addr: %x, Size: %x) prot failed. err: %x\n", 
				addr, len, GetLastError());
		}
	}

	return actlen;
}

size_t DbgEng::ReadMemory(void* addr, BYTE* buf, size_t len)
{
	size_t actlen;

	if (!ReadProcessMemory(m_hProcess, addr, buf, len, (SIZE_T* )&actlen)) {

		DWORD prot;

		if (!VirtualProtectEx(m_hProcess, addr, len, PAGE_READWRITE, &prot)) {

			Log(L_WARNING, "DbgEng::ReadMemory(Addr: %x, Size: %x) prot failed. err: %x\n", 
				addr, len, GetLastError());
			return 0;
		}

		if (!ReadProcessMemory(m_hProcess, addr, buf, len, (SIZE_T* )&actlen)) {

			if (GetLastError() != ERROR_PARTIAL_COPY) {

				Log(L_WARNING, "DbgEng::ReadMemory(Addr: %x, Size: %x) read failed. err: %x\n", 
					addr, len, GetLastError());
			}
		}

		if (!VirtualProtectEx(m_hProcess, addr, len, prot, &prot)) {

			Log(L_WARNING, "DbgEng::ReadMemory(Addr: %x, Size: %x) prot failed. err: %x\n", 
				addr, len, GetLastError());
		}
	}

	return actlen;
}

//////////////////////////////////////////////////////////////////////////

void DbgEng::FreezeThreads(DWORD exclusive)
{
	ThreadMap::iterator it;
	for (it = m_threads.begin(); it != m_threads.end(); it ++) {
		if (it->first != exclusive)
			::SuspendThread(it->second.hThread);
	}
}

void DbgEng::UnfreezeThreads(DWORD exclusive)
{
	ThreadMap::iterator it;
	for (it = m_threads.begin(); it != m_threads.end(); it ++) {
		if (it->first != exclusive)
			::ResumeThread(it->second.hThread);
	}
}

PVOID DbgEng::GetThreadStartAddress(HANDLE hThread)
{
	const ULONG ThreadQuerySetWin32StartAddress = 9;
	ULONG_PTR addr, len = sizeof(addr);
	if (!NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &addr, len, &len))
		return (PVOID )addr;

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

int DbgEng::Log(int level, LPCTSTR fmt, ...)
{
	if (level < m_logLevel)
		return 0;

	static char* LevelName[] = {
		"[DEBG]", 
		"[INFO]", 
		"[NOTI]", 
		"[WARN]", 
		"[ERRO]",
	};

	TCHAR buf[2048];
	strcpy_s(buf, sizeof(buf), LevelName[level]);

	const int PrefixLen = 6;

	va_list vlist;
	va_start(vlist, fmt);
	int r =_vsntprintf(&buf[PrefixLen], sizeof(buf) - PrefixLen, fmt, vlist);

	m_logProc(buf);
	va_end(vlist);
	return r;
}

//////////////////////////////////////////////////////////////////////////

inline bool DbgEng::DisableBreakPoint(BreakPoint& bp)
{
	if (bp.enabled == false)
		return false;

	switch (bp.type) {

	case BreakPoint::Execute:
		return WriteMemory((PVOID )bp.addr, bp.backup, BP_INST_LEN) == BP_INST_LEN;
		break;

	case BreakPoint::Access:
	case BreakPoint::Write:
	case BreakPoint::Execute2:
		RestoreMemBp(bp.addr, bp.width);
		break;
	}

	bp.enabled = false;
	return true;
}

inline bool DbgEng::EnableBreakPoint(BreakPoint& bp)
{
	if (bp.enabled == true)
		return false;

	switch (bp.type) {

	case BreakPoint::Execute:
		return WriteSoftBpInst((PVOID )bp.addr);
		break;

	case BreakPoint::Access:

		SetNoAccessBp(bp.addr, bp.width);
		break;

	case BreakPoint::Write:

		SetReadOnlyBp(bp.addr, bp.width);
		break;

	case BreakPoint::Execute2:
		SetNoAccessBp(bp.addr, bp.width);
		break;
	}

	bp.enabled = true;
	return true;
}

inline bool DbgEng::SetReadOnlyBp(ULONG_PTR addr, DWORD width)
{
	ULONG_PTR beginPg = PAGE_FRAME(addr);
	ULONG_PTR endPg = PAGE_FRAME(addr + width - 1);

	PageAttrMap::iterator it;

	for (ULONG_PTR i = beginPg; i <= endPg; i ++) {
		it = m_pageAttrs.find(i);

		PageAttr* pageAttr;
		if (it == m_pageAttrs.end()) {

			ULONG_PTR pageAddr = PAGE_ADDRESS(i);

			pageAttr = &m_pageAttrs[i];
			pageAttr->refCount = 1;
			pageAttr->originProt = GetMemProt( pageAddr );
			pageAttr->prot = PAGE_READONLY;
			SetMemProt(pageAddr, PAGE_SIZE, PAGE_READONLY);

		} else {
			pageAttr = &it->second;
			if (pageAttr->prot != PAGE_READONLY)
				return false;

			pageAttr->refCount ++;
		}		
	}

	return true;
}

inline bool DbgEng::SetNoAccessBp(ULONG_PTR addr, DWORD width)
{
	ULONG_PTR beginPg = PAGE_FRAME(addr);
	ULONG_PTR endPg = PAGE_FRAME(addr + width - 1);

	PageAttrMap::iterator it;

	for (ULONG_PTR i = beginPg; i <= endPg; i ++) {
		it = m_pageAttrs.find(i);

		PageAttr* pageAttr;
		if (it == m_pageAttrs.end()) {

			ULONG_PTR pageAddr = PAGE_ADDRESS(i);

			pageAttr = &m_pageAttrs[i];
			pageAttr->refCount = 1;
			pageAttr->originProt = GetMemProt( pageAddr );
			pageAttr->prot = PAGE_NOACCESS;
			SetMemProt(pageAddr, PAGE_SIZE, PAGE_NOACCESS);

		} else {
			pageAttr = &it->second;
			if (pageAttr->prot != PAGE_NOACCESS) {

				assert(false);
				return false;
			}

			pageAttr->refCount ++;
			assert(pageAttr->refCount <= 0x10);
		}		
	}

	return true;
}

inline bool DbgEng::RestoreMemBp(ULONG_PTR addr, DWORD width)
{
	ULONG_PTR beginPg = PAGE_FRAME(addr);
	ULONG_PTR endPg = PAGE_FRAME(addr + width - 1);

	PageAttrMap::iterator it;

	for (ULONG_PTR i = beginPg; i <= endPg; i ++) {
		it = m_pageAttrs.find(i);

		PageAttr* pageAttr;
		if (it == m_pageAttrs.end()) {

			assert(false);
			return false;

		} else {

			pageAttr = &it->second;

			if (-- pageAttr->refCount == 0) {

				ULONG_PTR pageAddr = i << PAGE_SHIFT;
				SetMemProt(pageAddr, PAGE_SIZE, pageAttr->originProt);
				m_pageAttrs.erase(it);
			}			
		}		
	}

	return true;
}

bool DbgEng::CanSetBreakPoint(BreakPoint& bp)
{
	// FIXEME
	return true;
}

ULONG DbgEng::SetBreakPoint(void* addr, BreakPoint::BreakPointType type /* = BreakPoint::Execute */ , 
							ULONG width /* = 1 */, bool once /* = false */, bool hw /* = false */, 
							bool enabled /* = true */ )
{
	assert(!hw); // hardware break point NOT supported

	BreakPoint bp;

	Util::Autolock lock(*this);	

	if (type == BreakPoint::Execute)
		width = 1;

	BpListIt it = _FindBreakPoint(addr, width);
	if (it != m_breakpoints.end())
		return it->num;

	bp.num = m_bpnum + 1;
	bp.type = type;
	bp.addr = (ULONG_PTR )addr;
	bp.hw = hw;	
	bp.width = width;
	bp.once = once;
	bp.enabled = false;

	if (!CanSetBreakPoint(bp)) {

		return 0;
	}

	FreezeThreads();
	
	if (type == BreakPoint::Execute) {

		if (enabled) {

			if (!EnableBreakPoint(bp)) {

				UnfreezeThreads();
				return 0;
			}
		}

	} else if (type == BreakPoint::Access || type == BreakPoint::Write || type == BreakPoint::Execute2) {

		if (enabled) {
			if (!EnableBreakPoint(bp)) {

				UnfreezeThreads();
				assert(false);
				return 0;
			}
		}

	} else {

		UnfreezeThreads();
		assert(false);
		return 0;
	}

	m_breakpoints.push_back(bp);
	m_bpnum ++;

	UnfreezeThreads();
	return bp.num;
}


bool DbgEng::RemoveBreakPoint(int num, bool freeze /* = true */)
{
	Util::Autolock lock(*this);

	if (freeze)
		FreezeThreads();

	BreakPointList::iterator it = GetBreakPoint(num);
	if (it == m_breakpoints.end()) {

		if (freeze)
			UnfreezeThreads();
		return false;
	}

	BreakPoint& bp = *it;
	DisableBreakPoint(bp);
	m_breakpoints.erase(it);

	if (freeze)
		UnfreezeThreads();
	return true;
}

void DbgEng::ClearBreakPoints(bool freeze /* = true */)
{
	Util::Autolock lock(*this);

	if (freeze)
		FreezeThreads();

	BreakPointList::iterator it;
	for (it = m_breakpoints.begin(); it != m_breakpoints.end(); it ++) {

		BreakPoint& bp = *it;
		DisableBreakPoint(bp);
	}

	m_breakpoints.clear();
	if (freeze)
		UnfreezeThreads();
}

bool DbgEng::DisableBreakPoint(int num, bool freeze /* = true */)
{
	Util::Autolock lock(*this);

	if (freeze)
		FreezeThreads();

	BreakPointList::iterator it = GetBreakPoint(num);

	if (it == m_breakpoints.end()) {

		if (freeze)
			UnfreezeThreads();
		return false;
	}

	BreakPoint& bp = *it;
	bool result = DisableBreakPoint(bp);

	if (freeze)
		UnfreezeThreads();

	return result;
}

bool DbgEng::EnableBreakPoint(int num, bool freeze /* = true */)
{
	Util::Autolock lock(*this);

	if (freeze)
		FreezeThreads();

	BreakPointList::iterator it = GetBreakPoint(num);

	if (it == m_breakpoints.end()) {

		if (freeze)
			UnfreezeThreads();
		return false;
	}

	BreakPoint& bp = *it;
	bool result = EnableBreakPoint(bp);

	if (freeze)
		UnfreezeThreads();

	return result;
}

void DbgEng::PostResumeAction(DWORD tid, ResumeAction& act)
{
	if (m_pendingActions.size() == 0)
		FreezeThreads(tid);

	ThreadInfo* thread = GetThread(tid);
	SetSingleFlag(thread->hThread);

	m_bpTid = tid;
	if (FindResumeAction(act.addr) == NULL) {

		m_pendingActions.push_back(act);
	}
}

void DbgEng::ResolveResumeAction(DWORD tid, EXCEPTION_DEBUG_INFO& info)
{
	// ULONG_PTR brkAddr = (ULONG_PTR )info.ExceptionRecord.ExceptionAddress;

	while (!m_pendingActions.empty()) {

		ResumeAction& act = m_pendingActions.back();
		if (act.type == ResumeAction::RestoreInst) {
			WriteSoftBpInst((PVOID )act.addr);

		} else if (act.type == ResumeAction::RestoreProt) {

			SetMemProt(act.addr, act.width, act.prot);
		}

		if (act.bpNum != 0) {

			RemoveBreakPoint(act.bpNum, false);
		}

		m_pendingActions.pop_back();
	}

	m_bpTid = 0;
	UnfreezeThreads(tid);
}

//////////////////////////////////////////////////////////////////////////

bool DbgEng::DbgLoop()
{
	if (m_state == StateNone)
		return false;

	m_stoploop = false;
	const DWORD Timeout = 1000;

	DEBUG_EVENT dbgEvent;
	EventHResult hr;

	do {
		if (!WaitForDebugEvent(&dbgEvent, Timeout)) {
			if (GetLastError() == ERROR_SEM_TIMEOUT)
				continue;

			Log(L_INFO, _T("WaitForDebugEvent() failed. %#x\n"), GetLastError());

			break;
		}

		switch (dbgEvent.dwDebugEventCode) {
		case EXCEPTION_DEBUG_EVENT:
			hr = OnException(dbgEvent.dwThreadId, dbgEvent.u.Exception);
			break;

		case CREATE_THREAD_DEBUG_EVENT:
			hr = OnCreateThread(dbgEvent.dwThreadId, dbgEvent.u.CreateThread);
			break;

		case CREATE_PROCESS_DEBUG_EVENT:
			hr = OnCreateProcess(dbgEvent.dwThreadId, dbgEvent.u.CreateProcessInfo);
			break;

		case EXIT_THREAD_DEBUG_EVENT:
			hr = OnExitThread(dbgEvent.dwThreadId, dbgEvent.u.ExitThread);
			break;

		case EXIT_PROCESS_DEBUG_EVENT:
			hr = OnExitProcess(dbgEvent.dwThreadId, dbgEvent.u.ExitProcess);
			break;

		case LOAD_DLL_DEBUG_EVENT:
			hr = OnLoadDll(dbgEvent.dwThreadId, dbgEvent.u.LoadDll);
			break;

		case UNLOAD_DLL_DEBUG_EVENT:
			hr = OnUnloadDll(dbgEvent.dwThreadId, dbgEvent.u.UnloadDll);
			break;

		case OUTPUT_DEBUG_STRING_EVENT:
			hr = OnDbgStr(dbgEvent.dwThreadId, dbgEvent.u.DebugString);
			break;

		case RIP_EVENT:
			hr = OnRipEvent(dbgEvent.dwThreadId, dbgEvent.u.RipInfo);
			break;

		default:
			// error
			hr = DbgContinue;
			break;
		}

		if (hr == DbgNext || hr == DbgBreak)
			hr = DbgContinue;

		BOOL cont = ContinueDebugEvent(dbgEvent.dwProcessId, dbgEvent.dwThreadId, 
			(DWORD )hr);

		if (!cont) {

			Log(L_INFO,_T("ContinueDebugEvent() failed. GetLastError() = %#x\n"), GetLastError());
			break;
		}

		m_lastEvent = dbgEvent;

	} while (!m_stoploop || m_bpTid != 0);

	ClearBreakPoints();

	Log(L_INFO, _T("Exit DbgLoop...\n"));

	return true;
}

EventHResult DbgEng::OnCreateThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info)
{
	Log(L_INFO, __LOGPREFIX__ "CreateThread: TID: %d, Start: %p\n", 
		tid, info.lpStartAddress);

	m_threads[tid] = info;
	ReadTeb(m_threads[tid]);

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnCreateThread(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}	

	return DbgContinue;
}

EventHResult DbgEng::OnCreateProcess(DWORD tid, CREATE_PROCESS_DEBUG_INFO& info)
{
	m_hProcess = info.hProcess;
	SetEvent(m_attachEvent);

	Log(L_INFO, __LOGPREFIX__  "CreateProcess: PID: %d, TID: %d, Start: %p\n", 
		m_pid, tid, info.lpStartAddress);

	m_procInfo = info;

	m_mainTid = tid;
	m_hMainThread = info.hThread;

	CREATE_THREAD_DEBUG_INFO threadInfo;
	threadInfo.hThread = info.hThread;
	threadInfo.lpStartAddress = info.lpStartAddress;
	threadInfo.lpThreadLocalBase = info.lpThreadLocalBase;
	m_threads[tid] = threadInfo;
	ReadTeb(m_threads[tid]);

	LOAD_DLL_DEBUG_INFO dllInfo;
	dllInfo.hFile = info.hFile;	
	dllInfo.fUnicode = info.fUnicode;
	dllInfo.lpBaseOfDll = info.lpBaseOfImage;
	dllInfo.lpImageName = info.lpImageName;
	dllInfo.nDebugInfoSize = info.nDebugInfoSize;
	dllInfo.dwDebugInfoFileOffset = info.dwDebugInfoFileOffset;
	std::string dllName = ProcessNewModule(tid, dllInfo);
	Log(L_INFO, __LOGPREFIX__ "LoadDll: Name: %s Base: %#x\n", dllName.c_str(), 
		dllInfo.lpBaseOfDll);

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnCreateProcess(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	// SetEvent(m_attachedEvent);
	return DbgContinue;
}

EventHResult DbgEng::OnExitThread(DWORD tid, EXIT_THREAD_DEBUG_INFO& info)
{
	Log(L_INFO, __LOGPREFIX__ "ExitThread: TID: %d, ExitCode: %#x\n", tid, 
		info.dwExitCode);

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnExitThread(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	m_threads.erase(tid);

	return DbgContinue;
}

EventHResult DbgEng::OnExitProcess(DWORD tid, EXIT_PROCESS_DEBUG_INFO& info)
{
	Log(L_INFO, __LOGPREFIX__ "ExitProcess: PID: %d, ExitCode: %#x\n", m_pid, 
		info.dwExitCode);

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnExitProcess(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	m_threads.erase(tid);
	m_dlls.clear();

	return DbgContinue;
}

std::string DbgEng::ProcessNewModule(DWORD tid, LOAD_DLL_DEBUG_INFO& info)
{
	ModuleInfo& modInfo = m_dlls[info.lpBaseOfDll];
	modInfo = info;
	memset(modInfo.szModName, 0, sizeof(modInfo.szModName));
	if (info.lpImageName == NULL) {

		GetModuleFileNameExA(m_hProcess, (HMODULE )info.lpBaseOfDll, modInfo.szModName, 
			sizeof(modInfo.szModName));
	} else {

		LPSTR modName;
		ReadMemory(info.lpImageName, (PBYTE )&modName, sizeof(modName));		
		if (modName != NULL) {
			WCHAR buf[MAX_PATH + 1];
			ReadMemory(modName, (PBYTE )buf, MAX_PATH);

			// FIXME, char convert
			USES_CONVERSION;
			strncpy(modInfo.szModName, W2A(buf), MAX_PATH);
		}
	}

	IMAGE_DOS_HEADER dosHdr;
	ReadMemory(modInfo.lpBaseOfDll, (PBYTE )&dosHdr, sizeof(dosHdr));
	IMAGE_NT_HEADERS ntHdrs;
	ReadMemory(MakePtr(modInfo.lpBaseOfDll, dosHdr.e_lfanew), (PBYTE )&ntHdrs, sizeof(ntHdrs));
	modInfo.nImageSize = ntHdrs.OptionalHeader.SizeOfImage;
	
	// MODULEINFO mi;
	// GetModuleInformation(m_hProcess, (HMODULE )info.lpBaseOfDll, &mi, sizeof(mi));
	modInfo.nImageSize = ntHdrs.OptionalHeader.SizeOfImage;
	modInfo.EntryPoint = (ULONG_PTR )info.lpBaseOfDll + ntHdrs.OptionalHeader.AddressOfEntryPoint;

	return modInfo.szModName;
}

EventHResult DbgEng::OnLoadDll(DWORD tid, LOAD_DLL_DEBUG_INFO& info)
{
	std::string dllName = ProcessNewModule(tid, info);

	Log(L_INFO, __LOGPREFIX__ "LoadDll: Name: %s Base: %#x\n", dllName.c_str(), 
		info.lpBaseOfDll);

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnLoadDll(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	return DbgContinue;
}

EventHResult DbgEng::OnUnloadDll(DWORD tid, UNLOAD_DLL_DEBUG_INFO& info)
{
	std::string dllName;
	DllMap::iterator it2 = m_dlls.find(info.lpBaseOfDll);
	if (it2 != m_dlls.end()) {

		dllName = it2->second.szModName;
	}

	Log(L_INFO, __LOGPREFIX__ "UnloadDll: Name: %s Base: %#x\n", dllName.c_str(), 
		info.lpBaseOfDll);

	m_dlls.erase(info.lpBaseOfDll);

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnUnloadDll(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	return DbgContinue;
}

EventHResult DbgEng::OnDbgStr(DWORD tid, OUTPUT_DEBUG_STRING_INFO& info)
{
	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnDbgStr(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	char buf[1024] = { 0 };
	size_t len = info.nDebugStringLength > sizeof(buf) - 1 ? sizeof(buf) - 1 :
		info.nDebugStringLength ;

	ReadMemory(info.lpDebugStringData, (BYTE* )buf, len);
	Log(L_INFO, __LOGPREFIX__ "DbgStr: %s\n", buf);

	return DbgContinue;
}

EventHResult DbgEng::OnRipEvent(DWORD tid, RIP_INFO& info)
{
	Log(L_INFO, __LOGPREFIX__ "\n");

	m_brktid = tid;

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnRipEvent(tid, info);

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	return DbgContinue;
}

EventHResult DbgEng::OnWaitted()
{
	Log(L_INFO, __LOGPREFIX__ "\n");

	DbgEventList::iterator it;
	for (it = m_events.begin(); it != m_events.end(); it ++) {
		DbgEvent* event = *it;
		EventHResult hr = event->OnWaitted();

		if (hr > DbgNext)
			return hr;

		if (hr == DbgBreak)
			break;
	}

	return DbgContinue;
}

EventHResult DbgEng::OnException(DWORD tid, EXCEPTION_DEBUG_INFO& info)
{
	ThreadInfo* thread = NULL;

	EventHResult hr = DbgExceptionNotHandled;
	// EventHResult hr = DbgContinue;
	bool handled = false;
	EXCEPTION_RECORD& excepRec = info.ExceptionRecord;

	m_brktid = tid;

	if (excepRec.ExceptionCode == STATUS_SINGLE_STEP) {

		Util::Autolock lock(*this);

		if (m_bpTid == tid) {
			
			ResolveResumeAction(tid, info);
			return DbgContinue;
		}

		hr = DbgContinue;
	}

	if (excepRec.ExceptionCode == STATUS_BREAKPOINT) {

		Util::Autolock lock(*this);

		BpListIt it = _FindBreakPoint(excepRec.ExceptionAddress);
		if (it != m_breakpoints.end()) {

			BreakPoint& bp = *it;
			DisableBreakPoint(bp);

			if (bp.once) {
				m_breakpoints.erase(it);
			}

			ResumeAction act;
			act.addr = (ULONG_PTR )excepRec.ExceptionAddress;
			act.width = 1;
			act.type = ResumeAction::RestoreInst;
			act.bpNum = 0;

			PostResumeAction(tid, act);
			handled = true;
		}

	} else if (excepRec.ExceptionCode == STATUS_ACCESS_VIOLATION) {

		Util::Autolock lock(*this);

		ULONG_PTR addr = 0;

		BpListIt it;

		if (excepRec.ExceptionInformation[1] == 0) {

			it = _FindBreakPoint(excepRec.ExceptionAddress);
			addr = (ULONG_PTR )excepRec.ExceptionAddress;

		} else {

			it = _FindBreakPoint((PVOID )excepRec.ExceptionInformation[1]);
			addr = excepRec.ExceptionInformation[1];
		}

		BreakPoint* bp = NULL;

		if (it != m_breakpoints.end()) {

			bp = &(*it);
			
			DbgEventList::iterator it;
			for (it = m_events.begin(); it != m_events.end(); it ++) {
				DbgEvent* event = *it;
				hr = event->OnBreakPoint(tid, *bp, info);

				if (hr != DbgNext)
					break;
			}			

		}

		{
			DWORD pageNum = PAGE_FRAME(addr);
			PageAttrMap::iterator it = m_pageAttrs.find(pageNum);
			if (it != m_pageAttrs.end()) {

				PageAttr& attr = it->second;
				ResumeAction act;
				act.type = ResumeAction::RestoreProt;
				act.addr = PAGE_ADDRESS(pageNum);
				act.width = PAGE_SIZE;
				act.prot = attr.prot;				

				if (bp != NULL && bp->once)
					act.bpNum = bp->num;
				else
					act.bpNum = 0;

				SetMemProt(act.addr, PAGE_SIZE, attr.originProt);
				PostResumeAction(tid, act);

				handled = true;
			}
		}
	}

	if (handled) {

		hr = DbgContinue;

	} else {

		DbgEventList::iterator it;
		EventHResult hr2;
		for (it = m_events.begin(); it != m_events.end(); it ++) {
			DbgEvent* event = *it;
			hr2 = event->OnException(tid, info);

			if (hr2 != DbgNext)
				break;
		}

		 // FIXME
		/*
		if (hr2 == DbgWait) {

			WaitForSingleObject(m_waitEvent);
		}
		*/
		
		if (hr2 != DbgNext && hr2 != DbgBreak) // FIXME
			hr = hr2;

		{
			PVOID addr = info.ExceptionRecord.ExceptionAddress;

			t_disasm da;
			char code[32];

			memset(&da, 0, sizeof(da));
			size_t len = ReadMemory(addr, (BYTE* )code, 
				sizeof(code));

			ulong l = Disasm(code, len, (ulong )addr, &da, DISASM_SIZE);
			l = Disasm(code, l, (ulong )addr, &da, DISASM_CODE);

			if (thread == NULL)
				thread = GetThread(tid);

			Log(L_INFO, __LOGPREFIX__ "Exception: TID: %d, Addr: %x, Code: %x, EIP: %p | %3i  %-24s  %-24s\n", 
				tid, addr, excepRec.ExceptionCode, GetProgPtr(thread->hThread), l, da.dump, da.result);
		}
	}

	return hr;
}
