// Plot.cpp : implementation file
//

#include "stdafx.h"
#include "Ba04Host.h"
#include "Plot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlot

CPlot::CPlot()
{
	BlackPen.CreatePen(PS_SOLID,1,COLORREF(0x00ff0000));
	Debug = 0;
	Data = 0;
}

CPlot::~CPlot()
{
}


BEGIN_MESSAGE_MAP(CPlot, CStatic)
	//{{AFX_MSG_MAP(CPlot)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlot message handlers

void CPlot::AddPoint(int x, int y)
{
	if(nData < bsize)
	{
		Data[head] = y + MaxY/2;
		++head;
		++nData;
		if(head == bsize)head = 0;
	}
	else
	{
		++tail;
		if(tail == bsize) tail = 0;
		Data[head++] = y + MaxY/2;
		if(head == bsize) head = 0;
	}
	if(Debug)
	{
//		char *s = new char[256];
//		sprintf(s,"New Data = %d\n",y);
//		Debug->SetWindowText(s);
//		delete[] s;
	}
	Invalidate();
}

void CPlot::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RECT clientrect;
	int x,y;

	GetClientRect(&clientrect);
	x = clientrect.right;
	y = clientrect.bottom;
	CBitmap BitmapPlot;		//this is where we will draw the histogram
	BitmapPlot.CreateCompatibleBitmap (&dc, x, y);

	CDC dcMem;
	dcMem.CreateCompatibleDC (&dc);

	
	CBrush brush (RGB (255, 255, 255));
	CBitmap* pOldBitmap = dcMem.SelectObject (&BitmapPlot);
	dcMem.FillRect (CRect (0, 0, x, y), &brush);
	CPen *OldPen = dcMem.SelectObject(&BlackPen);
	if(nData > 2)
	{
		int i=0;
		int index = tail;

		dcMem.MoveTo(i,Data[index++]);
		if(index == bsize) index = 0;
		for(i=1;i<nData;++i)
		{
			dcMem.LineTo(i,Data[index++]);
			if(index == bsize) index = 0;
		}
	}
	dc.BitBlt(0,0,x,y,&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(OldPen);
	dcMem.SelectObject (pOldBitmap);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CPlot::Test(CStatic *s)
{
	Debug = s;
}

void CPlot::InitPlot()
{
	RECT clientrect;
	GetClientRect(&clientrect);

	Data = new int[clientrect.right];
	bsize = clientrect.right;
	head = 0;
	tail = 0;
	nData = 0;
	MaxY = clientrect.bottom;
}
