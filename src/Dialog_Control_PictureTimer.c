
#include "StdInc.h"
#include "Core.h"
#include "Dialog.h"



// ============================================================================

void Picture_Timer_Go_Next_Pic(HWND hwndDlg, Scoreboard* TheScoreboard)
{
	Layout*    TheLayout;
	FileList*  TheFileList;
	int        Old_Current;
	int        New_Current;


	TheLayout = TheScoreboard->CurrentLayout;
	if (TheLayout)
	{
		TheFileList = &TheLayout->Picture_List;

		if (FileList_Get_Count(TheFileList) > 0)
		{
			Old_Current = TheLayout->Current_Picture;
			New_Current = Old_Current + 1;

			if (FileList_Get_File(TheFileList, New_Current))
			{
				TheLayout->Current_Picture = New_Current;
			}
			else
			{
				TheLayout->Current_Picture = 0;
			}

			if (TheLayout->Current_Picture != Old_Current)
			{
				Dialog_Control_Fill_Picture_List(hwndDlg);
				Scoreboard_Update_Picture_Box(TheScoreboard);
			}
		}
	}
}


// ============================================================================



void Picture_Timer_Init(Picture_Timer* TheTimer, Scoreboard* TheScoreboard, HWND TimerWin, UINT_PTR TheEventID)
{
	memset(TheTimer, 0, sizeof(Picture_Timer));

	TheTimer->RefScoreBoard = TheScoreboard;
	TheTimer->TimerWindow   = TimerWin;
	TheTimer->EventID       = TheEventID;
}




void Picture_Timer_Stop(Picture_Timer* TheTimer)
{
	if (TheTimer->Is_Active)
	{
		KillTimer(TheTimer->TimerWindow, TheTimer->EventID);

		TheTimer->TimerID   = 0;
		TheTimer->Is_Active = FALSE;
	}
}


void Picture_Timer_Event(Picture_Timer* TheTimer, WPARAM wParam)
{
	if (TheTimer)
	{
		Picture_Timer_Go_Next_Pic(TheTimer->TimerWindow, TheTimer->RefScoreBoard);
	}
}


void Picture_Timer_Start(Picture_Timer* TheTimer, DWORD TimerInterval_ms)
{
	Picture_Timer_Stop(TheTimer);

	if (TimerInterval_ms > 0)
	{
		TheTimer->TimerID   = SetTimer(TheTimer->TimerWindow, TheTimer->EventID, TimerInterval_ms, NULL);
		TheTimer->Is_Active = TRUE;
	}
}



