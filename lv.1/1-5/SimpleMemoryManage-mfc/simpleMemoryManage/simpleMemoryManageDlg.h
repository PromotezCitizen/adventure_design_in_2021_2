
// simpleMemoryManageDlg.h: 헤더 파일
//

#pragma once

#include "pch.h"

// CSimpleMemoryManageDlg 대화 상자
class CSimpleMemoryManageDlg : public CDialogEx
{
// 생성입니다.
public:
	CSimpleMemoryManageDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CSimpleMemoryManageDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLEMEMORYMANAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void initMem();
	void setNode(CHUNK* temp, int start_addr, int return_size);
	void releaseMem();// 모든 메모리 할당 해제

	void myalloc(int req_size);
	void myfree(int start_addr, int return_size);

	void memMerge();

	void bwfitSort();
	void bwfitSwap(CHUNK* temp_1, CHUNK* temp_2);

protected:
	CHUNK* available = NULL;
	int m_memAllocType = 0;
	int m_memAllocTypeSave = 0;

	int m_memAlloc = 0;
	int arr_idx = 0;

	int m_memSetVal = 0;
	
	CString s_memRadioBtn;
	CEdit m_memEdit;
	CEdit m_memSet;
	vector<MEMUSE> memusage;

	void PrintVecOnEdit_free(vector<MEMUSE> vec);
	void PrintVecOnEdit_alloc(vector<MEMUSE> vec);
	void MyFree(vector<MEMUSE> vec)		{ for (const auto& v : vec) myfree(v.start, v.usage); }
	void MyAlloc(vector<MEMUSE> vec)	{ for (const auto& v : vec) myalloc(v.usage); }

public:
	afx_msg void OnMemAllocTypeRadioBtn(UINT value);
	afx_msg void OnMemAllocRadioBtn(UINT value);
	afx_msg void OnResetClicked();
	afx_msg void OnMemsetClicked();
};
