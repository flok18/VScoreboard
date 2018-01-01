

#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"



#define             FileIO_FILE_ID_SIZE     18
static const char*  FileIO_File_ID        = "# <- ScoreBoard ->";
static const int    FileIO_File_Version   = 1;

#define             FileIO_GAME_ID_SIZE     17
static const char*  FileIO_Game_ID        = "# <- GameScore ->";
static const int    FileIO_Game_Version   = 1;


static int    Interp_Current_Font_Index;
static DWORD  Interp_Current_Color_Value;
static char   Interp_Relative_Path_Root[MAX_STR_SIZE_FILE];


static void Layout_Command_Element(Element* TheElement, char* Arg1)
{
	char*   Arg2;
	char*   Arg3;
	char*   Arg4;
	char*   Arg5;

	// No spaces in Args
	Deflate_Command(Arg1);

	Arg2 = Split_Command(Arg1, ',');
	Arg3 = Split_Command(Arg2, ',');
	Arg4 = Split_Command(Arg3, ',');
	Arg5 = Split_Command(Arg4, ',');

	if (*Arg4 == '\0') return;
	TheElement->Pos_X  = atoi(Arg1);
	TheElement->Pos_Y  = atoi(Arg2);
	TheElement->Size_X = atoi(Arg3);
	TheElement->Size_Y = atoi(Arg4);

	if (*Arg5 == '\0') TheElement->Orientation = ELEMENT_ORIENTATION_HORIZONTAL;
	else    		   TheElement->Orientation = atoi(Arg5);

	TheElement->Is_Configured = TRUE;
	TheElement->Font_Index    = Interp_Current_Font_Index;
	TheElement->Font_Color    = Interp_Current_Color_Value;
}

static void Layout_Command_Pic_Timer(Layout* TheLayout, char* Arg1)
{
	TheLayout->Picture_Timer_Interval = atoi(Arg1) * 1000;
}

static void Layout_Command_Pic_FileList(Layout* TheLayout, char* Arg1)
{
	char  FullPath[MAX_STR_SIZE_FILE];

	Trim_Command(Arg1);

	if (Is_Path_Absolute(Arg1))
	{
		strcpy_ss(FullPath, sizeof(FullPath), Arg1);
	}
	else
	{
		strcpy_ss(FullPath, sizeof(FullPath), Interp_Relative_Path_Root);
		strcat_ss(FullPath, sizeof(FullPath), Arg1);
	}

	FileList_Create(&TheLayout->Picture_List, FullPath);
}

static void Layout_Command_Background(Layout* TheLayout, char* Arg1)
{
	Trim_Command(Arg1);

	if (Is_Path_Absolute(Arg1))
	{
		strcpy_ss(TheLayout->Background_File, MAX_STR_SIZE_FILE, Arg1);
	}
	else
	{
		strcpy_ss(TheLayout->Background_File, MAX_STR_SIZE_FILE, Interp_Relative_Path_Root);
		strcat_ss(TheLayout->Background_File, MAX_STR_SIZE_FILE, Arg1);
	}
}


static void Layout_Command_ScB_String(char* String, char* Arg1)
{
	strcpy_ss(String, MAX_STR_SIZE_NAME, Arg1);
}

static void Layout_Command_Layout(Scoreboard* TheScoreboard, char* Arg1)
{
	Scoreboard_New_Layout(TheScoreboard, Arg1);
}

static void Layout_Command_Font(Scoreboard* TheScoreboard, char* Arg1)
{
	char*   Arg2;
	char*   Arg3;
	char*   Arg4;
	int     NewHeight;
	int     NewWeight;
	int     NewAngle;

	// No spaces in Args
	Deflate_Command(Arg1);
	Lower_Command(Arg1);

	Arg2 = Split_Command(Arg1, ',');
	Arg3 = Split_Command(Arg2, ',');
	Arg4 = Split_Command(Arg3, ',');

	if (*Arg2 == '\0') return;
	NewHeight = atoi(Arg2);

	if (*Arg3 == '\0') NewWeight = FW_NORMAL;
	else    		   NewWeight = atoi(Arg3);

	if (*Arg4 == '\0') NewAngle = 0;
	else               NewAngle = atoi(Arg4);

	Interp_Current_Font_Index = FontCollection_New_Font(&TheScoreboard->Fonts, Arg1, NewHeight, NewWeight, NewAngle);
}

static void Layout_Command_Color(char* Arg1)
{
	char*   Arg2;
	char*   Arg3;
	DWORD   Rval;
	DWORD   Gval;
	DWORD   Bval;

	Deflate_Command(Arg1);

	Arg2 = Split_Command(Arg1, ',');
	Arg3 = Split_Command(Arg2, ',');

	Rval = (DWORD)atoi(Arg1) & 0x000000FF;
	Gval = (DWORD)atoi(Arg2) & 0x000000FF;
	Bval = (DWORD)atoi(Arg3) & 0x000000FF;

	Interp_Current_Color_Value =  Rval | (Gval << 8) | (Bval << 16);
}



static void Layout_Interpreter(Scoreboard* TheScoreboard, char* CmdLine)
{
	char* Args;

	Args = Split_Command(CmdLine, '=');
	Deflate_Command(CmdLine);
	Lower_Command(CmdLine);

	if (!strcmp(CmdLine, "color"))
	{
		Layout_Command_Color(Args);
	}
	else if (!strcmp(CmdLine, "font"))
	{
		Layout_Command_Font(TheScoreboard, Args);
	}
	else if (!strcmp(CmdLine, "layout"))
	{
		Layout_Command_Layout(TheScoreboard, Args);
	}
	else if (!strcmp(CmdLine, "title"))
	{
		Layout_Command_ScB_String(TheScoreboard->Title, Args);
	}
	else if (!strcmp(CmdLine, "team1"))
	{
		Layout_Command_ScB_String(TheScoreboard->Team_1, Args);
	}
	else if (!strcmp(CmdLine, "team2"))
	{
		Layout_Command_ScB_String(TheScoreboard->Team_2, Args);
	}
	else
	{
		if (TheScoreboard->CurrentLayout == NULL) return;

		if (!strcmp(CmdLine, "background"))
		{
			Layout_Command_Background(TheScoreboard->CurrentLayout, Args);
		}
		else if (!strcmp(CmdLine, "pictures-path"))
		{
			Layout_Command_Pic_FileList(TheScoreboard->CurrentLayout, Args);
		}
		else if (!strcmp(CmdLine, "pictures-timer"))
		{
			Layout_Command_Pic_Timer(TheScoreboard->CurrentLayout, Args);
		}
		else if (!strcmp(CmdLine, "pictures-box"))
		{
			Layout_Command_Element(&TheScoreboard->CurrentLayout->Picture_Box, Args);
		}
		else if (!strcmp(CmdLine, "score-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SCORE_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SCORE_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-setnr"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_CURRENT_SET_NUM, Args);
		}
		else if (!strcmp(CmdLine, "score-sets-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SETS_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-sets-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SETS_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-set1-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_1_RESULT_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-set1-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_1_RESULT_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-set2-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_2_RESULT_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-set2-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_2_RESULT_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-set3-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_3_RESULT_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-set3-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_3_RESULT_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-set4-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_4_RESULT_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-set4-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_4_RESULT_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-set5-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_5_RESULT_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-set5-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SET_5_RESULT_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-timeouttimer"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_TIMEOUT_TIMER, Args);
		}
		else if (!strcmp(CmdLine, "score-timeouts-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_TIMEOUTS_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-timeouts-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_TIMEOUTS_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-timeouttext"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_TIMEOUT_TEXT, Args);
		}
		else if (!strcmp(CmdLine, "score-service-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-service-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "score-playerchanges-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_PLAYER_CHANGE_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "score-playerchanges-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_PLAYER_CHANGE_TEAM_2, Args);
		}
		else if (!strcmp(CmdLine, "text-game-title"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_GAME_TITLE, Args);
		}
		else if (!strcmp(CmdLine, "text-name-team1"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_NAME_TEAM_1, Args);
		}
		else if (!strcmp(CmdLine, "text-name-team2"))
		{
			Layout_Command_Element(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_NAME_TEAM_2, Args);
		}
		else
		{
			_dbg_print("Unknown Command: %s\n", CmdLine);
		}
	}
}


BOOL Import_Layout_From_File(Scoreboard* TheScoreboard, const char* Layout_File)
{
	char    Buffer[512];
	FILE*   FPtr;

	Interp_Current_Font_Index  = INVALID_INDEX;
	Interp_Current_Color_Value = 0x00000000;

	strcpy_ss(Interp_Relative_Path_Root, MAX_STR_SIZE_FILE, Layout_File);
	RemoveFileFromFullPath(Interp_Relative_Path_Root);

	FPtr = File_IO_Open(Layout_File, "rb");
	if (FPtr == NULL) return FALSE;

	while (InputStringToBuffer(Buffer, sizeof(Buffer), FPtr))
	{
		if (*Buffer)
		{
			if (*Buffer != ';')
			{
				Layout_Interpreter(TheScoreboard, Buffer);
			}
		}
	}

	fclose(FPtr);
	return TRUE;
}


// --------------------------------------------------------------------


BOOL Scoreboard_Save_Layouts_To_File(Scoreboard* TheScoreboard, const char* Scoreboard_File)
{
	FILE*    TmpFilePtr;

	TmpFilePtr = File_IO_Open(Scoreboard_File, "wb");
	if (TmpFilePtr)
	{
		// Dateikopf
		fwrite(FileIO_File_ID,       sizeof(char), FileIO_FILE_ID_SIZE, TmpFilePtr);
		fwrite(&FileIO_File_Version, sizeof(int),  1,                   TmpFilePtr);

		Scoreboard_Save(TheScoreboard, TmpFilePtr);

		fclose(TmpFilePtr);
		return TRUE;
	}
	return FALSE;
}


BOOL Scoreboard_Load_Layouts_From_File(Scoreboard* TheScoreboard, const char* Scoreboard_File)
{
	FILE*    TmpFilePtr;
	char     ThisID[FileIO_FILE_ID_SIZE];
	int      ThisVersion      = 0;
	BOOL     RetVal           = FALSE;

	TmpFilePtr = File_IO_Open(Scoreboard_File, "rb");
	if (TmpFilePtr)
	{
		// Dateikopf
		fread(ThisID,       sizeof(char), FileIO_FILE_ID_SIZE, TmpFilePtr);
		fread(&ThisVersion, sizeof(int),  1,                   TmpFilePtr);

		// Check Dateikopf
		if (memcmp(ThisID, FileIO_File_ID, FileIO_FILE_ID_SIZE) == 0)
		{
			if (ThisVersion == FileIO_File_Version)
			{
				Scoreboard_Load(TheScoreboard, TmpFilePtr);
				RetVal = TRUE;
			}
		}
		fclose(TmpFilePtr);
	}
	return RetVal;
}

// --------------------------------------------------------------------


BOOL Game_Save_Score_To_File(Game* TheGame, const char*  Game_File)
{
	FILE*    TmpFilePtr;

	TmpFilePtr = File_IO_Open(Game_File, "wb");
	if (TmpFilePtr)
	{
		// Dateikopf
		fwrite(FileIO_Game_ID,       sizeof(char), FileIO_GAME_ID_SIZE, TmpFilePtr);
		fwrite(&FileIO_Game_Version, sizeof(int),  1,                   TmpFilePtr);

		Game_Save(TheGame, TmpFilePtr);

		fclose(TmpFilePtr);
		return TRUE;
	}
	return FALSE;
}

BOOL Game_Load_Score_From_File(Game* TheGame, const char* Game_File)
{
	FILE*    TmpFilePtr;
	char     ThisID[FileIO_GAME_ID_SIZE];
	int      ThisVersion      = 0;
	BOOL     RetVal           = FALSE;

	TmpFilePtr = File_IO_Open(Game_File, "rb");
	if (TmpFilePtr)
	{
		// Dateikopf
		fread(ThisID,       sizeof(char), FileIO_GAME_ID_SIZE, TmpFilePtr);
		fread(&ThisVersion, sizeof(int),  1,                   TmpFilePtr);

		// Check Dateikopf
		if (memcmp(ThisID, FileIO_Game_ID, FileIO_GAME_ID_SIZE) == 0)
		{
			if (ThisVersion == FileIO_Game_Version)
			{
				Game_Load(TheGame, TmpFilePtr);
				RetVal = TRUE;
			}
		}
		fclose(TmpFilePtr);
	}
	return RetVal;
}

