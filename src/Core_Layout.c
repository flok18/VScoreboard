

#include "StdInc.h"
#include "Core.h"


void Layout_Init(Layout* TheLayout)
{
	int  ii;
	
	TheLayout->Name[0] = '\0';
	
	TheLayout->Background_File[0] = '\0';
	
	for (ii = 0; ii < NUM_SCORE_ELEMENTS; ++ii)
	{
		Element_Init(TheLayout->Score_Elements + ii);
	}

	FileList_Init(&TheLayout->Picture_List);
	Element_Init(&TheLayout->Picture_Box);
	
	TheLayout->Current_Picture        = INVALID_INDEX;
	TheLayout->Picture_Timer_Interval = 0;
}

void Layout_Release(Layout* TheLayout)
{
	int  ii;
	
	TheLayout->Name[0] = '\0';
	
	TheLayout->Background_File[0] = '\0';

	for (ii = 0; ii < NUM_SCORE_ELEMENTS; ++ii)
	{
		Element_Release(TheLayout->Score_Elements + ii);
	}

	FileList_Release(&TheLayout->Picture_List);
	Element_Release(&TheLayout->Picture_Box);
	
	TheLayout->Current_Picture        = INVALID_INDEX;
	TheLayout->Picture_Timer_Interval = 0;
}


// --------------------------------------------------------------------


void Layout_Save(Layout* TheLayout, FILE* TheFP)
{
	int  ii;
	
	fwrite(TheLayout->Name,             sizeof(char), MAX_STR_SIZE_NAME, TheFP);
	fwrite(TheLayout->Background_File,  sizeof(char), MAX_STR_SIZE_FILE, TheFP);

	for (ii = 0; ii < NUM_SCORE_ELEMENTS; ++ii)
	{
		Element_Save(TheLayout->Score_Elements + ii, TheFP);
	}
	
	FileList_Save(&TheLayout->Picture_List, TheFP);
	Element_Save(&TheLayout->Picture_Box, TheFP);

	fwrite(&TheLayout->Current_Picture, sizeof(int), 1, TheFP);
	fwrite(&TheLayout->Picture_Timer_Interval, sizeof(int), 1, TheFP);
	
}


void Layout_Load(Layout* TheLayout, FILE* TheFP)
{
	int  ii;
	
	fread(TheLayout->Name,             sizeof(char), MAX_STR_SIZE_NAME, TheFP);
	fread(TheLayout->Background_File,  sizeof(char), MAX_STR_SIZE_FILE, TheFP);
	
	for (ii = 0; ii < NUM_SCORE_ELEMENTS; ++ii)
	{
		Element_Load(TheLayout->Score_Elements + ii, TheFP);
	}	
	
	FileList_Load(&TheLayout->Picture_List, TheFP);
	Element_Load(&TheLayout->Picture_Box, TheFP);
	
	fread(&TheLayout->Current_Picture, sizeof(int), 1, TheFP);
	fread(&TheLayout->Picture_Timer_Interval, sizeof(int), 1, TheFP);
}


// --------------------------------------------------------------------


void Layout_Draw_Game_Status(Layout* TheLayout, Game* TheGame, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection)
{
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SCORE_TEAM_1, TheGame->Score_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SCORE_TEAM_2, TheGame->Score_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	
	if (TheGame->Service_Team_Nr == 1)
	{
		Element_Draw_Simple_Led_On(TheLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_1, 0x0000FF00, TheBackPic, TheBackbuffer);
		Element_Draw_Simple_Led_Off(TheLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_2, TheBackPic, TheBackbuffer);
	}
	else if (TheGame->Service_Team_Nr == 2)
	{
		Element_Draw_Simple_Led_Off(TheLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Draw_Simple_Led_On(TheLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_2, 0x0000FF00, TheBackPic, TheBackbuffer);
	}
	else
	{
		Element_Draw_Simple_Led_Off(TheLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Draw_Simple_Led_Off(TheLayout->Score_Elements + ELEMENT_ID_SERVICE_LED_TEAM_2, TheBackPic, TheBackbuffer);
	}
	
	
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_CURRENT_SET_NUM, TheGame->Current_Set_Nr, TheBackPic, TheBackbuffer, TheFontCollection);
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SETS_TEAM_1, TheGame->Sets_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SETS_TEAM_2, TheGame->Sets_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	
	
	if (TheGame->Set_1_Result.Score_Team_1 > 0 || TheGame->Set_1_Result.Score_Team_2 > 0)
	{
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_1_RESULT_TEAM_1, TheGame->Set_1_Result.Score_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_1_RESULT_TEAM_2, TheGame->Set_1_Result.Score_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	}
	else
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_1_RESULT_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_1_RESULT_TEAM_2, TheBackPic, TheBackbuffer);
	}
	if (TheGame->Set_2_Result.Score_Team_1 > 0 || TheGame->Set_2_Result.Score_Team_2 > 0)
	{
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_2_RESULT_TEAM_1, TheGame->Set_2_Result.Score_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_2_RESULT_TEAM_2, TheGame->Set_2_Result.Score_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	}
	else
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_2_RESULT_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_2_RESULT_TEAM_2, TheBackPic, TheBackbuffer);
	}
	if (TheGame->Set_3_Result.Score_Team_1 > 0 || TheGame->Set_3_Result.Score_Team_2 > 0)
	{
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_3_RESULT_TEAM_1, TheGame->Set_3_Result.Score_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_3_RESULT_TEAM_2, TheGame->Set_3_Result.Score_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	}
	else
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_3_RESULT_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_3_RESULT_TEAM_2, TheBackPic, TheBackbuffer);
	}
	if (TheGame->Set_4_Result.Score_Team_1 > 0 || TheGame->Set_4_Result.Score_Team_2 > 0)
	{
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_4_RESULT_TEAM_1, TheGame->Set_4_Result.Score_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_4_RESULT_TEAM_2, TheGame->Set_4_Result.Score_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	}
	else
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_4_RESULT_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_4_RESULT_TEAM_2, TheBackPic, TheBackbuffer);
	}
	if (TheGame->Set_5_Result.Score_Team_1 > 0 || TheGame->Set_5_Result.Score_Team_2 > 0)
	{
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_5_RESULT_TEAM_1, TheGame->Set_5_Result.Score_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_SET_5_RESULT_TEAM_2, TheGame->Set_5_Result.Score_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	}
	else
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_5_RESULT_TEAM_1, TheBackPic, TheBackbuffer);
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_SET_5_RESULT_TEAM_2, TheBackPic, TheBackbuffer);
	}
	
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_TIMEOUTS_TEAM_1, TheGame->Timeouts_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_TIMEOUTS_TEAM_2, TheGame->Timeouts_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
	
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_PLAYER_CHANGE_TEAM_1, TheGame->Player_Changes_Team_1, TheBackPic, TheBackbuffer, TheFontCollection);
	Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_PLAYER_CHANGE_TEAM_2, TheGame->Player_Changes_Team_2, TheBackPic, TheBackbuffer, TheFontCollection);
}

void Layout_Draw_Picture_Box(Layout* TheLayout, Picture* TheBackPic, Backbuffer* TheBackbuffer)
{
	FileData*  FData;
	Picture    LoadedPic;
	
	FData = FileList_Get_File(&TheLayout->Picture_List, TheLayout->Current_Picture);
	if (FData)
	{
		Picture_Init(&LoadedPic);
		
		if (Picture_Load(&LoadedPic, FData->FullFileName))
		{
			Picture_Resize(&LoadedPic, TheLayout->Picture_Box.Size_X, TheLayout->Picture_Box.Size_Y);
			Element_Draw_Simple_Picture(&TheLayout->Picture_Box, &LoadedPic, TheBackPic, TheBackbuffer);
		}
		
		Picture_Release(&LoadedPic);
	}
}

void Layout_Draw_Timer(Layout* TheLayout, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection, int TimeValue)
{
	if (TimeValue < 0)
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_TIMEOUT_TIMER, TheBackPic, TheBackbuffer);
	}
	else
	{
		Element_Draw_Simple_Number(TheLayout->Score_Elements + ELEMENT_ID_TIMEOUT_TIMER, TimeValue, TheBackPic, TheBackbuffer, TheFontCollection);
	}
}

void Layout_Draw_Timout_Text(Layout* TheLayout, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection, const char* TeamText)
{
	char FullTxt[MAX_STR_SIZE_NAME + 8];
	
	if (TeamText)
	{
		if (*TeamText)
		{
			strcpy_s(FullTxt, sizeof(FullTxt), "Timeout: ");
			strcat_s(FullTxt, sizeof(FullTxt), TeamText);
		}
		else
		{
			strcpy_s(FullTxt, sizeof(FullTxt), "Technical Timeout");
		}
		Element_Draw_Simple_Text(TheLayout->Score_Elements + ELEMENT_ID_TIMEOUT_TEXT, FullTxt, TheBackPic, TheBackbuffer, TheFontCollection);
	}
	else
	{
		Element_Erase_Background(TheLayout->Score_Elements + ELEMENT_ID_TIMEOUT_TEXT, TheBackPic, TheBackbuffer);
	}
	
}


