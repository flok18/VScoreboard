
#include "StdInc.h"
#include "Core.h"
#include "FLib1.h"
#include "Resource.h"
#include "Dialog.h"



void Dialog_Control_Button_Checkbox_Set_Timer(HWND hwndDlg)
{
	BOOL       CheckTimer;
	Layout*    TheLayout;

	CheckTimer = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_AUTO_PICTURE);

	TheLayout = Dialog_Control_ScoreBoard->CurrentLayout;
	if (TheLayout)
	{
		if (CheckTimer &&
		    FileList_Get_Count(&TheLayout->Picture_List) > 0 &&
			TheLayout->Picture_Timer_Interval > 0)
		{
			Picture_Timer_Start(&Dialog_Control_Picture_Timer, TheLayout->Picture_Timer_Interval);
		}
		else
		{
			Picture_Timer_Stop(&Dialog_Control_Picture_Timer);
		}
	}

}




void Dialog_Control_Button_Layout_Change(HWND hwndDlg)
{
	HWND        Ctrl;
	int         NewIndex;

	Ctrl = GetDlgItem(hwndDlg, IDC_COMBO_LAYOUT);

	NewIndex = ComboBox_GetIndex(Ctrl);
	if (NewIndex >= 0)
	{
		Scoreboard_Select_Layout(Dialog_Control_ScoreBoard, NewIndex, FALSE);

		Scoreboard_Update_Static_Text(Dialog_Control_ScoreBoard);
		Scoreboard_Update_Picture_Box(Dialog_Control_ScoreBoard);
		Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);

		Dialog_Control_Fill_Picture_List(hwndDlg);

		Dialog_Control_Button_Checkbox_Set_Timer(hwndDlg);
		Timeout_Timer_Update(&Dialog_Control_Timeout_Timer);
	}
}

void Dialog_Control_Auto_Layout_Change(HWND hwndDlg, int LayoutIndex, BOOL Reset_Pic_Index)
{
	HWND        Ctrl;

	if (LayoutIndex >= 0 &&
	    LayoutIndex < Dialog_Control_ScoreBoard->nLayout)
	{
		Ctrl = GetDlgItem(hwndDlg, IDC_COMBO_LAYOUT);
		ComboBox_SetIndex(Ctrl, LayoutIndex);

		Scoreboard_Select_Layout(Dialog_Control_ScoreBoard, LayoutIndex, Reset_Pic_Index);

		Scoreboard_Update_Static_Text(Dialog_Control_ScoreBoard);
		Scoreboard_Update_Picture_Box(Dialog_Control_ScoreBoard);
		Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);

		Dialog_Control_Fill_Picture_List(hwndDlg);

		Dialog_Control_Button_Checkbox_Set_Timer(hwndDlg);
		Timeout_Timer_Update(&Dialog_Control_Timeout_Timer);
	}
}





void Dialog_Control_Button_Picture_Set_Next(HWND hwndDlg)
{
	Picture_Timer_Go_Next_Pic(hwndDlg, Dialog_Control_ScoreBoard);
}


void Dialog_Control_Button_Picture_Set_Current(HWND hwndDlg)
{
	HWND       ListCtrl;
	int        NewIndex;
	FileList*  TheFileList;
	Layout*    TheLayout;


	ListCtrl = GetDlgItem(hwndDlg, IDC_LIST_PICTURES);
	NewIndex = ListBox_GetIndex(ListCtrl);
	if (NewIndex < 0) return;

	TheLayout = Dialog_Control_ScoreBoard->CurrentLayout;
	if (TheLayout)
	{
		TheFileList = &TheLayout->Picture_List;

		if (FileList_Get_File(TheFileList, NewIndex))
		{
			TheLayout->Current_Picture = NewIndex;

			Dialog_Control_Fill_Picture_List(hwndDlg);
			Scoreboard_Update_Picture_Box(Dialog_Control_ScoreBoard);
		}
	}
}


void Dialog_Control_Button_Picture_Move_Down(HWND hwndDlg)
{
	HWND       ListCtrl;
	FileList*  TheFileList;
	int        FilesCount;
	int        ListCount;
	BOOL*      SelArray;
	int        ii;

	ListCtrl = GetDlgItem(hwndDlg, IDC_LIST_PICTURES);

	if (Dialog_Control_ScoreBoard->CurrentLayout)
	{
		TheFileList = &Dialog_Control_ScoreBoard->CurrentLayout->Picture_List;

		FilesCount = FileList_Get_Count(TheFileList);
		if (FilesCount > 0)
		{
			ListCount = Get_Listbox_Selection(ListCtrl, &SelArray);
			if (ListCount > 0)
			{
				if (ListCount == FilesCount && SelArray[ListCount - 1] == FALSE)
				{
					for (ii = ListCount - 2; ii >= 0; --ii)
					{
						if (SelArray[ii])
						{
							FileList_Swap(TheFileList, ii, ii + 1);
							Swap_In_Bool_Array(SelArray, ii, ii + 1);
						}
					}

					Dialog_Control_Fill_Picture_List(hwndDlg);
					Set_Listbox_Selection(ListCtrl, SelArray, ListCount);
				}
				free(SelArray);
			}
		}
	}
}



void Dialog_Control_Button_Picture_Move_Up(HWND hwndDlg)
{
	HWND       ListCtrl;
	FileList*  TheFileList;
	int        FilesCount;
	int        ListCount;
	BOOL*      SelArray;
	int        ii;

	ListCtrl = GetDlgItem(hwndDlg, IDC_LIST_PICTURES);

	if (Dialog_Control_ScoreBoard->CurrentLayout)
	{
		TheFileList = &Dialog_Control_ScoreBoard->CurrentLayout->Picture_List;

		FilesCount = FileList_Get_Count(TheFileList);
		if (FilesCount > 0)
		{
			ListCount = Get_Listbox_Selection(ListCtrl, &SelArray);
			if (ListCount > 0)
			{
				if (ListCount == FilesCount && SelArray[0] == FALSE)
				{
					for (ii = 1; ii < ListCount; ++ii)
					{
						if (SelArray[ii])
						{
							FileList_Swap(TheFileList, ii, ii - 1);
							Swap_In_Bool_Array(SelArray, ii, ii - 1);
						}
					}

					Dialog_Control_Fill_Picture_List(hwndDlg);
					Set_Listbox_Selection(ListCtrl, SelArray, ListCount);
				}
				free(SelArray);
			}
		}
	}
}




void Dialog_Control_Button_Abort_Timeout(HWND hwndDlg)
{
	Timeout_Timer_Stop(&Dialog_Control_Timeout_Timer, FALSE);
}


void Dialog_Control_Button_Tech_Timeout(HWND hwndDlg)
{
	BOOL    CheckTimer;
	BOOL    CheckSound;
	BOOL    CheckLayout;


	if (Dialog_Control_Timeout_Timer.Is_Active) return;

	CheckTimer  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_TIMER);
	CheckSound  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_SOUND);
	CheckLayout = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_LAYOUT);

	if (CheckTimer)
	{
		Timeout_Timer_Start(&Dialog_Control_Timeout_Timer, 0, TIMEOUT_DURATION_TECH);
	}
	if (CheckSound)
	{
		Sound_Play_Long();
	}
	if (CheckLayout)
	{
		Dialog_Control_Auto_Layout_Change(hwndDlg, 1, FALSE);
	}
}

void Dialog_Control_Button_Timeout_Sub(HWND hwndDlg, int TeamNr)
{
	Game*   TheGame;
	BOOL    CheckTimer;
	int     NewVal;


	TheGame    = &Dialog_Control_ScoreBoard->Game_Status;
	CheckTimer = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_TIMER);

	if      (TeamNr == 1) NewVal = TheGame->Timeouts_Team_1;
	else if (TeamNr == 2) NewVal = TheGame->Timeouts_Team_2;
	else return;

	if (NewVal > 0)
	{
		if (CheckTimer)
		{
			Timeout_Timer_Stop(&Dialog_Control_Timeout_Timer, FALSE);
		}

		--NewVal;

		if      (TeamNr == 1) TheGame->Timeouts_Team_1 = NewVal;
		else if (TeamNr == 2) TheGame->Timeouts_Team_2 = NewVal;
	}

	Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
	Dialog_Control_Update_Game_Status(hwndDlg);
}


void Dialog_Control_Button_Timeout_Add(HWND hwndDlg, int TeamNr)
{
	Game*   TheGame;
	BOOL    CheckVal;
	BOOL    CheckTimer;
	BOOL    CheckSound;
	BOOL    CheckLayout;
	int     NewVal;


	if (Dialog_Control_Timeout_Timer.Is_Active) return;

	TheGame     = &Dialog_Control_ScoreBoard->Game_Status;
	CheckVal    = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_VALIDATE_TIMEOUTS);
	CheckTimer  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_TIMER);
	CheckSound  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_SOUND);
	CheckLayout = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_TIMEOUT_LAYOUT);


	if      (TeamNr == 1) NewVal = TheGame->Timeouts_Team_1;
	else if (TeamNr == 2) NewVal = TheGame->Timeouts_Team_2;
	else return;

	++NewVal;
	if (CheckVal)
	{
		if (NewVal > GAME_MAX_TIMEOUTS)
		{
			MsgBox_Info(hwndDlg, "Maximum an Timeouts erreicht...");
			return;
		}
	}

	if      (TeamNr == 1) TheGame->Timeouts_Team_1 = NewVal;
	else if (TeamNr == 2) TheGame->Timeouts_Team_2 = NewVal;


	if (CheckTimer)
	{
		Timeout_Timer_Start(&Dialog_Control_Timeout_Timer, TeamNr, TIMEOUT_DURATION_NORMAL);
	}
	if (CheckSound)
	{
		Sound_Play_Long();
	}
	if (CheckLayout)
	{
		Dialog_Control_Auto_Layout_Change(hwndDlg, 1, FALSE);
	}

	Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
	Dialog_Control_Update_Game_Status(hwndDlg);
}




void Dialog_Control_Button_PlayerChange_Edit(HWND hwndDlg, int TeamNr, int Val)
{
	Game*   TheGame;
	BOOL    CheckVal;
	BOOL    CheckSound;
	BOOL    CheckLayout;
	int     NewVal;


	TheGame     = &Dialog_Control_ScoreBoard->Game_Status;
	CheckVal    = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_VALIDATE_PLAYER_CHANGES);
	CheckSound  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_PLAYER_CHANGE_SOUND);
	CheckLayout = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_PLAYER_CHEANGE_LAYOUT);



	if      (TeamNr == 1) NewVal = TheGame->Player_Changes_Team_1;
	else if (TeamNr == 2) NewVal = TheGame->Player_Changes_Team_2;
	else return;


	NewVal += Val;
	if (NewVal < 0) return;

	if (CheckVal)
	{
		if (Val > 0 && NewVal > GAME_MAX_CHANGES)
		{
			MsgBox_Info(hwndDlg, "Maximum an Auswechslungen erreicht...");
			return;
		}
	}


	if      (TeamNr == 1) TheGame->Player_Changes_Team_1 = NewVal;
	else if (TeamNr == 2) TheGame->Player_Changes_Team_2 = NewVal;



	if (Val > 0)
	{
		if (CheckSound)
		{
			Sound_Play_Short();
		}
		if (CheckLayout)
		{
			Dialog_Control_Auto_Layout_Change(hwndDlg, 2, TRUE);
		}
	}

	Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
	Dialog_Control_Update_Game_Status(hwndDlg);
}



void Dialog_Control_Button_Score_End_Set(HWND hwndDlg, BOOL Update_Board)
{
	Game*       TheGame;
	BOOL        CheckEndSet;
	SetResult*  TheSetResult;


	TheGame      = &Dialog_Control_ScoreBoard->Game_Status;
	CheckEndSet  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_AUTO_SET_WIN);


	if (CheckEndSet)
	{
		if (TheGame->Sets_Team_1 >= GAME_WIN_SETS ||
		    TheGame->Sets_Team_2 >= GAME_WIN_SETS) return;
	}


	TheSetResult = Game_Get_Current_Set_Result(TheGame);
	if (TheSetResult)
	{
		TheSetResult->Score_Team_1          = (short)TheGame->Score_Team_1;
		TheSetResult->Score_Team_2          = (short)TheGame->Score_Team_2;
		TheSetResult->Timeouts_Team_1       = (short)TheGame->Timeouts_Team_1;
		TheSetResult->Timeouts_Team_2       = (short)TheGame->Timeouts_Team_2;
		TheSetResult->Player_Changes_Team_1 = (short)TheGame->Player_Changes_Team_1;
		TheSetResult->Player_Changes_Team_2 = (short)TheGame->Player_Changes_Team_2;
	}

	if (TheGame->Score_Team_1 > TheGame->Score_Team_2)
	{
		++(TheGame->Sets_Team_1);
	}
	else if (TheGame->Score_Team_2 > TheGame->Score_Team_1)
	{
		++(TheGame->Sets_Team_2);
	}

	if (TheGame->Current_Set_Nr < GAME_MAX_SETS &&
		TheGame->Sets_Team_1    < GAME_WIN_SETS &&
		TheGame->Sets_Team_2    < GAME_WIN_SETS)
	{
		++(TheGame->Current_Set_Nr);
		TheGame->Score_Team_1          = 0;
		TheGame->Score_Team_2          = 0;
		TheGame->Timeouts_Team_1       = 0;
		TheGame->Timeouts_Team_2       = 0;
		TheGame->Player_Changes_Team_1 = 0;
		TheGame->Player_Changes_Team_2 = 0;

		if (TheGame->Current_Set_Nr % 2) TheGame->Service_Team_Nr = 2;
		else                             TheGame->Service_Team_Nr = 1;
	}
	else
	{
		TheGame->Service_Team_Nr = 0;
	}

	if (Update_Board)
	{
		Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
		Dialog_Control_Update_Game_Status(hwndDlg);
	}
}


void Dialog_Control_Button_Score_Sub(HWND hwndDlg, int TeamNr)
{
	Game*       TheGame;
	BOOL        CheckEndSet;
	BOOL        RestoreLastSet   = FALSE;
	SetResult*  TheSetResult;


	TheGame      = &Dialog_Control_ScoreBoard->Game_Status;
	CheckEndSet  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_AUTO_SET_WIN);


	if (CheckEndSet)
	{
		if (TheGame->Score_Team_1 == 0 && TheGame->Score_Team_2 == 0)
		{
			TheSetResult = Game_Get_Any_Set_Result(TheGame, TheGame->Current_Set_Nr - 1);
			if (TheSetResult)
			{
				if ((TeamNr == 1 && TheSetResult->Score_Team_1 > TheSetResult->Score_Team_2 && TheGame->Sets_Team_1 > 0) ||
					(TeamNr == 2 && TheSetResult->Score_Team_2 > TheSetResult->Score_Team_1 && TheGame->Sets_Team_2 > 0))
				{
					RestoreLastSet = TRUE;
				}
			}
		}
		else if (TheGame->Current_Set_Nr == GAME_MAX_SETS && (TheGame->Score_Team_1 == GAME_TIEBREAK_WIN_POINTS || TheGame->Score_Team_2 == GAME_TIEBREAK_WIN_POINTS))
		{
			if (TeamNr == 1 && TheGame->Score_Team_1 == GAME_TIEBREAK_WIN_POINTS && TheGame->Sets_Team_1 == GAME_WIN_SETS)
			{
				--(TheGame->Sets_Team_1);
				SetResult_Reset(Game_Get_Current_Set_Result(TheGame));
			}
			else if (TeamNr == 2 && TheGame->Score_Team_2 == GAME_TIEBREAK_WIN_POINTS && TheGame->Sets_Team_2 == GAME_WIN_SETS)
			{
				--(TheGame->Sets_Team_2);
				SetResult_Reset(Game_Get_Current_Set_Result(TheGame));
			}
			RestoreLastSet = FALSE;
		}
		else if (TheGame->Current_Set_Nr < GAME_MAX_SETS && TheGame->Current_Set_Nr > 0 && (TheGame->Score_Team_1 == GAME_NORMAL_WIN_POINTS || TheGame->Score_Team_2 == GAME_NORMAL_WIN_POINTS))
		{
			if (TeamNr == 1 && TheGame->Score_Team_1 == GAME_NORMAL_WIN_POINTS && TheGame->Sets_Team_1 == GAME_WIN_SETS)
			{
				--(TheGame->Sets_Team_1);
				SetResult_Reset(Game_Get_Current_Set_Result(TheGame));
			}
			else if (TeamNr == 2 && TheGame->Score_Team_2 == GAME_NORMAL_WIN_POINTS && TheGame->Sets_Team_2 == GAME_WIN_SETS)
			{
				--(TheGame->Sets_Team_2);
				SetResult_Reset(Game_Get_Current_Set_Result(TheGame));
			}
			RestoreLastSet = FALSE;
		}
	}


	if (RestoreLastSet)
	{
		--(TheGame->Current_Set_Nr);

		TheSetResult = Game_Get_Current_Set_Result(TheGame);
		if (TheSetResult)
		{
			TheGame->Score_Team_1          = TheSetResult->Score_Team_1;
			TheGame->Score_Team_2          = TheSetResult->Score_Team_2;
			TheGame->Timeouts_Team_1       = TheSetResult->Timeouts_Team_1;
			TheGame->Timeouts_Team_2       = TheSetResult->Timeouts_Team_2;
			TheGame->Player_Changes_Team_1 = TheSetResult->Player_Changes_Team_1;
			TheGame->Player_Changes_Team_2 = TheSetResult->Player_Changes_Team_2;

			if (TheGame->Score_Team_1 > TheGame->Score_Team_2 && TheGame->Sets_Team_1 > 0)
			{
				--(TheGame->Sets_Team_1);
			}
			else if (TheGame->Score_Team_2 > TheGame->Score_Team_1 && TheGame->Sets_Team_2 > 0)
			{
				--(TheGame->Sets_Team_2);
			}

			SetResult_Reset(TheSetResult);
		}
	}

	if (TeamNr == 1)
	{
		if (TheGame->Score_Team_1 > 0)
		{
			--(TheGame->Score_Team_1);
		}
	}
	else if (TeamNr == 2)
	{
		if (TheGame->Score_Team_2 > 0)
		{
			--(TheGame->Score_Team_2);
		}
	}

	Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
	Dialog_Control_Update_Game_Status(hwndDlg);
}


void Dialog_Control_Button_Score_Add(HWND hwndDlg, int TeamNr)
{
	Game*       TheGame;
	int         EndScore;
	int         RefScore;
	int         OtherScore;
	BOOL        CheckEndSet;
	BOOL        CheckTimeOut;
	BOOL        IsEndSet      = FALSE;
	BOOL        IsTimeout     = FALSE;


	if (Check_Button_Timestamp(hwndDlg, 0)) return;

	TheGame      = &Dialog_Control_ScoreBoard->Game_Status;
	CheckEndSet  = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_AUTO_SET_WIN);
	CheckTimeOut = IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_AUTO_TIMEOUT);


	if (CheckEndSet)
	{
		if (TheGame->Sets_Team_1 >= GAME_WIN_SETS ||
		    TheGame->Sets_Team_2 >= GAME_WIN_SETS) return;
	}

	if (TeamNr == 1)
	{
		RefScore   = TheGame->Score_Team_1;
		OtherScore = TheGame->Score_Team_2;
	}
	else if (TeamNr == 2)
	{
		RefScore   = TheGame->Score_Team_2;
		OtherScore = TheGame->Score_Team_1;
	}
	else
	{
		return;
	}

	if (TheGame->Current_Set_Nr < GAME_MAX_SETS) EndScore = GAME_NORMAL_WIN_POINTS;
	else                                         EndScore = GAME_TIEBREAK_WIN_POINTS;


	++RefScore;

	if (CheckEndSet)
	{
		if (RefScore >= EndScore &&
		    RefScore - OtherScore > 1)
		{
			IsEndSet = TRUE;
		}
	}
	if (CheckTimeOut)
	{
		if (TheGame->Current_Set_Nr < GAME_MAX_SETS)
		{
			if (RefScore == 8 || RefScore == 16)
			{
				IsTimeout = TRUE;
			}
		}
	}

	if      (TeamNr == 1) TheGame->Score_Team_1 = RefScore;
	else if (TeamNr == 2) TheGame->Score_Team_2 = RefScore;
	TheGame->Service_Team_Nr = TeamNr;


	if (IsEndSet)
	{
		Dialog_Control_Button_Score_End_Set(hwndDlg, FALSE);
	}
	if (IsTimeout)
	{
		Dialog_Control_Button_Tech_Timeout(hwndDlg);
	}

	Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
	Dialog_Control_Update_Game_Status(hwndDlg);
}

