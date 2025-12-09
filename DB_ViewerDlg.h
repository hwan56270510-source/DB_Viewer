
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
