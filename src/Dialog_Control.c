
#include "StdInc.h"
#include "Core.h"
#include "FLib1.h"
#include "Resource.h"
#include "Dialog.h"


Scoreboard*  Dialog_Control_ScoreBoard  = NULL;

HINSTANCE    Dialog_Control_hInst       = NULL;


Picture_Timer  Dialog_Control_Picture_Timer;
Timeout_Timer  Dialog_Control_Timeout_Timer;



// ============================================================================


void Dialog_Control_Update_Game_Status(HWND hwndDlg)
{
	Game*         TheGame;
	char          LargeBuf[512];

	TheGame = &Dialog_Control_ScoreBoard->Game_Status;

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SCORE,  TheGame->Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SCORE, TheGame->Score_Team_2, FALSE);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_TIMEOUTS,  TheGame->Timeouts_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_TIMEOUTS, TheGame->Timeouts_Team_2, FALSE);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_PLAYER_CHANGES,  TheGame->Player_Changes_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_PLAYER_CHANGES, TheGame->Player_Changes_Team_2, FALSE);

	if (Dialog_Control_Is_Vertical_Summary)
	{
		Game_Build_Summary_Vertical(TheGame, LargeBuf, sizeof(LargeBuf));
	}
	else
	{
		Game_Build_Summary_Horizontal(TheGame, LargeBuf, sizeof(LargeBuf));
	}
	SetDlgItemText(hwndDlg, IDC_EDIT_SCORE_SUMMARY, LargeBuf);
}


void Dialog_Control_Fill_Layout_Combo(HWND hwndDlg)
{
	int   ii;
	HWND  Ctrl;

	Ctrl = GetDlgItem(hwndDlg, IDC_COMBO_LAYOUT);
	ComboBox_Clear(Ctrl);

	if (Dialog_Control_ScoreBoard->nLayout > 0)
	{
		for (ii = 0; ii < Dialog_Control_ScoreBoard->nLayout; ++ii)
		{
			ComboBox_AddItem(Ctrl, Dialog_Control_ScoreBoard->aLayout[ii].Name);
		}
		ComboBox_SetIndex(Ctrl, Scoreboard_Get_Layout_Index(Dialog_Control_ScoreBoard));
	}
}


void Dialog_Control_Fill_Picture_List(HWND hwndDlg)
{
	Layout*     TheLayout;
	int         ii;
	HWND        Ctrl;
	int         Num;
	FileData*   FData;
	int         AddedIndex;
	int         TopIndex;


	Ctrl = GetDlgItem(hwndDlg, IDC_LIST_PICTURES);
	TopIndex = ListBox_GetTopIndex(Ctrl);
	ListBox_Clear(Ctrl);

	TheLayout = Dialog_Control_ScoreBoard->CurrentLayout;
	if (TheLayout)
	{
		Num = FileList_Get_Count(&TheLayout->Picture_List);
		if (Num > 0)
		{
			for (ii = 0; ii < Num; ++ii)
			{
				FData = FileList_Get_File(&TheLayout->Picture_List, ii);
				if (FData == NULL) break;

				AddedIndex = ListBox_AddItem(Ctrl, FData->FullFileName + FData->FileNameOffset);
				ListBox_SetData(Ctrl, AddedIndex, (LPARAM)ii);
			}
			ListBox_SetIndex(Ctrl, TheLayout->Current_Picture);
			ListBox_SetSelectStatus(Ctrl, TheLayout->Current_Picture, TRUE);
			ListBox_SetTopIndex(Ctrl, TopIndex);
		}
	}
}


static void Dialog_Control_Menu_Click(HWND hwndDlg, int MenuID)
{
	switch (MenuID)
	{
		case 1001:  // Laden
			Dialog_Control_Menu_File_Load(hwndDlg);
			break;
		case 1002:  // Speichern
			Dialog_Control_Menu_File_Save(hwndDlg);
			break;
		case 1009:  // Beenden
			Window_SoftClose(hwndDlg);
			break;

		case 1101:  // Spiel Beschreibung
			Dialog_Control_Show_Edit_Description(hwndDlg);
			break;
		case 1102:  // Maunuell Bearbeiten
			Dialog_Control_Show_Edit_Game_Status(hwndDlg);
			break;
		case 1103:  // Layout Laden
			Dialog_Control_Menu_Layout_Load(hwndDlg);
			break;
		case 1104:  // Punktestand Laden
			Dialog_Control_Menu_Score_Load(hwndDlg);
			break;
		case 1105:  // Punktestand Speichern
			Dialog_Control_Menu_Score_Save(hwndDlg);
			break;
		case 1106:  // Manueller Update
			Dialog_Control_Do_Manual_Update(hwndDlg);
			break;

		case 1201:  // Vertikale Anordnung
			Dialog_Control_Set_Vertical_Summary(hwndDlg);
			break;
		case 1202:  // Warnung (5s) ausgeben
			Dialog_Control_Set_Timestamp_Warning(hwndDlg);
			break;

		case 1301:
			Sound_Play_Short();
			break;
		case 1302:
			Sound_Play_Long();
			break;
		case 1303:
			Sound_Abort();
			break;
	}
}

static void Dialog_Control_Command(HWND hwndDlg, int Ctrl_ID, int Notification_Code)
{
	if (Notification_Code == BN_CLICKED)
	{
		switch (Ctrl_ID)
		{
			case IDC_BUTTON_CHANGE_LAYOUT:
				Dialog_Control_Button_Layout_Change(hwndDlg);
				break;

			case IDC_BUTTON_HOME_SCORE_ADD:
				Dialog_Control_Button_Score_Add(hwndDlg, 1);
				break;
			case IDC_BUTTON_GUEST_SCORE_ADD:
				Dialog_Control_Button_Score_Add(hwndDlg, 2);
				break;
			case IDC_BUTTON_HOME_SCORE_SUB:
				Dialog_Control_Button_Score_Sub(hwndDlg, 1);
				break;
			case IDC_BUTTON_GUEST_SCORE_SUB:
				Dialog_Control_Button_Score_Sub(hwndDlg, 2);
				break;
			case IDC_BUTTON_SET_FINISH:
				Dialog_Control_Button_Score_End_Set(hwndDlg, TRUE);
				break;

			case IDC_BUTTON_HOME_PLAYER_CHANGE_ADD:
				Dialog_Control_Button_PlayerChange_Edit(hwndDlg, 1, 1);
				break;
			case IDC_BUTTON_GUEST_PLAYER_CHANGE_ADD:
				Dialog_Control_Button_PlayerChange_Edit(hwndDlg, 2, 1);
				break;
			case IDC_BUTTON_HOME_PLAYER_CHANGE_SUB:
				Dialog_Control_Button_PlayerChange_Edit(hwndDlg, 1, -1);
				break;
			case IDC_BUTTON_GUEST_PLAYER_CHANGE_SUB:
				Dialog_Control_Button_PlayerChange_Edit(hwndDlg, 2, -1);
				break;

			case IDC_BUTTON_HOME_TIMEOUT_ADD:
				Dialog_Control_Button_Timeout_Add(hwndDlg, 1);
				break;
			case IDC_BUTTON_GUEST_TIMEOUT_ADD:
				Dialog_Control_Button_Timeout_Add(hwndDlg, 2);
				break;
			case IDC_BUTTON_HOME_TIMEOUT_SUB:
				Dialog_Control_Button_Timeout_Sub(hwndDlg, 1);
				break;
			case IDC_BUTTON_GUEST_TIMEOUT_SUB:
				Dialog_Control_Button_Timeout_Sub(hwndDlg, 2);
				break;
			case IDC_BUTTON_TECH_TIMEOUT:
				Dialog_Control_Button_Tech_Timeout(hwndDlg);
				break;
			case IDC_BUTTON_ABORT_TIMER:
				Dialog_Control_Button_Abort_Timeout(hwndDlg);
				break;

			case IDC_BUTTON_PICTURE_DOWN:
				Dialog_Control_Button_Picture_Move_Down(hwndDlg);
				break;
			case IDC_BUTTON_PICTURE_UP:
				Dialog_Control_Button_Picture_Move_Up(hwndDlg);
				break;
			case IDC_BUTTON_PICTURE_SHOW:
				Dialog_Control_Button_Picture_Set_Current(hwndDlg);
				break;
			case IDC_BUTTON_PICTURE_NEXT:
				Dialog_Control_Button_Picture_Set_Next(hwndDlg);
				break;
			case IDC_CHECKBOX_AUTO_PICTURE:
				Dialog_Control_Button_Checkbox_Set_Timer(hwndDlg);
				break;
		}
	}
	else if (Notification_Code == LBN_DBLCLK)
	{
		switch (Ctrl_ID)
		{
			case IDC_LIST_PICTURES:
				Dialog_Control_Button_Picture_Set_Current(hwndDlg);
				break;
		}
	}
}


// ============================================================================


static void Dialog_Control_Close(HWND hwndDlg)
{
	Picture_Timer_Stop(&Dialog_Control_Picture_Timer);
	Timeout_Timer_Stop(&Dialog_Control_Timeout_Timer, FALSE);

	EndDialog(hwndDlg, 0);
}

static void Dialog_Control_Init(HWND hwndDlg)
{
    Menu_Start(hwndDlg);
		Menu_Begin("Datei");
			Menu_Add("Laden...",               1001);
			Menu_Add("Speichern...",           1002);
			Menu_Add(NULL, 0);
			Menu_Add("Beenden",                1009);
		Menu_End();
		Menu_Begin("Scoreboard");
			Menu_Add("Spiel Beschreibung...",     1101);
			Menu_Add("Maunuell Bearbeiten...",    1102);
			Menu_Add(NULL, 0);
			Menu_Add("Layout Laden...",           1103);
			Menu_Add(NULL, 0);
			Menu_Add("Punktestand Laden...",      1104);
			Menu_Add("Punktestand Speichern...",  1105);
			Menu_Add(NULL, 0);
			Menu_Add("Manueller Update",          1106);
		Menu_End();
		Dialog_Control_Settings_Menu = Menu_Begin("Einstellungen");
			Menu_Add("Vertikale Anordnung",       1201);
			Menu_Add("Warnung (5s) ausgeben",     1202);
		Menu_End();
		Menu_Begin("Hupe");
			Menu_Add("Manuell Kurz Hupen",        1301);
			Menu_Add("Manuell Lang Hupen",        1302);
			Menu_Add(NULL, 0);
			Menu_Add("Abbrechen",                 1303);
		Menu_End();

    Menu_End();

	Window_SetIcon_hInst(hwndDlg, IDI_APP_ICON, Dialog_Control_hInst);

	Dialog_Control_Fill_Layout_Combo(hwndDlg);
	Dialog_Control_Fill_Picture_List(hwndDlg);

	Dialog_Control_Update_Game_Status(hwndDlg);

	CheckDlgButton(hwndDlg, IDC_CHECKBOX_AUTO_SET_WIN, BST_CHECKED);
	CheckDlgButton(hwndDlg, IDC_CHECKBOX_VALIDATE_PLAYER_CHANGES, BST_CHECKED);
	CheckDlgButton(hwndDlg, IDC_CHECKBOX_VALIDATE_TIMEOUTS, BST_CHECKED);
	CheckDlgButton(hwndDlg, IDC_CHECKBOX_TIMEOUT_TIMER, BST_CHECKED);

	Picture_Timer_Init(&Dialog_Control_Picture_Timer, Dialog_Control_ScoreBoard, hwndDlg, TIMER_EV_ID_1);
	Timeout_Timer_Init(&Dialog_Control_Timeout_Timer, Dialog_Control_ScoreBoard, hwndDlg, TIMER_EV_ID_2);

	Monitor_Set_Primary_Pos(hwndDlg);
}


static INT_PTR CALLBACK Dialog_Control_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_INITDIALOG:
			Dialog_Control_Init(hwndDlg);
			break;

 		case WM_CLOSE:
			Dialog_Control_Close(hwndDlg);
			break;

 		case WM_COMMAND:
			if (lParam == 0 && (HIWORD(wParam) == 0 || HIWORD(wParam) == 1))
			{
				// Menu or Accelerator
				Dialog_Control_Menu_Click(hwndDlg, LOWORD(wParam));
			}
			else
			{
				// Control
				Dialog_Control_Command(hwndDlg, LOWORD(wParam), HIWORD(wParam));
			}
            break;

		case WM_TIMER:
			if      (wParam == TIMER_EV_ID_1) Picture_Timer_Event(&Dialog_Control_Picture_Timer, wParam);
			else if (wParam == TIMER_EV_ID_2) Timeout_Timer_Event(&Dialog_Control_Timeout_Timer, wParam);
			break;

        default:
            return FALSE;
    }
	return TRUE;
}

void Dialog_Control_Show(HWND Parent, Scoreboard* TheScoreBoard)
{
	if (TheScoreBoard)
	{
		Dialog_Control_ScoreBoard = TheScoreBoard;
		DialogBox(Dialog_Control_hInst, MAKEINTRESOURCE(IDD_DIALOG_CONTROL), Parent, Dialog_Control_DlgProc);
	}
}


