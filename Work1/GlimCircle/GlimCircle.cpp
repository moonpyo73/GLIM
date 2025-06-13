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

void GlimThreePoint::SetRadius(float radius)
{
	m_radius = radius;
}

float GlimThreePoint::GetRadius()
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

void GlimThreePoint::CreateImage(int nWidth, int nHeight)
{
	if (nWidth <= 0 || nHeight <= 0)
		return;
	m_image.Destroy(); // 이전 이미지가 있다면 해제
	// -nHeight는 음수로 설정하여 이미지가 위쪽에서 아래쪽으로 그려지도록 함
	m_image.Create(nWidth, -nHeight, 8);
	static RGBQUAD colorTable[256];
	for (int i = 0; i < 256; ++i)
	{
		colorTable[i].rgbRed = colorTable[i].rgbGreen = colorTable[i].rgbBlue = i;  // B
		colorTable[i].rgbReserved = 0;
	}
	m_image.SetColorTable(0, 256, colorTable); // 8비트 이미지의 색상 테이블을 흰색으로 설정

	int nPitch = m_image.GetPitch();
	if (nPitch < 0) // nPitch가 음수인 경우는 없지만, 안전하게 체크
	{
		AfxMessageBox(_T("Invalid image pitch."));
		return;
	}
	BYTE* pBits = (BYTE*)m_image.GetBits();
	if (pBits == nullptr) // 비트맵 데이터가 유효하지 않은 경우
	{
		AfxMessageBox(_T("Failed to get image bits."));
		return;
	}
	memset(pBits, 255, nPitch * m_image.GetHeight()); // 흰색으로 채우기 (8비트에서는 255가 흰색)
}

void GlimThreePoint::Display(CDC* pDC)
{
	if(!m_image.IsNull()) // nPitch가 음수인 경우는 없지만, 안전하게 체크
	{
		BYTE* pBits = (BYTE*)m_image.GetBits();
		int nPitch = m_image.GetPitch();
		if(nPitch <0) // nPitch가 음수인 경우는 없지만, 안전하게 체크
		{
			return;
		}
		memset(pBits, 255, nPitch * m_image.GetHeight()); // 흰색으로 채우기 (8비트에서는 255가 흰색)

		for (const auto& ptPoint : m_vtPoint)
		{
			DrawCircle(ptPoint, m_radius);
		}
		m_image.Draw(pDC->m_hDC, 0, 0);
	}
}

bool GlimThreePoint::isInCircle(CPoint ptCur, int& nInPos)
{
	bool bRes = false;
	nInPos = -1;
	for (uint8_t nPos = 0; nPos < m_vtPoint.size(); nPos++)
	{
		float distance = sqrt(pow(ptCur.x - m_vtPoint[nPos].x, 2) + pow(ptCur.y - m_vtPoint[nPos].y, 2));
		if (distance <= m_radius)
		{
			bRes = true;
			nInPos = nPos;
			break;
		}
	}
	return bRes;
}

void GlimThreePoint::DrawCircle(CPoint ptCenter, float rRadius, int nDrawType)
{
	BYTE* pBits = (BYTE*)m_image.GetBits();
	int bytesPerPixel = m_image.GetBPP() / 8; // 8비트 이미지이므로 1바이트
	int nPitch = m_image.GetPitch();
	for (int y = ptCenter.y - rRadius; y <= ptCenter.y + rRadius; ++y)
	{
		for (int x = ptCenter.x - rRadius; x <= ptCenter.x + rRadius; ++x)
		{
			if (x >= 0 && x < m_image.GetWidth() && y >= 0 && y < m_image.GetHeight())
			{
				double dist = sqrt(pow(x - ptCenter.x, 2) + pow(y - ptCenter.y, 2));
				double delta = abs(dist - rRadius);

				if (nDrawType == DRAW_PEN) // 원 테두리
				{
					if (delta < 1.0) // 원의 경계 근처
					{
						// 거리에 따라 색상 강도 조절 (안티앨리어싱 효과)
						BYTE grayScale = static_cast<BYTE>(255 * (1.0 - delta));
						BYTE* pPixel = pBits + (long long)y * nPitch + x * bytesPerPixel;
						*pPixel = min(255, max(0, 255 - grayScale)); // 배경색이 흰색이라고 가정
					}
				}
				else if (nDrawType == DRAW_BRUSH) // 채워진 원
				{
					if (dist <= rRadius)
					{
						BYTE* pPixel = pBits + (long long)y * nPitch + x * bytesPerPixel;
						*pPixel = 0; // 검은색으로 채우기
					}
					else if (dist > rRadius && dist < rRadius + 0.1) // 원의 경계 근처 (테두리 안티앨리어싱)
					{
						BYTE grayScale = static_cast<BYTE>(255 * (dist - rRadius));
						BYTE* pPixel = pBits + (long long)y * nPitch + x * bytesPerPixel;
						*pPixel = min(255, max(0, 255 - grayScale));
					}
				}
			}
		}
	}
}

GlimCircle::GlimCircle()
{
}

bool GlimCircle::CalculateCircle(CPoint& ptCenter, float & fRadius)
{
	if (m_vtPoint.size() != 3)
		return false;
	float x1 = m_vtPoint[0].x, y1 = m_vtPoint[0].y;
	float x2 = m_vtPoint[1].x, y2 = m_vtPoint[1].y;
	float x3 = m_vtPoint[2].x, y3 = m_vtPoint[2].y;

	float D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	if (D == 0) // 세 점이 한 직선 위에 있는 경우 원을 그릴 수 없음
		return false;

	float Ux = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
	float Uy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;

	ptCenter.x = Ux;
	ptCenter.y = Uy;
	fRadius = sqrt(pow(x1 - Ux, 2) + pow(y1 - Uy, 2));

	return true;
}

void GlimCircle::Display(CDC* pDC)
{
	GlimThreePoint::Display(pDC);
	CPoint ptCenter;
	float fRadius = 0.0F;
	if (CalculateCircle(ptCenter, fRadius)) 
	{// 세 점이 한 직선 위에 있는 경우 원을 그리지 않음.
		DrawCircle(ptCenter, fRadius, DRAW_PEN);
	}
	if(!m_image.IsNull())
	{
		m_image.Draw(pDC->m_hDC, 0, 0);
	}
}