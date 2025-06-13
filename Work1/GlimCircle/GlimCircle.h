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
	vector<CPoint> m_vtPoint;	// Ŭ���� ����
	REAL	m_radius;	// �� ������
	Color	m_color;	// �� ����
public:
	GlimThreePoint(); // ������

	void InitPoint(); // Ŭ���� ���� �ʱ�ȭ
	void SetRadius(REAL rRadius); // ������ ����
	REAL GetRadius(); // ������ ��ȯ
	void SetPoint(int nPos, CPoint ptPoint); // ���� ����
	// Ŭ�� ���� �� ����
	bool isInCircle(CPoint ptCur, int& nInPos);

	// ȭ�����
	virtual void Display(Graphics& graphic);

protected:
	// ���׸���
	virtual void DrawCircle(Graphics& graphic, CPoint ptCenter, REAL rRadius, int nType= DRAW_BRUSH);
};

// ������ �մ� ���� �׸��� Ŭ����
class GlimCircle : public GlimThreePoint
{
public:
	GlimCircle();	// ������

	virtual void Display(Graphics& graphic); // ȭ�����
	bool CalculateCircle(CPoint& ptCenter, REAL& fRadius); // �������� �� ���
};
