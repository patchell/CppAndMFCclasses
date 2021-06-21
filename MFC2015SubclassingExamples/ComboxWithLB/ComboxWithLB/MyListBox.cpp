// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ComboxWithLB.h"
#include "MyListBox.h"
#include "ComboBitmap.h"

// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{

}

CMyListBox::~CMyListBox()
{
//	UnsubclassWindow();
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
//	ON_WM_NCDESTROY()
END_MESSAGE_MAP()



// CMyListBox message handlers

void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

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
		m_pComboBox->OutputBitmap(lpDrawItemStruct, true);
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
		m_pComboBox->OutputBitmap(lpDrawItemStruct, false);
		//clean up DC
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// Focus
	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}
