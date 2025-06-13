// GlimCircle ���׸��� Ŭ���� ���� ����
//

#include <vector>
#pragma once

using namespace std;
#define DRAW_PEN	0
#define	DRAW_BRUSH	1

// Ŭ���� ������ ó���ϴ� Ŭ����
class GlimThreePoint
{
protected:
	CImage m_image;				// �̹��� �׸��� ��ü
	vector<CPoint> m_vtPoint;	// Ŭ���� ����
	float 	m_radius;	// �� ������
public:
	GlimThreePoint(); // ������

	void InitPoint(); // Ŭ���� ���� �ʱ�ȭ
	void SetRadius(float rRadius); // ������ ����
	float GetRadius(); // ������ ��ȯ
	void SetPoint(int nPos, CPoint ptPoint); // ���� ����
	// Ŭ�� ���� �� ����
	bool isInCircle(CPoint ptCur, int& nInPos);

	void CreateImage(int nWidth, int nHeight);
	void DestroyImage(); // �̹��� ��ü ����
	// ȭ�����
	virtual void Display(CDC* pDC);

protected:
	// ���׸���
	virtual void DrawCircle(CPoint ptCenter, float rRadius, int nType= DRAW_BRUSH);
};

// ������ �մ� ���� �׸��� Ŭ����
class GlimCircle : public GlimThreePoint
{
public:
	GlimCircle();	// ������

	virtual void Display(CDC* pDC); // ȭ�����
	bool CalculateCircle(CPoint& ptCenter, float & fRadius); // �������� �� ���
};
