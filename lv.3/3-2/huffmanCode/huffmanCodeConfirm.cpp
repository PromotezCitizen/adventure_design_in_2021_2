// huffmanCodeConfirm.cpp: 구현 파일
//

#include "pch.h"
#include "huffmanCode.h"
#include "afxdialogex.h"
#include "huffmanCodeConfirm.h"


// ChuffmanCodeConfirm 대화 상자

IMPLEMENT_DYNAMIC(ChuffmanCodeConfirm, CDialogEx)

ChuffmanCodeConfirm::ChuffmanCodeConfirm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONFIRM_TEXT, pParent)
{

}

ChuffmanCodeConfirm::~ChuffmanCodeConfirm()
{
}

void ChuffmanCodeConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChuffmanCodeConfirm, CDialogEx)
END_MESSAGE_MAP()


// ChuffmanCodeConfirm 메시지 처리기
