// MSG.h: interface for the MSG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSG_H__2A5D1A1E_54DB_48DF_A7F1_1D4FCD987644__INCLUDED_)
#define AFX_MSG_H__2A5D1A1E_54DB_48DF_A7F1_1D4FCD987644__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyHeap.h"
#include "Lock.h"

class AMSG  
{
public:
	short int MsgCmd;
	short int PayloadSize;
	short int MaxPayLoad;
	short int ReplyCmd;
	AMSG *next;
	char *Payload;
	AMSG();
	virtual ~AMSG();
	void PrepareMessage();
};

extern AMSG *CreateAMSG(unsigned nPayloadSize,char *npPayLoad,short int Cmd);
extern void DeleteAMSG(AMSG *pMSG);
extern void AMSGSetHeap(CMyHeap *pH);


class CMsgQueue  
{
	unsigned m_Nmsg;
	unsigned m_Tail;
	unsigned m_Head;
	unsigned m_Size;
	AMSG ** m_pMsgQueue;
	CDMLock * m_pMBlocker;
public:
	void Insert(AMSG *pM);
	AMSG * Remove(void);
	CMsgQueue(unsigned depth);
	virtual ~CMsgQueue();

};

#endif // !defined(AFX_MSG_H__2A5D1A1E_54DB_48DF_A7F1_1D4FCD987644__INCLUDED_)
