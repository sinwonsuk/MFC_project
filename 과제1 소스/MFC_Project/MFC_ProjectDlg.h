
// MFC_ProjectDlg.h: 헤더 파일
//

#pragma once
#include "Image_Dialog.h"



// CMFCProjectDlg 대화 상자
class CMFCProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	ImageDialog* m_pImageDialog;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeStartPosX();
	afx_msg void OnEnChangeStartPosY();	
	afx_msg void OnEnChangeEndPosX();
	afx_msg void OnEnChangeEndPosY();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnMove();
	
	afx_msg void OnBnClickedBtnOpen();

	int GetStartPosX() { return m_nStartPosX; }
	int GetStartPosY() { return m_nStartPosY; }
	int GetEndPosX() { return m_nEndPosX; }
	int GetEndPosY() { return m_EndPosY; }
	
	bool inspectCircle();

private:
	void Wait(DWORD dwMillisecond);
	void Init();
	void Save(int num);
	CString Load();
	void FindCenterpos(double& dCenterX, double& dCentery);
	void scratch_X(double dCenterX, double dCentery);

private:

	bool m_IsMoving = false;
	int m_nStartPosX;
	int m_nStartPosY;
	int m_nEndPosX;
	int m_EndPosY;

public:
	afx_msg void OnDestroy();
};
