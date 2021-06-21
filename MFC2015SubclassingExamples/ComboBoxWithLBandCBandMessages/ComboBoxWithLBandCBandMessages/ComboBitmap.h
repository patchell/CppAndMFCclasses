#pragma once

#include "MyListBox.h"

// CComboBitmap

class CComboBitmap : public CComboBox
{
	DECLARE_DYNAMIC(CComboBitmap)

public:
	CComboBitmap();
	virtual ~CComboBitmap();
	void SetSize(int width, int height);
	void OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected);
	int InsertBitmap(int nIndex, CBitmap *pBM, char *string);
	int AddBitmap(CBitmap *pBM, char *string);
	void DrawBitmap(CDC *pDC, CBitmap *pBM, CRect &rect);
	void DrawBitmap(CDC *pDC, CBitmap *pBM, CPoint &point);
	void EnableCheckbox(int nItem, int nFlag);
	int CheckBoxIsEnable(int nItem);
	void SetCheck(int nItem, int nFlag);
	int GetCheck(int nItem);
	int GetCheckBoxRect(int Item, CRect & rect);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void PreSubclassWindow();
	int m_nItemHeight;
	int m_nItemWidth;
	CMyListBox m_ListBox;
	virtual BOOL DestroyWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// Function gets the rectangle for the check box
};


