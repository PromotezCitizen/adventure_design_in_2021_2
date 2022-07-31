// MyAlloc.cpp: 구현 파일
//

#include "pch.h"
#include "simpleMemoryManage.h"
#include "afxdialogex.h"
#include "MyAlloc.h"


// CMyAlloc 대화 상자

IMPLEMENT_DYNAMIC(CMyAlloc, CDialogEx)

CMyAlloc::CMyAlloc(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MALLOC_DLG, pParent) {

}

CMyAlloc::~CMyAlloc() {
}

void CMyAlloc::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALLOC_SIZE, m_memAllocSize);
	DDX_Control(pDX, IDC_MEM_LEFT_TXT2, m_memEdit);
}

BEGIN_MESSAGE_MAP(CMyAlloc, CDialogEx)
	ON_BN_CLICKED(IDCALCLE, &CMyAlloc::OnBnClickedCalcle)
	ON_BN_CLICKED(IDC_ADD_MEM, &CMyAlloc::OnBnClickedAddMem)
END_MESSAGE_MAP()


// CMyAlloc 메시지 처리기
BOOL CMyAlloc::OnInitDialog() {
	CDialogEx::OnInitDialog();
	printVecOnEdit();
	return TRUE;
}

void CMyAlloc::OnBnClickedAddMem() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString memAllocSize;
	vector<MEMUSE> memleft;
	BOOL saveDatFlag = FALSE;
	int memsize;
	m_memAllocSize.GetWindowTextW(memAllocSize);

	if ((memAllocSize.Compare(_T("")) != 0)) {
		memsize = _tstoi(memAllocSize);
		memleft = _memleft;

		if		(_fit == B_FIT) sort(memleft.begin(), memleft.end(), [](MEMUSE& x, MEMUSE& y) { return x.start < y.start; });
		else if (_fit == W_FIT) sort(memleft.begin(), memleft.end(), [](MEMUSE& x, MEMUSE& y) { return x.start > y.start; });

		for (auto& t : memleft) {
			if (t.usage < memsize) continue;
			else {
				for (vector<MEMUSE>::iterator it = _memleft.begin(); it < _memleft.end(); it++) {
					if ((*it).start == t.start) {
						(*it).start += memsize;
						(*it).usage -= memsize;
						if ((*it).usage == 0) {
							_memleft.erase(it);
							break;
						}
						break;
					}
				}

				_returnval.push_back({ t.start, memsize });

				t.start += memsize;
				t.usage -= memsize;
				saveDatFlag = TRUE;
				break;
			}
		}
		if (!saveDatFlag) MessageBox(_T("메모리 공간이 부족합니다."));
	}

	m_memAllocSize.SetWindowTextW(_T(""));

	printVecOnEdit();
}

void CMyAlloc::printVecOnEdit() {
	CString temp, str;
	temp.Format(_T("메모리 추가 조건 : %s\r\n메모리 사용 가능한 범위\r\n"), (_fit == 0) ? _T("first fit") : ((_fit == 1) ? _T("best fit") : _T("worst fit")));
	str += temp;
	for (const auto& item : _memleft) {
		temp.Format(_T("[%d ~ %d]\r\n"), item.start, item.start + item.usage);
		str += temp;
	}

	m_memEdit.SetWindowTextW(str);
}