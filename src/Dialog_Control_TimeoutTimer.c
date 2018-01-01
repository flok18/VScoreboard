

#include "StdInc.h"
#include "Core.h"
#include "Resource.h"
#include "Dialog.h"



// ============================================================================


void Timeout_Timer_Init(Timeout_Timer* TheTimer, Scoreboard* TheScoreboard, HWND TimerWin, UINT_PTR TheEventID)
{
	memset(TheTimer, 0, sizeof(Timeout_Timer));

	TheTimer->RefScoreBoard = TheScoreboard;
	TheTimer->TimerWindow   = TimerWin;
	TheTimer->EventID       = TheEventID;
}

void Timeout_Timer_Stop(Timeout_Timer* TheTimer, BOOL Use_Auto_Sound)
{
	if (TheTimer->Is_Active)
	{
		KillTimer(TheTimer->TimerWindow, TheTimer->EventID);

		TheTimer->TimerID   = 0;
		TheTimer->Is_Active = FALSE;

		Scoreboard_Update_Timer(TheTimer->RefScoreBoard, -1);
		Scoreboard_Update_TimeoutText(TheTimer->RefScoreBoard, -1);

		if (Use_Auto_Sound)
        {
            if (IsDlgButtonChecked(TheTimer->TimerWindow, IDC_CHECKBOX_TIMEOUT_SOUND))
            {
                Sound_Play_Long();
            }
        }
	}
}


void Timeout_Timer_Event(Timeout_Timer* TheTimer, WPARAM wParam)
{
	DWORD  CurrentTimestamp;
	DWORD  TimeEllapsed;
	DWORD  NewSecTime;
	DWORD  OldSecTime;


	CurrentTimestamp = GetTickCount();
	TimeEllapsed = CurrentTimestamp - TheTimer->Last_Timestamp;
	TheTimer->Last_Timestamp = CurrentTimestamp;


	if (TimeEllapsed > TheTimer->Duration)
	{
		Timeout_Timer_Stop(TheTimer, TRUE);
	}
	else
	{
		OldSecTime = TheTimer->Duration / 1000;
		TheTimer->Duration -= TimeEllapsed;
		NewSecTime = TheTimer->Duration / 1000;

		if (OldSecTime != NewSecTime)
		{
			Scoreboard_Update_Timer(TheTimer->RefScoreBoard, (int)NewSecTime);
		}
	}
}


void Timeout_Timer_Update(Timeout_Timer* TheTimer)
{
	if (TheTimer->Is_Active)
	{
		Scoreboard_Update_TimeoutText(TheTimer->RefScoreBoard, TheTimer->TeamNr);
		Timeout_Timer_Event(TheTimer, TIMER_EV_ID_1);
	}
}


void Timeout_Timer_Start(Timeout_Timer* TheTimer, int TimerTeamNr, DWORD TimerDuration_ms)
{
	Timeout_Timer_Stop(TheTimer, FALSE);

	TheTimer->TimerID        = SetTimer(TheTimer->TimerWindow, TheTimer->EventID, 250, NULL);
	TheTimer->Is_Active      = TRUE;
	TheTimer->TeamNr         = TimerTeamNr;
	TheTimer->Duration       = TimerDuration_ms;
	TheTimer->Last_Timestamp = GetTickCount();

	Scoreboard_Update_TimeoutText(TheTimer->RefScoreBoard, TimerTeamNr);
	Scoreboard_Update_Timer(TheTimer->RefScoreBoard, (int)TimerDuration_ms / 1000);
}



