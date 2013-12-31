#pragma once

#include <gdiplus.h>
#include "PictureEx.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace Gdiplus;

class CEightQueensDlg : public CDialog {

	//- - - - - данные алгоритма перебора
	#define		CELLS 8				// ширина/высота доски и кол-во ферзей
	bool		A[CELLS];			// пуста ли i-ная горизонталь на доске
	bool		B[2*CELLS - 1];		// пуста ли i-ная диагональ(справа-налево /)
	bool		C[2*CELLS - 1];		// пуста ли i-ная диагональ(слева-направо \)
	int			X[CELLS];			// положение ферзя на i-ой вертикали
	Point		Pos[92][CELLS];		// решения задачи
	int			ShowingPos;			// какое из 92ух решений сейчас показывается (-1 - никакое)

	//- - - - - Отрисовка
	#define		FERZ_PIC L"ferz.PNG"
	#define		TRANSLATE 10
	Bitmap		*pFerzPic;
	ULONG_PTR	GdiPlusToken;
	Rect		DrawRect;			// в нем рисуется шахматная доска
	Rect		Cells[CELLS][CELLS];// массив клеток доски

	void DrawBoard(HDC hdc);
	void DrawPos(Graphics &gr);
	void DrawLines(Graphics &gr);
	Rect WinRectToGdi(RECT &rect);
	void Calc(int i);

	// - - - - - - - - - - - - - - - - 
public:
	CEightQueensDlg(CWnd* pParent = NULL);
	~CEightQueensDlg();

	enum { IDD = IDD_EIGHTQUEENS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CSpinButtonCtrl spinPos;
	CEdit editPos;
	afx_msg void OnEnChangeEditPos();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CButton chekLines;
	afx_msg void OnBnClickedCheckLines();
	CPictureEx picLogo;
};
