// ComboBitmap.cpp : implementation file
//

#include "stdafx.h"
#include "ComboxWithLB.h"
#include "MyListBox.h"
#include "ComboBitmap.h"


// CComboBitmap

IMPLEMENT_DYNAMIC(CComboBitmap, CComboBox)

CComboBitmap::CComboBitmap()
{

	m_nItemHeight = 0;
	m_nItemWidth = 0;
}

CComboBitmap::~CComboBitmap()
{
}


BEGIN_MESSAGE_MAP(CComboBitmap, CComboBox)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CComboBitmap message handlers

void CComboBitmap::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if (!IsWindowEnabled())
	{
		CBrush brDisabled(RGB(192, 192, 192)); // light gray
		CBrush* pOldBrush = pDC->SelectObject(&brDisabled);
		CPen penDisabled(PS_SOLID, 1, RGB(192, 192, 192));
		CPen* pOldPen = pDC->SelectObject(&penDisabled);
		OutputBitmap(lpDrawItemStruct, false);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		return;
	}
	// Selected
	if ((lpDrawItemStruct->itemState & ODS_SELECTED)
		&& (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brHighlight(::GetSysColor(COLOR_HIGHLIGHT));
		CBrush* pOldBrush = pDC->SelectObject(&brHighlight);
		CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
		CPen* pOldPen = pDC->SelectObject(&penHighlight);
		pDC->Rectangle(&lpDrawItemStruct->rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		OutputBitmap(lpDrawItemStruct, true);
		//clean up device context
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// De-Selected
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED)
		&& (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brWindow(::GetSysColor(COLOR_WINDOW));
		CBrush* pOldBrush = pDC->SelectObject(&brWindow);
		CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
		CPen* pOldPen = pDC->SelectObject(&penHighlight);
		pDC->Rectangle(&lpDrawItemStruct->rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		OutputBitmap(lpDrawItemStruct, false);
		//clean up DC
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// Focus
	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CComboBitmap::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth = (m_nItemWidth + 2);
	lpMeasureItemStruct->itemHeight = (m_nItemHeight + 2);
}


void CComboBitmap::PreSubclassWindow()
{
	COMBOBOXINFO Info;

	// TODO: Add your specialized code here and/or call the base class
	
	ASSERT(GetStyle() & CBS_DROPDOWNLIST);
	ASSERT(GetStyle() & CBS_OWNERDRAWVARIABLE);
	ASSERT(GetStyle() & CBS_HASSTRINGS);
}


void CComboBitmap::DrawBitmap(CDC *pDC, CBitmap *pBM, CPoint &point)
{
	BITMAP bm;
	pBM->GetBitmap(&bm);
	int w = bm.bmWidth;
	int h = bm.bmHeight;
	CDC memDC; memDC.CreateCompatibleDC(pDC);
	CBitmap *pBmpOld = memDC.SelectObject(pBM);
	pDC->BitBlt(point.x, point.y, w, h, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpOld);
}

void CComboBitmap::DrawBitmap(CDC *pDC, CBitmap *pBM, CRect &rect)
{
	BITMAP bm;
	pBM->GetBitmap(&bm);
	int w = bm.bmWidth;
	int h = bm.bmHeight;
	CPoint point;
	point.x = rect.left + ((rect.right - rect.left) / 2) - (w / 2);
	point.y = rect.top + ((rect.bottom - rect.top) / 2) - (h / 2);
	DrawBitmap(pDC, pBM, point);
}

int CComboBitmap::AddBitmap(CBitmap *pBM, char *string)
{
	return InsertBitmap(CComboBox::GetCount(), pBM, string);
}

int CComboBitmap::InsertBitmap(int nIndex, CBitmap *pBM, char *string)
{
	int n = CComboBox::InsertString(nIndex, string ? CString(string) : CString("") );
	if (n != CB_ERR && n != CB_ERRSPACE)
	{
		SetItemData(n, (DWORD)pBM);
		BITMAP bm;
		pBM->GetBitmap(&bm);
		SetSize(bm.bmWidth, bm.bmHeight);
	}
	return n;
}

void CComboBitmap::OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected)
{
	CBitmap *pBM = (CBitmap*)(lpDIS->itemData);
	if (pBM && pBM != (const CBitmap *)(0xffffffff))
	{
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CString string;
		//----------------------------------------------
		// If the listbox isn't  empty (itemID == -1)
		// AND if we are not drawing the editbox 
		// portion of the ComboBox, then get the string
		// if any, to display
		//----------------------------------------------
		if ((lpDIS->itemID != -1) && !(lpDIS->itemState & ODS_COMBOBOXEDIT))
			GetLBText(lpDIS->itemID, string);
		if (string.IsEmpty())
			DrawBitmap(pDC, pBM, CRect(lpDIS->rcItem));
		else
		{
			CPoint point;
			point.x = lpDIS->rcItem.left + 2;
			point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2);
			DrawBitmap(pDC, pBM, point);
			CRect rcText(lpDIS->rcItem);
			rcText.DeflateRect(m_nItemWidth + 4, 0, 0, 0);
			pDC->DrawText(string, rcText, DT_SINGLELINE | DT_VCENTER);
		}
	}
}

void CComboBitmap::SetSize(int width, int height)
{
	if (width > m_nItemWidth)
		m_nItemWidth = width;
	if (height > m_nItemHeight)
		m_nItemHeight = height;
	for (int i = -1; i < GetCount(); i++)
		SetItemHeight(i, m_nItemHeight + 6);
}


BOOL CComboBitmap::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_ListBox.UnsubclassWindow();
	return CComboBox::DestroyWindow();
}


HBRUSH CComboBitmap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	//------------------------------------------------
	// This is where we subclass the controls we want
	// to subclass.
	//-------------------------------------------------
	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		if (m_ListBox.GetSafeHwnd() == NULL)
		{
			m_ListBox.m_pComboBox = this;
			m_ListBox.SubclassWindow(pWnd->GetSafeHwnd());
		}
		break;
	case CTLCOLOR_EDIT:
		break;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
