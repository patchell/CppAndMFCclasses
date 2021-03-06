#pragma once


// CMyListBox
class CComboBitmap;

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	CComboBitmap *m_pComboBox;
//	virtual BOOL DestroyWindow();
//	afx_msg void OnNcDestroy();
};


