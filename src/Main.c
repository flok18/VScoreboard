
#include "StdInc.h"
#include "Core.h"
#include "FLib1.h"
#include "Dialog.h"



Scoreboard    MyScoreBoard;


void Main_Entry(void)
{
	char   Init_File[MAX_STR_SIZE_FILE];
	BOOL   LoadRet;


	Frontbuffer_Register_Class(Dialog_Control_hInst);
	Scoreboard_Init(&MyScoreBoard);
	Sound_Init();
	Monitor_Init();

	if (Get_CommandLine_File(Get_Win32_CommandLine(), Init_File, sizeof(Init_File)))
	{
		if (Has_File_Extension(Init_File, ".game"))
		{
			LoadRet = Scoreboard_Load_Layouts_From_File(&MyScoreBoard, Init_File);
		}
		else
		{
			LoadRet = Import_Layout_From_File(&MyScoreBoard, Init_File);
		}
		if (LoadRet == FALSE)
		{
			MsgBox_Fehler(NULL, "Fehler beim laden der Datei...");
		}
	}

	if (Scoreboard_Create_Window(&MyScoreBoard))
	{
		Scoreboard_Select_Layout(&MyScoreBoard, 0, TRUE);

		Scoreboard_Update_Static_Text(&MyScoreBoard);
		Scoreboard_Update_Picture_Box(&MyScoreBoard);
		Scoreboard_Update_Game_Status(&MyScoreBoard);

		Dialog_Control_Show(NULL, &MyScoreBoard);
	}
	else
	{
		MsgBox_Fehler(NULL, "Fehler beim erzeugen des Scoreboard-Fensters...");
	}

	Scoreboard_Release(&MyScoreBoard);
	Frontbuffer_Unregister_Class();
}

// ============================================================================

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdline, int nCmdShow)
{
	Dialog_Control_hInst = hInst;

	if (SetApplicationRunning("Global\\ScoreBoard_Mutex") == FALSE) return 1;
	SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED | ES_CONTINUOUS);

	Main_Entry();

	SetThreadExecutionState(ES_CONTINUOUS);
	SetApplicationRunning(NULL);

	return 0;
}

int __cdecl main(int argc, char** argv)
{
	Dialog_Control_hInst = GetModuleHandle(NULL);
	Main_Entry();
}

