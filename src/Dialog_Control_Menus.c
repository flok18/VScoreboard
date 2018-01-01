

#include "StdInc.h"
#include "Core.h"
#include "FLib1.h"
#include "Dialog.h"


HMENU  Dialog_Control_Settings_Menu        = NULL;
BOOL   Dialog_Control_Is_Vertical_Summary  = FALSE;



static DWORD  Button_Check_Timestamps[3]  = {0, 0, 0};
static BOOL   Button_Do_Timestamp_Check   = FALSE;



BOOL Check_Button_Timestamp(HWND hwndDlg, int Button_Type_Index)
{
	DWORD  CurrentTick;

	CurrentTick = GetTickCount();

	if (Button_Do_Timestamp_Check)
	{
		if (CurrentTick - Button_Check_Timestamps[Button_Type_Index] < 5000)
		{
			if (MsgBox_Frage(hwndDlg, "Es sind seit der letzten Aktion keine 5s vergangen..." CRLF
									  "Wollen Sie fortfahren ?") == FALSE)
			{
				return TRUE;
			}
		}
	}

	Button_Check_Timestamps[Button_Type_Index] = CurrentTick;
	return FALSE;
}




void Dialog_Control_Do_Manual_Update(HWND hwndDlg)
{
	Scoreboard_Update_Static_Text(Dialog_Control_ScoreBoard);
	Scoreboard_Update_Picture_Box(Dialog_Control_ScoreBoard);
	Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);

	Dialog_Control_Fill_Layout_Combo(hwndDlg);
	Dialog_Control_Fill_Picture_List(hwndDlg);

	Dialog_Control_Update_Game_Status(hwndDlg);
}


void Dialog_Control_Set_Timestamp_Warning(HWND hwndDlg)
{
	if (Button_Do_Timestamp_Check)
	{
		Button_Do_Timestamp_Check = FALSE;
		CheckMenuItem(Dialog_Control_Settings_Menu, 1202, MF_BYCOMMAND|MF_UNCHECKED);
	}
	else
	{
		Button_Do_Timestamp_Check = TRUE;
		CheckMenuItem(Dialog_Control_Settings_Menu, 1202, MF_BYCOMMAND|MF_CHECKED);
	}
}


void Dialog_Control_Set_Vertical_Summary(HWND hwndDlg)
{
	if (Dialog_Control_Is_Vertical_Summary)
	{
		Dialog_Control_Is_Vertical_Summary = FALSE;
		CheckMenuItem(Dialog_Control_Settings_Menu, 1201, MF_BYCOMMAND|MF_UNCHECKED);
	}
	else
	{
		Dialog_Control_Is_Vertical_Summary = TRUE;
		CheckMenuItem(Dialog_Control_Settings_Menu, 1201, MF_BYCOMMAND|MF_CHECKED);
	}
	Dialog_Control_Update_Game_Status(hwndDlg);
}


void Dialog_Control_Show_Edit_Description(HWND hwndDlg)
{
	if (Dialog_Description_Show(hwndDlg, Dialog_Control_ScoreBoard))
	{
		Scoreboard_Update_Static_Text(Dialog_Control_ScoreBoard);
	}
}

void Dialog_Control_Show_Edit_Game_Status(HWND hwndDlg)
{
	if (Dialog_Game_Show(hwndDlg, &Dialog_Control_ScoreBoard->Game_Status))
	{
		Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
		Dialog_Control_Update_Game_Status(hwndDlg);
	}
}



void Dialog_Control_Menu_File_Save(HWND hwndDlg)
{
    char    NewPath[MAX_PATH];

	*NewPath = '\0';
	if (Shell_Dialog_GetFile_Save_hInst(Dialog_Control_hInst, hwndDlg,
                                        " Datei Speichern... ", " Game - Datei (*.game)|*.game",
                                        NewPath, sizeof(NewPath), NULL))
	{
		if (Scoreboard_Save_Layouts_To_File(Dialog_Control_ScoreBoard, NewPath) == FALSE)
		{
			MsgBox_Fehler(hwndDlg, "Fehler beim speichern...");
		}
	}
}

void Dialog_Control_Menu_File_Load(HWND hwndDlg)
{
    char         NewPath[MAX_PATH];
	Scoreboard   LoadSCB;

	*NewPath = '\0';
	if (Shell_Dialog_GetFile_Open_hInst(Dialog_Control_hInst, hwndDlg,
                                        " Datei Laden... ", " Game - Datei (*.game)|*.game| Alle Dateien|*.*",
                                        NewPath, sizeof(NewPath), NULL))
	{
		Scoreboard_Init(&LoadSCB);

		if (Scoreboard_Load_Layouts_From_File(&LoadSCB, NewPath))
		{
			Scoreboard_Move_Data(&LoadSCB, Dialog_Control_ScoreBoard, TRUE);

			Scoreboard_Select_Layout(Dialog_Control_ScoreBoard, 0, TRUE);
			Dialog_Control_Do_Manual_Update(hwndDlg);
		}
		else
		{
			MsgBox_Fehler(hwndDlg, "Fehler beim laden...");
		}

		Scoreboard_Release(&LoadSCB);
	}
}

void Dialog_Control_Menu_Layout_Load(HWND hwndDlg)
{
    char         NewPath[MAX_PATH];
	Scoreboard   LoadSCB;

	*NewPath = '\0';
	if (Shell_Dialog_GetFile_Open_hInst(Dialog_Control_hInst, hwndDlg,
                                        " Layout Laden... ", " Layout - Datei (*.ini)|*.ini| Alle Dateien|*.*",
                                        NewPath, sizeof(NewPath), NULL))
	{
		Scoreboard_Init(&LoadSCB);

		if (Import_Layout_From_File(&LoadSCB, NewPath))
		{
			Scoreboard_Move_Data(&LoadSCB, Dialog_Control_ScoreBoard, FALSE);

			Scoreboard_Select_Layout(Dialog_Control_ScoreBoard, 0, TRUE);
			Dialog_Control_Do_Manual_Update(hwndDlg);
		}
		else
		{
			MsgBox_Fehler(hwndDlg, "Fehler beim laden...");
		}

		Scoreboard_Release(&LoadSCB);
	}
}

void Dialog_Control_Menu_Score_Save(HWND hwndDlg)
{
    char    NewPath[MAX_PATH];

	*NewPath = '\0';
	if (Shell_Dialog_GetFile_Save_hInst(Dialog_Control_hInst, hwndDlg,
                                        " Datei Speichern... ", " Punktestand - Datei (*.score)|*.score",
                                        NewPath, sizeof(NewPath), NULL))
	{
		if (Game_Save_Score_To_File(&Dialog_Control_ScoreBoard->Game_Status, NewPath) == FALSE)
		{
			MsgBox_Fehler(hwndDlg, "Fehler beim speichern...");
		}
	}
}

void Dialog_Control_Menu_Score_Load(HWND hwndDlg)
{
    char    NewPath[MAX_PATH];
	Game    LoadGam;

	*NewPath = '\0';
	if (Shell_Dialog_GetFile_Open_hInst(Dialog_Control_hInst, hwndDlg,
                                        " Datei Laden... ", " Punktestand - Datei (*.score)|*.score| Alle Dateien|*.*",
                                        NewPath, sizeof(NewPath), NULL))
	{
		if (Game_Load_Score_From_File(&LoadGam, NewPath))
		{
			Game_Copy(&LoadGam, &Dialog_Control_ScoreBoard->Game_Status);

			Scoreboard_Update_Game_Status(Dialog_Control_ScoreBoard);
			Dialog_Control_Update_Game_Status(hwndDlg);
		}
		else
		{
			MsgBox_Fehler(hwndDlg, "Fehler beim laden...");
		}
	}
}


