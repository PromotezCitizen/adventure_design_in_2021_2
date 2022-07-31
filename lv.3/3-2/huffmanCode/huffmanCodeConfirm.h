#pragma once
#include "afxdialogex.h"


// ChuffmanCodeConfirm 대화 상자

class ChuffmanCodeConfirm : public CDialogEx
{
	DECLARE_DYNAMIC(ChuffmanCodeConfirm)

public:
	ChuffmanCodeConfirm(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ChuffmanCodeConfirm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIRM_TEXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
