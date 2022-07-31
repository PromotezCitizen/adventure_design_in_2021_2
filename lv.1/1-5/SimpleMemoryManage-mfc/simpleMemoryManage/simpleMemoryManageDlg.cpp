
// simpleMemoryManageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "simpleMemoryManage.h"
#include "simpleMemoryManageDlg.h"
#include "afxdialogex.h"
#include "MyFree.h"
#include "MyAlloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSimpleMemoryManageDlg 대화 상자



CSimpleMemoryManageDlg::CSimpleMemoryManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DLG, pParent)
	, m_memAllocType(0) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSimpleMemoryManageDlg::~CSimpleMemoryManageDlg() {
	releaseMem();
}

void CSimpleMemoryManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MEM_1, m_memAlloc);
	DDX_Radio(pDX, IDC_FIT_1, m_memAllocType);
	DDX_Control(pDX, IDC_MEM_ALLOC_TXT, m_memEdit);
	DDX_Control(pDX, IDC_MEM_DAT, m_memSet);
}

BEGIN_MESSAGE_MAP(CSimpleMemoryManageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_MEM_1, IDC_MEM_2, CSimpleMemoryManageDlg::OnMemAllocRadioBtn)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_FIT_1, IDC_FIT_3, CSimpleMemoryManageDlg::OnMemAllocTypeRadioBtn)
	ON_BN_CLICKED(IDC_RESET_BTN, CSimpleMemoryManageDlg::OnResetClicked)
	ON_BN_CLICKED(IDC_MEM_SET, CSimpleMemoryManageDlg::OnMemsetClicked)
END_MESSAGE_MAP()


// CSimpleMemoryManageDlg 메시지 처리기

BOOL CSimpleMemoryManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	available = NULL;
	m_memAllocType = 0;
	m_memAllocTypeSave = 0;

	m_memAlloc = 0;
	arr_idx = 0;

	m_memSetVal = 0;

	CString str;
	str.Format(_T("! 각 메모리 할당 방식을 선택하면 모든 로그가 초기화됩니다.\r\n\
! 현재 메모리 최대값은 %d입니다.\r\n\
! 메모리 설정은 아래의 버튼으로 가능합니다.\r\n"), m_memSetVal == 0 ? MAX_MEM : m_memSetVal);
	m_memEdit.SetWindowTextW(str);

	initMem();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSimpleMemoryManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSimpleMemoryManageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSimpleMemoryManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimpleMemoryManageDlg::initMem() {
	available = new CHUNK;
	available->link = NULL;
	available->mem_left_size = (m_memSetVal == 0 ? MAX_MEM : m_memSetVal); // 추후에 사용자가 최대 메모리 크기 설정
	available->mem_start = 0;
}

void CSimpleMemoryManageDlg::setNode(CHUNK* temp, int start_addr, int return_size) { // 노드 설정
	temp->link = NULL;
	temp->mem_start = start_addr;
	temp->mem_left_size = return_size;
}

void CSimpleMemoryManageDlg::releaseMem() { // 모든 메모리 할당 해제
	CHUNK* temp = available;
	CHUNK* del = temp;

	while (temp != NULL) {
		temp = temp->link;
		delete del;
		del = temp;
	}
}

void CSimpleMemoryManageDlg::myalloc(int req_size) {
	CHUNK* current = available; // 삭제할 노드
	CHUNK* del_link = NULL; // 삭제할 노드의 이전 노드를 검색
	int return_mem_start;
	while (current != NULL) { // 메모리 할당할 영역을 검색
		if (req_size <= current->mem_left_size) {
			// 조건 : 요청한 크기가 현재 노드의 남은 용량보다 작은 경우
			return_mem_start = current->mem_start;
			current->mem_start += req_size;
			current->mem_left_size -= req_size;
			// 현재 노드의 값을 변경
			if (current->mem_left_size == 0) { // 남은 메모리 용량이 0인 경우 메모리 할당해제 필요
				del_link = available; // 헤드노드부터 검색
				if (current == available) { // 현재 노드가 헤드노드인 경우
					available = current->link;
				}
				else { // 현재 노드가 헤드 노드가 아닌 경우
					while (del_link->link != current) { // 삭제할 노드의 이전 노드를 검색
						del_link = del_link->link;
					}
					del_link->link = current->link;
				}
				delete current;
			}
			// 메모리 시작 주소 반환
			// alloc dlg에서 처리. void로 설정
			return;
		}
		current = current->link;
	}
	// error : 메모리 할당 실패
	// 해당사항 없음. alloc dlg에서 처리.
}

void CSimpleMemoryManageDlg::myfree(int start_addr, int return_size) {
	CHUNK* temp = available;

	if (available != NULL){
		while(temp->link != NULL) temp = temp->link;
		temp->link = new CHUNK;
		temp = temp->link;
	}
	else {
		available = new CHUNK;
		temp = available;
	}
	temp->link = NULL;
	setNode(temp, start_addr, return_size);

	switch (m_memAllocType) {
	case F_FIT:
		memMerge();
		break;
	case B_FIT: // 정렬 없으면 오류 발생
	case W_FIT:
		bwfitSort();
		memMerge();
		bwfitSort();
		break;
	}
}

void CSimpleMemoryManageDlg::memMerge() {	// best, worst fit 메모리 병합
	CHUNK* current;				// 현재 노드
	CHUNK* temp;				// 찾고있는 노드
	CHUNK* head = available;	// 검색의 시작이 되는 노드
	CHUNK* mid_node;			// 찾고있는 노드의 이전을 가리키는 노드

	while (head != NULL) {
		current = head; // head를 직접 건드리지 않겠다는 굳은 의지
		temp = current->link; // 찾고있는 노드 != 현재 노드

		while (temp != NULL) {
			if (current->mem_start + current->mem_left_size == temp->mem_start) {
				// 현재 노드의 메모리 시작주소 + 메모리 남은 용량 == 찾고있는 노드의 메모리 시작주소 ? 병합 : 다음 노드
				current->mem_left_size += temp->mem_left_size; // 현재 노드의 남은 메모리 용량에 찾고있는 노드의 남은 메모리 용량을 더함
				mid_node = current;	// 찾고있는 노드의 이전 노드를 찾는 과정
				while (mid_node->link != temp) {
					mid_node = mid_node->link;
				}
				mid_node->link = temp->link;	// 1→2→3→4 노드가 있을 때 temp node = 3일 경우, mid node = 2
												// mid node->link = temp node->link로 인해 1→2→4로 변경됨
				delete temp; // 찾고있는 노드 메모리 해제
				temp = current->link;
				continue;	// 찾고있는 노드를 현재 노드의 next값으로 변경, while문 계속 진행
			}
			else if (current->mem_start == temp->mem_start + temp->mem_left_size) {
				// 현재노드의 메모리 시작주소 == 찾고있는 노드의; 메모리 시작주소 + 남은 용량 ? 병합 : 다음 노드
				current->mem_start = temp->mem_start;
				current->mem_left_size += temp->mem_left_size;
				current->link = temp->link;
				// current 노드의 메모리 정보를 합병한 내용으로 변경
				delete temp;
				temp = current->link;
				continue;
			}
			temp = temp->link;
		}
		head = head->link;
	}

}

void CSimpleMemoryManageDlg::bwfitSort() {
	CHUNK* temp_1 = available, * temp_2 = NULL;
	// 전체 노드를 순화하며 데이터 swap
	for (; temp_1->link != NULL; temp_1 = temp_1->link) {
		for (temp_2 = temp_1->link; temp_2 != NULL; temp_2 = temp_2->link) {
			if		(m_memAllocType == B_FIT)	if (temp_1->mem_start > temp_2->mem_start) bwfitSwap(temp_1, temp_2);
			else if (m_memAllocType == W_FIT)	if (temp_2->mem_start > temp_1->mem_start) bwfitSwap(temp_1, temp_2);
		}
	}
}

void CSimpleMemoryManageDlg::bwfitSwap(CHUNK* temp_1, CHUNK* temp_2) { // 데이터 swap
	int mem_left_size, mem_start;

	mem_start = temp_1->mem_start;
	temp_1->mem_start = temp_2->mem_start;
	temp_2->mem_start = mem_start;

	mem_left_size = temp_1->mem_left_size;
	temp_1->mem_left_size = temp_2->mem_left_size;
	temp_2->mem_left_size = mem_left_size;
}

void CSimpleMemoryManageDlg::OnMemAllocTypeRadioBtn(UINT value) {
	UpdateData(TRUE);

	if (m_memAllocType != m_memAllocTypeSave){
		int IDC_FIT_ARR[3] = { IDC_FIT_1, IDC_FIT_2, IDC_FIT_3 };


		CButton* pCheck;
		pCheck = (CButton*)GetDlgItem(IDC_FIT_ARR[m_memAllocTypeSave]);
		pCheck->SetCheck(FALSE);

		m_memAllocTypeSave = m_memAllocType;

		releaseMem();
		initMem();
		memusage.clear();

		CString str;
		str.Format(_T("! 각 메모리 할당 방식을 선택하면 모든 로그가 초기화됩니다.\r\n\
! 현재 메모리 최대값은 %d입니다.\r\n\
! 메모리 설정은 아래의 버튼으로 가능합니다.\r\n"), m_memSetVal == 0 ? MAX_MEM : m_memSetVal);
		m_memEdit.SetWindowTextW(str);
	}
}

void CSimpleMemoryManageDlg::OnMemAllocRadioBtn(UINT value) {
	UpdateData(TRUE);
	vector<MEMUSE> memleft;

	if (m_memAlloc == 0) {
		vector<MEMUSE> memAlloced;
		CMyAlloc* dlg = new CMyAlloc;
		CHUNK* temp = available;

		while (temp != NULL) {
			memleft.push_back({ temp->mem_start, temp->mem_left_size });
			temp = temp->link;
		}

		dlg->setDlgFitType(m_memAllocType);
		dlg->setDlgVector(memleft);

		if (dlg->DoModal() == IDOK) {

			memAlloced = dlg->getMemAllocVector();
			PrintVecOnEdit_alloc(dlg->getMemAllocVector());

			memusage.insert(memusage.end(), memAlloced.begin(), memAlloced.end());
			sort(memusage.begin(), memusage.end(), [](MEMUSE& x, MEMUSE& y) { return x.start < y.start; });

			MyAlloc(memAlloced);
		}
	}
	else {
		CMyFree* dlg = new CMyFree();
		dlg->setDlgVector(memusage);

		if (dlg->DoModal() == IDOK) {
			memusage = dlg->getDlgVecvtor();
			PrintVecOnEdit_free(dlg->getMemEraseVector());

			MyFree(dlg->getMemEraseVector());
		}
	}

}
void CSimpleMemoryManageDlg::PrintVecOnEdit_alloc(vector<MEMUSE> vec) {
	CString temp, result;

	m_memEdit.GetWindowTextW(result);

	for (const auto& v : vec) {
		temp.Format(_T("! mem allocated(size %4d)\r\n"), v.usage);
		result += temp;
	}

	m_memEdit.SetWindowTextW(result);
}

void CSimpleMemoryManageDlg::PrintVecOnEdit_free(vector<MEMUSE> vec) {
	CString temp, result;
	
	m_memEdit.GetWindowTextW(result);

	for (const auto& v : vec) {
		temp.Format(_T("! mem deleted at %4d(size %4d)\r\n"), v.start, v.usage);
		result += temp;
	}

	m_memEdit.SetWindowTextW(result);
}

void CSimpleMemoryManageDlg::OnResetClicked() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int IDC_FIT_ARR[3] = { IDC_FIT_1, IDC_FIT_2, IDC_FIT_3 };
	int IDC_MEM_ARR[2] = { IDC_MEM_1, IDC_MEM_2 };

	CButton* pCheck;
	pCheck = (CButton*)GetDlgItem(IDC_FIT_ARR[m_memAllocType]);
	pCheck->SetCheck(FALSE);

	pCheck = (CButton*)GetDlgItem(IDC_MEM_ARR[m_memAlloc]);
	pCheck->SetCheck(FALSE);

	pCheck = (CButton*)GetDlgItem(IDC_FIT_1);
	pCheck->SetCheck(TRUE);

	pCheck = (CButton*)GetDlgItem(IDC_MEM_1);
	pCheck->SetCheck(TRUE);

	m_memAllocType = 0;
	m_memAllocTypeSave = 0;
	m_memAlloc = 0;

	releaseMem();
	initMem();
	memusage.clear();

	CString str;
	str.Format(_T("! 각 메모리 할당 방식을 선택하면 모든 로그가 초기화됩니다.\r\n\
! 현재 메모리 최대값은 %d입니다.\r\n\
! 메모리 설정은 아래의 버튼으로 가능합니다.\r\n"), m_memSetVal == 0 ? MAX_MEM : m_memSetVal);
	m_memEdit.SetWindowTextW(str);
}

void CSimpleMemoryManageDlg::OnMemsetClicked() {
	CString str;
	int setVal;
	m_memSet.GetWindowTextW(str);
	if (str.Compare(_T("")) != 0) {
		setVal = _ttoi(str);

		if (setVal > 0) {
			m_memSetVal = setVal;

			releaseMem();
			initMem();

			CString str;
			str.Format(_T("! 각 메모리 할당 방식을 선택하면 모든 로그가 초기화됩니다.\r\n\
! 현재 메모리 최대값은 %d입니다.\r\n\
! 메모리 설정은 아래의 버튼으로 가능합니다.\r\n"), m_memSetVal == 0 ? MAX_MEM : m_memSetVal);
			m_memEdit.SetWindowTextW(str);
		}
	}
	m_memSet.SetWindowTextW(_T(""));
}