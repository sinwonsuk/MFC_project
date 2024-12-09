// Image_Dialog.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_Project.h"
#include "afxdialogex.h"
#include "Image_Dialog.h"
#include "MFC_ProjectDlg.h"

// Image_Dialog 대화 상자

IMPLEMENT_DYNAMIC(ImageDialog, CDialogEx)

ImageDialog::ImageDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHECK, pParent)
	, m_CenterPos(_T(""))
{
	m_pParentDialog = pParent;
}


ImageDialog::~ImageDialog()
{
}



void ImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_CenterPos);
}


BEGIN_MESSAGE_MAP(ImageDialog, CDialogEx)
	ON_WM_PAINT()


	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// Image_Dialog 메시지 처리기


BOOL ImageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int nWidth = 900;
	int nWHeight = 480;
	int nBpp = 8;

	// 다이얼로그 이동 
	MoveWindow(0, 0, nWidth, nWHeight);

	// 이미지 생성 
	m_Image.Create(nWidth, -nWHeight, nBpp);

	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_Image.SetColorTable(0, 256, rgb);
	}
	int nPitch = m_Image.GetPitch();

	// 형변환
	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	// 메모리 fm 부터 nWidth * nWHeight 까지 0 으로 
	memset(fm, 0, nWidth * nWHeight);

	return TRUE;
}

void ImageDialog::DrawCircleRandom(int startpos_x, int startpos_y)
{
	m_CircleRectCheck = true;

	int nRandom = m_Image.GetHeight();

	// 랜덤한 크기
	int nSizeX = rand() % nRandom + 20;
	int nSizeY = rand() % nRandom + 20;

	m_rtCircleRect.SetRect(0, 0, nSizeX, nSizeY);

	// 중심점으로 이동 
	// ex) startpos_x = 0,startpos_y = 0,size_x = 50,size_y =50
	// 이라면 Rect를 size_x/2 를 왼쪽으로 이동 size_y/2를 위로 이동시켜 중심점을 구함

	m_rtCircleRect.MoveToX(startpos_x - nSizeX / 2);
	m_rtCircleRect.MoveToY(startpos_y - nSizeY / 2);

	// OnPaint() 화면 갱신 요청
	Invalidate();
}

void ImageDialog::ClearEllipse()
{
	m_CircleRectCheck = false; // 원 그리기 비활성화
	Invalidate();           // 화면 갱신 요청
}


void ImageDialog::OnPaint()
{
	CPaintDC dc(this);

	m_Image.Draw(dc, 0, 0);

	// True 일떄만 원 그리기 
	if (m_CircleRectCheck) {
		dc.Ellipse(m_rtCircleRect);
	}
}

void ImageDialog::MoveCircle()
{
	CMFCProjectDlg* parent = (CMFCProjectDlg*)m_pParentDialog;

	int nStartposX = parent->GetStartPosX();
	int nStartposY = parent->GetStartPosY();

	int endpos_x = parent->GetEndPosX();
	int endpos_y = parent->GetEndPosY();

	int pitch = m_Image.GetPitch(); // 한 행의 메모리 크기
	int height = m_Image.GetHeight();    // 이미지 높이

	Normalize();

	// 형변환
	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	// 초기화 
	for (size_t y = 0; y < m_Image.GetHeight(); y++) {
		for (size_t x = 0; x < m_Image.GetWidth(); x++)
		{
			fm[y * pitch + x] = 0;
		}
	}

	CreateCircle(nStartposX, nStartposY, fm, pitch);

	Invalidate(false);
}





bool ImageDialog::IsInnerCircle(int nX, int nY, int nRadius, int nCenterX, int nCenterY)
{
	if (nX < 0) 
		return false;
	if (nY < 0)
		return false;
	if (nY >= m_Image.GetHeight())
		return false;
	if (nX > m_Image.GetWidth())
		return false;
			
	bool bRet = false;
	double dx = nX - nCenterX;
	double dy = nY - nCenterY;
	double dDist = dx * dx + dy * dy;

	if (dDist <= nRadius * nRadius)
		bRet = true;
	
	return bRet;
}

void ImageDialog::Normalize()
{
	CMFCProjectDlg* parent = (CMFCProjectDlg*)m_pParentDialog;

	int nStartposX = parent->GetStartPosX();
	int nStartposY = parent->GetStartPosY();

	int nEndposX = parent->GetEndPosX();
	int nEndposY = parent->GetEndPosY();

	float magnitude = sqrt((nEndposX - nStartposX) * (nEndposX - nStartposX) + (nEndposY - nStartposY) * (nEndposY - nStartposY));

	if (magnitude != 0.0f)
	{
		m_nCircleMoveX += (nEndposX - nStartposX) / magnitude;
		m_nCircleMoveY += (nEndposY - nStartposY) / magnitude;
	}
}

void ImageDialog::ShowCenterPos(float dCenterX, float dCenterY)
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_TEXT);

	if (pStatic == nullptr)
		return;

	CString strBuffer = L"";
	strBuffer.Format(_T("%.f,%.f"), dCenterX, dCenterY);
	m_CenterPos = strBuffer;

	float resize = 6;
	pStatic->SetWindowPos(&CWnd::wndBottom, dCenterX+ resize, dCenterY- resize, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	
	UpdateData(FALSE);
}

void ImageDialog::InitCenterPosText()
{
	m_CenterPos = L"";

	UpdateData(FALSE);
}

HBRUSH ImageDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	int Ret = pWnd->GetDlgCtrlID();

	if (Ret == IDC_STATIC_TEXT) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void ImageDialog::CreateCircle(int nStartposX, int nStartposY, unsigned char* fm, int pitch)
{
	int nStartX = nStartposX - m_nDiameter / 2;
	int nStartY = nStartposY - m_nDiameter / 2;

	int nPosY = nStartY + m_nCircleMoveY;
	int nPosX = nStartX + m_nCircleMoveX;

	int n_Radius = m_nDiameter / 2;

	int n_CenterX = nStartposX + m_nCircleMoveX;
	int n_CenterY = nStartposY + m_nCircleMoveY;

	for (int y = nPosY; y < m_nDiameter + nPosY; y++) {
		for (int x = nPosX; x < m_nDiameter + nPosX; x++) {
			if (IsInnerCircle(x, y, n_Radius, n_CenterX, n_CenterY)){
				fm[y * pitch + x] = 255;
			}
		}
	}
}