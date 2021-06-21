
// ComboBoxDlg.h : header file
//

#pragma once
#include "ComboBitmap.h"


// CComboBoxDlg dialog
class CComboBoxDlg : public CDialogEx
{
// Construction
public:
	CComboBoxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMBOBOX_DIALOG };
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
	CBitmap m_Bm1;
	CBitmap m_Bm2;
	CBitmap m_Bm3;
public:
	// Our Subclassed Combox
	CComboBitmap m_ComboBox;
};
