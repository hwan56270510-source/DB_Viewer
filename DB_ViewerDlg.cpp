
// DB_ViewerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DB_Viewer.h"
#include "DB_ViewerDlg.h"
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


// CDBViewerDlg 대화 상자



CDBViewerDlg::CDBViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DB_VIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDBViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDBViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD_CSV, &CDBViewerDlg::OnBnClickedBtnLoadCsv)
	ON_STN_CLICKED(IDC_STATIC_BOOK, &CDBViewerDlg::OnStnClickedStaticBook)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDBViewerDlg 메시지 처리기

BOOL CDBViewerDlg::OnInitDialog()
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

void CDBViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDBViewerDlg::OnPaint()
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

		// 아이콘 생성.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialogEx::OnPaint();

	// 이미지 스케일 확대 출력 
	CWnd* pWnd = GetDlgItem(IDC_STATIC_BOOK);
	if (pWnd && !m_bookImg.IsNull())
	{
		CDC* pDC = pWnd->GetDC();
		CRect rc;
		pWnd->GetClientRect(rc);

		// Picture Control 크기에 맞게 Stretch
		m_bookImg.StretchBlt(
			pDC->m_hDC,
			0, 0,
			rc.Width(), rc.Height(),
			SRCCOPY
		);

		pWnd->ReleaseDC(pDC);
	}
	// 월인천강지곡의 글자에 박스 그리는 부분 구현부
	if (!m_DB.m_Chars.IsEmpty())
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_BOOK);
		if (pWnd && !m_bookImg.IsNull())
		{
			CDC* pDC = pWnd->GetDC();
			CRect rc;
			pWnd->GetClientRect(rc);

			// 이미지 크기를 컨트롤 크기로 비율 맞추는 부분
			float scaleX = (float)rc.Width() / (float)m_bookImg.GetWidth();
			float scaleY = (float)rc.Height() / (float)m_bookImg.GetHeight();

			CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);


			for (int i = 0; i < m_DB.m_Chars.GetSize(); i++)
			{
				if (m_DB.m_Chars[i].m_sheet != m_nCurSheet)
					continue;   // 현재 장이 아니면 넘어감

				int x = (int)(m_DB.m_Chars[i].m_sx * scaleX);
				int y = (int)(m_DB.m_Chars[i].m_sy * scaleY);
				int w = (int)(m_DB.m_Chars[i].m_width * scaleX);
				int h = (int)(m_DB.m_Chars[i].m_height * scaleY);

				// 펜 색/두께 결정
				CPen pen;
				if (i == m_selectChar)
				{
					pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));   // 선택된 글자는 빨강
				}
				else
				{
					pen.CreatePen(PS_SOLID, 1, RGB(0, 200, 0));   // 나머지 글자는 초록 유지
				}

				CPen* pOldPen = pDC->SelectObject(&pen);
				pDC->Rectangle(x, y, x + w, y + h);
				pDC->SelectObject(pOldPen);
			}
			pDC->SelectObject(pOldBrush);
			pWnd->ReleaseDC(pDC);

		}
	}

}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDBViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDBViewerDlg::OnBnClickedBtnLoadCsv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// CSV 파일 선택
	CFileDialog dlg(TRUE, L"csv", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"CSV Files (*.csv)|*.csv||");

	if (dlg.DoModal() != IDOK)
		return;

	// CSV파일 경로 얻어오기
	CString path = dlg.GetPathName();

	// CSV 파일 파싱실행
	if (m_DB.ReadCSVFile(path))
	{
		CString msg;
		msg.Format(L"CSV 로딩을 성공헸습니다.\n\n총 글자 수: %d\n총 장 수: %d",
			m_DB.m_nChar, m_DB.m_nSheet);

		int pos = path.ReverseFind(L'\\');
		if (pos != -1)
			m_strBookFolder = path.Left(pos);

		AfxMessageBox(msg);
		LoadBookImage(1);
		m_selectChar = 0;
		Invalidate();
	}
	else
	{
		AfxMessageBox(L"CSV 파일 로딩 실패");
	}
}


void CDBViewerDlg::LoadBookImage(int sheet)
{
	if (m_strBookFolder.IsEmpty())
		return;   // 폴더 경로가 없는 경우 그냥 리턴

	CString path;
	// 책 폴더\01\001.jpg 이런 식으로 경로 만들기
	path.Format(L"C:\\VTK_TOOL\\01_scan\\%03d.jpg", sheet);

	m_bookImg.Destroy();  
	m_bookImg.Load(path);  
}

void CDBViewerDlg::OnStnClickedStaticBook()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 마우스 왼쪽 버튼 클릭시 글자에 네모칸이 초록색으로 다시 그려지면서
//글자가 선택되었음을 알려주는 기능을 하도록 설계
void CDBViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pBookCtrl = GetDlgItem(IDC_STATIC_BOOK);
	if (pBookCtrl == nullptr)
		return;

	if (m_bookImg.IsNull())
		return;

	if (m_DB.m_Chars.IsEmpty())
		return;

	// Picture Control(IDC_STATIC_BOOK)의 위치영역을 얻어오는 부분
	CRect rcBookCtrl;
	pBookCtrl->GetWindowRect(&rcBookCtrl);
	ScreenToClient(&rcBookCtrl);

	// 마우스로 클릭했을 때 클릭이 책 이미지 내부에 있는지 판별
	if (rcBookCtrl.PtInRect(point) == FALSE)
		return;

	//마우스로 클릭한 좌표는 장치좌표, 이를 논리좌표로 변환
	int localX = point.x - rcBookCtrl.left;
	int localY = point.y - rcBookCtrl.top;

	// 화면 스케일 보정
	float scaleX = (float)rcBookCtrl.Width() / (float)m_bookImg.GetWidth();
	float scaleY = (float)rcBookCtrl.Height() / (float)m_bookImg.GetHeight();

	for (int i = 0; i < m_DB.m_Chars.GetSize(); i++)
	{
		if (m_DB.m_Chars[i].m_sheet != m_nCurSheet)
			continue;

		int x = (int)(m_DB.m_Chars[i].m_sx * scaleX);
		int y = (int)(m_DB.m_Chars[i].m_sy * scaleY);
		int w = (int)(m_DB.m_Chars[i].m_width * scaleX);
		int h = (int)(m_DB.m_Chars[i].m_height * scaleY);

		//클릭이 이 글자 박스 안에 들어왔는지 판정
		if (localX >= x && localX <= x + w)
		{
			if (localY >= y && localY <= y + h)
			{
				m_selectChar = i;

				//Invalidate()를 사용해서 다른 글자를 선택시 초록 상자를 다시 그리기
				Invalidate();
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
