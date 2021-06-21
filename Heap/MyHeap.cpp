/**************************************************************************
** This file is used to manage heaps
**
** Created July 2, 2008 by Jim Patchell
**
** Now, this is derived from malloc and free that comes with WinAVR and
** following is the copyright notice that was in THAT code.  It is mostly
** unmodified.
** -----------------------------------------------
Copyright (c) 2002, 2004 Joerg Wunsch
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
** -----------------------------------------------------------------------
** Code first modified on July 2, 2008
**************************************************************************/
// MyHeap.cpp: implementation of the CMyHeap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyHeap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyHeap::CMyHeap(unsigned size)
{
	m_pHeap = new char[size];		//allocate memory used for heap
	Start = m_pHeap;
	End = &Start[size-1];
	BrkVal = Start;
	Margin = 32;		//default value for stack margin...not needed..really
	__flp = 0;			//free list is empty
	m_pBlocker = new CDMLock(CString("MyHeapBlocker"));
}

CMyHeap::~CMyHeap()
{
	delete[] m_pHeap;		//return this heap to the real heap
}

void * CMyHeap::Malloc(unsigned int len)
{
	struct __freelist *fp1, *fp2;
	char *cp;
	size_t s, avail;

	/*
	 * Our minimum chunk size is the size of a pointer (plus the
	 * size of the "sz" field, but we don't need to account for
	 * this), otherwise we could not possibly fit a freelist entry
	 * into the chunk later.
	 */
	m_pBlocker->Lock();
	if (len < sizeof(struct __freelist) - sizeof(size_t))
		len = sizeof(struct __freelist) - sizeof(size_t);

	/*
	 * First, walk the free list and try finding a chunk that
	 * would match exactly.  If we found one, we are done.  While
	 * walking, note down the size of the largest chunk we found
	 * that would still fit the request -- we need it for step 2.
	 *
	 */
	for (s = 0, fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1->sz == len) {
			/*
			 * Found it.  Disconnect the chunk from the
			 * freelist, and return it.
			 */
			if (fp2)
				fp2->nx = fp1->nx;
			else
				__flp = fp1->nx;
			m_pBlocker->Unlock();
			return &(fp1->nx);
		}
		if (fp1->sz > len) {
			if (s == 0 || fp1->sz < s)
				s = fp1->sz;
		}
	}
	/*
	 * Step 2: If we found a chunk on the freelist that would fit
	 * (but was too large), look it up again and use it, since it
	 * is our closest match now.  Since the freelist entry needs
	 * to be split into two entries then, watch out that the
	 * difference between the requested size and the size of the
	 * chunk found is large enough for another freelist entry; if
	 * not, just enlarge the request size to what we have found,
	 * and use the entire chunk.
	 */
	if (s) {
		if (s - len < sizeof(struct __freelist))
			len = s;
		for (fp1 = __flp, fp2 = 0;
		     fp1;
		     fp2 = fp1, fp1 = fp1->nx) {
			if (fp1->sz == s) {
				if (len == s) {
					/*
					 * Use entire chunk; same as
					 * above.
					 */
					if (fp2)
						fp2->nx = fp1->nx;
					else
						__flp = fp1->nx;
					m_pBlocker->Unlock();
					return &(fp1->nx);
				}
				/*
				 * Split them up.  Note that we leave
				 * the first part as the new (smaller)
				 * freelist entry, and return the
				 * upper portion to the caller.  This
				 * saves us the work to fix up the
				 * freelist chain; we just need to
				 * fixup the size of the current
				 * entry, and note down the size of
				 * the new chunk before returning it
				 * to the caller.
				 */
				cp = (char *)fp1;
				s -= len;
				cp += s;
				fp2 = (struct __freelist *)cp;
				fp2->sz = len;
				fp1->sz = s - sizeof(size_t);
				m_pBlocker->Unlock();
				return &(fp2->nx);
			}
		}
	}
	/*
	 * Step 3: If the request could not be satisfied from a
	 * freelist entry, just prepare a new chunk.  This means we
	 * need to obtain more memory first.  The largest address just
	 * not allocated so far is remembered in the brkval variable.
	 * Under Unix, the "break value" was the end of the data
	 * segment as dynamically requested from the operating system.
	 * Since we don't have an operating system, just make sure
	 * that we don't collide with the stack.
	 */
	if (BrkVal == 0)
		BrkVal = Start;
	cp = End;
	avail = cp - BrkVal;
	/*
	 * Both tests below are needed to catch the case len >= 0xfffe.
	 */
	if (avail >= len && avail >= len + sizeof(size_t)) {
		fp1 = (struct __freelist *)BrkVal;
		BrkVal += len + sizeof(size_t);
		fp1->sz = len;
		m_pBlocker->Unlock();
		return &(fp1->nx);
	}
	/*
	 * Step 4: There's no help, just fail. :-/
	 */
	m_pBlocker->Unlock();
	return 0;
}

void CMyHeap::Free(void *p)
{
	struct __freelist *fp1, *fp2, *fpnew;
	char *cp1, *cp2, *cpnew;

	/* ISO C says free(NULL) must be a no-op */
	if (p == 0)
		return;

	m_pBlocker->Lock();
	cpnew = (char *) p;
	cpnew -= sizeof(size_t);
	fpnew = (struct __freelist *)cpnew;
	fpnew->nx = 0;

	/*
	 * Trivial case first: if there's no freelist yet, our entry
	 * will be the only one on it.
	 */
	if (__flp == 0) {
		__flp = fpnew;
		m_pBlocker->Unlock();
		return;
	}

	/*
	 * Now, find the position where our new entry belongs onto the
	 * freelist.  Try to aggregate the chunk with adjacent chunks
	 * if possible.
	 */
	for (fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1 < fpnew)
			continue;
		cp1 = (char *)fp1;
		fpnew->nx = fp1;
		if ((char *)&(fpnew->nx) + fpnew->sz == cp1) {
			/* upper chunk adjacent, assimilate it */
			fpnew->sz += fp1->sz + sizeof(size_t);
			fpnew->nx = fp1->nx;
		}
		if (fp2 == 0) {
			/* new head of freelist */
			__flp = fpnew;
			m_pBlocker->Unlock();
			return;
		}
		break;
	}
	/*
	 * Note that we get here either if we hit the "break" above,
	 * or if we fell off the end of the loop.  The latter means
	 * we've got a new topmost chunk.  Either way, try aggregating
	 * with the lower chunk if possible.
	 */
	fp2->nx = fpnew;
	cp2 = (char *)&(fp2->nx);
	if (cp2 + fp2->sz == cpnew) {
		/* lower junk adjacent, merge */
		fp2->sz += fpnew->sz + sizeof(size_t);
		fp2->nx = fpnew->nx;
	}
	m_pBlocker->Unlock();
}

unsigned CMyHeap::HeapSizeFree()
{
	//----------------------------------
	// walk the free list and calculate
	// the number of free bytes
	//
	// A return value of 0 could mean
	// that malloc just hasn't been called
	// yet.
	//----------------------------------
	struct __freelist *pHF;
	size_t FreeMem = 0;

	m_pBlocker->Lock();
	pHF = __flp;

	while(pHF)
	{
		FreeMem += pHF->sz;
		pHF = pHF->nx;
	}
	if (BrkVal == 0)	BrkVal = Start;
	FreeMem += (size_t)(End - BrkVal);
	m_pBlocker->Unlock();
	return FreeMem;
}
