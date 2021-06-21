// ComboBoxItem.h: interface for the CComboBoxItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBOBOXITEM_H__1AB9B149_7B76_4789_88CC_EA788542FCB4__INCLUDED_)
#define AFX_COMBOBOXITEM_H__1AB9B149_7B76_4789_88CC_EA788542FCB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComboBoxItem  
{
public:
	int m_nCheckboxChecked;
	int m_nCheckboxEnabled;
	CBitmap * m_pbmBitmap;
	CComboBoxItem();
	virtual ~CComboBoxItem();

};

#endif // !defined(AFX_COMBOBOXITEM_H__1AB9B149_7B76_4789_88CC_EA788542FCB4__INCLUDED_)
