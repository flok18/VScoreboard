

#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"


// --------------------------------------------------------------------


void Scoreboard_Init(Scoreboard* TheScoreboard)
{
	Frontbuffer_Init(&TheScoreboard->Target_Display);
	Backbuffer_Init(&TheScoreboard->Target_Backbuffer);
	Picture_Init(&TheScoreboard->Target_Picture);

	memset(TheScoreboard->Title,  0, sizeof(MAX_STR_SIZE_NAME));
	memset(TheScoreboard->Team_1, 0, sizeof(MAX_STR_SIZE_NAME));
	memset(TheScoreboard->Team_2, 0, sizeof(MAX_STR_SIZE_NAME));

	FontCollection_Init(&TheScoreboard->Fonts);

	TheScoreboard->aLayout = NULL;
	TheScoreboard->nLayout = 0;
	TheScoreboard->CurrentLayout = NULL;

	Game_Reset(&TheScoreboard->Game_Status);
}

void Scoreboard_Release(Scoreboard* TheScoreboard)
{
	int  ii;

	Frontbuffer_Release(&TheScoreboard->Target_Display);
	Backbuffer_Release(&TheScoreboard->Target_Backbuffer);
	Picture_Release(&TheScoreboard->Target_Picture);

	FontCollection_Release(&TheScoreboard->Fonts);

	if (TheScoreboard->aLayout)
	{
		for (ii = 0; ii < TheScoreboard->nLayout; ++ii)
		{
			Layout_Release(TheScoreboard->aLayout + ii);
		}
		free(TheScoreboard->aLayout);
	}
	TheScoreboard->nLayout       = 0;
	TheScoreboard->aLayout       = NULL;
	TheScoreboard->CurrentLayout = NULL;
}


void Scoreboard_Clear_Layouts(Scoreboard* TheScoreboard)
{
	int  ii;

	Backbuffer_Release_Font(&TheScoreboard->Target_Backbuffer);
	FontCollection_Release(&TheScoreboard->Fonts);

	if (TheScoreboard->aLayout)
	{
		for (ii = 0; ii < TheScoreboard->nLayout; ++ii)
		{
			Layout_Release(TheScoreboard->aLayout + ii);
		}
		free(TheScoreboard->aLayout);
	}
	TheScoreboard->nLayout       = 0;
	TheScoreboard->aLayout       = NULL;
	TheScoreboard->CurrentLayout = NULL;
}


void Scoreboard_Move_Data(Scoreboard* FromScoreboard, Scoreboard* ToScoreboard, BOOL Include_Game)
{
	Scoreboard_Clear_Layouts(ToScoreboard);

	memcpy(ToScoreboard->Title,  FromScoreboard->Title,  MAX_STR_SIZE_NAME);
	memcpy(ToScoreboard->Team_1, FromScoreboard->Team_1, MAX_STR_SIZE_NAME);
	memcpy(ToScoreboard->Team_2, FromScoreboard->Team_2, MAX_STR_SIZE_NAME);

	memcpy(&ToScoreboard->Fonts, &FromScoreboard->Fonts, sizeof(FontCollection));

	ToScoreboard->aLayout       = FromScoreboard->aLayout;
	ToScoreboard->nLayout       = FromScoreboard->nLayout;
	ToScoreboard->CurrentLayout = FromScoreboard->CurrentLayout;

	if (Include_Game)
	{
		memcpy(&ToScoreboard->Game_Status, &FromScoreboard->Game_Status, sizeof(Game));
	}


	FontCollection_Init(&FromScoreboard->Fonts);

	FromScoreboard->aLayout = NULL;
	FromScoreboard->nLayout = 0;
	FromScoreboard->CurrentLayout = NULL;
}



// --------------------------------------------------------------------

void Scoreboard_Save(Scoreboard* TheScoreboard, FILE* TheFP)
{
	int      ii;

	// Titel
	fwrite(TheScoreboard->Title,  sizeof(char), MAX_STR_SIZE_NAME, TheFP);
	fwrite(TheScoreboard->Team_1, sizeof(char), MAX_STR_SIZE_NAME, TheFP);
	fwrite(TheScoreboard->Team_2, sizeof(char), MAX_STR_SIZE_NAME, TheFP);

	// Fonts
	FontCollection_Save(&TheScoreboard->Fonts, TheFP);

	// Layouts
	fwrite(&TheScoreboard->nLayout, sizeof(int), 1, TheFP);
	for (ii = 0; ii < TheScoreboard->nLayout; ++ii)
	{
		Layout_Save(TheScoreboard->aLayout + ii, TheFP);
	}

	// Game
	Game_Save(&TheScoreboard->Game_Status, TheFP);
}

void Scoreboard_Load(Scoreboard* TheScoreboard, FILE* TheFP)
{
	int      ii;

	// Titel
	fread(TheScoreboard->Title,  sizeof(char), MAX_STR_SIZE_NAME, TheFP);
	fread(TheScoreboard->Team_1, sizeof(char), MAX_STR_SIZE_NAME, TheFP);
	fread(TheScoreboard->Team_2, sizeof(char), MAX_STR_SIZE_NAME, TheFP);

	// Fonts
	FontCollection_Load(&TheScoreboard->Fonts, TheFP);

	// Layouts
	fread(&TheScoreboard->nLayout, sizeof(int), 1, TheFP);
	if (TheScoreboard->nLayout > 0)
	{
		TheScoreboard->aLayout = malloc(TheScoreboard->nLayout * sizeof(Layout));
		for (ii = 0; ii < TheScoreboard->nLayout; ++ii)
		{
			Layout_Load(TheScoreboard->aLayout + ii, TheFP);
		}
	}
	else
	{
		TheScoreboard->nLayout = 0;
		TheScoreboard->aLayout = NULL;
	}
	TheScoreboard->CurrentLayout = NULL;

	// Game
	Game_Load(&TheScoreboard->Game_Status, TheFP);
}

// --------------------------------------------------------------------


BOOL Scoreboard_Create_Window(Scoreboard* TheScoreboard)
{
	POINT            WinPos;
	POINT            WinSize;

	Monitor_Get_Secondary_Rect(&WinPos, &WinSize);

	return Frontbuffer_Create_Window(&TheScoreboard->Target_Display, &TheScoreboard->Target_Backbuffer,
	                                 WinPos.x, WinPos.y, WinSize.x, WinSize.y);
}


int Scoreboard_New_Layout(Scoreboard* TheScoreboard, const char* Name)
{
	int  LastLayoutIndex;

	LastLayoutIndex = TheScoreboard->nLayout;

	++(TheScoreboard->nLayout);
	TheScoreboard->aLayout       = ReAllocMem(TheScoreboard->aLayout, TheScoreboard->nLayout * sizeof(Layout));
	TheScoreboard->CurrentLayout = TheScoreboard->aLayout + LastLayoutIndex;

	Layout_Init(TheScoreboard->CurrentLayout);

	strcpy_ss(TheScoreboard->CurrentLayout->Name, MAX_STR_SIZE_NAME, Name);

	return LastLayoutIndex;
}

int Scoreboard_Get_Layout_Index(Scoreboard* TheScoreboard)
{
	if (TheScoreboard->CurrentLayout)
	{
		return (int)(TheScoreboard->CurrentLayout - TheScoreboard->aLayout);
	}
	return INVALID_INDEX;
}


// --------------------------------------------------------------------


void Scoreboard_Select_Layout(Scoreboard* TheScoreboard, int Index, BOOL Reset_Pic_Index)
{
	Layout*  NewLayout;

	Picture_Release(&TheScoreboard->Target_Picture);

	if (Index >= 0 && Index < TheScoreboard->nLayout)
	{
		NewLayout = TheScoreboard->aLayout + Index;
		TheScoreboard->CurrentLayout = NewLayout;

		if (NewLayout->Background_File[0])
		{
			if (!Picture_Load(&TheScoreboard->Target_Picture, NewLayout->Background_File))
			{
				_dbg_print("Error Loading Pic: %s\n", NewLayout->Background_File);
			}
		}

		// Preselect Picture Index 0 if Current is not Valid
		if (Reset_Pic_Index)
		{
			NewLayout->Current_Picture = 0;
		}
		if (FileList_Get_File(&NewLayout->Picture_List, NewLayout->Current_Picture) == NULL)
		{
			if (FileList_Get_Count(&NewLayout->Picture_List) > 0)
			{
				NewLayout->Current_Picture = 0;
			}
			else
			{
				NewLayout->Current_Picture = INVALID_INDEX;
			}
		}
	}
	else
	{
		TheScoreboard->CurrentLayout = NULL;
	}

	// Copy Background Picture to Backbuffer
	if (TheScoreboard->Target_Picture.Pixels)
	{
		Backbuffer_Resize(&TheScoreboard->Target_Backbuffer,
						  TheScoreboard->Target_Picture.Size_X, TheScoreboard->Target_Picture.Size_Y,
						  TheScoreboard->Target_Display.DC);

		Pixel_Transfer_Copy(TheScoreboard->Target_Backbuffer.Mem, TheScoreboard->Target_Backbuffer.Size_X, TheScoreboard->Target_Backbuffer.Size_Y,
							TheScoreboard->Target_Picture.Pixels, TheScoreboard->Target_Picture.Size_X, TheScoreboard->Target_Picture.Size_Y,
							0, 0, 0, 0, TheScoreboard->Target_Picture.Size_X, TheScoreboard->Target_Picture.Size_Y);
	}
	else
	{
		Backbuffer_Resize(&TheScoreboard->Target_Backbuffer,
						  TheScoreboard->Target_Display.Size_X, TheScoreboard->Target_Display.Size_Y,
						  TheScoreboard->Target_Display.DC);

		Pixel_Erase_Background(TheScoreboard->Target_Backbuffer.Mem,
							   TheScoreboard->Target_Backbuffer.Size_X, TheScoreboard->Target_Backbuffer.Size_Y,
							   0x00000000);
	}
}


// --------------------------------------------------------------------


void Scoreboard_Update_Static_Text(Scoreboard* TheScoreboard)
{
	if (TheScoreboard->CurrentLayout && TheScoreboard->Target_Backbuffer.Mem)
	{
		Element_Draw_Simple_Text(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_GAME_TITLE,  TheScoreboard->Title,  &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer, &TheScoreboard->Fonts);
		Element_Draw_Simple_Text(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_NAME_TEAM_1, TheScoreboard->Team_1, &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer, &TheScoreboard->Fonts);
		Element_Draw_Simple_Text(TheScoreboard->CurrentLayout->Score_Elements + ELEMENT_ID_NAME_TEAM_2, TheScoreboard->Team_2, &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer, &TheScoreboard->Fonts);
		Frontbuffer_Update(&TheScoreboard->Target_Display);
	}
}


void Scoreboard_Update_Game_Status(Scoreboard* TheScoreboard)
{
	if (TheScoreboard->CurrentLayout && TheScoreboard->Target_Backbuffer.Mem)
	{
		Layout_Draw_Game_Status(TheScoreboard->CurrentLayout, &TheScoreboard->Game_Status,
		                        &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer, &TheScoreboard->Fonts);
		Frontbuffer_Update(&TheScoreboard->Target_Display);
	}
}

void Scoreboard_Update_Picture_Box(Scoreboard* TheScoreboard)
{
	if (TheScoreboard->CurrentLayout && TheScoreboard->Target_Backbuffer.Mem)
	{
		Layout_Draw_Picture_Box(TheScoreboard->CurrentLayout, &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer);
		Frontbuffer_Update(&TheScoreboard->Target_Display);
	}
}


void Scoreboard_Update_Timer(Scoreboard* TheScoreboard, int Value)
{
	if (TheScoreboard->CurrentLayout && TheScoreboard->Target_Backbuffer.Mem)
	{
		Layout_Draw_Timer(TheScoreboard->CurrentLayout, &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer, &TheScoreboard->Fonts, Value);
		Frontbuffer_Update(&TheScoreboard->Target_Display);
	}
}

void Scoreboard_Update_TimeoutText(Scoreboard* TheScoreboard, int TeamNr)
{
	const char* TeamText = NULL;

	if (TheScoreboard->CurrentLayout && TheScoreboard->Target_Backbuffer.Mem)
	{
		if      (TeamNr == 0) TeamText = "";
		else if (TeamNr == 1) TeamText = TheScoreboard->Team_1;
		else if (TeamNr == 2) TeamText = TheScoreboard->Team_2;

		Layout_Draw_Timout_Text(TheScoreboard->CurrentLayout, &TheScoreboard->Target_Picture, &TheScoreboard->Target_Backbuffer, &TheScoreboard->Fonts, TeamText);
		Frontbuffer_Update(&TheScoreboard->Target_Display);
	}
}



// --------------------------------------------------------------------


