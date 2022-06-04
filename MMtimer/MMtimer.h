#pragma once

class CMMtimer
{
	UINT m_TimerID;
	DWORD m_Resolution;
	static void CALLBACK TimerCallback(
		UINT TimerID,
		UINT message_DontUse,
		DWORD UserData,
		DWORD DontUse1,
		DWORD DontUse2
	);
public:
	CMMtimer() {
		m_TimerID = 0;
		m_Resolution = 0;
	};
	virtual ~CMMtimer() {}
	virtual MMRESULT Create(UINT delay, UINT TimerType);
	virtual void Kill() {
		timeKillEvent(m_TimerID);
		if (m_Resolution > 0)
		{
			EndPeriod();
		}
	}
	MMRESULT BeginPeriod(DWORD timerRes) {
		return timeBeginPeriod(timerRes);
	}
	MMRESULT EndPeriod() {
		MMRESULT mmResult;
		if(m_Resolution)
			mmResult = timeEndPeriod(m_Resolution);;
		m_Resolution = 0;
		return mmResult;
	}
	DWORD GetTime() { return timeGetTime(); }
	MMRESULT GetSystemTime(MMTIME* SysTime, UINT cbmmt) {
		return timeGetSystemTime(SysTime, cbmmt);
	}
	MMRESULT GetDevCaps(TIMECAPS* timeCaps, UINT cbtc) {
		return timeGetDevCaps(timeCaps, cbtc);
	}
	virtual void DoCallback(
		UINT TimerID
	) {}
};

