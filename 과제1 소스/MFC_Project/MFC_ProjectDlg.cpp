
// MFC_ProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_Project.h"
#include "MFC_ProjectDlg.h"
#include "afxdialogex.h"
#include "Image_Dialog.h"
#include <atlstr.h> 
#include <string>
#include "atlimage.h"
#include <iostream>
//#include <opencv2/opencv.hpp>
//using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMFCProjectDlg 대화 상자

CMFCProjectDlg::CMFCProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_PROJECT_DIALOG, pParent)
	, m_nStartPosX(0)
	, m_nStartPosY(0)
	, m_nEndPosX(0)
	, m_EndPosY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_START_POS_X, m_nStartPosX);
	DDX_Text(pDX, IDC_START_POS_Y, m_nStartPosY);
	DDX_Text(pDX, IDC_End_POS_X, m_nEndPosX);
	DDX_Text(pDX, IDC_End_POS_Y, m_EndPosY);
}

BEGIN_MESSAGE_MAP(CMFCProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_START_POS_X, &CMFCProjectDlg::OnEnChangeStartPosX)
	ON_EN_CHANGE(IDC_START_POS_Y, &CMFCProjectDlg::OnEnChangeStartPosY)
	ON_EN_CHANGE(IDC_End_POS_X, &CMFCProjectDlg::OnEnChangeEndPosX)
	ON_EN_CHANGE(IDC_End_POS_Y, &CMFCProjectDlg::OnEnChangeEndPosY)
	ON_BN_CLICKED(IDCANCEL, &CMFCProjectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_DRAW, &CMFCProjectDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CMFCProjectDlg::OnBnClickedBtnMove)
//	ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDC_BTN_OPEN, &CMFCProjectDlg::OnBnClickedBtnOpen)

END_MESSAGE_MAP()

// CMFCProjectDlg 메시지 처리기

BOOL CMFCProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 새 다이얼로그 생성 
	Init();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCProjectDlg::OnEnChangeStartPosX()
{
	UpdateData(true);
}
void CMFCProjectDlg::OnEnChangeStartPosY()
{
	UpdateData(true);
}
void CMFCProjectDlg::OnEnChangeEndPosX()
{
	UpdateData(true);
}
void CMFCProjectDlg::OnEnChangeEndPosY()
{
	UpdateData(true);
}

void CMFCProjectDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CMFCProjectDlg::OnBnClickedBtnDraw()
{
	m_pImageDialog->DrawCircleRandom(m_nStartPosX,m_nStartPosY);
}

void CMFCProjectDlg::OnBnClickedBtnMove()
{	
	int nNum = 0;

	m_pImageDialog->InitCenterPosText();
	m_pImageDialog->ClearEllipse();

	while (true) {	

		float nCurrentposY = m_pImageDialog->GetCircleMoveY()+ m_nStartPosY;

		float nCurrentPosX = m_pImageDialog->GetCircleMoveX()+ m_nStartPosX;

		if (static_cast<int>(nCurrentposY) == m_EndPosY && static_cast<int>(nCurrentPosX) == m_nEndPosX)
		{
			break;
		}

		m_pImageDialog->MoveCircle();
		
		// 이미지 저장 
		Save(nNum);

		// sleep 함수 오류로 인해 바꿈
		Wait(10);
		nNum++;

	}
	// 초기화 
	m_pImageDialog->SetCircleMoveX(0);
	m_pImageDialog->SetCircleMoveY(0);
}
void CMFCProjectDlg::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount64();

	while (GetTickCount64() - dwStart < dwMillisecond) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


void CMFCProjectDlg::OnBnClickedBtnOpen()
{
	CString strPathName = Load();

	m_pImageDialog->InitCenterPosText();

	if (m_pImageDialog->m_Image != nullptr && strPathName != L"") {		
		double dCenterX = 0;
		double dCenterY = 0;
		//FindCenterposOpenCV(strPathName, dCenterX, dCenterY);
		if (inspectCircle() == true){
			FindCenterpos(dCenterX, dCenterY);
			scratch_X(dCenterX, dCenterY);
			m_pImageDialog->ShowCenterPos(dCenterX, dCenterY);
		}		
	}
}

bool CMFCProjectDlg::inspectCircle()
{
	int n_Width = m_pImageDialog->m_Image.GetWidth();
	int n_Height = m_pImageDialog->m_Image.GetHeight();
	int n_Pitch = m_pImageDialog->m_Image.GetPitch();
	int n_Radius = m_pImageDialog->GetDiameter()/2;
	int n_CircleArea = n_Radius * n_Radius * 3.14;

	unsigned char* fm = (unsigned char*)m_pImageDialog->m_Image.GetBits();

	int n_Check = 0;
	int n_Persent = 0;

	for (size_t y = 0; y < n_Height; y++) {
		for (size_t x = 0; x < n_Width; x++) {
			if (fm[y * n_Pitch + x] > 128)
				n_Check++;			
		}
	}
	
	//95퍼센트 일치하면
	if (n_Check >= n_CircleArea *0.95)
		return true;
	else{
		AfxMessageBox(_T("원이 아닙니다"));
		return false;
	}	
}

void CMFCProjectDlg::Init()
{
	m_pImageDialog = new ImageDialog(this);
	m_pImageDialog->Create(IDD_DIALOG_CHECK, this);
	m_pImageDialog->ShowWindow(SW_SHOW);
}

void CMFCProjectDlg::Save(int num)
{
	TCHAR cPath[256] = { 0, };

	GetModuleFileName(NULL, cPath, 256);

	CString folderPath(cPath);

	for (size_t i = 0; i < 3; i++)
		 folderPath = folderPath.Left(folderPath.ReverseFind('\\'));

	folderPath.Format(_T("%s\\image\\circle%d.bmp"), folderPath.GetString(), num);

	m_pImageDialog->m_Image.Save(folderPath);
}

CString CMFCProjectDlg::Load()
{
	CString str = _T("All files(*.*)|*.*|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	CString strPathName;

	if (dlg.DoModal() == IDOK)
	{	
		m_pImageDialog->ClearEllipse();
		strPathName = dlg.GetPathName();

		if (m_pImageDialog->m_Image != nullptr)
		{
			m_pImageDialog->m_Image.Destroy();
		}

		// 새 이미지 로드
		m_pImageDialog->m_Image.Load(strPathName);

		m_pImageDialog->Invalidate();
	}

	return strPathName;

}

void CMFCProjectDlg::FindCenterposOpenCV(CString strPathName, double&dCenterX, double& dCentery)
{
	//CString cStr = strPathName;
	//CT2CA convertedString(cStr);

	//std::string str = std::string(convertedString);

	//// 입력 이미지 로드
	//Mat image = imread(str, IMREAD_GRAYSCALE);
	//if (image.empty()) {
	//	cout << "Image not found!" << endl;
	//	return;
	//}

	//// 블러 처리
	//Mat blurred;
	//GaussianBlur(image, blurred, Size(9, 9), 2);

	//// HoughCircles로 원 탐지
	//vector<Vec3f> circles;
	//HoughCircles(blurred, circles, HOUGH_GRADIENT, 1, 30, 150, 30);

	////잘린 원 중심 좌표 저장
	//vector<Point> cutCircleCenters;

	//for (size_t i = 0; i < circles.size(); i++) {
	//	float x = circles[i][0];
	//	float y = circles[i][1];
	//	float r = circles[i][2];

	//	// 원의 경계 비율 계산
	//	int totalPixels = 0, validPixels = 0;
	//	for (int theta = 0; theta < 360; theta++) {
	//		int xi = x + r * cos(theta * CV_PI / 180.0);
	//		int yi = y + r * sin(theta * CV_PI / 180.0);
	//		totalPixels++;
	//		if (xi >= 0 && xi < image.cols && yi >= 0 && yi < image.rows) {
	//			validPixels++;
	//		}
	//	}
	//	cutCircleCenters.push_back(Point(cvRound(x), cvRound(y)));
	//}
	//// 결과 출력
	//cout << "원 중심 좌표: " << endl;
	//for (const auto& center : cutCircleCenters) {
	//	cout << "x: " << center.x << ", y: " << center.y << endl;
	//	dCenterX = center.x;
	//	dCentery = center.y;
	//}

}

void CMFCProjectDlg::FindCenterpos(double& dCenterX, double& dCenterY)
{
	unsigned char* fm = (unsigned char*)m_pImageDialog->m_Image.GetBits();
	int nWidth = m_pImageDialog->m_Image.GetWidth();
	int nHeight = m_pImageDialog->m_Image.GetHeight();
	int nPitch = m_pImageDialog->m_Image.GetPitch();

	int nTh = 0x80;
	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;

	CRect rect(0, 0, nWidth, nHeight);

	for (int j = rect.top; j < rect.bottom; j++) {
		for (int i = rect.left; i < rect.right; i++) {
			if (fm[j * nPitch + i] > nTh) {
				nSumX += i;
				nSumY += j;
				nCount++;
			}
		}
	}

	dCenterX = (double)nSumX / nCount;
	dCenterY = (double)nSumY / nCount;

	cout << dCenterX << "\t" << dCenterY << endl;

	m_pImageDialog->Invalidate();
}

void CMFCProjectDlg::scratch_X(double dCenterX, double dCentery)
{
	unsigned char* fm = (unsigned char*)m_pImageDialog->m_Image.GetBits();

	int nWidth = m_pImageDialog->m_Image.GetWidth();
	int nHeight = m_pImageDialog->m_Image.GetHeight();
	int nPitch = m_pImageDialog->m_Image.GetPitch();
	int nRadius = m_pImageDialog->GetDiameter() / 2;

	for (int i = -nRadius; i < nRadius; i++) {
		if (dCentery + i < 0) 
			continue;	
		if (dCenterX + i < 0)
			continue;	
		if (dCenterX + i > nWidth)
		    continue;	
		if (dCentery + i > nHeight)
			continue;

		fm[static_cast<int>(dCentery + i) * nPitch + static_cast<int>(dCenterX + i)] = 0;
	}

	for (int i = -nRadius; i < nRadius; i++) {
		if (dCentery - i < 0 ) 
			continue;	
		if (dCenterX + i < 0 ) 
			continue;		
		if (dCenterX + i > nWidth ) 
			continue;	
		if (dCentery - i > nHeight) 
			continue;
		
		fm[static_cast<int>(dCentery - i) * nPitch + static_cast<int>(dCenterX + i)] = 0;
	}
}



