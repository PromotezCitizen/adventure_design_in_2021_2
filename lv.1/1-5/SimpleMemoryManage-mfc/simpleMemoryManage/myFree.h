#pragma once
#include "afxdialogex.h"


// CMyFree 대화 상자

class CMyFree : public CDialogEx
{
	DECLARE_DYNAMIC(CMyFree)

public:
	CMyFree(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyFree();
	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FREE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	vector<MEMUSE> _memusage;
	vector<MEMUSE> _returnval;
	CEdit m_memUsage;
	CEdit m_memDelEdit;

	void printVecOnEdit();

public:
	void setDlgVector(vector<MEMUSE> memusage) { _memusage = memusage; }
	vector<MEMUSE> getDlgVecvtor() { return _memusage; }
	vector<MEMUSE> getMemEraseVector() { return _returnval; }
	afx_msg void OnBnClickedDelMem();
	afx_msg void OnBnClickedCalcle() { EndDialog(IDCANCEL); }
};
