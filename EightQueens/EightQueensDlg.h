#pragma once

#include <gdiplus.h>
#include "PictureEx.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace Gdiplus;

class CEightQueensDlg : public CDialog {

	//- - - - - ������ ��������� ��������
	#define		CELLS 8				// ������/������ ����� � ���-�� ������
	bool		A[CELLS];			// ����� �� i-��� ����������� �� �����
	bool		B[2*CELLS - 1];		// ����� �� i-��� ���������(������-������ /)
	bool		C[2*CELLS - 1];		// ����� �� i-��� ���������(�����-������� \)
	int			X[CELLS];			// ��������� ����� �� i-�� ���������
	Point		Pos[92][CELLS];		// ������� ������
	int			ShowingPos;			// ����� �� 92�� ������� ������ ������������ (-1 - �������)

	//- - - - - ���������
	#define		FERZ_PIC L"ferz.PNG"
	#define		TRANSLATE 10
	Bitmap		*pFerzPic;
	ULONG_PTR	GdiPlusToken;
	Rect		DrawRect;			// � ��� �������� ��������� �����
	Rect		Cells[CELLS][CELLS];// ������ ������ �����

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
