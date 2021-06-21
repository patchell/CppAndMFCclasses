#if !defined(AFX_PLOT_H__AD63FFCD_59C7_4A43_9F7A_B622C8A5FD7B__INCLUDED_)
#define AFX_PLOT_H__AD63FFCD_59C7_4A43_9F7A_B622C8A5FD7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Plot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlot window

class CPlot : public CStatic
{
// Construction
public:
	CPlot();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlot)
	//}}AFX_VIRTUAL

// Implementation
public:
	int MaxY;
	CStatic * Debug;
	void InitPlot(void);
	int nData;
	int bsize;
	int tail;
	int head;
	int * Data;
	CPen BlackPen;
	void Test(CStatic *s);
	void AddPoint(int x, int y);
	virtual ~CPlot();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlot)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOT_H__AD63FFCD_59C7_4A43_9F7A_B622C8A5FD7B__INCLUDED_)
