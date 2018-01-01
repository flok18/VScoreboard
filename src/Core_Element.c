
#include "StdInc.h"
#include "Core.h"


// --------------------------------------------------------------------


void Element_Init(Element* TheLayout)
{
	memset(TheLayout, 0, sizeof(Element));
	
	TheLayout->Font_Index = INVALID_INDEX;
}

void Element_Release(Element* TheLayout)
{
	TheLayout->Is_Configured = FALSE;
	TheLayout->Font_Index    = INVALID_INDEX;
}


// --------------------------------------------------------------------


void Element_Save(Element* TheElement, FILE* TheFP)
{
	fwrite(TheElement, sizeof(Element), 1, TheFP);
}

void Element_Load(Element* TheElement, FILE* TheFP)
{
	fread(TheElement, sizeof(Element), 1, TheFP);
}


// --------------------------------------------------------------------


void Element_Erase_Background(Element* TheElement, Picture* TheBackPic, Backbuffer* TheBackbuffer)
{
	if (TheElement->Is_Configured)
	{
		if (TheBackPic->Pixels)
		{
			Pixel_Transfer_Copy(TheBackbuffer->Mem, TheBackbuffer->Size_X, TheBackbuffer->Size_Y,
								TheBackPic->Pixels, TheBackPic->Size_X, TheBackPic->Size_Y,
								TheElement->Pos_X, TheElement->Pos_Y,
								TheElement->Pos_X, TheElement->Pos_Y, TheElement->Size_X, TheElement->Size_Y);
		}
		else
		{
			Pixel_Erase_Rectangle(TheBackbuffer->Mem, TheBackbuffer->Size_X, TheBackbuffer->Size_Y, 0x00000000,
								  TheElement->Pos_X, TheElement->Pos_Y, TheElement->Size_X, TheElement->Size_Y);
		}
	}
}


void Element_Draw_Simple_Text(Element* TheElement, const char* TextToDraw, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection)
{
	RECT   StrArea;
	
	if (TheElement->Is_Configured)
	{
		Element_Erase_Background(TheElement, TheBackPic, TheBackbuffer);
		
		if (Backbuffer_Set_Font(TheBackbuffer, TheElement->Font_Index, TheElement->Font_Color, TheFontCollection))
		{			
			StrArea.left   = TheElement->Pos_X;
			StrArea.top    = TheElement->Pos_Y;
			StrArea.right  = StrArea.left + TheElement->Size_X;
			StrArea.bottom = StrArea.top  + TheElement->Size_Y;

			// DrawText, DrawTextEx, ExtTextOut, PolyTextOut, TabbedTextOut, TextOut
			DrawText(TheBackbuffer->DC, TextToDraw, (int)strlen(TextToDraw), &StrArea,
			         DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
		}		
	}	
}

void Element_Draw_Simple_Number(Element* TheElement, int NumValue, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection)
{
	char   StrBuffer[32];
	RECT   StrArea;
	
	if (TheElement->Is_Configured)
	{
		Element_Erase_Background(TheElement, TheBackPic, TheBackbuffer);
		
		if (Backbuffer_Set_Font(TheBackbuffer, TheElement->Font_Index, TheElement->Font_Color, TheFontCollection))
		{			
			_itoa_s(NumValue, StrBuffer, sizeof(StrBuffer), 10);
			
			StrArea.left   = TheElement->Pos_X;
			StrArea.top    = TheElement->Pos_Y;
			StrArea.right  = StrArea.left + TheElement->Size_X;
			StrArea.bottom = StrArea.top  + TheElement->Size_Y;

			// DrawText, DrawTextEx, ExtTextOut, PolyTextOut, TabbedTextOut, TextOut
			DrawText(TheBackbuffer->DC, StrBuffer, (int)strlen(StrBuffer), &StrArea,
			         DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
		}		
	}	
}

void Element_Draw_Simple_Led_On(Element* TheElement, DWORD Color, Picture* TheBackPic, Backbuffer* TheBackbuffer)
{
	if (TheElement->Is_Configured)
	{	
		Pixel_Erase_Rectangle(TheBackbuffer->Mem, TheBackbuffer->Size_X, TheBackbuffer->Size_Y, Color,
							  TheElement->Pos_X, TheElement->Pos_Y, TheElement->Size_X, TheElement->Size_Y);
	}	
}

void Element_Draw_Simple_Led_Off(Element* TheElement, Picture* TheBackPic, Backbuffer* TheBackbuffer)
{
	if (TheElement->Is_Configured)
	{	
		Pixel_Transfer_Copy(TheBackbuffer->Mem, TheBackbuffer->Size_X, TheBackbuffer->Size_Y,
							TheBackPic->Pixels, TheBackPic->Size_X, TheBackPic->Size_Y,
							TheElement->Pos_X, TheElement->Pos_Y,
							TheElement->Pos_X, TheElement->Pos_Y, TheElement->Size_X, TheElement->Size_Y);
	}	
}


void Element_Draw_Simple_Picture(Element* TheElement, Picture* FrontPic, Picture* TheBackPic, Backbuffer* TheBackbuffer)
{
	int       Draw_Position_X;
	int       Draw_Position_Y;
	
	if (TheElement->Is_Configured)
	{
		Element_Erase_Background(TheElement, TheBackPic, TheBackbuffer);
		
		if (FrontPic->Pixels)
		{
			Draw_Position_X = TheElement->Pos_X + (TheElement->Size_X - FrontPic->Size_X) / 2;
			Draw_Position_Y = TheElement->Pos_Y + (TheElement->Size_Y - FrontPic->Size_Y) / 2;

			Pixel_Transfer_Copy(TheBackbuffer->Mem, TheBackbuffer->Size_X, TheBackbuffer->Size_Y,
								FrontPic->Pixels, FrontPic->Size_X, FrontPic->Size_Y,
								Draw_Position_X, Draw_Position_Y,
								0, 0, FrontPic->Size_X, FrontPic->Size_Y);
		}
	}	
}

