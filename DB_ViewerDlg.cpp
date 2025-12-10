
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
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TYPE, &CDBViewerDlg::OnDeltaposSpinType)
END_MESSAGE_MAP()


// CDBViewerDlg 메시지 처리기

BOOL CDBViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TYPE);
	if (pSpin)
	{
		pSpin->SetRange32(1, 20); 
		pSpin->SetPos32(1);   
		SetDlgItemInt(IDC_EDIT_TYPE, 1);
	}
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	CWnd* pModelWnd = GetDlgItem(IDC_STATIC_MODEL);   // ← 모형 Picture Control ID
	if (pModelWnd)
	{
		InitVtkModelWindow(pModelWnd->GetSafeHwnd());
	}

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	ModifyStyle(WS_THICKFRAME, 0); //창의 크기를 고정하는 방법 대신 테두리 크기 조정을 막아두는 방법 선택 (블로그 참고)
	SetDlgItemInt(IDC_EDIT_TYPE, 1);
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
	CWnd* pCharImgCtrl = GetDlgItem(IDC_STATIC_CHAR_IMAGE);
	if (pCharImgCtrl && !m_charImg.IsNull())
	{
		CDC* pDC = pCharImgCtrl->GetDC();
		CRect rc;
		pCharImgCtrl->GetClientRect(rc);

		m_charImg.StretchBlt(
			pDC->m_hDC,
			0, 0,
			rc.Width(), rc.Height(),
			SRCCOPY
		);

		pCharImgCtrl->ReleaseDC(pDC);
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
		msg.Format(L"CSV 로딩을 성공했습니다.\n\n총 글자 수: %d\n총 장 수: %d",
			m_DB.m_nChar, m_DB.m_nSheet);

		int pos = path.ReverseFind(L'\\');
		if (pos != -1)
			m_strBookFolder = path.Left(pos);

		AfxMessageBox(msg);
		LoadSelectedCharImage();
		LoadSelectedCharSTL();
		LoadBookImage(1);
		m_selectChar = 0;
		UpdateTypeSpinInfo();
		InfoText();
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
				InfoText();
				LoadSelectedCharImage();
				LoadSelectedCharSTL();
				UpdateTypeSpinInfo();
				//Invalidate()를 사용해서 다른 글자를 선택시 초록 상자를 다시 그리기
				Invalidate();
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDBViewerDlg::InfoText()
{
	if (m_DB.m_Chars.IsEmpty())
		return;

	if (m_selectChar < 0 || m_selectChar >= m_DB.m_Chars.GetSize())
		return;

	SCharInfo info = m_DB.m_Chars[m_selectChar];

	CString str;
	str.Format(L"%s\n%d장 %d행 %d번",
		info.m_char,
		info.m_sheet,
		info.m_line,
		info.m_order);

	SetDlgItemText(IDC_STATIC_CHARINFO, str);

}
////////////////////////////////////////////////////////
void CDBViewerDlg::LoadSelectedCharImage()
{
	if (m_DB.m_Chars.IsEmpty())
		return;

	if (m_selectChar < 0 || m_selectChar >= m_DB.m_Chars.GetSize())
		return;

	SCharInfo info = m_DB.m_Chars[m_selectChar];

	// 글자의 개별 이미지가 들어있는 폴더 경로 생성
	CString charFolder;
	charFolder.Format(L"C:\\VTK_TOOL\\03_type\\%s", info.m_char);

	// 그 안에 있는 첫 번째 하위 폴더 찾기
	CString subFolderPath;
	WIN32_FIND_DATA fd;
	HANDLE hFind;

	CString searchPath = charFolder + L"\\*";
	hFind = FindFirstFile(searchPath, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				wcscmp(fd.cFileName, L".") != 0 &&
				wcscmp(fd.cFileName, L"..") != 0)
			{
				subFolderPath = charFolder + L"\\" + fd.cFileName;
				break;
			}
		} while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}

	if (subFolderPath.IsEmpty())
		return;

	// 그 안에 있는 첫 번째 PNG 파일 찾기
	CString imgPath;
	searchPath = subFolderPath + L"\\*.png";
	hFind = FindFirstFile(searchPath, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		imgPath = subFolderPath + L"\\" + fd.cFileName;
		FindClose(hFind);
	}
	else
	{
		return;
	}

	m_charImg.Destroy();
	m_charImg.Load(imgPath);

	Invalidate();  // 화면 갱신
}

void CDBViewerDlg::UpdateCharOrderInfo()
{
	if (m_DB.m_Chars.IsEmpty())
		return;

	if (m_selectChar < 0 || m_selectChar >= m_DB.m_Chars.GetSize())
		return;

	// 현재 선택된 글자
	SCharInfo cur = m_DB.m_Chars[m_selectChar];

	int totalCount = 0;    // 같은 글자 전체 개수
	int curOrder = 0;      // 그중 지금이 몇 번째인지 (1부터 시작)

	for (int i = 0; i < m_DB.m_Chars.GetSize(); i++)
	{
		if (m_DB.m_Chars[i].m_char == cur.m_char)
		{
			totalCount++;

			if (i == m_selectChar)
			{
				curOrder = totalCount;   // 몇 번째인지 기억
			}
		}
	}

	//스핀 컨트롤 범위 설정
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TYPE);
	if (pSpin != nullptr)
	{
		pSpin->SetRange(1, totalCount);
		pSpin->SetPos(curOrder);
	}

	CString strCount;
	strCount.Format(L"/ %d개", totalCount);
	SetDlgItemText(IDC_STATIC_TYPES, strCount);
}

void CDBViewerDlg::UpdateTypeSpinInfo()
{
	if (m_DB.m_Chars.IsEmpty())
		return;

	if (m_selectChar < 0 || m_selectChar >= m_DB.m_Chars.GetSize())
		return;

	CString targetChar = m_DB.m_Chars[m_selectChar].m_char;

	int totalCount = 0;
	int currentOrder = 0;

	for (int i = 0; i < m_DB.m_Chars.GetSize(); i++)
	{
		if (m_DB.m_Chars[i].m_char == targetChar)
		{
			totalCount++;

			if (i == m_selectChar)
				currentOrder = totalCount;
		}
	}

	// 스핀컨트롤의 스핀 범위 설정
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TYPE);
	if (pSpin)
	{
		pSpin->SetRange(1, totalCount);
	}

	// 현재 몇 번째인지 Edit에 표시
	SetDlgItemInt(IDC_EDIT_TYPE, currentOrder);

	CString str;
	str.Format(L"/ %d개", totalCount);
	SetDlgItemText(IDC_STATIC_TYPES, str);
}



//VTK구현부//
void CDBViewerDlg::InitVtkModelWindow(void* hWnd)
{
	// 이미 초기화되어 있으면 다시 안 함
	if (m_vtkModelWindow != nullptr)
		return;

	// 인터랙터 생성 + 트랙볼 스타일
	m_vtkModelInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_vtkModelInteractor->SetInteractorStyle(
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());

	// 렌더러 생성 (배경 밝은 회색)
	m_vtkModelRenderer = vtkSmartPointer<vtkRenderer>::New();
	m_vtkModelRenderer->SetBackground(0.9, 0.9, 0.9);

	// 렌더 윈도우 생성해서 Picture Control HWND에 붙이기
	m_vtkModelWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_vtkModelWindow->SetParentId(hWnd);
	m_vtkModelWindow->SetInteractor(m_vtkModelInteractor);
	m_vtkModelWindow->AddRenderer(m_vtkModelRenderer);
	m_vtkModelWindow->Render();
}

void CDBViewerDlg::LoadSelectedCharSTL()
{
	if (!m_vtkModelWindow || !m_vtkModelRenderer)
		return;

	if (m_DB.m_Chars.IsEmpty())
		return;

	if (m_selectChar < 0 || m_selectChar >= m_DB.m_Chars.GetSize())
		return;

	// 현재 선택된 글자 정보
	SCharInfo info = m_DB.m_Chars[m_selectChar];

	// 스핀에 찍힌 "몇 번째 활자" 값 (1부터 시작)
	UINT nIndex = GetDlgItemInt(IDC_EDIT_TYPE);
	if (nIndex == 0)
		nIndex = 1;

	// STL 파일 경로 만들기
	// 예시: C:\VTK_TOOL\04_3d\110A11A10000_1.stl
	CString stlPath;
	stlPath.Format(L"C:\\VTK_TOOL\\04_3d\\%s_%u.stl",
		info.m_char,
		nIndex);

	// VTK STL 리더로 파일 읽기
	auto reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(CT2A(stlPath));
	reader->Update();

	// 매퍼
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	// 액터
	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	// 이전에 그려진 것들 싹 제거
	m_vtkModelRenderer->RemoveAllViewProps();

	// 새 액터 추가 + 카메라 리셋
	m_vtkModelRenderer->AddActor(actor);
	m_vtkModelRenderer->ResetCamera();

	// 렌더링
	m_vtkModelWindow->Render();
}


void CDBViewerDlg::OnDeltaposSpinType(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	*pResult = 0;

	if (m_DB.m_Chars.IsEmpty())
		return;

	if (m_selectChar < 0 || m_selectChar >= m_DB.m_Chars.GetSize())
		return;

	// 현재 글자
	CString targetChar = m_DB.m_Chars[m_selectChar].m_char;

	// 현재 스핀 값
	int curVal = GetDlgItemInt(IDC_EDIT_TYPE);
	int newVal = curVal - pNMUpDown->iDelta;
	// iDelta : 위로 클릭하면 -1, 아래 클릭하면 +1

	if (newVal < 1)
		newVal = 1;

	// 같은 글자가 전체 몇 개인지 다시 계산
	int totalCount = 0;
	for (int i = 0; i < m_DB.m_Chars.GetSize(); i++)
	{
		if (m_DB.m_Chars[i].m_char == targetChar)
			totalCount++;
	}
	if (totalCount <= 0)
		return;

	if (newVal > totalCount)
		newVal = totalCount;

	// 스핀/에디트에 값 반영
	SetDlgItemInt(IDC_EDIT_TYPE, newVal);

	// newVal 번째 같은 글자를 찾아서 m_selectChar 이동
	int cnt = 0;
	for (int i = 0; i < m_DB.m_Chars.GetSize(); i++)
	{
		if (m_DB.m_Chars[i].m_char == targetChar)
		{
			cnt++;
			if (cnt == newVal)
			{
				m_selectChar = i;
				break;
			}
		}
	}

	// 글자 정보 / PNG / STL / 카운트 전부 갱신
	InfoText();
	LoadSelectedCharImage();
	LoadSelectedCharSTL();
	UpdateTypeSpinInfo();

	// 책 이미지 상자(네모)만 다시 그리기
	Invalidate(FALSE);
}



void CDBViewerDlg::OnItemChangedListChars(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNM = (NM_LISTVIEW*)pNMHDR;

	// 상태 변경이 아닌 경우 : 처리하지 않음
	if (!(pNM->uChanged & LVIF_STATE))
	{
		*pResult = 0;
		return;
	}

	// 선택된 상태가 아닐 경우 : 처리하지 않음
	if (!(pNM->uNewState & LVIS_SELECTED))
	{
		*pResult = 0;
		return;
	}

	int sel = pNM->iItem;

	// 선택된 글자의 이미지 출력
	ShowSelectedComponentImage(sel);

	*pResult = 0;
}


void CDBViewerDlg::ShowSelectedComponentImage(int index)
{
	if (m_compList.IsEmpty())
		return;   //구성 글자 리스트가 비어 있으면 종료

	if (index < 0 || index >= m_compList.GetSize())
		return;   //인덱스 범위 초과시 막아주는 역할

	CString compChar = m_compList[index];

	// 이미지 경로
	CString path;
	path.Format(L"C:\\VTK_TOOL\\03_type\\%s\\1\\%s.png", compChar, compChar);

	m_charImg.Destroy();
	m_charImg.Load(path);

	Invalidate(FALSE);
}


