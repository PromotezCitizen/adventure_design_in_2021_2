
// huffmanCodeDlg.h: 헤더 파일
//

#pragma once


// ChuffmanCodeDlg 대화 상자
class ChuffmanCodeDlg : public CDialogEx
{
// 생성입니다.
public:
	ChuffmanCodeDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HUFFMANCODE_DIALOG };
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

protected:
	CString rFilepath, wFilepath;
	BYTE BOM[2];

	DECODE_A* DecodeHead = NULL;
	UINT DecodeDataCnt = 0, DecodeTreeCnt = 0;
	map<BYTE, string> HuffmanMap;
	string WriteString;
	string ReadString;

	BOOL ReadFlag = 0, EncodeFlag = 0;
	CString ZipRate = 0;
	UINT OriginFileSize = 0, ResultFileSize = 0;

	void GetZipRate();


public:
	CString mFilepath;
	afx_msg void OnBnClickedLoadBtn();
	afx_msg void OnBnClickedSaveBtn();
	afx_msg void OnBnClickedCloseBtn();

	void SetText(int, string);

	void EncodeHuffman();
	void SaveEncodeHuffman();

	void writeFILE(int);
	void writeFILEbin(int);
	void writeFILEtxt();

	void DecodeHuffman();
	void SaveDecodeHuffman();

	void decodingFILE(DECODE_A*, int);

	int decodeHuffman2File(FILE*);
	void deleteTREE(DECODE_A*);
};
