
// DB_ViewerDlg.h: 헤더 파일
//

#pragma once
// SCharInfo 구조체 정의부
struct SCharInfo
{
    CString m_char;   // 유니코드 형태의 12자리 CString 타입
    int m_type;
    int m_sheet;
    int m_sx;
    int m_sy;
    int m_line;
    int m_order;
    int m_width;
    int m_height;
};

//CTypeDB 클래스 정의부
class CTypeDB
{
public:
    int m_nSheet;   // 전체 장 수
    int m_nChar;    // 전체 글자 수

    CArray<SCharInfo, SCharInfo&> m_Chars;

    BOOL ReadCSVFile(CString filename); // 객체 구성 성공 : TRUE 리턴// 실패 : FALSE
};

// CDBViewerDlg 대화 상자
class CDBViewerDlg : public CDialogEx
{
// 생성입니다.
public:
	CDBViewerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DB_VIEWER_DIALOG };
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
};
