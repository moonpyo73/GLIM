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
	m_image.Destroy(); // ���� �̹����� �ִٸ� ����
	// -nHeight�� ������ �����Ͽ� �̹����� ���ʿ��� �Ʒ������� �׷������� ��
	m_image.Create(nWidth, -nHeight, 8);
	static RGBQUAD colorTable[256];
	for (int i = 0; i < 256; ++i)
	{
		colorTable[i].rgbRed = colorTable[i].rgbGreen = colorTable[i].rgbBlue = i;  // B
		colorTable[i].rgbReserved = 0;
	}
	m_image.SetColorTable(0, 256, colorTable); // 8��Ʈ �̹����� ���� ���̺��� ������� ����

	int nPitch = m_image.GetPitch();
	if (nPitch < 0) // nPitch�� ������ ���� ������, �����ϰ� üũ
	{
		AfxMessageBox(_T("Invalid image pitch."));
		return;
	}
	BYTE* pBits = (BYTE*)m_image.GetBits();
	if (pBits == nullptr) // ��Ʈ�� �����Ͱ� ��ȿ���� ���� ���
	{
		AfxMessageBox(_T("Failed to get image bits."));
		return;
	}
	memset(pBits, 255, nPitch * m_image.GetHeight()); // ������� ä��� (8��Ʈ������ 255�� ���)
}

void GlimThreePoint::Display(CDC* pDC)
{
	if(!m_image.IsNull()) // nPitch�� ������ ���� ������, �����ϰ� üũ
	{
		BYTE* pBits = (BYTE*)m_image.GetBits();
		int nPitch = m_image.GetPitch();
		if(nPitch <0) // nPitch�� ������ ���� ������, �����ϰ� üũ
		{
			return;
		}
		memset(pBits, 255, nPitch * m_image.GetHeight()); // ������� ä��� (8��Ʈ������ 255�� ���)

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
	int bytesPerPixel = m_image.GetBPP() / 8; // 8��Ʈ �̹����̹Ƿ� 1����Ʈ
	int nPitch = m_image.GetPitch();
	for (int y = ptCenter.y - rRadius; y <= ptCenter.y + rRadius; ++y)
	{
		for (int x = ptCenter.x - rRadius; x <= ptCenter.x + rRadius; ++x)
		{
			if (x >= 0 && x < m_image.GetWidth() && y >= 0 && y < m_image.GetHeight())
			{
				double dist = sqrt(pow(x - ptCenter.x, 2) + pow(y - ptCenter.y, 2));
				double delta = abs(dist - rRadius);

				if (nDrawType == DRAW_PEN) // �� �׵θ�
				{
					if (delta < 1.0) // ���� ��� ��ó
					{
						// �Ÿ��� ���� ���� ���� ���� (��Ƽ�ٸ���� ȿ��)
						BYTE grayScale = static_cast<BYTE>(255 * (1.0 - delta));
						BYTE* pPixel = pBits + (long long)y * nPitch + x * bytesPerPixel;
						*pPixel = min(255, max(0, 255 - grayScale)); // ������ ����̶�� ����
					}
				}
				else if (nDrawType == DRAW_BRUSH) // ä���� ��
				{
					if (dist <= rRadius)
					{
						BYTE* pPixel = pBits + (long long)y * nPitch + x * bytesPerPixel;
						*pPixel = 0; // ���������� ä���
					}
					else if (dist > rRadius && dist < rRadius + 0.1) // ���� ��� ��ó (�׵θ� ��Ƽ�ٸ����)
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
	if (D == 0) // �� ���� �� ���� ���� �ִ� ��� ���� �׸� �� ����
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
	{// �� ���� �� ���� ���� �ִ� ��� ���� �׸��� ����.
		DrawCircle(ptCenter, fRadius, DRAW_PEN);
	}
	if(!m_image.IsNull())
	{
		m_image.Draw(pDC->m_hDC, 0, 0);
	}
}