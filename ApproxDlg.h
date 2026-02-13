// Аппроксимация с графикойDlg.h : header file
//

#pragma once
#include <string>

// ApproxDlg dialog
class ApproxDlg : public CDialog
{
	// Construction
public:
	ApproxDlg(CWnd* pParent = NULL);	// standard constructor

	bool t;
	float* y;
	float* gp;

	CPen pen, * oldpen;
	CBrush brush, * oldbrush;
	//CPen pen,*oldpen;

// Dialog Data
	enum { IDD = IDD_MY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
	void Draw(CDC*, CRect, float*, int, float, float, int, bool);
	float* draw_points(int, float, float);
	float* grpt(float*, int, int, int, float, float, CDC*);
	float** Create_matrix(int, float*, float*, int);
	float* slu_gauss(float**, int);
	void Osi(CDC*, CRect, double, double, float, float);
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void DrawToPoint(CDC* pDc, CRect r, double x_point, double y_point);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int power;
	float otkl;
	CString poly;
	CString nev;
	CRect r1;
	double GkoefX;
	double GkoefY;
};
