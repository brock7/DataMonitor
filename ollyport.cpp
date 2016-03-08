// ollyport.cpp: implementation of the ollyport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DbgEng.h"
#include "ollyport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

extern DbgEng* g_dbgEng;

//////////////////////////////////////////////////////////////////////////
#define _DUMMY_FUNC(NAME)	__declspec(naked) void NAME() \
	{ \
			OutputDebugString(#NAME "() called\n"); \
			__asm {ret} \
	}

_DUMMY_FUNC(_Addsorteddata);
// _DUMMY_FUNC(_Addtolist);
_DUMMY_FUNC(_Analysecode);
_DUMMY_FUNC(_Assemble);
_DUMMY_FUNC(_Broadcast);
_DUMMY_FUNC(_Browsefilename);
_DUMMY_FUNC(_Calculatecrc);
_DUMMY_FUNC(_Checkcondition);
_DUMMY_FUNC(_Compress);
_DUMMY_FUNC(_Createdumpwindow);
_DUMMY_FUNC(_Createlistwindow);
_DUMMY_FUNC(_Createsorteddata);
_DUMMY_FUNC(_Decodeaddress);
_DUMMY_FUNC(_Decodecharacter);
_DUMMY_FUNC(_Decodefullvarname);
_DUMMY_FUNC(_Decodeknownargument);
_DUMMY_FUNC(_Decodename);
_DUMMY_FUNC(_Decoderange);
_DUMMY_FUNC(_Decoderelativeoffset);
_DUMMY_FUNC(_Decodethreadname);
_DUMMY_FUNC(_Decompress);
_DUMMY_FUNC(_Defaultbar);
_DUMMY_FUNC(_Deletebreakpoints);
_DUMMY_FUNC(_Deletehardwarebreakpoint);
_DUMMY_FUNC(_Deletenamerange);
_DUMMY_FUNC(_Deletenonconfirmedsorteddata);
_DUMMY_FUNC(_Deletesorteddata);
_DUMMY_FUNC(_Deletesorteddatarange);
_DUMMY_FUNC(_Demanglename);
_DUMMY_FUNC(_Destroysorteddata);
_DUMMY_FUNC(_Disasm);
_DUMMY_FUNC(_Disassembleback);
_DUMMY_FUNC(_Disassembleforward);
_DUMMY_FUNC(_Discardquicknames);
_DUMMY_FUNC(_Error);
_DUMMY_FUNC(_Expression);
_DUMMY_FUNC(_Findallcommands);
_DUMMY_FUNC(_Finddecode);
_DUMMY_FUNC(_Findfileoffset);
_DUMMY_FUNC(_Findfixup);
_DUMMY_FUNC(_Findimportbyname);
_DUMMY_FUNC(_Findlabel);
_DUMMY_FUNC(_Findlabelbyname);
_DUMMY_FUNC(_Findmemory);
_DUMMY_FUNC(_Findmodule);
_DUMMY_FUNC(_Findname);
_DUMMY_FUNC(_Findnextname);
_DUMMY_FUNC(_Findreferences);
_DUMMY_FUNC(_Findsorteddata);
_DUMMY_FUNC(_Findsorteddataindex);
_DUMMY_FUNC(_Findsorteddatarange);
_DUMMY_FUNC(_Findstrings);
// _DUMMY_FUNC(_Findthread);
_DUMMY_FUNC(_Flash);
_DUMMY_FUNC(_Get3dnow);
_DUMMY_FUNC(_Getaddressfromline);
_DUMMY_FUNC(_Getasmfindmodel);
_DUMMY_FUNC(_Getbprelname);
_DUMMY_FUNC(_Getbreakpointtype);
// _DUMMY_FUNC(_Getcputhreadid);
_DUMMY_FUNC(_Getdisassemblerrange);
_DUMMY_FUNC(_Getfloat);
_DUMMY_FUNC(_Getfloat10);
_DUMMY_FUNC(_Gethexstring);
_DUMMY_FUNC(_Getline);
_DUMMY_FUNC(_Getlinefromaddress);
_DUMMY_FUNC(_Getlong);
_DUMMY_FUNC(_Getmmx);
_DUMMY_FUNC(_Getnextbreakpoint);
_DUMMY_FUNC(_Getresourcestring);
_DUMMY_FUNC(_Getsortedbyselection);
_DUMMY_FUNC(_Getsourcefilelimits);
// _DUMMY_FUNC(_Getstatus);
_DUMMY_FUNC(_Gettext);
_DUMMY_FUNC(_Go);
_DUMMY_FUNC(_Guardmemory);
_DUMMY_FUNC(_Havecopyofmemory);
_DUMMY_FUNC(_Infoline);
_DUMMY_FUNC(_Insertname);
_DUMMY_FUNC(_Isretaddr);
_DUMMY_FUNC(_IstextA);
_DUMMY_FUNC(_IstextW);
_DUMMY_FUNC(_Manualbreakpoint);
_DUMMY_FUNC(_Mergequicknames);
_DUMMY_FUNC(_Message);
_DUMMY_FUNC(_Newtablewindow);
_DUMMY_FUNC(_Painttable);
// _DUMMY_FUNC(_Plugingetvalue);
_DUMMY_FUNC(_Pluginreadintfromini);
_DUMMY_FUNC(_Pluginreadstringfromini);
_DUMMY_FUNC(_Pluginsaverecord);
_DUMMY_FUNC(_Pluginwriteinttoini);
_DUMMY_FUNC(_Pluginwritestringtoini);
_DUMMY_FUNC(_Print3dnow);
_DUMMY_FUNC(_Printfloat10);
_DUMMY_FUNC(_Printfloat4);
_DUMMY_FUNC(_Printfloat8);
_DUMMY_FUNC(_Progress);
_DUMMY_FUNC(_Quickinsertname);
_DUMMY_FUNC(_Quicktablewindow);
// _DUMMY_FUNC(_Readmemory);
_DUMMY_FUNC(_Redrawdisassembler);
_DUMMY_FUNC(_Registerotclass);
_DUMMY_FUNC(_Registerpluginclass);
_DUMMY_FUNC(_Selectandscroll);
_DUMMY_FUNC(_Setbreakpoint);
_DUMMY_FUNC(_Setcpu);
_DUMMY_FUNC(_Sethardwarebreakpoint);
_DUMMY_FUNC(_Setmembreakpoint);
_DUMMY_FUNC(_Showsourcefromaddress);
_DUMMY_FUNC(_Sortsorteddata);
_DUMMY_FUNC(_Suspendprocess);
_DUMMY_FUNC(_Tablefunction);
_DUMMY_FUNC(_Unregisterpluginclass);
_DUMMY_FUNC(_Updatelist);
_DUMMY_FUNC(_Walkreference);
// _DUMMY_FUNC(_Writememory);
_DUMMY_FUNC(_Findhittrace);
_DUMMY_FUNC(_Findnextruntraceip);
_DUMMY_FUNC(_Findprevruntraceip);
_DUMMY_FUNC(_Getruntraceprofile);
_DUMMY_FUNC(_Getruntraceregisters);
_DUMMY_FUNC(_Modifyhittrace);
_DUMMY_FUNC(_Runtracesize);
_DUMMY_FUNC(_Scrollruntracewindow);
_DUMMY_FUNC(_Createprofilewindow);
_DUMMY_FUNC(_Decodeascii);
_DUMMY_FUNC(_Decodeunicode);
_DUMMY_FUNC(_Deleteruntrace);
_DUMMY_FUNC(_Deletewatch);
_DUMMY_FUNC(_Findallsequences);
_DUMMY_FUNC(_Findnextproc);
_DUMMY_FUNC(_Findprevproc);
_DUMMY_FUNC(_Findprocbegin);
_DUMMY_FUNC(_Findprocend);
_DUMMY_FUNC(_Findsymbolicname);
_DUMMY_FUNC(_Findunknownfunction);
_DUMMY_FUNC(_Get3dnowxy);
_DUMMY_FUNC(_Getasmfindmodelxy);
_DUMMY_FUNC(_Getfloat10xy);
_DUMMY_FUNC(_Getfloatxy);
_DUMMY_FUNC(_Gethexstringxy);
_DUMMY_FUNC(_Getlinexy);
_DUMMY_FUNC(_Getlongxy);
_DUMMY_FUNC(_Getmmxxy);
_DUMMY_FUNC(_Getoriginaldatasize);
_DUMMY_FUNC(_Getproclimits);
_DUMMY_FUNC(_Gettableselectionxy);
_DUMMY_FUNC(_Gettextxy);
_DUMMY_FUNC(_Getwatch);
_DUMMY_FUNC(_Injectcode);
_DUMMY_FUNC(_Insertwatch);
_DUMMY_FUNC(_Isfilling);
_DUMMY_FUNC(_Issuspicious);
_DUMMY_FUNC(_OpenEXEfile);
_DUMMY_FUNC(_Printsse);
_DUMMY_FUNC(_Readcommand);
_DUMMY_FUNC(_Restoreallthreads);
_DUMMY_FUNC(_Runsinglethread);
_DUMMY_FUNC(_Setdisasm);
_DUMMY_FUNC(_Startruntrace);
_DUMMY_FUNC(_Stringtotext);
_DUMMY_FUNC(_Walkreferenceex);
_DUMMY_FUNC(_Animate);
_DUMMY_FUNC(_Creatertracewindow);
_DUMMY_FUNC(_Createthreadwindow);
_DUMMY_FUNC(_Createwatchwindow);
_DUMMY_FUNC(_Createwinwindow);
_DUMMY_FUNC(_Deletehardwarebreakbyaddr);
_DUMMY_FUNC(_Dumpbackup);
_DUMMY_FUNC(_Hardbreakpoints);
_DUMMY_FUNC(_Sendshortcut);
_DUMMY_FUNC(_Setdumptype);
_DUMMY_FUNC(_Settracecondition);
_DUMMY_FUNC(_Findalldllcalls);
_DUMMY_FUNC(_Followcall);
_DUMMY_FUNC(_Getregxy);
_DUMMY_FUNC(_Isprefix);
_DUMMY_FUNC(_Tempbreakpoint);
_DUMMY_FUNC(_Attachtoactiveprocess);
_DUMMY_FUNC(_Createpatchwindow);
_DUMMY_FUNC(_Settracecount);
_DUMMY_FUNC(_Settracepauseoncommands);
_DUMMY_FUNC(_Getbreakpointtypecount);
_DUMMY_FUNC(_Setbreakpointext);
_DUMMY_FUNC(_Listmemory);
_DUMMY_FUNC(__GetExceptDllInfo);
_DUMMY_FUNC(___CPPdebugHook);

//////////////////////////////////////////////////////////////////////////

OLLY_API void cdecl Addtolist(long addr,int highlight,char *fmt, ...)
{
	TCHAR buf[2048];
	va_list vlist;
	va_start(vlist, fmt);
	int r =_vsntprintf(buf, sizeof(buf), fmt, vlist);
	g_dbgEng->Log(L_INFO, "Addtolist() - %s\n", buf);
	va_end(vlist);
}

OLLY_API t_thread* cdecl Findthread(ulong threadid)
{	
	g_dbgEng->Log(L_DEBUG, "Findthread() - %d\n", threadid);

	return OllyPort::instance().FindThread(threadid);	
}

OLLY_API ulong cdecl Getcputhreadid(void)
{
	g_dbgEng->Log(L_DEBUG, "Getcputhreadid()\n");
	return g_dbgEng->GetBreakThreadId();
}

OLLY_API t_status cdecl Getstatus(void)
{
	g_dbgEng->Log(L_DEBUG, "Getstatus()\n");
	DbgEng::DbgState st = g_dbgEng->GetState();

	t_status stats[] = {
		STAT_NONE, 
		STAT_RUNNING, 
		STAT_STOPPED, 
		STAT_EVENT, 
		STAT_EVENT, 
	};

	return stats[(int )st];
}

OLLY_API int cdecl Plugingetvalue(int type)
{
	g_dbgEng->Log(L_DEBUG, "Plugingetvalue() - type: %d\n", type);

	int result = 0;

	switch (type) {
	case VAL_HPROCESS:
		result = (int ) g_dbgEng->GetProcessHandle();
		break;

	case VAL_PROCESSID:
		result = (int ) g_dbgEng->GetProcessId();
		break;

	case VAL_HMAINTHREAD:
		result = (int ) g_dbgEng->GetMainThreadHandle();
		break;

	case VAL_MAINTHREADID:
		result = (int ) g_dbgEng->GetMainThreadId();
		break;
	};

	return result;
}

OLLY_API ulong cdecl Readmemory(void *buf,ulong addr,ulong size,int mode)
{
	g_dbgEng->Log(L_DEBUG, "Readmemory()\n");

	g_dbgEng->ReadMemory((void* )addr, (BYTE* )buf, size);

	return 0;
}

OLLY_API ulong cdecl Writememory(void *buf,ulong addr,ulong size,int mode)
{
	g_dbgEng->Log(L_DEBUG, "Writememory()\n");
	g_dbgEng->WriteMemory((void* )addr, (BYTE* )buf, size);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

OllyPort::OllyPort()
{

}

bool OllyPort::InitOllyPort()
{
	CFileFind finder;
	

	BOOL bWorking = finder.FindFile(_T(".\\plugin\\*.dll"));
	HWND mainWnd = AfxGetMainWnd()->GetSafeHwnd();

	g_dbgEng->RegisterEventHandle(*this);

	while (bWorking) {

		bWorking = finder.FindNextFile();

		HMODULE hMod = LoadLibrary(finder.GetFilePath());

		if (hMod) {
			if (!InitPlugin(hMod, finder.GetFileName(), mainWnd))
				FreeLibrary(hMod);
		}
	}

	return true;
}

#define TO_STRING(N)					#N
#define INIT_OLLY_CALLBACK(NAME)		pi.NAME = (T##NAME )GetProcAddress(hMod, TO_STRING(_##NAME));

bool OllyPort::InitPlugin(HMODULE hMod, LPCTSTR ModName, HWND hwnd)
{
	PluginInfo pi;

	memset(&pi, 0, sizeof(pi));
	pi.hMod = hMod;

	pi.ODBG_Plugindata = (TODBG_Plugindata )GetProcAddress(hMod, "_ODBG_Plugindata");
	if (pi.ODBG_Plugindata == NULL)
		return false;

	pi.ODBG_Plugininit = (TODBG_Plugininit )GetProcAddress(hMod, "_ODBG_Plugininit");
	if (pi.ODBG_Plugininit == NULL)
		return false;

	INIT_OLLY_CALLBACK(ODBG_Pluginmainloop);
	INIT_OLLY_CALLBACK(ODBG_Pluginsaveudd);
	INIT_OLLY_CALLBACK(ODBG_Pluginmainloop);
	INIT_OLLY_CALLBACK(ODBG_Pluginuddrecord);
	INIT_OLLY_CALLBACK(ODBG_Pluginmenu);
	INIT_OLLY_CALLBACK(ODBG_Pluginaction);
	INIT_OLLY_CALLBACK(ODBG_Pluginshortcut);
	INIT_OLLY_CALLBACK(ODBG_Pluginreset);
	INIT_OLLY_CALLBACK(ODBG_Pluginclose);
	INIT_OLLY_CALLBACK(ODBG_Plugindestroy);
	INIT_OLLY_CALLBACK(ODBG_Paused);
	INIT_OLLY_CALLBACK(ODBG_Pausedex);
	INIT_OLLY_CALLBACK(ODBG_Plugincmd);

	if (pi.ODBG_Plugininit(OLLY_VERSION, hwnd, NULL) != 0) {
		g_dbgEng->Log(L_WARNING, "ODBG_Plugininit() failed. - plugin: %s\n", ModName);
		return false;
	}

	m_pluginMap[ModName] = pi;
	return true;
}

void OllyPort::RaisePausedEx(DWORD tid, DEBUG_EVENT* DbgEvent)
{
	PluginMap::iterator it;
	CONTEXT ctx;
	t_thread* thread = OllyPort::instance().FindThread(tid);

	t_reg* reg = NULL;

	if (thread != NULL) {

		reg = &thread->reg;
		thread->oldreg = *reg;
		thread->oldregvalid = 1;

		ctx.ContextFlags = CONTEXT_ALL;
		GetThreadContext(thread->thread, &ctx);
		ContextToReg(tid, ctx, *reg);
	}

	for (it = m_pluginMap.begin(); it != m_pluginMap.end(); it ++) {

		PluginInfo& pluginInfo = it->second;
	
		if (pluginInfo.ODBG_Pausedex) {			
			
			pluginInfo.ODBG_Pausedex(PP_EVENT, 0, reg, DbgEvent);

		} else if (pluginInfo.ODBG_Paused) {

			pluginInfo.ODBG_Paused(PP_EVENT, reg);
		}
	}
}

#define TEB_SIZE			0xfb8

void OllyPort::ContextToReg(DWORD tid, const CONTEXT& ctx, t_reg& reg)
{
	ulong old_fs = reg.base[SEG_FS];
	memset(&reg, 0, sizeof(reg));
	
	reg.threadid = tid;
	reg.r[REG_EAX] = ctx.Eax;
	reg.r[REG_EBX] = ctx.Ebx;
	reg.r[REG_ECX] = ctx.Ecx;
	reg.r[REG_EDX] = ctx.Edx;
	reg.r[REG_ESI] = ctx.Esi;
	reg.r[REG_EDI] = ctx.Edi;
	reg.r[REG_EBP] = ctx.Ebp;
	reg.r[REG_ESP] = ctx.Esp;
	reg.s[SEG_CS] = ctx.SegCs;
	reg.s[SEG_DS] = ctx.SegDs;
	reg.s[SEG_ES] = ctx.SegEs;
	reg.s[SEG_FS] = ctx.SegFs;
	reg.s[SEG_GS] = ctx.SegGs;
	reg.s[SEG_SS] = ctx.SegSs;

	reg.base[SEG_CS] = 0;
	reg.base[SEG_DS] = 0;
	reg.base[SEG_ES] = 0;
	reg.base[SEG_FS] = old_fs;
	reg.base[SEG_GS] = 0;
	reg.base[SEG_SS] = 0;

	reg.limit[SEG_CS] = 0xffffffff;
	reg.limit[SEG_DS] = 0xffffffff;
	reg.limit[SEG_ES] = 0xffffffff;
	reg.limit[SEG_FS] = TEB_SIZE;
	reg.limit[SEG_GS] = 0xffffffff;
	reg.limit[SEG_SS] = 0xffffffff;

	reg.ip = ctx.Eip;
	reg.flags = ctx.EFlags;
	reg.drlin[0] = ctx.Dr0;
	reg.drlin[1] = ctx.Dr1;
	reg.drlin[2] = ctx.Dr2;
	reg.drlin[3] = ctx.Dr3;
	reg.dr6 = ctx.Dr6;
	reg.dr7 = ctx.Dr7;
	memcpy(reg.FloatRegisterArea, &ctx.FloatSave, sizeof(reg.FloatRegisterArea));
}

void OllyPort::InitOllyThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info, t_thread& ollyThread)
{
	ollyThread.threadid = tid;
	ollyThread.entry = (ulong )info.lpStartAddress;
	ollyThread.context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(info.hThread ,&ollyThread.context);
	ollyThread.thread = info.hThread;
	ollyThread.regvalid = 1;
	ContextToReg(tid, ollyThread.context, ollyThread.reg);
	ollyThread.reg.base[SEG_FS] = (ulong )info.lpThreadLocalBase;
	if (g_dbgEng->GetMainThreadId() == tid)
		ollyThread.type = TY_MAIN;
	else
		ollyThread.type = TY_NEW;
}

//////////////////////////////////////////////////////////////////////////
#define _CALC_DEBUG_EVENT(INFO)				CONTAINING_RECORD(&INFO, DEBUG_EVENT, u)

// EventHResult OllyPort::OnException(DWORD tid, EXCEPTION_DEBUG_INFO& info)
EventHResult OllyPort::OnBreakPoint(DWORD tid, BreakPoint& bp, EXCEPTION_DEBUG_INFO& info)
{
	/* if (!g_dbgEng->IsBreakPointException(info)) */ {

		static ULONG RaiseCount = 0;
		if (++ RaiseCount > 200) {
			RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));
			RaiseCount = 0;
		}
	}
	
	return DbgNext;
}

EventHResult OllyPort::OnCreateThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info)
{
	t_thread thread;
	InitOllyThread(tid, info, thread);
	m_threads[tid] = thread;

	RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));
	return DbgNext;
}

EventHResult OllyPort::OnCreateProcess(DWORD tid, CREATE_PROCESS_DEBUG_INFO& info)
{
	CREATE_THREAD_DEBUG_INFO threadInfo;
	threadInfo.hThread = info.hThread;
	threadInfo.lpStartAddress = info.lpStartAddress;
	threadInfo.lpThreadLocalBase = info.lpThreadLocalBase;

	t_thread thread;
	InitOllyThread(tid, threadInfo, thread);
	m_threads[tid] = thread;

	RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));

	return DbgNext;
}

EventHResult OllyPort::OnExitThread(DWORD tid, EXIT_THREAD_DEBUG_INFO& info)
{
	RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));

	m_threads.erase(tid);

	return DbgNext;
}

EventHResult OllyPort::OnExitProcess(DWORD tid, EXIT_PROCESS_DEBUG_INFO& info)
{
	RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));

	m_threads.erase(tid);

	return DbgNext;
}

EventHResult OllyPort::OnLoadDll(DWORD tid, LOAD_DLL_DEBUG_INFO& info)
{
	RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));
	return DbgNext;
}

EventHResult OllyPort::OnUnloadDll(DWORD tid, UNLOAD_DLL_DEBUG_INFO & info)
{
	RaisePausedEx(tid, _CALC_DEBUG_EVENT(info));
	return DbgNext;
}

EventHResult OllyPort::OnDbgStr(DWORD tid, OUTPUT_DEBUG_STRING_INFO& info)
{	
	return DbgNext;
}

EventHResult OllyPort::OnRipEvent(DWORD tid, RIP_INFO& info)
{
	return DbgNext;
}

