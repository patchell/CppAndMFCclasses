// OverlappedSerial.h: interface for the COverlappedSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERLAPPEDSERIAL_H__9391C394_648E_47A8_A46E_507F3DC7FBC2__INCLUDED_)
#define AFX_OVERLAPPEDSERIAL_H__9391C394_648E_47A8_A46E_507F3DC7FBC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaudRate.h"

#define SER_PORT_MODE_EXIT		0
#define SER_PORT_MODE_REOPEN	1

extern BaudRate BR;

class COverlappedSerial  
{
public:
	char * m_pPortName;
	int LoopFlag;
	CWinThread *pThread;
	DWORD dwCommErrors;
	DWORD dwEventMask;
	DWORD dwLastError;
	void (* pfCallback)(void *p);
	void *pvCallbackParam;
	unsigned m_PortCharMessage;
// Construction
public:
	int ErrorNumber;
	char * GetPortName(void);
	COverlappedSerial(const char *pName);
	virtual ~COverlappedSerial();
	//-------------------------------------------------------
	// com port methodes
	//-------------------------------------------------------
	int StartPort(int port,int Baud,unsigned MsgStop,unsigned MsgChar,HWND hWnd, int InBuffSize = 8192,int OutBuffSize = 8192);
	void StopPort(void);
	int ReadComPort(char *b,int count);
	int GetComPort(void);
	int CharsInCommBuffer(void);
	BOOL WaitForCommEv(void);
	DWORD CheckForCommEvent(BOOL bWait);
	void WritePort(char *b,int n);
	int WorkerTerminate(void);
	void SetCharInCallback(void (__cdecl *pfunc)(void *p),void *pParam);
	//----------------------------------------------
	// private methodes
	//----------------------------------------------
private:
	int OpenComPort(int BaudRate,int TimeOut,int InBuffSize = 8192,int OutBuffSize = 8192);
	void CloseComPort(void);
	//----------------------------------------------
	// private data
	//----------------------------------------------
private:
	static UINT WorkerThread(LPVOID pParam);
	DWORD m_dwLastErr;
	COMMTIMEOUTS m_ctoTimeouts;
	COMMTIMEOUTS m_ctoOLD;
	DCB m_dcbConfig;
	DCB m_oldDCB;
	COMSTAT m_csStatus;
	HANDLE m_hPort;
	int m_PortStoppedMessage;
	HWND m_hWnd;
	int m_BaudRate;
	OVERLAPPED olRead;
	OVERLAPPED olWrite;
	OVERLAPPED olWait;
};

#endif // !defined(AFX_OVERLAPPEDSERIAL_H__9391C394_648E_47A8_A46E_507F3DC7FBC2__INCLUDED_)
