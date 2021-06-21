
// ComboBoxwithLBandCheckBoxDlg.h : header file
//

#pragma once
#include "ComboBitmap.h"


// CComboBoxwithLBandCheckBoxDlg dialog
class CComboBoxwithLBandCheckBoxDlg : public CDialog
{
// Construction
public:
	CComboBoxwithLBandCheckBoxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMBOBOXWITHLBANDCHECKBOX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBitmap m_ComboBox;
	CBitmap m_Bm1;
	CBitmap m_Bm2;
	CBitmap m_Bm3;
};
