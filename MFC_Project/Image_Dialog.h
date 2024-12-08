#pragma once
#include "afxdialogex.h"


// Image_Dialog 대화 상자

class ImageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ImageDialog)

public:
	ImageDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.

	void DrawCircleRandom(int startpos_x, int startpos_y);

	virtual ~ImageDialog();
	void CreateCircle(int nStartposX, int nStartposY, unsigned char* fm, int pitch);
	CImage m_Image;
	float m_nCircleMoveX = 0;
	float m_nCircleMoveY = 0;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHECK };
#endif
private:
	CRect m_rtCircleRect;
	bool m_CircleRectCheck = true;

	int m_nDiameter = 100;
	CWnd* m_pParentDialog;
	
	CString m_CenterPos;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SetCircleMoveY(float _m_nCircleMoveY){ m_nCircleMoveY = _m_nCircleMoveY; }
	void SetCircleMoveX(float _m_CircleMoveX){ m_nCircleMoveX = _m_CircleMoveX; }
	CRect GetRtCircleRect() { return m_rtCircleRect; }
	bool GetCircleRectCheck() { return m_CircleRectCheck; }
	float GetCircleMoveX() { return m_nCircleMoveX; }
	float GetCircleMoveY() { return m_nCircleMoveY; }
	int GetDiameter() { return m_nDiameter; }
	//CImage GetImage() { return m_Image; }
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void ClearEllipse();
	void MoveCircle();
	bool IsInnerCircle(int x, int y, int radius, int center, int centery);
	void Normalize();
	void ShowCenterPos(float dCenterX, float dCenterY);
	void InitCenterPosText();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
