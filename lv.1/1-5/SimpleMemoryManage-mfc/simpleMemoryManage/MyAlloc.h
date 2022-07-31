#pragma once
#include "afxdialogex.h"
#include <algorithm>

// CMyAlloc 대화 상자

class CMyAlloc : public CDialogEx
{
	DECLARE_DYNAMIC(CMyAlloc)

public:
	CMyAlloc(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyAlloc();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MALLOC_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	CEdit m_memAllocLoc;
	CEdit m_memAllocSize;
	CEdit m_memEdit;
	afx_msg void OnBnClickedCalcle() { EndDialog(IDCANCEL); }

	vector<MEMUSE> _memleft;
	vector<MEMUSE> _returnval;
	int _fit = 0;

	void printVecOnEdit();

public:
	void setDlgVector(vector<MEMUSE> memleft) { _memleft = memleft; }
	void setDlgFitType(int fit) { _fit = fit; }
	vector<MEMUSE> getDlgVecvtor() { return _memleft; }
	vector<MEMUSE> getMemAllocVector() { return _returnval; }
	afx_msg void OnBnClickedAddMem();
};
