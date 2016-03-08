#if !defined(AFX_PROCESSDLG_H__C63CF255_83B3_4206_B5FD_44E3FF3B49E3__INCLUDED_)
#define AFX_PROCESSDLG_H__C63CF255_83B3_4206_B5FD_44E3FF3B49E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessDlg.h : header file
//

#define __countof(ARR)			(sizeof(ARR) / sizeof(ARR[0]))
#include <Tlhelp32.h>

class DataProv {
public:
	virtual bool GetFirstData(DWORD& id, CString& name) = 0;
	virtual bool GetNextData(DWORD& id, CString& name) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg dialog

class CProcessDlg : public CDialog
{
// Construction
public:
	CProcessDlg(DataProv* dataPrev, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessDlg)
	enum { IDD = IDD_PROCESS };
	CListBox	m_listProcess;
	//}}AFX_DATA

	bool		m_idhex;
	DWORD		m_dwSel;
	CString		m_strSel;
	CString		m_title;
	bool		m_multisel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	DataProv*	m_dataPrev;
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessDlg)
	virtual void OnOK();
	afx_msg void OnRefresh();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:

	afx_msg void OnLbnDblclkListProcess();
	afx_msg int OnVKeyToItem( 
		UINT nKey, 
		CListBox* pListBox, 
		UINT nIndex  
		);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

//////////////////////////////////////////////////////////////////////////

class ProcDataProv: public DataProv {
public:
	ProcDataProv()
	{
		m_hSnapshot = NULL;
	}

	~ProcDataProv()
	{

	}

	virtual bool GetFirstData(DWORD& id, CString& name)
	{
		m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);

		if (!::Process32First(m_hSnapshot, &pe))
			return false;

		id = pe.th32ProcessID;
		name = pe.szExeFile;
		return true;
	}

	virtual bool GetNextData(DWORD& id, CString& name)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);

		if (!::Process32Next(m_hSnapshot, &pe)) {

			CloseHandle(m_hSnapshot);
			return false;
		}

		id = pe.th32ProcessID;
		name = pe.szExeFile;
		return true;        
	}

protected:
	HANDLE			m_hSnapshot;
};

//////////////////////////////////////////////////////////////////////////

class MemDataProv: public DataProv {
public:
	MemDataProv(HANDLE hProcess): m_hProcess(hProcess)
	{
		
	}

	~MemDataProv()
	{

	}

	struct MemInfo: MEMORY_BASIC_INFORMATION {

		TCHAR		comment[MAX_PATH];
	};

	struct IdNamePair {
		DWORD	id;
		TCHAR	name[256];
	};

	CString MergeIdName(IdNamePair idNames[], size_t n, DWORD id)
	{
		CString tmp, r;
		for (size_t i = 0; i < n; i ++) {
			if (id & idNames[i].id) {
				tmp.Format(_T("[%s]"), idNames[i].name);
				r += tmp;
			}
		}

		return r;
	}

	CString GetMemTypeName(DWORD type)
	{
		IdNamePair types[] = {
			{ MEM_IMAGE, _T("I") }, 
			{ MEM_MAPPED, _T("M") }, 
			{ MEM_PRIVATE, _T("P") }, 
		};

		return MergeIdName(types, __countof(types), type);
	}

	CString GetMemProtName(DWORD prot)
	{
		IdNamePair prots[] = {
			{ PAGE_NOACCESS, _T("A") }, 
			{ PAGE_READONLY, _T("R") },          
			{ PAGE_READWRITE, _T("RW") }, 
			{ PAGE_WRITECOPY, _T("C") }, 
			{ PAGE_EXECUTE, _T("E") }, 
			{ PAGE_EXECUTE_READ , _T("ER") }, 
			{ PAGE_EXECUTE_READWRITE, _T("ERW") }, 
			{ PAGE_EXECUTE_WRITECOPY, _T("EC") }, 
			{ PAGE_GUARD, _T("G") }, 
			{ PAGE_NOCACHE, _T("N") }, 
			// { PAGE_WRITECOMBINE , _T("") }, 
		};

		return MergeIdName(prots, __countof(prots), prot);
	}

	void GetMemComment(MemInfo& memInfo)
	{
		DWORD r = GetModuleFileNameEx(m_hProcess, (HMODULE )memInfo.BaseAddress, 
			memInfo.comment, sizeof(memInfo.comment));
		if (!r)
			memInfo.comment[0] = 0;
	}

	bool GetData(DWORD& id, CString& name)
	{
		m_memInfoVec.push_back(MemInfo());
		MemInfo& memInfo = m_memInfoVec[m_num];

		while (m_base < 0x80000000) {

			SIZE_T r = VirtualQueryEx(m_hProcess, (PVOID )m_base, &memInfo, 
				sizeof(MEMORY_BASIC_INFORMATION));

			if (r != sizeof(MEMORY_BASIC_INFORMATION)) {
				return false;
			}

			if (memInfo.State != MEM_COMMIT) {
				m_base += memInfo.RegionSize;
				continue;
			}

			id = m_num + 1;
			CString type = GetMemTypeName(memInfo.Type);
			CString prot = GetMemProtName(memInfo.Protect);
			GetMemComment(memInfo);
            
			name.Format(_T("%p - %p  %s  %s  %s"), memInfo.BaseAddress, 
				((ULONG_PTR )memInfo.BaseAddress) + memInfo.RegionSize, 
				(LPCTSTR )type, (LPCTSTR )prot, (LPCTSTR )memInfo.comment);

			m_base += memInfo.RegionSize;
			m_num ++;
			return true;
		}

		return false;
	}

	virtual bool GetFirstData(DWORD& id, CString& name)
	{
		m_base = 0;
		m_num = 0;

		return GetData(id, name);
	}

	virtual bool GetNextData(DWORD& id, CString& name)
	{
		return GetData(id, name);
	}

	typedef std::vector<MemInfo> MemInfoVec;

	MemInfoVec		m_memInfoVec;

protected:
	HANDLE			m_hProcess;
	ULONG_PTR		m_base;
	int				m_num;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSDLG_H__C63CF255_83B3_4206_B5FD_44E3FF3B49E3__INCLUDED_)
