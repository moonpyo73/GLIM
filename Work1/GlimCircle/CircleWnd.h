// CircleWnd.h: ��� ����
//
#pragma once

#include <afxwin.h>
#include <gdiplus.h> // GDI+ ��� ����
#include "GlimCircle.h"

using namespace Gdiplus;
// CircleWnd

class CircleWnd : public CWnd
{
	DECLARE_DYNAMIC(CircleWnd)

public:
	CircleWnd();
	virtual ~CircleWnd();

protected:
	GlimCircle	m_GlimCircle;	// GlimCircle ��ü
	int	m_nClickCount;	// Ŭ�� ���� Ƚ�� (0~2 : Ŭ��, 3 : �̵�)
	int m_nMovePos;	// ���õ� Ŭ������(-1 : �⺻��(���þȵ�), 0~2 : Ŭ������)
	CWnd* m_pDisplayPoint[3];	// Ŭ������ ��� ��Ʈ��
	bool m_bDrag;			// ���콺 �巡�� ����
	CWinThread* m_pThread;	// ���׸��� ������
	bool m_bThreadRun;	// ������ ���� ����
	int m_nCircleCount;	// ���׸��� ����

	static UINT ThreadCircle(LPVOID pParam);	// ������ �Լ�

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void DrawText(CWnd* pWnd, CString strText);

	void SetPointRadius(REAL rRadi);
	REAL GetPointRadius();

	// Ŭ�� ���� ��ġ ǥ�� ��Ʈ�� ����
	void SetWndDisplayPoints(CWnd* pDisplayPoint1, CWnd* pDisplayPoint2, CWnd* pDisplayPoint3);
	// Ŭ�� ���׸��� �ʱ�ȭ
	void InitCircle();
	bool StartRandomCircle(int nCount = 10);
	void StopRandomCircle();
	void RandomCircleProc();
};
