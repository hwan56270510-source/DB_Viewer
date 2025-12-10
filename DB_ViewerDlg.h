
// DB_ViewerDlg.h: 헤더 파일
//

#pragma once
#include "TypeDB.h"
// SCharInfo 구조체 정의부

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

public:
	CTypeDB m_DB;

protected:
	//월인천강지곡 원본 이미지 
	CImage m_bookImg;
	CImage m_charImg;

	// 현재 선택된 글자 인덱스
	int m_nCurSheet = 1;
	int m_selectChar = 0;

	// typeDB.csv가 있는 폴더의 경로를 저장
	CString m_strBookFolder;

	// 장 번호에 해당하는 책 이미지를 로딩해주는 함수
	void LoadBookImage(int sheet);
	void InfoText();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoadCsv();
	afx_msg void OnStnClickedStaticBook();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void LoadSelectedCharImage();
	afx_msg void UpdateCharOrderInfo();
	afx_msg void UpdateTypeSpinInfo();
	afx_msg void OnDeltaposSpinType(NMHDR* pNMHDR, LRESULT* pResult);

};


