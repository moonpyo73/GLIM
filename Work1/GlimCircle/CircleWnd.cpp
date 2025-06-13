// CircleWnd.cpp: 구현 파일
//

#include "pch.h"
#include "GlimCircleApp.h"
#include "CircleWnd.h"

// CircleWnd

IMPLEMENT_DYNAMIC(CircleWnd, CWnd)

CircleWnd::CircleWnd()  
	: m_nCircleMode(0) // 초기화: 0 - 클릭, 1 - 랜덤  
	, m_bDrag(false) // 초기화: 드래그 상태를 false로 설정  
	, m_GlimCircle() // GlimCircle 객체 초기화  
	, m_bThreadRun(false)  
	, m_nCircleCount(0)  
	, m_pThread(nullptr)  
	, m_nClickCount(0)  
	, m_nMovePos(-1)  
	, m_pDisplayPoint{ nullptr, nullptr, nullptr }  
{  
}

CircleWnd::~CircleWnd()
{
}

BEGIN_MESSAGE_MAP(CircleWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CircleWnd 메시지 처리기
void CircleWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 배경색으로 클라이언트 영역 채우기
	CRect rect;
	GetClientRect(&rect);
	m_GlimCircle.CreateImage(rect.Width(), rect.Height()); // 이미지 생성
	m_GlimCircle.Display(&dc);
}

void CircleWnd::DrawText(CWnd* pWnd, CString strText)
{
	if(pWnd != nullptr)
	{
		pWnd->SetWindowText(strText);
	}
}

// 클릭 지점 위치 표시 컨트롤 설정
void CircleWnd::SetWndDisplayPoints(CWnd* pDisplayPoint1, CWnd* pDisplayPoint2, CWnd* pDisplayPoint3)
{
	m_pDisplayPoint[0] = pDisplayPoint1;
	m_pDisplayPoint[1] = pDisplayPoint2;
	m_pDisplayPoint[2] = pDisplayPoint3;
	DrawText(m_pDisplayPoint[0], _T("-"));
	DrawText(m_pDisplayPoint[1], _T("-"));
	DrawText(m_pDisplayPoint[2], _T("-"));
}

// 클릭 원그리기 초기화
void CircleWnd::InitCircle()
{
	CRect rect;
	GetClientRect(&rect);
	m_GlimCircle.CreateImage(rect.Width(), rect.Height()); // 이미지 생성
	m_nCircleMode = 0; // 클릭 모드로 초기화
	m_nClickCount = 0;
	m_nMovePos = -1;
	m_bDrag = false;
	m_GlimCircle.InitPoint();
	DrawText(m_pDisplayPoint[0], _T("-"));
	DrawText(m_pDisplayPoint[1], _T("-"));
	DrawText(m_pDisplayPoint[2], _T("-"));
	theApp.GetMainWnd()->PostMessage(WM_CIRCLE_RANDOM, RANDOM_DISABLE);
	Invalidate();
}

void CircleWnd::SetPointRadius(float rRadi)
{
	m_GlimCircle.SetRadius(rRadi);
	Invalidate();
}

float CircleWnd::GetPointRadius()
{
	return m_GlimCircle.GetRadius();
}

void CircleWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nCircleMode == CIRCLE_CLICK && m_nClickCount >= 3)
	{
		if (m_GlimCircle.isInCircle(point, m_nMovePos))
		{
			SetCapture(); // 마우스 캡처 시작
			m_bDrag = TRUE;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CircleWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nCircleMode == CIRCLE_CLICK && m_nClickCount < 3)
	{
		m_GlimCircle.SetPoint(m_nClickCount, point);
		m_nClickCount++;
		Invalidate();
		if (m_nClickCount == 3)
		{
			theApp.GetMainWnd()->PostMessage(WM_CIRCLE_RANDOM, RANDOM_ENABLE);
			CPoint ptCenter;
			float radius = 0.0f;
			if (m_nClickCount == 3 && m_GlimCircle.CalculateCircle(ptCenter, radius) == false)
			{
				AfxMessageBox(_T("원 그리기 실패! 3개 점이 직선입니다."), MB_ICONERROR | MB_OK);
			}
		}
	}
	else
	{
		if (m_bDrag == TRUE)
		{
			ReleaseCapture();
			m_bDrag = FALSE;
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CircleWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_nCircleMode == CIRCLE_CLICK )// 클릭 모드일 때만 처리
	{
		if (m_nClickCount < 3)
		{
			if (m_pDisplayPoint[m_nClickCount] != nullptr)
			{
				CString strText;
				strText.Format(_T("%d, %d"), point.x, point.y);
				DrawText(m_pDisplayPoint[m_nClickCount], strText);
			}
		}
		else
		{
			if (m_bDrag == true)
			{
				CString strText;
				strText.Format(_T("%d, %d"), point.x, point.y);
				DrawText(m_pDisplayPoint[m_nMovePos], strText);

				m_GlimCircle.SetPoint(m_nMovePos, point);
				Invalidate();
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

bool CircleWnd::StartRandomCircle(int nCount)
{
	m_nCircleMode = CIRCLE_RANDOM; // 랜덤 모드로 설정
	m_nCircleCount = nCount;
	m_bThreadRun = true;
	m_pThread = AfxBeginThread(ThreadCircle, this);
	if(m_pThread == nullptr)
	{
		m_bThreadRun = false;
		AfxMessageBox(_T("스레드 시작 실패!"), MB_ICONERROR | MB_OK);
		return false;
	}
	theApp.GetMainWnd()->PostMessage(WM_CIRCLE_RANDOM, RANDOM_START);
	return true;
}

void CircleWnd::StopRandomCircle()
{
	if(m_pThread != nullptr && m_bThreadRun == true)
	{
		m_bThreadRun = false;
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		m_pThread = nullptr;
	}
}

void CircleWnd::RandomCircleProc()
{
	srand((unsigned int)time(nullptr)); // 난수 초기화
	int nCount = 0;
	while(nCount++ < m_nCircleCount && m_bThreadRun == true)
	{
		CPoint ptCenter;
		float radius = 0.0f;
		CRect rect;
		GetClientRect(&rect);
		CPoint ptPoint1 = CPoint(rand() % rect.Width(), rand() % rect.Height());
		CPoint ptPoint2 = CPoint(rand() % rect.Width(), rand() % rect.Height());
		CPoint ptPoint3 = CPoint(rand() % rect.Width(), rand() % rect.Height());
		m_GlimCircle.SetPoint(0, ptPoint1);
		m_GlimCircle.SetPoint(1, ptPoint2);
		m_GlimCircle.SetPoint(2, ptPoint3);
		CString strText;
		strText.Format(_T("%d, %d"), ptPoint1.x, ptPoint1.y);
		DrawText(m_pDisplayPoint[0], strText);
		strText.Format(_T("%d, %d"), ptPoint2.x, ptPoint2.y);
		DrawText(m_pDisplayPoint[1], strText);
		strText.Format(_T("%d, %d"), ptPoint3.x, ptPoint3.y);
		DrawText(m_pDisplayPoint[2], strText);
		Invalidate();
		WaitForSingleObject(m_pThread->m_hThread, 500);
	}
	m_bThreadRun = false; // 스레드 종료 플래그 설정
	m_pThread = nullptr; // 스레드 포인터 초기화
	theApp.GetMainWnd()->PostMessage(WM_CIRCLE_RANDOM, RANDOM_STOP);
}

UINT CircleWnd::ThreadCircle(LPVOID pParam)
{
	CircleWnd* pThis = (CircleWnd*)pParam;
	if (pThis == nullptr)
		return 0;
	pThis->RandomCircleProc();
	return 0;
}
