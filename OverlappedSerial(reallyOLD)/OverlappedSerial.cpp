// OverlappedSerial.cpp: implementation of the COverlappedSerial class.
//
//////////////////////////////////////////////////////////////////////
//
// This is an overlapped serial class for Windows that allows
// asyncronous RS232 communication from windows
//
// created May 22, 2007
// by Jim Patchell
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OverlappedSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BaudRate BR;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COverlappedSerial::COverlappedSerial(const char * pPortName)
{
	//------------------------------------------------------------------
	// This fucntion creates a com port object
	//
	// parameters:
	//	pPortName.........name of com port to open (COM1,COM2,etc)
	//
	//------------------------------------------------------------------
	m_pPortName = new char[strlen(pPortName)+1];
	strcpy(m_pPortName,pPortName);
	m_hPort = INVALID_HANDLE_VALUE;	// port is not opened yet
	pfCallback = 0;			//no callback function
	pvCallbackParam = 0;	//no callback parameters
}

COverlappedSerial::~COverlappedSerial()
{
	delete[] m_pPortName;	//delete memory used for port name
}

//---------------------------------------------------------------------------
// Comm Port Functions
//---------------------------------------------------------------------------

int COverlappedSerial::OpenComPort(int BRate,int TimeOut,int InBuffSize,int OutBuffSize)
{
	//-----------------------------------------------------------------------
	// This function opens up the comport and configures it
	//
	// parameters:
	//	BRate........bits per second port will run at
	//	TimeOut.........Timeout in milliseconds for port read
	//
	// return value:
	//	0........success
	//	other....fail
	//-----------------------------------------------------------------------
	bool  CurrDCB = false, CurrTmo = false; ;
	DWORD flags ;


	if( m_hPort != 0 && m_hPort != INVALID_HANDLE_VALUE )
	{
		ErrorNumber = 1;
		return 0;
	}

	//--------------------------------------------------------------------------
	// Open COMM Port :: Read/Write, NO share, NO Inheritance ( Req for COMM), 
	// Port Exists ( Req for COMM),
	// no'Attributes' for COMM, and OVERLAPPED IO (asynchronous )
	// No Template (Req for COMM)
	//--------------------------------------------------------------------------
	m_hPort = CreateFile(m_pPortName,GENERIC_READ | GENERIC_WRITE,0,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH  | FILE_FLAG_OVERLAPPED, NULL );
	if( m_hPort == INVALID_HANDLE_VALUE )
	{
		ErrorNumber = 2;
		return 1  ;
	}
//  *** 'Open' went OK
//
	memset(&olRead,0,sizeof(OVERLAPPED));	//overlapped descriptor for read event
	memset(&olWrite,0,sizeof(OVERLAPPED));	//overlapped descriptor for write event
	memset(&olWait,0,sizeof(OVERLAPPED));	//overlapped descriptor for wait event

	olRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	//create event for read
	olWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	//create event for write
	olWait.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	//create event for wait

	if(!SetCommMask(m_hPort,EV_RXCHAR))
	{
		ErrorNumber = 4;
		goto BackPeddle;
	}

	if(olRead.hEvent && olWrite.hEvent && olWait.hEvent)
	{
//		m_Static_Debug.SetWindowText("Events OK");
	}
	else
	{
		ErrorNumber = 3;
		return 1;
	}
	if(!GetCommState( m_hPort,&m_oldDCB ) )
	{
		ErrorNumber = 5;
		goto BackPeddle;
	}
	if(!GetCommTimeouts( m_hPort, &m_ctoOLD ) )
	{
		ErrorNumber = 6;
		goto BackPeddle;
	}
	if(!ClearCommError( m_hPort,&m_dwLastErr, &m_csStatus ) )
	{
		ErrorNumber = 7;
		goto BackPeddle;
	}
//  *** Fill Current with Past, for completeness
	m_ctoTimeouts = m_ctoOLD ;
	m_dcbConfig  = m_oldDCB;
// 
	m_ctoTimeouts.ReadIntervalTimeout          = 100;	     // between characters
	m_ctoTimeouts.ReadTotalTimeoutConstant     = 5 ;		// for 1st char
	m_ctoTimeouts.ReadTotalTimeoutMultiplier   = 2 ;      // * #chr for AbsMax
	m_ctoTimeouts.WriteTotalTimeoutConstant    = 0 ;      // Max Len w/o #chrs
	m_ctoTimeouts.WriteTotalTimeoutMultiplier  = 0 ;      // * #chr for AbsMax
	CurrTmo = true;
	if(!SetCommTimeouts( m_hPort, &m_ctoTimeouts ) )
	{
		ErrorNumber = 8;
		goto BackPeddle;
	}

	//  :<BRate>,N,8,1 & No HW Handshajke.
	m_dcbConfig.BaudRate     = BR.GetBaudRate(BRate);
	m_dcbConfig.fParity      = false ;
	m_dcbConfig.ByteSize     = 8;
	m_dcbConfig.Parity       = NOPARITY;
	m_dcbConfig.StopBits     = ONESTOPBIT ;
	m_dcbConfig.fOutX        = false;
	m_dcbConfig.fInX	       = false ;
	m_dcbConfig.fOutxCtsFlow = false;
	m_dcbConfig.fOutxDsrFlow = false;
	m_dcbConfig.fRtsControl  = RTS_CONTROL_DISABLE ;
	m_dcbConfig.fDtrControl  = DTR_CONTROL_DISABLE ;
	m_dcbConfig.fNull        = false;
	CurrDCB            = true;
	if(!SetCommState( m_hPort, &m_dcbConfig ))	
	{
		ErrorNumber = 9;
		goto BackPeddle;
	}

	flags = PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR ;
	if(!PurgeComm( m_hPort, flags ) )	
	{
		ErrorNumber = 10;
		goto BackPeddle;
	}
	if(!SetupComm( m_hPort, InBuffSize, OutBuffSize ) )	
	{
		ErrorNumber = 11;
		goto BackPeddle;
	}

	return 0;
//
BackPeddle:
	m_dwLastErr = GetLastError();
	ClearCommError( m_hPort, &m_dwLastErr, &m_csStatus );
	if ( CurrTmo ) SetCommTimeouts( m_hPort, &m_ctoOLD ) ;
	if ( CurrDCB ) SetCommState( m_hPort, &m_dcbConfig ) ;
	CloseHandle( m_hPort ) ;
//	m_Static_Debug.SetWindowText("Error Opening Com Port");
	return 1 ;
}

void COverlappedSerial::CloseComPort()
{
	//-------------------------------------
	// destroy events
	//-------------------------------------
	if(olRead.hEvent) CloseHandle(olRead.hEvent);
	if(olWrite.hEvent) CloseHandle(olWrite.hEvent);
	if(olWait.hEvent) CloseHandle(olWait.hEvent);
	//-------------------------------------
	// destroy port handle
	//-------------------------------------
	if( m_hPort != 0 )  CloseHandle( m_hPort );
	m_hPort = 0;
}

int COverlappedSerial::GetComPort()
{
	//--------------------------------------------
	// Get a character from the comm port
	//
	// return value: returns character from port
	//--------------------------------------------
   unsigned char b;
	int retval;

	retval = ReadComPort((char *)&b,1);
	if(retval == 1) retval = (int)b;
   return retval;
}

int COverlappedSerial::ReadComPort(char *b, int count)
{
	//------------------------------------------------------
	// read characters from comm port into buffer
	//
	// parameters:
	//	b............pointer to buffer to add characters to
	//	count........total number of characters to read
	//
	// return value:
	//	returns -1 on fail (no characters read)
	//	returns number of characters read on success
	//------------------------------------------------------
	DWORD rcount;
	COMSTAT ComStat;
	int retval;

	::ClearCommError(m_hPort,&dwCommErrors,&ComStat);
	if(count > (int)ComStat.cbInQue)
		count = ComStat.cbInQue;
	if(count > 0)
	{
		if(!ReadFile(m_hPort,b,count,&rcount,&olRead))
		{
			//did it fail because I/O is still pending?
			dwLastError = ::GetLastError();
			if(dwLastError == ERROR_IO_PENDING)
			{
				while(!GetOverlappedResult(m_hPort,&olRead,&rcount,TRUE))
				{
					dwLastError = ::GetLastError();
					if(dwLastError != ERROR_IO_INCOMPLETE)
					{
						::ClearCommError(m_hPort,&dwCommErrors,&ComStat);
						break;
					}
					//not finished yet
				}
			}
		}
		else
		{
			::ClearCommError(m_hPort,&dwCommErrors,&ComStat);
		}
	}

   if(!count)	//if count == 0, then there was a timeout, duh!
   	retval = -1;
   else
   	retval = (int)rcount;
   return retval;
}

void COverlappedSerial::WritePort(char *b, int n)
{
	//----------------------------------------------------------
	// write buffer of characters to comm port
	//
	// parameters:
	//	b...........pointer to buffer of characters to write
	//	n...........number of characters to write
	//----------------------------------------------------------

	COMSTAT ComStat;
	DWORD count;


	if(!WriteFile(m_hPort,b,n,&count,&olWrite))
	{
		//Did it fail because I/O is still pending?
		dwLastError = ::GetLastError();
		if(dwLastError == ERROR_IO_PENDING)
		{
			while(!GetOverlappedResult(m_hPort,&olWrite,&count,TRUE))
			{
				dwLastError = ::GetLastError();
				if(dwLastError != ERROR_IO_INCOMPLETE)
				{
					::ClearCommError(m_hPort,&dwCommErrors,&ComStat);
					break;
				}
			}
			//Not finished, wait for it
		}
	}
	else
	{
		::ClearCommError(m_hPort,&dwCommErrors,&ComStat);
	}
}

DWORD COverlappedSerial::CheckForCommEvent(BOOL bWait)
{
	DWORD dwBytesRead;
	COMSTAT ComStat;

	if(!GetOverlappedResult(m_hPort,&olWait,&dwBytesRead,bWait))
	{
		if(::GetLastError() != ERROR_IO_INCOMPLETE)
		{
			//an error occured
			ClearCommError(m_hPort,&dwCommErrors,&ComStat);
			WaitForCommEv();
		}
		return 0;
	}
	return dwEventMask;
}

BOOL COverlappedSerial::WaitForCommEv()
{
	dwLastError = ERROR_SUCCESS;
	dwEventMask = 0;

	BOOL bRetVal = ::WaitCommEvent(m_hPort,&dwEventMask,&olWait);
	if(!bRetVal)
	{
		dwLastError = ::GetLastError();
		if(dwLastError == ERROR_IO_PENDING)
			bRetVal = TRUE;
	}
	return bRetVal;
}

int COverlappedSerial::CharsInCommBuffer()
{
	//-------------------------------------------------------
	// This function returns the number of characters in the
	// RX Buffer
	//-------------------------------------------------------
	COMSTAT CommStatus;
	DWORD error;

	ClearCommError(m_hPort,&error,&CommStatus);
	return (int)CommStatus.cbInQue;
}

void COverlappedSerial::SetCharInCallback(void (__cdecl *pfunc)(void *p),void *pParam)
{
	pfCallback = pfunc;
	pvCallbackParam = pParam;
}

//--------------------------------------------------------------------------
// Worker Thread Functions
//--------------------------------------------------------------------------


UINT COverlappedSerial::WorkerThread(LPVOID pParam)
{
	//---------------------------------------------------------------
	// This is where we get characters back from the comm port
	// It seems like a nessesary evil
	// we block here waiting for characters
	// Depending on how the class is configured, we either
	// will post a message to a window when a character come in
	// or we will call a function somewhere that handles the
	// 
	DWORD dwEventMask;
	COverlappedSerial *pCOLS = (COverlappedSerial *)pParam;
	int c;

	SetCommMask(pCOLS->m_hPort,EV_RXCHAR);	//set event mask for incoming
	pCOLS->WaitForCommEv();	//no idea why I need this...but I do

	while(pCOLS->LoopFlag)
	{
		//------------------------------------------------------
		// this is where we wait for a comm event to occure...
		//------------------------------------------------------
		dwEventMask = pCOLS->CheckForCommEvent(TRUE);
		if(dwEventMask & EV_RXCHAR)
		{
			//--------------------------------------------------
			// There is data, so, we need to get it....or is there?
			//--------------------------------------------------
			while(pCOLS->CharsInCommBuffer())
			{
				//----------------------------------------------------
				// The reason for this while loop is that Windows does
				// not count the number of EV_RXCHAR events.  It is 
				// posible to have 20 chars in the input buffer
				// generate only one EV_RXCHAR event
				// So, it is also posible to get an event without
				// there being any characters if you accidently read
				// more chars than you should have.  So we only call
				// GetComPort only when there are real characters to
				// be read.
				// Also, we do not wait for another event until we have
				// completely emptied that buffer.
				//-----------------------------------------------------
				if(pCOLS->pfCallback)
					pCOLS->pfCallback(pCOLS->pvCallbackParam);
				else
				{
					c = pCOLS->GetComPort();	//get character from port
					PostMessage(pCOLS->m_hWnd,pCOLS->m_PortCharMessage,WPARAM(c),0);
				}
			}
			pCOLS->WaitForCommEv();	//This makes the thread wait for the next character
		}
	}
	//--------------------------------------------------
	// post message to indicate thread is shutting down
	//--------------------------------------------------
	PostMessage(pCOLS->m_hWnd,pCOLS->m_PortStoppedMessage,0,0);
	return 0;
}

int COverlappedSerial::WorkerTerminate(void)
{
	//-----------------------------------------------------------
	// This function is called once the application gets the
	// notification that the worker thread has terminated
	//-----------------------------------------------------------
	DWORD dwExitCode;
	do
	{
		::GetExitCodeThread (pThread->m_hThread, &dwExitCode);
		//--------------------------------------------------------
		// in order fot the exit code to change, we need to force
		// a context switch.  We do this by going to sleep for
		// zero milliseconds...I personally find this to be
		// klugey, but this is windows we are talking about here
		//--------------------------------------------------------
		Sleep(0L);
	}while(dwExitCode);
	if(dwExitCode == 0)	// Is The thread is still running.
	{
		CloseComPort();
		delete pThread;
	}
	return 0;
}

int COverlappedSerial::StartPort(int Timeout,int Baud,unsigned MsgStop,unsigned MsgChar,HWND hWnd, int InBuffSize,int OutBuffSize) 
{
	// TODO: Add your control notification handler code here
	//--------------------------------------------------------------
	// Start up a serial port
	// parameters:
	//	Timeout.......timeout value for data into the port
	//					we wait for this amount of time for a char
	//					and then we abort.
	//	Baud..........Baudrate of port
	//	MsgStop.......This is the value of the message that the
	//					worker thread will return when it terminates
	//	MsgChar.......This is the value of the message that returns
	//					a character back to the process that is interested
	//	hWnd..........This is the handle of the window that the messages
	//					are going to be returned to
	//	InBuffSize....Desired size of Comm In Buffer Size (default=8192)
	//	OutBuffSize...Desired size of Comm Out Buffer Size (default = 8192)
	//
	// return value:
	//		0 = success, non-zero is failure
	//---------------------------------------------------------------
	int retval;

	m_hWnd = hWnd;			//set handle of window to send thread stopped message to
	m_PortStoppedMessage = MsgStop;	//set the message value
	m_PortCharMessage = MsgChar;	//message to send when we have a character
	m_BaudRate = Baud;
	if(OpenComPort(Baud,Timeout,InBuffSize,OutBuffSize))
	{
		retval = 1;
	}
	else
	{
		//-------------------------------------------------
		// Create thread...
		//-------------------------------------------------
		pThread = AfxBeginThread(COverlappedSerial::WorkerThread,this,THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pThread->m_bAutoDelete = FALSE;
		LoopFlag = 1;
		pThread->ResumeThread();	//start up worker task
		retval = 0;
	}
	return retval;
}

void COverlappedSerial::StopPort() 
{
	//-----------------------------------------------------
	// This funciton is called to stop the worker thread
	// that is handling the reception of characters from
	// the com port.
	// The worker thread will send a message to the window
	// that is interested in this.  The window will then
	// respond by calling WorkerTerminate().
	//-----------------------------------------------------
	// setting loop flag to zero will cause the loop in 
	// the worker thread to stop looping...this might be
	// a little redundent, since the SetEvent call is also
	// used to stop the thread as well...
	// but this will definately insure that the thread will
	// stop when we want it to...
	//-----------------------------------------------------
	LoopFlag = 0;
	//-----------------------------------------------------
	// We need to signal the event that is being used to
	// wait for something to happen in the serial port
	// so that we can quit the thread that is doing
	// the com port recieve data task...otherwise, it will
	// just wait there forever and ever....
	//-----------------------------------------------------
	::SetEvent(olWait.hEvent);
}


char * COverlappedSerial::GetPortName()
{
	return m_pPortName;
}
