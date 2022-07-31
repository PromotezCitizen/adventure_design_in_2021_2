// MyFree.cpp: 구현 파일
//

#include "pch.h"
#include "simpleMemoryManage.h"
#include "afxdialogex.h"
#include "MyFree.h"


// CMyFree 대화 상자

IMPLEMENT_DYNAMIC(CMyFree, CDialogEx)

CMyFree::CMyFree(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FREE_DLG, pParent)
{

}

CMyFree::~CMyFree()
{
}

void CMyFree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEM_LEFT_TXT, m_memUsage);
	DDX_Control(pDX, IDC_DEL_EDIT, m_memDelEdit);
}


BEGIN_MESSAGE_MAP(CMyFree, CDialogEx)
	ON_BN_CLICKED(IDC_DEL_MEM, &CMyFree::OnBnClickedDelMem)
	ON_BN_CLICKED(IDCALCLE, &CMyFree::OnBnClickedCalcle)
END_MESSAGE_MAP()


// CMyFree 메시지 처리기

BOOL CMyFree::OnInitDialog() {
	CDialogEx::OnInitDialog();
	printVecOnEdit();
	return TRUE;
}

void CMyFree::OnBnClickedDelMem() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CString temp;
	m_memDelEdit.GetWindowTextW(temp);
	if (temp.Compare(_T("")) == 0) return;

	int delMemStart = _tstoi(temp);

	for (vector<MEMUSE>::iterator it = _memusage.begin(); it < _memusage.end(); it++) {
		if ((*it).start == delMemStart) {
			_returnval.push_back(*it);
			_memusage.erase(it);
			break;
		}
	}

	m_memDelEdit.SetWindowTextW(_T(""));

	printVecOnEdit();
}

void CMyFree::printVecOnEdit() {
	CString temp, str;
	for (const auto& item : _memusage) {
		temp.Format(_T("mem start - %d, mem usage - %d\r\n"), item.start, item.usage);
		str += temp;
	}

	m_memUsage.SetWindowTextW(str);
}
