#include "pch.h"

void CALLBACK CMMtimer::TimerCallback(
	UINT TimerID,
	UINT message_DontUse,
	DWORD UserData,
	DWORD DontUse1,
	DWORD DontUse2
)
{
	CMMtimer* pMMtmr = (CMMtimer*)UserData;
	pMMtmr->DoCallback(TimerID);
}

MMRESULT CMMtimer::Create(
	UINT delay,
	UINT TimerType
)
{
	//---------------------------------------------------------
	// Create
	// I am going to be really naughty here.  Microsoft frowns
	// on having the timer resolution set really fine, but I
	// am going to do it anyway.  The application I have in
	// mind requres really good accuracy (Music App).
	// 
	// parameters:
	//	delay...... The time between ticks.  Should be
	//				Greater than 1, lass than ???
	//	TimerType...Determins what happens when the timer has
	//				met its goal (delay)
	//---------------------------------------------------------
	MMRESULT mmResult;
	
	m_Resolution = 1;
	mmResult = BeginPeriod(m_Resolution);	//finest resolution
	if (TIMERR_NOCANDO == mmResult)
		printf("Could not begin Period Bad Resolution(?) %d\n", m_Resolution);
	else
	{
		mmResult = timeSetEvent(
			delay,
			m_Resolution,
			(LPTIMECALLBACK)CMMtimer::TimerCallback,
			DWORD_PTR(this),
			TimerType
		);
		if (0 < mmResult)
			m_TimerID = mmResult;
		else
			EndPeriod();
	}
	return mmResult;
}
