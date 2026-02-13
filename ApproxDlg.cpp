// ApproxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Approx.h"
#include "ApproxDlg.h"
#include <math.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ApproxDlg dialog
ApproxDlg::ApproxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ApproxDlg::IDD, pParent)
	, power(1)
	, otkl(0)
	, nev(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ApproxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, power);
	DDV_MinMaxInt(pDX, power, 0, 16);
	DDX_Text(pDX, IDC_EDIT1, otkl);
	DDX_Text(pDX, IDC_EDIT3, poly);
	DDX_Text(pDX, IDC_NEV, nev);
}

BEGIN_MESSAGE_MAP(ApproxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &ApproxDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// ApproxDlg message handlers

BOOL ApproxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	t = false;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void ApproxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ApproxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

		CWnd* frameWnd = GetDlgItem(ID_GRAF);
		CDC* frameDC = frameWnd->GetDC();
		CRect r;
		frameWnd->GetClientRect(&r1);
		frameDC->Rectangle(&r1);

		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldpen = frameDC->SelectObject(&pen);

		frameDC->MoveTo(0, r1.Height() / 2);
		frameDC->LineTo(r1.Width(), r1.Height() / 2);
		frameDC->MoveTo(5, 0);
		frameDC->LineTo(5, r1.Height());
		frameDC->SelectObject(oldpen);

		frameDC->TextOutW(7, 1, _T("y"));
		frameDC->TextOutW(r1.Width() - 12, r1.Height() / 2 + 3, _T("x"));

		pen.DeleteObject();
		if (t)
		{
			float a = 0;
			float b = 4;
			int N = 16;
			int n = power;

			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			oldpen = frameDC->SelectObject(&pen);
			brush.CreateSolidBrush(RGB(0, 0, 255));
			oldbrush = frameDC->SelectObject(&brush);

			//// Нарисуем точки///
			ApproxDlg::y = draw_points(N, a, b);
			ApproxDlg::Draw(frameDC, r1, ApproxDlg::y, N, a, b, 2, true);
			int width = r1.Width();

			frameDC->SelectObject(oldpen);
			frameDC->SelectObject(oldbrush);
			brush.DeleteObject();
			pen.DeleteObject();

			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			brush.CreateSolidBrush(RGB(0, 0, 0));
			oldpen = frameDC->SelectObject(&pen);
			oldbrush = frameDC->SelectObject(&brush);

			/// Нарисуем график ////
			ApproxDlg::gp = grpt(ApproxDlg::y, n, N, width, a, b, frameDC);
			//ApproxDlg::Draw(frameDC,r1,ApproxDlg::gp,width*2,a,b,1,false);

			frameDC->SelectObject(oldpen);
			frameDC->SelectObject(oldbrush);
			brush.DeleteObject();
			pen.DeleteObject();
			UpdateData(0);
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ApproxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void ApproxDlg::Draw(CDC* q, CRect r, float* y, int N, float c, float b, int d, bool s)
{
	float xmax = b;
	float ymax = y[0], ymin = y[0];
	for (int i = 1; i < N; i++)
	{
		if (y[i] > ymax) ymax = y[i];
		if (y[i] < ymin) ymin = y[i];
	}
	ymax = max(abs(ymax), abs(ymin));
	double coefX = (double)(r.Width() - 5 - d) / xmax;
	double coefY = (double)r.Height() / (2 * ymax);

	GkoefX = (double)(r.Width() - 5 - d) / xmax;
	GkoefY = (double)r.Height() / (2 * ymax);
	if (s)
	{
		Osi(q, r, coefY, coefX, ymax, xmax);
	}
	for (int i = 0; i < N; i++)
	{
		q->MoveTo((c + i * (b - c) / (N - 1)) * coefX + 5, -y[i] * coefY + r.Height() / 2);
		q->Ellipse(
			int((c + i * (b - c) / (N - 1)) * coefX - d + 5),
			int(-y[i] * coefY + r.Height() / 2 + d),
			int((c + i * (b - c) / (N - 1)) * coefX + d + 5),
			int(-y[i] * coefY + r.Height() / 2 - d));
	}
}
void ApproxDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	otkl = 0;
	CWnd* frameWnd = GetDlgItem(ID_GRAF);
	t = true;
	CWnd* mainDlgWnd = this;
	CDC* mainDlgDC = GetDC();
	CDC* frameDC = frameWnd->GetDC();
	CRect r;
	frameWnd->GetClientRect(&r);
	poly = new char[20 * power + 2];
	InvalidateRect(r);
}
float* ApproxDlg::draw_points(int N, float c, float b)
{
	FILE* f = fopen("data.txt", "wt");
	float* begpt = new float[N];
	double r = (double)rand() / RAND_MAX;
	int k;
	if (r > 0.5) k = 1;
	else k = 2;
	fprintf(f, "\t x\t\t|  y\n\n");
	for (int i = 0; i < N; i++)
	{
		double x = c + i * (b - c) / (N - 1);

		//begpt[i]=(x-pow(x,2)/2+pow(x,3)/3-pow(x,4)/4)*(1+k*r)/5;

		begpt[i] = (x - pow(x, 2) / 2 + pow(x, 3) / 3 - pow(x, 4) / 4) * (1 + pow((-1.), k) * r / 5);//	///// Функция
		fprintf(f, "%i\t| %f\t|  %f\n", i + 1, x, begpt[i]);
	}
	return begpt;
}
float** ApproxDlg::Create_matrix(int N, float* xpt, float* ypt, int n)
{
	float** A = new float* [n + 1];
	for (int i = 0; i < (n + 1); i++)
	{
		A[i] = new float[2 * n + 4];
	}
	for (int a = 0; a < (2 * n + 1); a++)
	{
		int i = 0;
		float s = 0;
		for (int q = 0; q < N; q++)
		{
			s = s + pow((xpt[q]), a);
		}
		if (a <= n)
		{
			for (int j = a; j >= 0; j--)
			{
				A[i][j] = s;
				i++;
			}
		}
		else
		{
			int t = a - n;
			int j = n;
			for (int i = t; i < n + 1; i++)
			{
				A[i][j] = s;
				j--;
			}
		}
	}
	for (int i = 0; i < n + 1; i++)
	{
		float s = 0;
		for (int j = 0; j < N; j++)
		{
			s = s + ypt[j] * pow((xpt[j]), i);
		}
		A[i][n + 1] = s;
	}
	for (int i = 0; i < n + 1; i++)
	{
		for (int j = 0; j < n + 2; j++)
		{
			A[i][j + n + 2] = A[i][j];
		}
	}
	return A;
}
float* ApproxDlg::slu_gauss(float** A, int n)
{
	float* resh = new float[n + 1];
	for (int k = 0; k < n; k++)
	{
		float A_max = abs(A[k][k]);
		int p = k;
		for (int i = k + 1; i < n + 1; i++)
		{
			if (A_max < abs(A[i][k]))
			{
				A_max = abs(A[i][k]);
				p = i;
			}
		}
		for (int j = k; j < n + 2; j++)
		{
			float c = A[k][j];
			A[k][j] = A[p][j];
			A[p][j] = c;
		}

		for (int i = k + 1; i < n + 1; i++)
		{
			for (int j = k + 1; j < n + 2; j++)
			{
				A[i][j] = A[i][j] - A[i][k] * A[k][j] / A[k][k];
			}
		}
	}
	resh[n] = A[n][n + 1] / A[n][n];
	for (int i = n - 1; i >= 0; i--)
	{
		float s = A[i][n + 1];
		for (int j = n; j >= i + 1; j--)
		{
			s = s - A[i][j] * resh[j];
		}
		resh[i] = s / A[i][i];
	}
	char str[10000] = " ";
	for (int i = 0; i < n + 1; i++)
	{
		float s = A[i][2 * n + 3];
		for (int j = n + 2; j < (2 * n + 3); j++)
		{
			s = s - A[i][j] * resh[j - n - 2];
		}
		char rr[1000];
		strcat(str, "  ");
		sprintf(rr, "%d", i);
		strcat(str, rr);
		strcat(str, ")");
		strcat(str, " ");
		sprintf(rr, "%8.6f", s);
		strcat(str, rr);
	}
	nev = CString(str);
	return resh;
}

float* ApproxDlg::grpt(float* y, int n, int N, int width, float a, float b, CDC* pDc)
{
	otkl = 0;
	float* x = new float[N];
	for (int i = 0; i < N; i++)
	{
		x[i] = a + i * (b - a) / (N - 1);
	}
	float** f = ApproxDlg::Create_matrix(N, x, y, n);
	float* c = ApproxDlg::slu_gauss(f, n);
	double* xpt = new double[(int)(GkoefX * width)];
	float* yn = new float[(int)(GkoefX * width)];

	for (int i = 1; i<int(GkoefX * r1.Width()); i++)
	{
		xpt[i] = (double)i / (double)(GkoefX * width) * 4;
		float sum = 0;
		for (int j = 0; j < n + 1; j++)
		{
			sum = sum + c[j] * pow(xpt[i], j);
		}
		yn[i] = sum;

		pDc->SetPixel(xpt[i] * GkoefX, r1.Height() / 2 - yn[i] * GkoefY, RGB(0, 20, 255));
		//pDc->LineTo(xpt[i]*GkoefX, r1.Height()/2-yn[i]*GkoefY);
	}
	for (int i = 0; i < N; i++)
	{
		double sum = 0;
		for (int j = 0; j < n + 1; j++)
		{
			sum = sum + c[j] * pow(x[i], j);
		}
		otkl += pow(sum - y[i], 2);
	}
	char str[1000] = " ";
	for (int i = 0; i < n + 1; i++)
	{
		if (c[i] == 0) continue;
		if (c[i] < 0) strcat(str, "-");
		else if (i > 0) strcat(str, "+");
		char coef[10];
		sprintf(coef, "%7.4f", abs(c[i]));
		strcat(str, coef);
		if (i == 0) continue;
		strcat(str, "*x");
		if (i == 1) continue;
		strcat(str, "^");
		sprintf(coef, "%d", i);
		strcat(str, coef);
	}
	poly = CString(str);
	delete[] x;
	delete[] f;
	delete[] c;
	delete[] xpt;
	return yn;
}
void ApproxDlg::Osi(CDC* pDc, CRect r, double coefy, double coefx, float ymax, float xmax)
{
	CPen pen, * oldpen;
	CFont font, * oldfont;
	pen.CreatePen(2, 1, RGB(170, 170, 170));
	oldpen = pDc->SelectObject(&pen);
	font.CreateFontW(9, 5, 3, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	oldfont = pDc->SelectObject(&font);
	//pa->MoveTo(0,r.Height()/2);

	for (int i = 1; i < 10; i++)//горизонтальная линия
	{
		pDc->MoveTo(0, r.Height() / 2 + ymax * i * coefy / 10.);
		pDc->LineTo(r.Width(), r.Height() / 2 + ymax * i * coefy / 10.);
		char s[20];
		float ch = -ymax * i / 10;
		sprintf(s, "%2.1f", ch);
		pDc->TextOut(10, r.Height() / 2 + ymax * i * coefy / 10 - 7., CString(s));

		ch = ymax * i / 10;
		pDc->MoveTo(0, r.Height() / 2 - ymax * i * coefy / 10.);
		pDc->LineTo(r.Width(), r.Height() / 2 - ymax * i * coefy / 10.);
		sprintf(s, "%2.1f", ch);
		pDc->TextOut(10, r.Height() / 2 - ymax * i * coefy / 10 - 7., CString(s));
	}
	for (int i = 1; i < 10; i += 2)//вертикальная линия
	{
		pDc->MoveTo(5 + xmax * i * coefx / 10., 0);
		pDc->LineTo(5 + xmax * i * coefx / 10., r.Height());
		char s[4];
		float ch = xmax * i / 10;
		sprintf(s, "%2.1f", ch);
		pDc->TextOut(5 + xmax * i * coefx / 10. + 2, r.Height() / 2 + 1, CString(s));
	}

	pDc->SelectObject(oldpen);
	pen.DeleteObject();
}

void ApproxDlg::DrawToPoint(CDC* pDc, CRect r, double x_point, double y_point)
{
	pDc->SetPixel(x_point, y_point, RGB(255, 0, 0));
}