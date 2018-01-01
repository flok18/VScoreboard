
#include "StdInc.h"
#include "Core.h"
#include "Resource.h"
#include "FLib1.h"
#include "Dialog.h"


static Scoreboard*    Dialog_Description_ScoreBoard   = NULL;


// ============================================================================


static void Dialog_Description_OK(HWND hwndDlg)
{
	GetDlgItemText(hwndDlg, IDC_EDIT_GAME_DESC, Dialog_Description_ScoreBoard->Title, MAX_STR_SIZE_NAME);
	GetDlgItemText(hwndDlg, IDC_EDIT_HOME_TEAM_DESC, Dialog_Description_ScoreBoard->Team_1, MAX_STR_SIZE_NAME);
	GetDlgItemText(hwndDlg, IDC_EDIT_GUEST_TEAM_DESC, Dialog_Description_ScoreBoard->Team_2, MAX_STR_SIZE_NAME);

	EndDialog(hwndDlg, 1);
}


// ============================================================================

static void Dialog_Description_Command(HWND hwndDlg, int Ctrl_ID, int Notification_Code)
{
	if (Notification_Code == BN_CLICKED)
	{
		switch (Ctrl_ID)
		{
			case IDCANCEL:
				Window_SoftClose(hwndDlg);
				break;
			case IDOK:
				Dialog_Description_OK(hwndDlg);
				break;
		}
	}
}



// ============================================================================


static void Dialog_Description_Init(HWND hwndDlg)
{
	SetDlgItemText(hwndDlg, IDC_EDIT_GAME_DESC, Dialog_Description_ScoreBoard->Title);
	SetDlgItemText(hwndDlg, IDC_EDIT_HOME_TEAM_DESC, Dialog_Description_ScoreBoard->Team_1);
	SetDlgItemText(hwndDlg, IDC_EDIT_GUEST_TEAM_DESC, Dialog_Description_ScoreBoard->Team_2);
}


static INT_PTR CALLBACK Dialog_Description_DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_INITDIALOG:
			Dialog_Description_Init(hwndDlg);
			break;

 		case WM_CLOSE:
			EndDialog(hwndDlg, 0);
			break;

 		case WM_COMMAND:
			Dialog_Description_Command(hwndDlg, LOWORD(wParam), HIWORD(wParam));
            break;

        default:
            return FALSE;
    }
	return TRUE;
}

int Dialog_Description_Show(HWND Parent, Scoreboard* TheScoreBoard)
{
	int  Ret_Status  = 0;

	if (TheScoreBoard)
	{
		Dialog_Description_ScoreBoard = TheScoreBoard;
		Ret_Status = (int)DialogBox(Dialog_Control_hInst, MAKEINTRESOURCE(IDD_DIALOG_DESCRIPTION), Parent, Dialog_Description_DlgProc);
	}

	return Ret_Status;
}


