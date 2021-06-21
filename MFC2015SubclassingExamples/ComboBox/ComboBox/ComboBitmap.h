#pragma once


// CComboBitmap

class CComboBitmap : public CComboBox
{
	DECLARE_DYNAMIC(CComboBitmap)
public:
	int m_nItemWidth;
	int m_nItemHeight;
public:
	CComboBitmap();
	virtual ~CComboBitmap();
	void SetSize(int width, int height);
	void OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected);
	int InsertBitmap(int nIndex, CBitmap *pBM, char *string);
	void DrawBitmap(CDC *pDC, CBitmap *pBM, CRect &rect);
	void DrawBitmap(CDC *pDC, CBitmap *pBM, CPoint &point);
	int AddBitmap(CBitmap *pBM, char *string);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void PreSubclassWindow();
};


