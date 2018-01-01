

#include "StdInc.h"
#include "Core.h"
#include "Resource.h"
#include "FLib1.h"
#include "Dialog.h"


static Game*    Dialog_Game_Game   = NULL;


// ============================================================================


static void Dialog_Game_OK(HWND hwndDlg)
{
	HWND  Combo_Serve;
	HWND  Combo_SetNr;

	Combo_Serve = GetDlgItem(hwndDlg, IDC_COMBO_SERVE_TEAM);
	Combo_SetNr = GetDlgItem(hwndDlg, IDC_COMBO_CURRENT_SET_NR);

	Dialog_Game_Game->Score_Team_1 = GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SCORE,  NULL, FALSE);
	Dialog_Game_Game->Score_Team_2 = GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SCORE, NULL, FALSE);

	Dialog_Game_Game->Sets_Team_1 = GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SETS_WON,  NULL, FALSE);
	Dialog_Game_Game->Sets_Team_2 = GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SETS_WON, NULL, FALSE);

	Dialog_Game_Game->Service_Team_Nr = ComboBox_GetIndex(Combo_Serve);

	Dialog_Game_Game->Set_1_Result.Score_Team_1 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_1_RESULT,  NULL, FALSE);
	Dialog_Game_Game->Set_1_Result.Score_Team_2 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_1_RESULT, NULL, FALSE);
	Dialog_Game_Game->Set_2_Result.Score_Team_1 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_2_RESULT,  NULL, FALSE);
	Dialog_Game_Game->Set_2_Result.Score_Team_2 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_2_RESULT, NULL, FALSE);
	Dialog_Game_Game->Set_3_Result.Score_Team_1 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_3_RESULT,  NULL, FALSE);
	Dialog_Game_Game->Set_3_Result.Score_Team_2 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_3_RESULT, NULL, FALSE);
	Dialog_Game_Game->Set_4_Result.Score_Team_1 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_4_RESULT,  NULL, FALSE);
	Dialog_Game_Game->Set_4_Result.Score_Team_2 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_4_RESULT, NULL, FALSE);
	Dialog_Game_Game->Set_5_Result.Score_Team_1 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_5_RESULT,  NULL, FALSE);
	Dialog_Game_Game->Set_5_Result.Score_Team_2 = (short)GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_5_RESULT, NULL, FALSE);

	Dialog_Game_Game->Current_Set_Nr = ComboBox_GetIndex(Combo_SetNr) + 1;

	Dialog_Game_Game->Timeouts_Team_1 = GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_TIMEOUTS,  NULL, FALSE);
	Dialog_Game_Game->Timeouts_Team_2 = GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_TIMEOUTS, NULL, FALSE);

	Dialog_Game_Game->Player_Changes_Team_1 = GetDlgItemInt(hwndDlg, IDC_EDIT_HOME_PLAYER_CHANGES,  NULL, FALSE);
	Dialog_Game_Game->Player_Changes_Team_2 = GetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_PLAYER_CHANGES, NULL, FALSE);

	EndDialog(hwndDlg, 1);
}

static void Dialog_Game_Reset(HWND hwndDlg)
{
	HWND  Combo_Serve;
	HWND  Combo_SetNr;

	Combo_Serve = GetDlgItem(hwndDlg, IDC_COMBO_SERVE_TEAM);
	Combo_SetNr = GetDlgItem(hwndDlg, IDC_COMBO_CURRENT_SET_NR);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SCORE,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SCORE, 0, FALSE);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SETS_WON,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SETS_WON, 0, FALSE);

	ComboBox_SetIndex(Combo_Serve, 1);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_1_RESULT,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_1_RESULT, 0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_2_RESULT,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_2_RESULT, 0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_3_RESULT,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_3_RESULT, 0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_4_RESULT,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_4_RESULT, 0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_5_RESULT,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_5_RESULT, 0, FALSE);

	ComboBox_SetIndex(Combo_SetNr, 0);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_TIMEOUTS,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_TIMEOUTS, 0, FALSE);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_PLAYER_CHANGES,  0, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_PLAYER_CHANGES, 0, FALSE);
}

// ============================================================================

static void Dialog_Game_Command(HWND hwndDlg, int Ctrl_ID, int Notification_Code)
{
	if (Notification_Code == BN_CLICKED)
	{
		switch (Ctrl_ID)
		{
			case IDCANCEL:
				Window_SoftClose(hwndDlg);
				break;
			case IDOK:
				Dialog_Game_OK(hwndDlg);
				break;
			case IDC_BUTTON_RESET:
				Dialog_Game_Reset(hwndDlg);
				break;
		}
	}
}


// ============================================================================


static void Dialog_Game_Init(HWND hwndDlg)
{
	HWND  Combo_Serve;
	HWND  Combo_SetNr;

	Combo_Serve = GetDlgItem(hwndDlg, IDC_COMBO_SERVE_TEAM);
	Combo_SetNr = GetDlgItem(hwndDlg, IDC_COMBO_CURRENT_SET_NR);

	ComboBox_AddItem(Combo_Serve, " ----");
	ComboBox_AddItem(Combo_Serve, "Heim Team");
	ComboBox_AddItem(Combo_Serve, "Gast Team");

	ComboBox_AddItem(Combo_SetNr, " 1. Satz");
	ComboBox_AddItem(Combo_SetNr, " 2. Satz");
	ComboBox_AddItem(Combo_SetNr, " 3. Satz");
	ComboBox_AddItem(Combo_SetNr, " 4. Satz");
	ComboBox_AddItem(Combo_SetNr, " 5. Satz");


	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SCORE,  Dialog_Game_Game->Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SCORE, Dialog_Game_Game->Score_Team_2, FALSE);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SETS_WON,  Dialog_Game_Game->Sets_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SETS_WON, Dialog_Game_Game->Sets_Team_2, FALSE);

	ComboBox_SetIndex(Combo_Serve, Dialog_Game_Game->Service_Team_Nr);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_1_RESULT,  (int)Dialog_Game_Game->Set_1_Result.Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_1_RESULT, (int)Dialog_Game_Game->Set_1_Result.Score_Team_2, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_2_RESULT,  (int)Dialog_Game_Game->Set_2_Result.Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_2_RESULT, (int)Dialog_Game_Game->Set_2_Result.Score_Team_2, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_3_RESULT,  (int)Dialog_Game_Game->Set_3_Result.Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_3_RESULT, (int)Dialog_Game_Game->Set_3_Result.Score_Team_2, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_4_RESULT,  (int)Dialog_Game_Game->Set_4_Result.Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_4_RESULT, (int)Dialog_Game_Game->Set_4_Result.Score_Team_2, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_SET_5_RESULT,  (int)Dialog_Game_Game->Set_5_Result.Score_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_SET_5_RESULT, (int)Dialog_Game_Game->Set_5_Result.Score_Team_2, FALSE);

	ComboBox_SetIndex(Combo_SetNr, Dialog_Game_Game->Current_Set_Nr - 1);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_TIMEOUTS,  Dialog_Game_Game->Timeouts_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_TIMEOUTS, Dialog_Game_Game->Timeouts_Team_2, FALSE);

	SetDlgItemInt(hwndDlg, IDC_EDIT_HOME_PLAYER_CHANGES,  Dialog_Game_Game->Player_Changes_Team_1, FALSE);
	SetDlgItemInt(hwndDlg, IDC_EDIT_GUEST_PLAYER_CHANGES, Dialog_Game_Game->Player_Changes_Team_2, FALSE);

}


static INT_PTR CALLBACK Dialog_Game_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_INITDIALOG:
			Dialog_Game_Init(hwndDlg);
			break;

 		case WM_CLOSE:
			EndDialog(hwndDlg, 0);
			break;

 		case WM_COMMAND:
			Dialog_Game_Command(hwndDlg, LOWORD(wParam), HIWORD(wParam));
            break;

        default:
            return FALSE;
    }
	return TRUE;
}

int Dialog_Game_Show(HWND Parent, Game* TheGame)
{
	int  Ret_Status  = 0;

	if (TheGame)
	{
		Dialog_Game_Game = TheGame;
		Ret_Status = (int)DialogBox(Dialog_Control_hInst, MAKEINTRESOURCE(IDD_DIALOG_GAME), Parent, Dialog_Game_DlgProc);
	}

	return Ret_Status;
}


