// GlimCircle 원그리기 클래스 정의 파일
//

#include <vector>
#pragma once

using namespace std;
#define DRAW_PEN	0
#define	DRAW_BRUSH	1

// 클릭된 세점을 처리하는 클래스
class GlimThreePoint
{
protected:
	CImage m_image;				// 이미지 그리기 객체
	vector<CPoint> m_vtPoint;	// 클릭된 세점
	float 	m_radius;	// 원 반지름
public:
	GlimThreePoint(); // 생성자

	void InitPoint(); // 클릭된 세점 초기화
	void SetRadius(float rRadius); // 반지름 설정
	float GetRadius(); // 반지름 반환
	void SetPoint(int nPos, CPoint ptPoint); // 세점 설정
	// 클릭 지점 원 여부
	bool isInCircle(CPoint ptCur, int& nInPos);

	void CreateImage(int nWidth, int nHeight);
	void DestroyImage(); // 이미지 객체 삭제
	// 화면출력
	virtual void Display(CDC* pDC);

protected:
	// 원그리기
	virtual void DrawCircle(CPoint ptCenter, float rRadius, int nType= DRAW_BRUSH);
};

// 세점을 잇는 원을 그리는 클래스
class GlimCircle : public GlimThreePoint
{
public:
	GlimCircle();	// 생성자

	virtual void Display(CDC* pDC); // 화면출력
	bool CalculateCircle(CPoint& ptCenter, float & fRadius); // 세점으로 원 계산
};
