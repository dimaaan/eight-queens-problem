#include "stdafx.h"
#include "EightQueens.h"
#include "EightQueensDlg.h"
#include ".\eightqueensdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//-----------------------------------------------------------------------------------------
CEightQueensDlg::CEightQueensDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEightQueensDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CEightQueensDlg::~CEightQueensDlg() {}
//-----------------------------------------------------------------------------------------
void CEightQueensDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_POS, spinPos);
	DDX_Control(pDX, IDC_EDIT_POS, editPos);
	DDX_Control(pDX, IDC_CHECK_LINES, chekLines);
	DDX_Control(pDX, IDC_LOGO, picLogo);
}
//-----------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CEightQueensDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	//	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POS, OnDeltaposSpinPos)
	ON_EN_CHANGE(IDC_EDIT_POS, OnEnChangeEditPos)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_LINES, OnBnClickedCheckLines)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------
BOOL CEightQueensDlg::OnInitDialog() {
	int x = 0, y = 0, CellWidth, CellHeight, i;
	GdiplusStartupInput si;

	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	picLogo.Load("l.dat");
	picLogo.Draw();

	GdiplusStartup(&GdiPlusToken, &si, NULL);
	pFerzPic = ::new Bitmap(L"ferz.PNG");
	ShowingPos = -1;
	spinPos.SetBuddy(&editPos);

	DrawRect.X		= 10; 
	DrawRect.Y		= DrawRect.X; 
	DrawRect.Width	= 400;
	DrawRect.Height = DrawRect.Width;

	CellWidth  = DrawRect.Width  / CELLS;
	CellHeight = DrawRect.Height / CELLS;

	for(i = 0; i < CELLS; i++) {
		for(int i2 = 0; i2 < CELLS; i2++) {
			Cells[i][i2].X = x;
			Cells[i][i2].Y = y;
			Cells[i][i2].Width = CellWidth;
			Cells[i][i2].Height = CellHeight;
			y += CellHeight;
		}
		y = 0;
		x += CellWidth;
	}

	for(i = 0; i < 8;  i++) A[i] = X[i] = true;
	for(i = 0; i < 15; i++) B[i] = C[i] = true;

	Calc(0);

	return TRUE;
}
//-----------------------------------------------------------------------------------------
void CEightQueensDlg::OnDestroy() {
	CDialog::OnDestroy();
	GdiplusShutdown(GdiPlusToken);
}
//-----------------------------------------------------------------------------------------
void CEightQueensDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CPaintDC dc(this);
		CDC BackBuf;
		CBitmap bmp;
		CRect ClRect;

		BackBuf.CreateCompatibleDC(&dc);
		GetClientRect(&ClRect);
		bmp.CreateCompatibleBitmap(&dc, ClRect.Width(), ClRect.Height());
		BackBuf.SelectObject(&bmp);
		DrawBoard(BackBuf.m_hDC);
		dc.BitBlt(0, 0, ClRect.Width(), ClRect.Height(), &BackBuf, 0, 0, SRCCOPY);
	}
}
//-----------------------------------------------------------------------------------------
void CEightQueensDlg::OnEnChangeEditPos() {
	CString s;
	int tmp;

	editPos.GetWindowText(s);
	tmp = atoi((LPCTSTR) s);
	if(tmp >= 92) {
		ShowingPos = -1;
		this->RedrawWindow();
		return;
	}
	ShowingPos = tmp;
	this->RedrawWindow();
}
//-----------------------------------------------------------------------------------------
HCURSOR CEightQueensDlg::OnQueryDragIcon()		{return static_cast<HCURSOR>(m_hIcon);}
BOOL	CEightQueensDlg::OnEraseBkgnd(CDC* pDC)	{return true;}
void	CEightQueensDlg::OnBnClickedCheckLines(){RedrawWindow();}
//-----------------------------------------------------------------------------------------
// конвертирует структуру RECT в Gdiplus::Rect
Rect CEightQueensDlg::WinRectToGdi(RECT &rect) {
	Rect res;

	res.X = rect.left;
	res.Y = rect.top;
	res.Width = abs(rect.right - rect.left);
	res.Height = abs(rect.bottom - rect.top);
	return res;
}
//-----------------------------------------------------------------------------------------
// рисует шахматную доску и все, что на ней
void CEightQueensDlg::DrawBoard(HDC hdc) {
	Graphics gr(hdc);
	SolidBrush BlackBrush(Color(0,0,0)),
				WhiteBrush(Color(255,255,255));
	GraphicsPath FramePath;
	SolidBrush FrameBrush(Color(120,140,160));
	RECT ClientRect;
	bool Black = true;
	
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	this->GetClientRect(&ClientRect);
	LinearGradientBrush BackBrush(WinRectToGdi(ClientRect), Color(200,200,200), 
		Color(100,100,100), LinearGradientModeVertical);
	gr.FillRectangle(&BackBrush, WinRectToGdi(ClientRect));

	// рисуем рамку
	FramePath.StartFigure();
	FramePath.AddLine(DrawRect.X, DrawRect.Y, 
		DrawRect.X + DrawRect.Width, DrawRect.Y);
	FramePath.AddLine(DrawRect.X + DrawRect.Width, 
		DrawRect.Y, DrawRect.X + DrawRect.Width, DrawRect.Y + DrawRect.Height);
	FramePath.AddLine(DrawRect.X + DrawRect.Width, 
		DrawRect.Y + DrawRect.Height, DrawRect.X, DrawRect.Y + DrawRect.Height);
	FramePath.CloseFigure();
	gr.TranslateTransform((REAL) TRANSLATE, (REAL) TRANSLATE);
	gr.FillPath(&FrameBrush, &FramePath);
	gr.TranslateTransform(-TRANSLATE, -TRANSLATE);
	gr.DrawPath(&Pen(Color(0,0,0), 4), &FramePath);
	gr.TranslateTransform((REAL) TRANSLATE, (REAL) TRANSLATE);

	// рисуем клетки
	for(int i = 0; i < CELLS; i++) {
		for(int i2 = 0; i2 < CELLS; i2++) {
			Black ? gr.FillRectangle(&BlackBrush, Cells[i][i2]) : 
					gr.FillRectangle(&WhiteBrush, Cells[i][i2]);
			Black = !Black;
		}
		Black = !Black;
	}

	// рисуем ферзей
	if(ShowingPos >= 0)
		DrawPos(gr);

	// рисуем линии
	if(chekLines.GetCheck() == BST_CHECKED)
		DrawLines(gr);
}
//-----------------------------------------------------------------------------------------
// рисует на доске решение № ShowingPos
void CEightQueensDlg::DrawPos(Graphics &gr) {
	for(int i = 0; i < CELLS; i++) {
		gr.DrawImage(pFerzPic, Cells[Pos[ShowingPos][i].X][Pos[ShowingPos][i].Y]);
	}
}
//-----------------------------------------------------------------------------------------
// рисует линии по которым ходят ферзи
void CEightQueensDlg::DrawLines(Graphics &gr) {
	if(ShowingPos == -1) return;

	Pen LinePen(Color(128, 255,0,0), 3);
	Rect *pR = NULL;
	int x = 0, y = 0,
		HalfW = DrawRect.Width / CELLS / 2,
		HalfH = DrawRect.Height / CELLS / 2,
		DrawRLeft = DrawRect.X + DrawRect.Width - TRANSLATE,
		DrawRTop = DrawRect.Y - TRANSLATE,
		DrawRBottom = DrawRect.Y + DrawRect.Height - TRANSLATE;
	
	LinePen.SetDashStyle(DashStyleDot);
	for(int i = 0; i < CELLS; i++) {
		pR = &Cells[Pos[ShowingPos][i].X][Pos[ShowingPos][i].Y];
		gr.DrawLine(&LinePen, pR->X, pR->Y + HalfH, 0, pR->Y + HalfH);
		gr.DrawLine(&LinePen, pR->X + pR->Width, pR->Y + HalfH, DrawRLeft, pR->Y + HalfH);
		gr.DrawLine(&LinePen, pR->X + pR->Width/2, pR->Y, pR->X + pR->Width / 2, DrawRTop);
		gr.DrawLine(&LinePen, pR->X + pR->Width/2, 
			pR->Y + pR->Width, pR->X + pR->Width/2, DrawRBottom);
		x = Pos[ShowingPos][i].X;
		y = Pos[ShowingPos][i].Y;
		while(x > 0 && y > 0) { x--; y--; }
		gr.DrawLine(&LinePen, pR->X, pR->Y, Cells[x][y].X, Cells[x][y].Y);
		x = Pos[ShowingPos][i].X;
		y = Pos[ShowingPos][i].Y;
		while(x < CELLS - 1 && y > 0) { x++; y--;}
		gr.DrawLine(&LinePen, pR->X + pR->Width, pR->Y, 
			Cells[x][y].X + pR->Width, Cells[x][y].Y);
		x = Pos[ShowingPos][i].X;
		y = Pos[ShowingPos][i].Y;
		while(x < CELLS - 1 && y < CELLS - 1) { x++; y++;}
		gr.DrawLine(&LinePen, pR->X + pR->Width, pR->Y + pR->Height, 
			Cells[x][y].X + pR->Width, Cells[x][y].Y + Cells[x][y].Height);
		x = Pos[ShowingPos][i].X;
		y = Pos[ShowingPos][i].Y;
		while(x > 0 && y < CELLS - 1) { x--; y++;}
		gr.DrawLine(&LinePen, pR->X, pR->Y + pR->Height, 
			Cells[x][y].X, Cells[x][y].Y + Cells[x][y].Height);
	}
}
//-----------------------------------------------------------------------------------------
/* находит нужные позиции ферзей используя перебор с возвратами.
Позиции записываются в массив Pos[][] */
void CEightQueensDlg::Calc(int i) {
	int j;
	static int t = 0;
	const int INDEX_SHIFT = CELLS - 1;

	for(j = 0; j < CELLS; j++) { // перебираем все клетки на i-ой горизонтали
		if(!(A[j] && B[i+j] && C[i-j+INDEX_SHIFT]))
			continue; // клетка (i;j) "простреливается" другими ферзями
		// ставим ферзя в позицию (i;j)
		X[i] = j;
		A[j] = B[i+j] = C[i-j+INDEX_SHIFT] = false;
		if(i < CELLS - 1)
			Calc(i + 1); // ставим следующего ферзя
		else {
			for(int k = 0; k < CELLS; k++) {
				Pos[t][k].X = X[k];
				Pos[t][k].Y = k;
			}
			t++;
		}
		/* попытка расстановки провалена. Т.к. следующего ферзя ставить некуда.
		Убираем ферзя с доски и переходим на следующую итерацию, попробуя поставить
		ферзя на горизонталь ниже */
		A[j] = B[i+j] = C[i-j+INDEX_SHIFT] = true;
	}
}