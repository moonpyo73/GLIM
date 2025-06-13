
// GlimCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GlimCircleApp.h"
#include "GlimCircleDlg.h"
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


// CGlimCircleDlg 대화 상자



CGlimCircleDlg::CGlimCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIMCIRCLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCircleWnd = nullptr;
}

CGlimCircleDlg::~CGlimCircleDlg()
{
	if (m_pCircleWnd != nullptr)
	{
		delete m_pCircleWnd;
		m_pCircleWnd = nullptr;
	}
}

void CGlimCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGlimCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CGlimCircleDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CGlimCircleDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_RAND, &CGlimCircleDlg::OnBnClickedButtonRand)
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_CIRCLE_RANDOM, &CGlimCircleDlg::OnCircleRandom)
END_MESSAGE_MAP()


// CGlimCircleDlg 메시지 처리기

BOOL CGlimCircleDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_RAND)->EnableWindow(FALSE);

	m_pCircleWnd = new CircleWnd();
	if (!m_pCircleWnd->Create(NULL, NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("CircleWnd 생성 실패 \n");
		return FALSE; // fail to create
	}

	CRect rect;
	GetClientRect(&rect);
	rect.top += CIRCLE_TOP;
	rect.left += CIRCLE_LEFT;
	rect.right -= CIRCLE_RIGHT;
	rect.bottom -= CIRCLE_BOTTOM;
	m_pCircleWnd->SetWndDisplayPoints(GetDlgItem(IDC_STATIC_POINT1), GetDlgItem(IDC_STATIC_POINT2), GetDlgItem(IDC_STATIC_POINT3));
	m_pCircleWnd->MoveWindow(&rect);
	m_pCircleWnd->ShowWindow(SW_SHOW);
	m_pCircleWnd->InitCircle();

	CString strRadius;
	strRadius.Format(_T("%.1f"), m_pCircleWnd->GetPointRadius());
	GetDlgItem(IDC_EDIT_RADIUS)->SetWindowText(strRadius);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGlimCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGlimCircleDlg::OnPaint()
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
HCURSOR CGlimCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGlimCircleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_pCircleWnd != nullptr)
	{
		CRect rect;
		GetClientRect(&rect);
		rect.top += CIRCLE_TOP;
		rect.left += CIRCLE_LEFT;
		rect.right -= CIRCLE_RIGHT;
		rect.bottom -= CIRCLE_BOTTOM;
		m_pCircleWnd->MoveWindow(&rect);
	}
}

void CGlimCircleDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{// 윈도우 최소 크기 설정
	lpMMI->ptMinTrackSize.x = 600; // 최소 너비
	lpMMI->ptMinTrackSize.y = 400; // 최소 높이
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CGlimCircleDlg::OnBnClickedButtonApply()
{
	CString strRadius;
	GetDlgItem(IDC_EDIT_RADIUS)->GetWindowText(strRadius);
	float radius = (float) _ttof(strRadius);
	if (strRadius.IsEmpty() || radius <= 0.0F)
	{
		AfxMessageBox(_T("반지름을 입력하세요."));
		GetDlgItem(IDC_EDIT_RADIUS)->SetFocus();
		return;
	}
	m_pCircleWnd->SetPointRadius(radius);
}

void CGlimCircleDlg::OnBnClickedButtonInit()
{
	if (m_pCircleWnd != nullptr)
	{
		m_pCircleWnd->InitCircle();
		RedrawWindow();
	}
}

void CGlimCircleDlg::OnBnClickedButtonRand()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_RAND);
	CString strText;
	pButton->GetWindowText(strText);
	if (strText == _T("랜덤 이동"))
	{
		if(m_pCircleWnd->StartRandomCircle() == false)
		{
			AfxMessageBox(_T("랜덤 이동을 시작할 수 없습니다."));
			return;
		}
	}
	else
	{
		m_pCircleWnd->StopRandomCircle();
	}
}

afx_msg LRESULT CGlimCircleDlg::OnCircleRandom(WPARAM wParam, LPARAM lParam)
{
	CButton* pBtnInit = (CButton*)GetDlgItem(IDC_BUTTON_INIT);
	CButton* pBtnRandom = (CButton*)GetDlgItem(IDC_BUTTON_RAND);
	switch (wParam)
	{
	default:
	case RANDOM_DISABLE:
		pBtnRandom->EnableWindow(FALSE);	// 랜덤 버튼 비활성화
		pBtnRandom->SetWindowText(_T("랜덤 이동"));
		break;
	case RANDOM_ENABLE:
		pBtnRandom->EnableWindow(TRUE);	// 랜덤 버튼 활성화
		pBtnRandom->SetWindowText(_T("랜덤 이동"));
		break;
	case RANDOM_START:
		pBtnInit->EnableWindow(FALSE);	// 초기화 버튼 비활성화
		pBtnRandom->SetWindowText(_T("랜덤 중지"));
		break;
	case RANDOM_STOP:
		pBtnInit->EnableWindow(TRUE);	// 초기화 버튼 활성화
		pBtnRandom->SetWindowText(_T("랜덤 이동"));
		break;
	}
	return 0;
}
