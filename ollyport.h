// ollyport.h: interface for the ollyport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLLYPORT_H__10176EEA_EAA7_4FD1_8F64_FDBC7651F94C__INCLUDED_)
#define AFX_OLLYPORT_H__10176EEA_EAA7_4FD1_8F64_FDBC7651F94C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OLLY_VERSION			0x6e

//////////////////////////////////////////////////////////////////////////
// support ollydbg plugin

#ifdef __cplusplus
  #define extc           extern "C"    // Assure that names are not mangled
#else
  #define extc           extern
#endif

#define OLLY_API		

#define Addtolist			_Addtolist
#define Findthread			_Findthread
#define Getcputhreadid		_Getcputhreadid
#define Getstatus			_Getstatus
#define Plugingetvalue		_Plugingetvalue
#define Readmemory			_Readmemory
#define Writememory			_Writememory

//////////////////////////////////////////////////////////////////////////

#define VAL_HINST              1       // Current program instance
#define VAL_HWMAIN             2       // Handle of the main window
#define VAL_HWCLIENT           3       // Handle of the MDI client window
#define VAL_NCOLORS            4       // Number of common colors
#define VAL_COLORS             5       // RGB values of common colors
#define VAL_BRUSHES            6       // Handles of common color brushes
#define VAL_PENS               7       // Handles of common color pens
#define VAL_NFONTS             8       // Number of common fonts
#define VAL_FONTS              9       // Handles of common fonts
#define VAL_FONTNAMES          10      // Internal font names
#define VAL_FONTWIDTHS         11      // Average widths of common fonts
#define VAL_FONTHEIGHTS        12      // Average heigths of common fonts
#define VAL_NFIXFONTS          13      // Actual number of fixed-pitch fonts
#define VAL_DEFFONT            14      // Index of default font
#define VAL_NSCHEMES           15      // Number of color schemes
#define VAL_SCHEMES            16      // Color schemes
#define VAL_DEFSCHEME          17      // Index of default colour scheme
#define VAL_DEFHSCROLL         18      // Default horizontal scroll
#define VAL_RESTOREWINDOWPOS   19      // Restore window positions from .ini
#define VAL_HPROCESS           20      // Handle of Debuggee
#define VAL_PROCESSID          21      // Process ID of Debuggee
#define VAL_HMAINTHREAD        22      // Handle of main thread
#define VAL_MAINTHREADID       23      // Thread ID of main thread
#define VAL_MAINBASE           24      // Base of main module in the process
#define VAL_PROCESSNAME        25      // Name of the active process
#define VAL_EXEFILENAME        26      // Name of the main debugged file
#define VAL_CURRENTDIR         27      // Current directory for debugged process
#define VAL_SYSTEMDIR          28      // Windows system directory
#define VAL_DECODEANYIP        29      // Decode registers dependless on EIP
#define VAL_PASCALSTRINGS      30      // Decode Pascal-style string constants
#define VAL_ONLYASCII          31      // Only printable ASCII chars in dump
#define VAL_DIACRITICALS       32      // Allow diacritical symbols in strings
#define VAL_GLOBALSEARCH       33      // Search from the beginning of block
#define VAL_ALIGNEDSEARCH      34      // Search aligned to item's size
#define VAL_IGNORECASE         35      // Ignore case in string search
#define VAL_SEARCHMARGIN       36      // Floating search allows error margin
#define VAL_KEEPSELSIZE        37      // Keep size of hex edit selection
#define VAL_MMXDISPLAY         38      // MMX display mode in dialog
#define VAL_WINDOWFONT         39      // Use calling window's font in dialog
#define VAL_TABSTOPS           40      // Distance between tab stops
#define VAL_MODULES            41      // Table of modules (.EXE and .DLL)
#define VAL_MEMORY             42      // Table of allocated memory blocks
#define VAL_THREADS            43      // Table of active threads
#define VAL_BREAKPOINTS        44      // Table of active breakpoints
#define VAL_REFERENCES         45      // Table with found references
#define VAL_SOURCELIST         46      // Table of source files
#define VAL_WATCHES            47      // Table of watches
#define VAL_CPUFEATURES        50      // CPU feature bits
#define VAL_TRACEFILE          51      // Handle of run trace log file
#define VAL_ALIGNDIALOGS       52      // Whether to align dialogs
#define VAL_CPUDASM            53      // Dump descriptor of CPU Disassembler
#define VAL_CPUDDUMP           54      // Dump descriptor of CPU Dump
#define VAL_CPUDSTACK          55      // Dump descriptor of CPU Stack
#define VAL_APIHELP            56      // Name of selected API help file
#define VAL_HARDBP             57      // Whether hardware breakpoints enabled
#define VAL_PATCHES            58      // Table of patches
#define VAL_HINTS              59      // Sorted data with analysis hints

//////////////////////////////////////////////////////////////////////////

// Reasons why debugged application was paused, as a first argument in call to
// ODBG_Paused(), ODBG_Pausedex() and ODBG_Plugincmd().
#define PP_MAIN                0x0003  // Mask to extract main reason
#define   PP_EVENT             0x0000  // Paused on debugging event
#define   PP_PAUSE             0x0001  // Paused on user's request
#define   PP_TERMINATED        0x0002  // Application terminated
// Extended reasons in ODBG_Pausedex().
#define PP_BYPROGRAM           0x0004  // Debugging event caused by program
#define PP_INT3BREAK           0x0010  // INT3 breakpoint
#define PP_MEMBREAK            0x0020  // Memory breakpoint
#define PP_HWBREAK             0x0040  // Hardware breakpoint
#define PP_SINGLESTEP          0x0080  // Single-step trap
#define PP_EXCEPTION           0x0100  // Exception, like division by 0
#define PP_ACCESS              0x0200  // Access violation
#define PP_GUARDED             0x0400  // Guarded page

//////////////////////////////////////////////////////////////////////////


#define REG_EAX        0               // Indexes of general-purpose registers
#define REG_ECX        1               // in t_reg.
#define REG_EDX        2
#define REG_EBX        3
#define REG_ESP        4
#define REG_EBP        5
#define REG_ESI        6
#define REG_EDI        7

#define SEG_UNDEF     -1
#define SEG_ES         0               // Indexes of segment/selector registers
#define SEG_CS         1               // in t_reg.
#define SEG_SS         2
#define SEG_DS         3
#define SEG_FS         4
#define SEG_GS         5
//////////////////////////////////////////////////////////////////////////

// Please note: Although types here contain mostly unique bit assignments, it's
// not really necessary. Same bits, except for reserved general types, can be
// freely shared between different types of sorted data.
// General item types:
#define TY_NEW         0x00000001      // Item is new
#define TY_CONFIRMED   0x00000002      // Item still exists
#define TY_MAIN        0x00000004      // Main item (thread or module)
#define TY_INVALID     0x00000008      // Invalid type (item does not exist)
#define TY_SELECTED    0x80000000      // Reserved for multiple selection
// Module-specific types:
#define TY_REPORTED    0x00000010      // Stop on module was reported
// Reference-specific types:
#define TY_REFERENCE   0x00000020      // Item is a real reference
#define TY_ORIGIN      0x00000040      // Item is a search origin
// Breakpoint-specific types:
#define TY_STOPAN      0x00000080      // Stop animation if TY_ONESHOT
#define TY_SET         0x00000100      // Code INT3 is in memory
#define TY_ACTIVE      0x00000200      // Permanent breakpoint
#define TY_DISABLED    0x00000400      // Permanent disabled breakpoint
#define TY_ONESHOT     0x00000800      // Temporary stop
#define TY_TEMP        0x00001000      // Temporary breakpoint
#define TY_KEEPCODE    0x00002000      // Set and keep command code
#define TY_KEEPCOND    0x00004000      // Keep condition unchanged (0: remove)
#define TY_NOUPDATE    0x00008000      // Don't redraw breakpoint window
#define TY_RTRACE      0x00010000      // Pseudotype of run trace breakpoint
// Namelist-specific types:
#define TY_EXPORT      0x00010000      // Exported name
#define TY_IMPORT      0x00020000      // Imported name
#define TY_LIBRARY     0x00040000      // Name extracted from object file
#define TY_LABEL       0x00080000      // User-defined name
#define TY_ANYNAME     0x000F0000      // Any of the namelist flags above
#define TY_KNOWN       0x00100000      // Name of known function
// Memory-specific types:
#define TY_DEFHEAP     0x00020000      // Contains default heap
#define TY_HEAP        0x00040000      // Contains non-default heap
#define TY_SFX         0x00080000      // Contains self-extractor
#define TY_CODE        0x00100000      // Contains image of code section
#define TY_DATA        0x00200000      // Contains image of data section
#define TY_IMPDATA     0x00400000      // Memory block includes import data
#define TY_EXPDATA     0x00800000      // Memory block includes export data
#define TY_RSRC        0x01000000      // Memory block includes resources
#define TY_RELOC       0x02000000      // Memory block includes relocation data
#define TY_STACK       0x04000000      // Contains stack of some thread
#define TY_THREAD      0x08000000      // Contains data block of some thread
#define TY_HEADER      0x10000000      // COFF header
#define TY_ANYMEM      0x1FFE0000      // Any of the memory flags above
#define TY_GUARDED     0x20000000      // NT only: guarded memory block
// Procedure data-specific types:
#define TY_PURE        0x00004000      // No side effects except in stack
#define TY_PASCAL      0x00010000      // Procedure ends with RET nnn
#define TY_C           0x00020000      // ADD ESP,nnn after call to procedure
#define TY_NOTENTRY    0x00100000      // Not necessarily entry point
// Switch data-specific types.
#define TY_CHARSW      0x00100000      // ASCII switch
#define TY_WMSW        0x00200000      // Window message switch
#define TY_EXCEPTSW    0x00400000      // Exception switch
// Stack walk data-specific types.
#define TY_RELIABLE    0x01000000      // Reliable call
#define TY_GUESSED     0x02000000      // Not a real entry, just guessed
#define TY_BELONGS     0x04000000      // Not a real entry, just belongs to proc
// Call tree-specific types.
#define TY_RECURSIVE   0x00000100      // Routine calls self
#define TY_TERMINAL    0x00000200      // Leaf function, doesn't call others
#define TY_SYSTEM      0x00000400      // Function resides in system DLL
#define TY_DIRECT      0x00000800      // Called directly
#define TY_NODATA      0x00001000      // Not analyzed or outside procedure
#define TY_DUMMY       0x00002000      // Consists of single RET command
#define TY_NOSIDE      0x00004000      // No side effects except in stack

//////////////////////////////////////////////////////////////////////////

typedef unsigned char  uchar;          // Unsigned character (byte)
typedef unsigned short ushort;         // Unsigned short
typedef unsigned int   uint;           // Unsigned integer
typedef unsigned long  ulong;          // Unsigned long

typedef struct t_reg {                 // Excerpt from context
  int            modified;             // Some regs modified, update context
  int            modifiedbyuser;       // Among modified, some modified by user
  int            singlestep;           // Type of single step, SS_xxx
  ulong          r[8];                 // EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI
  ulong          ip;                   // Instruction pointer (EIP)
  ulong          flags;                // Flags
  int            top;                  // Index of top-of-stack
//  long double    f[8];                 // Float registers, f[top] - top of stack
  BYTE           FloatRegisterArea[SIZE_OF_80387_REGISTERS];
  char           tag[8];               // Float tags (0x3 - empty register)
  ulong          fst;                  // FPU status word
  ulong          fcw;                  // FPU control word
  ulong          s[6];                 // Segment registers ES,CS,SS,DS,FS,GS
  ulong          base[6];              // Segment bases
  ulong          limit[6];             // Segment limits
  char           big[6];               // Default size (0-16, 1-32 bit)
  ulong          dr6;                  // Debug register DR6
  ulong          threadid;             // ID of thread that owns registers
  ulong          lasterror;            // Last thread error or 0xFFFFFFFF
  int            ssevalid;             // Whether SSE registers valid
  int            ssemodified;          // Whether SSE registers modified
  char           ssereg[8][16];        // SSE registers
  ulong          mxcsr;                // SSE control and status register
  int            selected;             // Reports selected register to plugin
  ulong          drlin[4];             // Debug registers DR0..DR3
  ulong          dr7;                  // Debug register DR7
} t_reg;

typedef struct t_thread {              // Information about active threads
  ulong          threadid;             // Thread identifier
  ulong          dummy;                // Always 1
  ulong          type;                 // Service information, TY_xxx
  HANDLE         thread;               // Thread handle
  ulong          datablock;            // Per-thread data block
  ulong          entry;                // Thread entry point
  ulong          stacktop;             // Working variable of Listmemory()
  ulong          stackbottom;          // Working variable of Listmemory()
  CONTEXT        context;              // Actual context of the thread
  t_reg          reg;                  // Actual contents of registers
  int            regvalid;             // Whether reg is valid
  t_reg          oldreg;               // Previous contents of registers
  int            oldregvalid;          // Whether oldreg is valid
  int            suspendcount;         // Suspension count (may be negative)
  long           usertime;             // Time in user mode, 1/10th ms, or -1
  long           systime;              // Time in system mode, 1/10th ms, or -1
  ulong          reserved[16];         // Reserved for future compatibility
} t_thread;

OLLY_API void cdecl Addtolist(long addr,int highlight,char *format, ...);
OLLY_API t_thread* cdecl Findthread(ulong threadid);
OLLY_API ulong cdecl Getcputhreadid(void);

typedef enum t_status {                // Thread/process status
  STAT_NONE=0,                         // Thread/process is empty
  STAT_STOPPED,                        // Thread/process suspended
  STAT_EVENT,                          // Processing debug event, process paused
  STAT_RUNNING,                        // Thread/process running
  STAT_FINISHED,                       // Process finished
  STAT_CLOSING                         // Process is requested to terminate
} t_status;

OLLY_API t_status Getstatus(void);
OLLY_API int Plugingetvalue(int type);

OLLY_API ulong Readmemory(void *buf,ulong addr,ulong size,int mode);
OLLY_API ulong Writememory(void *buf,ulong addr,ulong size,int mode);

//////////////////////////////////////////////////////////////////////////
struct t_module;
typedef int (cdecl* TODBG_Plugindata)(char shortname[32]);

typedef int (cdecl* TODBG_Plugininit)(int ollydbgversion,HWND hw,
										ulong *features);
typedef void (cdecl* TODBG_Pluginmainloop)(DEBUG_EVENT *debugevent);

typedef void (cdecl* TODBG_Pluginsaveudd)(t_module *pmod,int ismainmodule);
typedef int  (cdecl* TODBG_Pluginuddrecord)(t_module *pmod,int ismainmodule,
											 ulong tag,ulong size,void *data);

typedef int  (cdecl* TODBG_Pluginmenu)(int origin,char data[4096],void *item);

typedef void (cdecl* TODBG_Pluginaction)(int origin,int action,void *item);
typedef int  (cdecl* TODBG_Pluginshortcut)(
	int origin,int ctrl,int alt,int shift,int key,
	void *item);

typedef void (cdecl* TODBG_Pluginreset)(void);

typedef int  (cdecl* TODBG_Pluginclose)(void);

typedef void (cdecl* TODBG_Plugindestroy)(void);
typedef int  (cdecl* TODBG_Paused)(int reason,t_reg *reg);
typedef int  (cdecl* TODBG_Pausedex)(int reasonex,int dummy,t_reg *reg,
									  DEBUG_EVENT *debugevent);
typedef int  (cdecl* TODBG_Plugincmd)(int reason,t_reg *reg,char *cmd);

class OllyPort: public DbgEvent {
	OllyPort();
	
public:

	~OllyPort()
	{

	}

	static OllyPort& instance()
	{
		static OllyPort inst;
		return inst;
	}

	bool InitOllyPort();

	t_thread* FindThread(ulong tid)
	{
		Threads::iterator it = m_threads.find(tid);
		if (it == m_threads.end())
			return NULL;

		return &it->second;
	}
	
protected:
	bool InitPlugin(HMODULE hMod, LPCTSTR ModName, HWND hwnd);	
	void RaisePausedEx(DWORD tid, DEBUG_EVENT* DbgEvent);
	void InitOllyThread(DWORD tid, CREATE_THREAD_DEBUG_INFO& info, t_thread& ollythread);
	void ContextToReg(DWORD tid, const CONTEXT& ctx, t_reg& reg);

	//////////////////////////////////////////////////////////////////////////
	
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

protected:

	struct PluginInfo {
		HMODULE					hMod;
		TODBG_Plugindata		ODBG_Plugindata;
		TODBG_Plugininit		ODBG_Plugininit;
		TODBG_Pluginmainloop	ODBG_Pluginmainloop;
		TODBG_Pluginsaveudd		ODBG_Pluginsaveudd;
		TODBG_Pluginuddrecord	ODBG_Pluginuddrecord;
		TODBG_Pluginmenu		ODBG_Pluginmenu;
		TODBG_Pluginaction		ODBG_Pluginaction;
		TODBG_Pluginshortcut	ODBG_Pluginshortcut;
		TODBG_Pluginreset		ODBG_Pluginreset;
		TODBG_Pluginclose		ODBG_Pluginclose;
		TODBG_Plugindestroy		ODBG_Plugindestroy;
		TODBG_Paused			ODBG_Paused;
		TODBG_Pausedex			ODBG_Pausedex;
		TODBG_Plugincmd			ODBG_Plugincmd;
	};

	typedef std::map<std::string, PluginInfo> PluginMap;
	PluginMap		m_pluginMap;

	typedef std::map<ulong, t_thread> Threads;
	Threads			m_threads;
};

#endif // !defined(AFX_OLLYPORT_H__10176EEA_EAA7_4FD1_8F64_FDBC7651F94C__INCLUDED_)
