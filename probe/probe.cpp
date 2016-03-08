// probe.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"

static void Init();

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

    return TRUE;
}

void DbgPrintf(LPCTSTR fmt, ...)
{
	TCHAR str[2048];

	va_list varg;
	va_start(varg, fmt);
	_vsntprintf(str, _countof(str), fmt, varg);
	va_end(varg);

	OutputDebugString(str);
}

void * ( __cdecl* Real_malloc)(size_t);

static void * __cdecl Mine_malloc(size_t size)
{
	void* p = Real_malloc(size);

	if (size >= 128)
		DbgPrintf(_T("malloc: addr = %p, size = %d"), p, size);

	return p;
}

void * ( __cdecl* Real_new)(size_t);

void * Mine_new(size_t size)
{
	void* p = Real_new(size);

	if (size >= 512)
		DbgPrintf(_T("new: addr = %p, size = %d"), p, size);

	return p;
}

const LPSTR opNewName = "??2@YAPAXI@Z";

void Init()
{
	// hook malloc

	/*
	void * ( __cdecl* _malloc)(size_t);

	_malloc = (void* ( __cdecl* )(size_t ) ) DetourFindFunction(
		"msvcrt.dll", "malloc");
		// "msvcr80.dll", "malloc");

	Real_malloc = (void* ( __cdecl* )(size_t ) ) DetourFunction(
		(PBYTE )_malloc, (PBYTE )Mine_malloc);

	DbgPrintf("Probe::Init(), Real_malloc = %p, Mine_malloc = %p\n", 
		_malloc, Mine_malloc);
	*/

	// hook new

	void * ( __cdecl* _new)(size_t);

	_new = (void* ( __cdecl* )(size_t ) ) DetourFindFunction(
		"msvcr80.dll", opNewName);

	DbgPrintf("Probe::Init(), _new = %p\n", _new);

	Real_new = (void* ( __cdecl* )(size_t ) ) DetourFunction(
		(PBYTE )_new, (PBYTE )Mine_new);

	DbgPrintf("Probe::Init(), Real_new = %p, Mine_new = %p\n", 
		_new, Mine_new);
}
