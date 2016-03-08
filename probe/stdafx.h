// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include "detours.h"
#include <malloc.h>
#include <stdio.h>
#include <tchar.h>
#include <stdarg.h>

#define _countof(array) (sizeof(array)/sizeof(array[0]))

// TODO: 在此处引用程序要求的附加头文件
