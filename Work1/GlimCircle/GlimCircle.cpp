#include "pch.h"
#include "GlimCircleApp.h"
#include "GlimCircle.h"

GlimThreePoint::GlimThreePoint()
{
	m_radius = 5.0F;
}

void GlimThreePoint::InitPoint()
{
	m_vtPoint.clear();
}

void GlimThreePoint::SetRadius(REAL radius)
{
	m_radius = radius;
}

REAL GlimThreePoint::GetRadius()
{
	return m_radius;
}

void GlimThreePoint::SetPoint(int nPos, CPoint ptPoint)
{
	if (nPos == m_vtPoint.size())
	{
		m_vtPoint.push_back(ptPoint);
	}
	else if (nPos < 3 && nPos < m_vtPoint.size())
	{
		m_vtPoint[nPos] = ptPoint;
	}
}

void GlimThreePoint::Display(Graphics& graphic)
{
	for (const auto& ptPoint : m_vtPoint)
	{
		DrawCircle(graphic, ptPoint, m_radius);
	}
}

bool GlimThreePoint::isInCircle(CPoint ptCur, int& nInPos)
{
	bool bRes = false;
	nInPos = -1;
	for (uint8_t nPos = 0; nPos < m_vtPoint.size(); nPos++)
	{
		REAL distance = sqrt(pow(ptCur.x - m_vtPoint[nPos].x, 2) + pow(ptCur.y - m_vtPoint[nPos].y, 2));
		if (distance <= m_radius)
		{
			bRes = true;
			nInPos = nPos;
			break;
		}
	}
	return bRes;
}

void GlimThreePoint::DrawCircle(Graphics& graphic, CPoint ptCenter, REAL rRadius, int nDrawType)
{
	RectF circleRect(ptCenter.x - rRadius, ptCenter.y - rRadius, rRadius * 2, rRadius * 2);
	if (nDrawType == DRAW_PEN)
	{
		Pen pen(Color::Black, 1.0F);
		// Draw the filled circle
		graphic.DrawEllipse(&pen, circleRect);
	}
	else if (nDrawType == DRAW_BRUSH)
	{
		SolidBrush brush(Color::Black);
		graphic.FillEllipse(&brush, circleRect);
	}
}

GlimCircle::GlimCircle()
{
}

bool GlimCircle::CalculateCircle(CPoint& ptCenter, REAL& fRadius)
{
	if (m_vtPoint.size() != 3)
		return false;
	REAL x1 = m_vtPoint[0].x, y1 = m_vtPoint[0].y;
	REAL x2 = m_vtPoint[1].x, y2 = m_vtPoint[1].y;
	REAL x3 = m_vtPoint[2].x, y3 = m_vtPoint[2].y;

	REAL D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	if (D == 0) // 세 점이 한 직선 위에 있는 경우 원을 그릴 수 없음
		return false;

	REAL Ux = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
	REAL Uy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;

	ptCenter.x = Ux;
	ptCenter.y = Uy;
	fRadius = sqrt(pow(x1 - Ux, 2) + pow(y1 - Uy, 2));

	return true;
}

void GlimCircle::Display(Graphics& graphic)
{
	CPoint ptCenter;
	REAL fRadius = 0.0F;
	if (CalculateCircle(ptCenter, fRadius)) 
	{// 세 점이 한 직선 위에 있는 경우 원을 그리지 않음.
		DrawCircle(graphic, ptCenter, fRadius, DRAW_PEN);
	}
	GlimThreePoint::Display(graphic);
}