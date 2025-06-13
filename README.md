# GlimCircle 프로젝트

## 개요
**GlimCircle** 프로젝트는 MFC 기반의 윈도우 애플리케이션으로, 사용자가 마우스를 이용해 3개의 점을 지정하고, 이 점들을 기준으로 원을 그리거나 다양한 원 관련 기능을 시각적으로 제공합니다.  
주요 클래스는 `GlimCircle`, `CircleWnd`, 그리고 대화상자(`CGlimCircleDlg`)입니다.

---

## 주요 클래스 설명

### 1. GlimCircle (GlimThreePoint 클래스상속)

- **역할:**  
  3개의 점을 이용해 원의 중심과 반지름을 계산하고, 원을 그리는 기능을 담당합니다.
- **주요 기능:**
  - 세 점을 받아 원의 중심과 반지름 계산
  - 원의 표시 및 반지름 설정
  - 점이 원 내부에 있는지 판별
- **주요 메서드:**
  - `CalculateCircle(CPoint& ptCenter, float& fRadius)`: 세 점으로 원의 중심과 반지름 계산
  - `Display(CDC* pDC)`: 원을 화면에 표시
  - `SetRadius(float rRadius)`, `GetRadius()`: 반지름 설정/조회
  - `SetPoint(int nPos, CPoint ptPoint)`: 점 위치 설정

---

### 2. CircleWnd

- **역할:**  
  원 그리기와 관련된 사용자 인터페이스를 제공하는 커스텀 윈도우 클래스입니다.  
  마우스 입력을 받아 점을 지정하고, 원을 그리며, 랜덤 원 생성 등 다양한 기능을 제공합니다.
- **주요 멤버:**
  - `GlimCircle m_GlimCircle`: 원 계산 및 그리기 담당 객체
  - `int m_nClickCount`: 클릭 횟수(점 지정 상태)
  - `bool m_bDrag`: 드래그 상태
  - 기타 원 관련 상태 변수
- **주요 메서드:**
  - `OnPaint()`: 원 및 점을 윈도우에 그림
  - `OnLButtonDown/Up`, `OnMouseMove`: 마우스 입력 처리
  - `SetPointRadius(REAL rRadi)`: 점의 반지름 설정
  - `InitCircle()`: 원 상태 초기화
  - `StartRandomCircle(int nCount)`, `StopRandomCircle()`: 랜덤 원 생성/중지

---

### 3. CGlimCircleDlg

- **역할:**  
  애플리케이션의 메인 대화상자 클래스.  
  내부에 `CircleWnd`를 포함하여 원 그리기 UI를 제공합니다.
- **주요 기능:**
  - 대화상자 초기화 및 윈도우 메시지 처리
  - 원 관련 버튼(적용, 초기화, 랜덤) 이벤트 처리
  - 대화상자 크기 조정 및 ui 업데이트

---

## 사용 예시

1. 프로그램 실행 후, 마우스로 3개의 점을 지정하면 해당 점을 지나는 원이 그려집니다.
2. 반지름, 점 위치 등은 UI를 통해 조정할 수 있습니다.
3. 랜덤 원 생성 기능을 통해 여러 개의 원을 자동으로 그릴 수 있습니다.
4. 다이얼로그 창의 크기를 조정할 수 있으며, 원의 크기와 위치는 자동으로 조정됩니다.
---

## 파일 구조

- `GlimCircle.h/cpp` : 원 계산 및 그리기 로직
- `CircleWnd.h/cpp` : 원 그리기 전용 커스텀 윈도우
- `GlimCircleDlg.h/cpp` : 메인 대화상자 및 UI 이벤트 처리

---

## 빌드 및 실행

Visual Studio 2022에서 프로젝트를 열고 빌드 후 실행하면 됩니다.

---

## 라이선스

본 프로젝트는 개인 학습용으로 자유롭게 사용할 수 있습니다.