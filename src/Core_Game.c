
#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"


/*
	ToDo:
	- History (Undo) Funktion
*/


void SetResult_Reset(SetResult* TheSetResult)
{
	if (TheSetResult)
	{
		memset(TheSetResult, 0, sizeof(SetResult));
	}
}

void Game_Reset(Game* TheGame)
{
	if (TheGame)
	{
		TheGame->Score_Team_1    = 0;
		TheGame->Score_Team_2    = 0;
		TheGame->Service_Team_Nr = 2;
		TheGame->Current_Set_Nr  = 1;
		TheGame->Sets_Team_1     = 0;
		TheGame->Sets_Team_2     = 0;

		SetResult_Reset(&TheGame->Set_1_Result);
		SetResult_Reset(&TheGame->Set_2_Result);
		SetResult_Reset(&TheGame->Set_3_Result);
		SetResult_Reset(&TheGame->Set_4_Result);
		SetResult_Reset(&TheGame->Set_5_Result);

		TheGame->Timeouts_Team_1        = 0;
		TheGame->Timeouts_Team_2        = 0;
		TheGame->Player_Changes_Team_1  = 0;
		TheGame->Player_Changes_Team_2  = 0;
	}
}

void Game_Copy(Game* FromGame, Game* ToGame)
{
	memcpy(ToGame, FromGame, sizeof(Game));
}


// --------------------------------------------------------------------


void Game_Save(Game* TheGame, FILE* TheFP)
{
	fwrite(TheGame, sizeof(Game), 1, TheFP);
}

void Game_Load(Game* TheGame, FILE* TheFP)
{
	fread(TheGame, sizeof(Game), 1, TheFP);
}


// --------------------------------------------------------------------


SetResult* Game_Get_Any_Set_Result(Game* TheGame, int SetNumber)
{
	switch (SetNumber)
	{
		case 1: return &TheGame->Set_1_Result;
		case 2: return &TheGame->Set_2_Result;
		case 3: return &TheGame->Set_3_Result;
		case 4: return &TheGame->Set_4_Result;
		case 5: return &TheGame->Set_5_Result;
	}
	return NULL;
}

SetResult* Game_Get_Current_Set_Result(Game* TheGame)
{
	return Game_Get_Any_Set_Result(TheGame, TheGame->Current_Set_Nr);
}




// --------------------------------------------------------------------


static int Game_Build_Summary_Horizontal_Add_Set(char* Buffer, size_t BufferSize, int SetNr, short Res1, short Res2)
{
	int  RetNumChars  = 0;

	if (Res1 > 0 || Res2 > 0)
	{
		RetNumChars = sprintf_ss(Buffer, BufferSize,
				                 CRLF " %2i:      %2i:%2i   ",
				                 SetNr, Res1, Res2);
	}
	return RetNumChars;
}

void Game_Build_Summary_Horizontal(Game* TheGame, char* Out_Buffer, size_t Buffer_Size)
{
	const char*   ServeMark1;
	const char*   ServeMark2;
	int           CurSize;


	ServeMark1 = " ";
	ServeMark2 = " ";
	if      (TheGame->Service_Team_Nr == 1) ServeMark1 = "*";
	else if (TheGame->Service_Team_Nr == 2) ServeMark2 = "*";


	CurSize = sprintf_ss(Out_Buffer, Buffer_Size,
	                     " %2i:    %s %2i:%2i %s" CRLF
	                     "-----------------------" CRLF
			             "          %2i:%2i   " CRLF
	                     "-----------------------",
			             TheGame->Current_Set_Nr, ServeMark1, TheGame->Score_Team_1, TheGame->Score_Team_2, ServeMark2,
			             TheGame->Sets_Team_1, TheGame->Sets_Team_2);

	CurSize += Game_Build_Summary_Horizontal_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 1, TheGame->Set_1_Result.Score_Team_1, TheGame->Set_1_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Horizontal_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 2, TheGame->Set_2_Result.Score_Team_1, TheGame->Set_2_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Horizontal_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 3, TheGame->Set_3_Result.Score_Team_1, TheGame->Set_3_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Horizontal_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 4, TheGame->Set_4_Result.Score_Team_1, TheGame->Set_4_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Horizontal_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 5, TheGame->Set_5_Result.Score_Team_1, TheGame->Set_5_Result.Score_Team_2);
}

static int Game_Build_Summary_Vertical_Add_Set(char* Buffer, size_t BufferSize, int Zeile, short Res1, short Res2)
{
	int  RetNumChars;

	if (Res1 > 0 || Res2 > 0)
	{
		if (Zeile)
		{
			RetNumChars = sprintf_ss(Buffer, BufferSize, " %2i", Res2);
		}
		else
		{
			RetNumChars = sprintf_ss(Buffer, BufferSize, " %2i", Res1);
		}
	}
	else
	{
		strcpy_ss(Buffer, BufferSize, "   ");
		RetNumChars = 3;
	}

	return RetNumChars;
}


void Game_Build_Summary_Vertical(Game* TheGame, char* Out_Buffer, size_t Buffer_Size)
{
	const char*   ServeMark1;
	const char*   ServeMark2;
	int           CurSize;


	ServeMark1 = " ";
	ServeMark2 = " ";
	if      (TheGame->Service_Team_Nr == 1) ServeMark1 = "*";
	else if (TheGame->Service_Team_Nr == 2) ServeMark2 = "*";


	CurSize = sprintf_ss(Out_Buffer, Buffer_Size,
						 "    %i         1  2  3  4  5 "  CRLF
						 "----------------------------" CRLF,
						 TheGame->Current_Set_Nr);

	CurSize += sprintf_ss(Out_Buffer + CurSize, Buffer_Size - CurSize,
	                      " %s %2i | %2i |",
	                     ServeMark1, TheGame->Score_Team_1, TheGame->Sets_Team_1);

	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 0, TheGame->Set_1_Result.Score_Team_1, TheGame->Set_1_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 0, TheGame->Set_2_Result.Score_Team_1, TheGame->Set_2_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 0, TheGame->Set_3_Result.Score_Team_1, TheGame->Set_3_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 0, TheGame->Set_4_Result.Score_Team_1, TheGame->Set_4_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 0, TheGame->Set_5_Result.Score_Team_1, TheGame->Set_5_Result.Score_Team_2);

	strcpy_ss(Out_Buffer + CurSize, Buffer_Size - CurSize, CRLF "----------------------------" CRLF);
	CurSize += (2 + 28 + 2);

	CurSize += sprintf_ss(Out_Buffer + CurSize, Buffer_Size - CurSize,
	                      " %s %2i | %2i |",
	                     ServeMark2, TheGame->Score_Team_2, TheGame->Sets_Team_2);

	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 1, TheGame->Set_1_Result.Score_Team_1, TheGame->Set_1_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 1, TheGame->Set_2_Result.Score_Team_1, TheGame->Set_2_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 1, TheGame->Set_3_Result.Score_Team_1, TheGame->Set_3_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 1, TheGame->Set_4_Result.Score_Team_1, TheGame->Set_4_Result.Score_Team_2);
	CurSize += Game_Build_Summary_Vertical_Add_Set(Out_Buffer + CurSize, Buffer_Size - CurSize, 1, TheGame->Set_5_Result.Score_Team_1, TheGame->Set_5_Result.Score_Team_2);

	strcpy_ss(Out_Buffer + CurSize, Buffer_Size - CurSize, CRLF "----------------------------");
}


