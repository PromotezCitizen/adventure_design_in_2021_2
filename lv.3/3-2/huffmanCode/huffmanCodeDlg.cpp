
// huffmanCodeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "huffmanCode.h"
#include "huffmanCodeDlg.h"
#include "afxdialogex.h"

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


// ChuffmanCodeDlg 대화 상자



ChuffmanCodeDlg::ChuffmanCodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HUFFMANCODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	BOM[0] = 0; BOM[1] = 0;
	ReadFlag = FALSE;
	EncodeFlag = FALSE;
	DecodeHead = NULL;
	OriginFileSize = 0;
	ResultFileSize = 0;
}

void ChuffmanCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, mFilepath);
}

BEGIN_MESSAGE_MAP(ChuffmanCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_BTN, &ChuffmanCodeDlg::OnBnClickedLoadBtn)
	ON_BN_CLICKED(IDC_CLOSE_BTN, &ChuffmanCodeDlg::OnBnClickedCloseBtn)
	ON_BN_CLICKED(IDC_SAVE_BTN, &ChuffmanCodeDlg::OnBnClickedSaveBtn)
END_MESSAGE_MAP()


// ChuffmanCodeDlg 메시지 처리기

BOOL ChuffmanCodeDlg::OnInitDialog()
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void ChuffmanCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void ChuffmanCodeDlg::OnPaint()
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
HCURSOR ChuffmanCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ChuffmanCodeDlg::OnBnClickedLoadBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CFileDialog dlg(
		TRUE,
		NULL,
		NULL,
		OFN_OVERWRITEPROMPT,
		L"all file(*.*) | *.*||",
		this
	);

	if (dlg.DoModal() == IDOK) {
		HuffmanMap.clear();
		SetDlgItemText(IDC_STATIC1, L"");

		rFilepath = dlg.GetPathName();
		mFilepath = rFilepath;

		FILE* file = fopen((CStringA)rFilepath, "r");
		if (file != NULL){
			fread(BOM, sizeof(BYTE), 2, file);
			fclose(file);
			if (BOM[0] + BOM[1] < 509) {
				BOM[0] = 0; BOM[1] = 0;
			}

			if (BOM[0] + BOM[1] == 510)	DecodeHuffman();
			else						EncodeHuffman();

			ReadFlag = TRUE;
		}

		SetDlgItemText(IDC_OUTPUT_TXT, _T(""));
		SetDlgItemText(IDC_HUFFMAN_BIT, _T(""));
		SetText(IDC_INPUT_TXT, "");
	}

	UpdateData(FALSE);
}

void ChuffmanCodeDlg::SetText(int nID, string result = "") {
	CString str, result_str("");
	ULONGLONG fileEnd;
	BYTE* data;
	int i = 0;

	if (result.length() == 0) {
		CFile file;
		file.Open(rFilepath, CFile::modeRead);

		fileEnd = file.SeekToEnd();
		file.SeekToBegin();

		data = new BYTE[fileEnd];
		file.Read(data, fileEnd * sizeof(BYTE));

		file.Close();
	}
	else {
		fileEnd = result.length();
		data = new BYTE[fileEnd];
		for (int i = 0; i < fileEnd; i++) {
			data[i] = result.c_str()[i];
		}
	}
	WCHAR unicode = 0;
	UINT flag_UTF16 = data[0] + data[1];

	if (data[0] + data[1] == 0xFF + 0xFE) i = 2;
	else if ((data[0] == 0xEF) && (data[1] == 0xBB) && (data[2] == 0xBF)) i = 3;

	setlocale(LC_ALL, "ko-KR");

	for (i; i < fileEnd;) {
		if (flag_UTF16 == 509) { // UTF-16
			unicode = UTF_16(data, i);
			i += 2;
		}
		else if ((data[i] & 0xE0) == 0xE0 && (data[i + 1] & 0x80) == 0x80 && (data[i + 2] & 0x80) == 0x80) { // UTF-8
			unicode = UTF_8_3BYTE(data, i);
			i += 3;
		}
		else if ((data[i] & 0xC0) == 0xC0 && (data[i + 1] & 0x80) == 0x80) { // UTF-8
			unicode = UTF_8_2BYTE(data, i);
			i += 2;
		}
		//else if ((0xA1 <= (data[i] & 0xFF) && (data[i] & 0xFF) <= 0xFE) && (0xA1 <= (data[i + 1] & 0xFF) && (data[i + 1] & 0xFF) <= 0xFE)) { // ansi
		//	ansi의 경우 오류난다...
		//  ansi는 일단 미지원

		//	char temp_a[2];

		//	asdf[0] = data[i] & 0xFF;
		//	asdf[0] = asdf[0] << 8;
		//	asdf[1] = asdf[0] + data[i + 1];
		//	asdf[0] = asdf[0] + asdf[1];

		//	temp_a[0] = data[i];
		//	temp_a[1] = data[i+1];

		//	//temp = asdf[0];

		//	MultiByteToWideChar(CP_ACP, 0, temp_a, -1, &temp, sizeof(temp));

		//	i += 2;
		//}
		else { // ASCII
			unicode = data[i];
			i += 1;
		}

		str = unicode;
		result_str += str;
	}

	SetDlgItemText(nID, result_str);
}

void ChuffmanCodeDlg::OnBnClickedSaveBtn(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (ReadFlag == TRUE){
		ReadFlag = FALSE;
		if (EncodeFlag == TRUE) SaveEncodeHuffman(); 
		else					SaveDecodeHuffman();

		GetZipRate();
		deleteTREE(DecodeHead);
		HuffmanMap.clear();
	}
}

void ChuffmanCodeDlg::deleteTREE(DECODE_A* tree) {
	if (tree != NULL) {
		deleteTREE(tree->left_node);
		deleteTREE(tree->right_node);
		delete tree;
	}
}

void ChuffmanCodeDlg::OnBnClickedCloseBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(0);
}


void ChuffmanCodeDlg::EncodeHuffman() {

	map<USHORT, UINT> HuffmanFreq;
	priority_queue<TREE_A, vector<TREE_A>, comp_a> pq;

	FILE* file = fopen((CStringA)rFilepath, "r+b");
	BYTE index = 0;

	if (file == NULL) {
		MessageBox(L"해당 파일이 없습니다");
		return;
	}

	EncodeFlag = TRUE;

	while (fscanf(file, "%c", &index) != EOF) {
		HuffmanFreq[index]++;
	}
	appendPq(pq, HuffmanFreq);

	TREE_A* parent = new TREE_A;
	while (pq.size() >= 2) { appendFreqTree(pq, parent); }

	makeHuffmanMap(parent, HuffmanMap);

	deleteFreqTree(parent);

	fclose(file);
}

void ChuffmanCodeDlg::SaveEncodeHuffman() {
	CFileDialog dlg(
		FALSE,
		NULL,
		NULL,
		OFN_OVERWRITEPROMPT,
		L"huffman data(*.huf) | *.huf||",
		this
	);

	if (dlg.DoModal() == IDOK){
		wFilepath = dlg.GetPathName();

		FILE* file = fopen((CStringA)rFilepath, "r+b"); // 파일 읽기 위해
		int huff_size = 0; // 참조형
		WriteString = encodeFile2Huffman(file, huff_size, HuffmanMap);
		fclose(file);

		writeFILE(huff_size);

		MessageBox(L"파일 부호화를 완료했습니다.");
	}
}

void ChuffmanCodeDlg::writeFILE(int num_char) {
	writeFILEbin(num_char);
	writeFILEtxt();
}


void ChuffmanCodeDlg::writeFILEbin(int num_char) {
	FILE* wfile = fopen((CStringA)wFilepath, "w+b");

	UINT buffer = 0;
	UCHAR len = 0;
	UINT huff_size = HuffmanMap.size();


	BYTE iter = 0xFF;
	fwrite(&iter, sizeof(BYTE), 1, wfile);
	fwrite(&iter, sizeof(BYTE), 1, wfile);

	fwrite(BOM, sizeof(BYTE), 2, wfile);

	fwrite(&huff_size, sizeof(int), 1, wfile);
	fwrite(&num_char, sizeof(int), 1, wfile);

	string HuffmanMapBitset;

	for (auto e : HuffmanMap) {
		len = e.second.length();

		for (int idx = 0; idx < len; idx++) { // 길이 31까지만 가능
			if (e.second[idx] == '1') buffer |= 1;
			buffer = buffer << 1;
		}
		buffer = buffer >> 1;

		fwrite(&e.first, sizeof(UCHAR), 1, wfile);
		fwrite(&len, sizeof(UCHAR), 1, wfile);
		fwrite(&buffer, sizeof(UINT), 1, wfile);

		bitset<8> ef(e.first);
		bitset<8> l(len);
		HuffmanMapBitset += ef.to_string() + "|";
		HuffmanMapBitset += l.to_string() + "|";
		HuffmanMapBitset += e.second + "\r\n";

		buffer = 0;
	}

	HuffmanMapBitset.erase(HuffmanMapBitset.length() - 2, HuffmanMapBitset.length());;

	CString HuffmanStr;
	HuffmanStr = HuffmanMapBitset.c_str();

	SetDlgItemText(IDC_HUFFMAN_BIT, HuffmanStr);

	fclose(wfile);
}

void ChuffmanCodeDlg::writeFILEtxt() {
	FILE* wfile = fopen((CStringA)wFilepath, "a+b");
	BYTE buffer = 0;

	CString HuffmanString;
	HuffmanString = WriteString.c_str();
	SetDlgItemText(IDC_OUTPUT_TXT, HuffmanString);

	for (UINT idx = 0; idx < WriteString.length(); idx++) {
		if (WriteString[idx] == '1') buffer |= 1;

		if (idx % 8 == 7) {
			fwrite(&buffer, sizeof(BYTE), 1, wfile);
			buffer = 0;
		}
		buffer = buffer << 1;
	}

	BYTE temp = 7 - (WriteString.length() % 8);
	buffer = buffer << temp;
	if (temp != 7) fwrite(&buffer, sizeof(BYTE), 1, wfile);

	fclose(wfile);
}

/* =============================================================== */	

void ChuffmanCodeDlg::DecodeHuffman() {
	string ss("");
	FILE* file = fopen((CStringA)rFilepath, "r+b");

	if (file == NULL) {
		MessageBox(L"해당 파일이 없습니다");
		return;
	}

	HuffmanMap.clear();

	EncodeFlag = FALSE;
	DecodeDataCnt = decodeHuffman2File(file);

	if (DecodeDataCnt > 0) {
		DecodeHead = new DECODE_A;
		decodingTREE(DecodeHead, HuffmanMap);
	}

	fclose(file);
}

int ChuffmanCodeDlg::decodeHuffman2File(FILE* file) {
	string ss = "";
	UCHAR id, len;
	UINT data;
	int cnt_encoded, cnt_numchar;

	BYTE iter;
	fread(&iter, sizeof(BYTE), 1, file);
	fread(&iter, sizeof(BYTE), 1, file);

	fread(BOM, sizeof(BYTE), 2, file);

	fread(&cnt_encoded, sizeof(int), 1, file);
	fread(&cnt_numchar, sizeof(int), 1, file);

	DecodeTreeCnt = cnt_encoded;

	for (int idx = 0; idx < cnt_encoded; idx++) {
		fread(&id, sizeof(UCHAR), 1, file);
		fread(&len, sizeof(UCHAR), 1, file);
		fread(&data, sizeof(int), 1, file);

		bitset<32>bs(data); // bit연산으로 원래 허프만코드 복호화

		for (int i = 0; i < len; i++) {
			if (bs[0]) ss = "1" + ss;
			else ss = "0" + ss;
			bs = bs >> 1;
		}

		HuffmanMap[id] = ss;
		ss = "";
	}

	return cnt_numchar;
}

void ChuffmanCodeDlg::SaveDecodeHuffman() {

	if (DecodeDataCnt > 0){
		decodingFILE(DecodeHead, DecodeDataCnt);
	}
}

void ChuffmanCodeDlg::decodingFILE(DECODE_A* tree, int cnt) {
	CFileDialog dlg(
		FALSE,
		NULL,
		NULL,
		OFN_OVERWRITEPROMPT,
		L"All files(*.*) | *.*||",
		this
	);

	if (dlg.DoModal() == IDOK) {
		wFilepath = dlg.GetPathName();

		FILE* rfile = fopen((CStringA)rFilepath, "r+b");
		FILE* wfile = fopen((CStringA)wFilepath, "w+b");

		BYTE data;
		DECODE_A* temp = tree;

		string as;

		// sizeof(BYTE)*2 : 구분자
		// sizeof(BYTE)*2 : BOM
		// sizeof(int)*2 : 허프만 사이즈 + 글자 수
		// DecodeTreeCnt*6 : 저장된 헤더
		//fseek(rfile, sizeof(BYTE)*2, SEEK_SET);
		fseek(rfile, sizeof(BYTE) * 2 + sizeof(BYTE) * 2 + sizeof(int) * 2 + DecodeTreeCnt * 6, SEEK_SET);


		for (int i = 0; i < cnt; ) {
			if (i == cnt) break;
			fread(&data, sizeof(BYTE), 1, rfile);
			bitset<8> bs(data);

			for (int bit = 7; bit > -1; bit--) {
				if (bs[bit] == 1) {
					if (temp->right_node == NULL) {
						ReadString += temp->data;
						fwrite(&temp->data, sizeof(BYTE), 1, wfile);
						temp = tree->right_node;
						if (++i == cnt) break;
						continue;
					}
					temp = temp->right_node;
				}
				else {
					if (temp->left_node == NULL) {
						ReadString += temp->data;
						fwrite(&temp->data, sizeof(BYTE), 1, wfile);
						temp = tree->left_node;
						if (++i == cnt) break;
						continue;
					}
					temp = temp->left_node;
				}
			}
		}
		fclose(rfile);
		fclose(wfile);

		SetDlgItemText(IDC_HUFFMAN_BIT, _T(""));
		SetText(IDC_OUTPUT_TXT, ReadString);

		MessageBox(L"파일 복호화를 완료했습니다.");
	}
}


void ChuffmanCodeDlg::GetZipRate() {
	FILE* ofile = fopen((CStringA)rFilepath, "r+b");
	FILE* rfile = fopen((CStringA)wFilepath, "r+b");

	fseek(ofile, 0, SEEK_END);
	fseek(rfile, 0, SEEK_END);

	OriginFileSize = ftell(ofile);
	ResultFileSize = ftell(rfile);

	ZipRate.Format(_T("압축비는 1 : %0.6f 입니다."), (double)ResultFileSize / OriginFileSize);

	SetDlgItemText(IDC_STATIC1, ZipRate);

	fclose(ofile);
	fclose(rfile);
}