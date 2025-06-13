
// GlimCircleDlg.h: 헤더 파일
//
#pragma once
#include "CircleWnd.h"

// Cirlce 크기 보정
#define CIRCLE_TOP		10
#define CIRCLE_LEFT		260
#define	CIRCLE_RIGHT	10
#define	CIRCLE_BOTTOM	10

// CGlimCircleDlg 대화 상자
class CGlimCircleDlg : public CDialogEx
{
// 생성입니다.
public:
	CGlimCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CGlimCircleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIMCIRCLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CircleWnd* m_pCircleWnd;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonRand();
protected:
	afx_msg LRESULT OnCircleRandom(WPARAM wParam, LPARAM lParam);
};
