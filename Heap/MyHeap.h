// MyHeap.h: interface for the CMyHeap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYHEAP_H__2C6BFC36_90F5_400B_86E9_2AB0D1C9E847__INCLUDED_)
#define AFX_MYHEAP_H__2C6BFC36_90F5_400B_86E9_2AB0D1C9E847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DisplayStatic.h"
#include "Lock.h"

struct __freelist {
	size_t sz;
	struct __freelist *nx;
};

class CMyHeap  
{
	char *Start;		//start of heap
	char *End;			//end of heap
	char *BrkVal;		//beginning of heap
	size_t Margin;
	struct __freelist *__flp;
public:
	CDMLock * m_pBlocker;
	CDisplayStatic * pDB;
	char * m_pHeap;
	unsigned HeapSizeFree(void);
	void Free(void *pMem);
	void * Malloc(unsigned size);
	CMyHeap(unsigned size);
	virtual ~CMyHeap();

};

#endif // !defined(AFX_MYHEAP_H__2C6BFC36_90F5_400B_86E9_2AB0D1C9E847__INCLUDED_)
