// CircleWnd.h: 헤더 파일
//
#pragma once

#include <afxwin.h>
#include <gdiplus.h> // GDI+ 헤더 포함
#include "GlimCircle.h"

using namespace Gdiplus;
// CircleWnd

#define	CIRCLE_CLICK	0	// 클릭 모드
#define CIRCLE_RANDOM	1	// 랜덤 모드

#define WM_CIRCLE_RANDOM	(WM_USER + 100)	// 랜덤 원그리기 메시지
#define RANDOM_DISABLE		0				// 랜덤 원그리기 활성화
#define RANDOM_ENABLE		1				// 랜덤 원그리기 비활성화
#define RANDOM_START		2				// 랜덤 원그리기 시작
#define RANDOM_STOP			3				// 랜덤 원그리기 중지

class CircleWnd : public CWnd
{
	DECLARE_DYNAMIC(CircleWnd)

public:
	CircleWnd();
	virtual ~CircleWnd();

protected:
	UINT m_nCircleMode;			// 모드 (CIRCLE_CLICK, CIRCLE_RANDOM)
	GlimCircle	m_GlimCircle;	// GlimCircle 객체
	int	m_nClickCount;			// 클리 지점 횟수 (0~2 : 클릭, 3 : 이동)
	int m_nMovePos;				// 선택된 클릭지점(-1 : 기본값(선택안됨), 0~2 : 클릭지점)
	CWnd* m_pDisplayPoint[3];	// 클릭지점 출력 컨트롤
	bool m_bDrag;				// 마우스 드래그 여부
	CWinThread* m_pThread;		// 원그리기 스레드
	bool m_bThreadRun;			// 스레드 실행 여부
	int m_nCircleCount;			// 원그리기 개수

	static UINT ThreadCircle(LPVOID pParam);	// 스레드 함수

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void DrawText(CWnd* pWnd, CString strText);

	void SetPointRadius(REAL rRadi);
	REAL GetPointRadius();

	// 클릭 지점 위치 표시 컨트롤 설정
	void SetWndDisplayPoints(CWnd* pDisplayPoint1, CWnd* pDisplayPoint2, CWnd* pDisplayPoint3);
	// 클릭 원그리기 초기화
	void InitCircle();
	bool StartRandomCircle(int nCount = 10);
	void StopRandomCircle();
	void RandomCircleProc();
};
