

#include "StdInc.h"
#include "Core.h"


// --------------------------------------------------------------------


void Backbuffer_Init(Backbuffer* TheBackbuffer)
{
	memset(TheBackbuffer, 0, sizeof(Backbuffer));
	TheBackbuffer->Current_Font_Index = INVALID_INDEX;
}

void Backbuffer_Release(Backbuffer* TheBackbuffer)
{
	if (TheBackbuffer->DC)
	{
		if (TheBackbuffer->Orig_Font)  SelectObject(TheBackbuffer->DC, TheBackbuffer->Orig_Font);
		if (TheBackbuffer->Orig_Brush) SelectObject(TheBackbuffer->DC, TheBackbuffer->Orig_Brush);
		if (TheBackbuffer->Orig_Pen)   SelectObject(TheBackbuffer->DC, TheBackbuffer->Orig_Pen);
		if (TheBackbuffer->Orig_Bmp)   SelectObject(TheBackbuffer->DC, TheBackbuffer->Orig_Bmp);

		if (TheBackbuffer->Bmp)
		{
			DeleteObject(TheBackbuffer->Bmp);
		}
		DeleteDC(TheBackbuffer->DC);
	}
	TheBackbuffer->Size_X     = 0;
	TheBackbuffer->Size_Y     = 0;
	TheBackbuffer->DC         = NULL;
	TheBackbuffer->Bmp        = NULL;
	TheBackbuffer->Mem        = NULL;
	TheBackbuffer->Orig_Bmp   = NULL;
	TheBackbuffer->Orig_Font  = NULL;
	TheBackbuffer->Orig_Brush = NULL;
	TheBackbuffer->Orig_Pen   = NULL;

	TheBackbuffer->Current_Font_Index  = INVALID_INDEX;
	TheBackbuffer->Current_Font_Color  = 0x00000000;

}

void Backbuffer_Release_Font(Backbuffer* TheBackbuffer)
{
	if (TheBackbuffer->DC && TheBackbuffer->Orig_Font)
	{
		SelectObject(TheBackbuffer->DC, TheBackbuffer->Orig_Font);
		
		TheBackbuffer->Current_Font_Index  = INVALID_INDEX;
		TheBackbuffer->Current_Font_Color  = 0x00000000;
	}
}


// --------------------------------------------------------------------


void Backbuffer_Create(Backbuffer* TheBackbuffer, int Sx, int Sy, HDC Templ_Window)
{
    BITMAPINFO       bmi;


	TheBackbuffer->Size_X   = Sx;
	TheBackbuffer->Size_Y   = Sy;

	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize           = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth          = Sx;
	bmi.bmiHeader.biHeight         = -Sy;  // Order pixels from top to bottom
	bmi.bmiHeader.biPlanes         = 1;
	bmi.bmiHeader.biBitCount       = 32;
	bmi.bmiHeader.biCompression    = BI_RGB;

	TheBackbuffer->DC          = CreateCompatibleDC(Templ_Window);
	TheBackbuffer->Bmp         = CreateDIBSection(Templ_Window, &bmi, DIB_RGB_COLORS, (void**)&TheBackbuffer->Mem, NULL, 0);

	TheBackbuffer->Orig_Bmp    = (HBITMAP)SelectObject(TheBackbuffer->DC, TheBackbuffer->Bmp);
	TheBackbuffer->Orig_Font   = GetCurrentObject(TheBackbuffer->DC, OBJ_FONT);
	TheBackbuffer->Orig_Brush  = (HBRUSH)SelectObject(TheBackbuffer->DC, GetStockObject(DC_BRUSH));
	TheBackbuffer->Orig_Pen    = (HPEN)SelectObject(TheBackbuffer->DC, GetStockObject(DC_PEN));
	
	TheBackbuffer->Current_Font_Index  = INVALID_INDEX;
	TheBackbuffer->Current_Font_Color  = 0x00000000;
	
	SetBkColor(TheBackbuffer->DC, 0x00FFFFFF);
	SetBkMode(TheBackbuffer->DC, TRANSPARENT);
	SetTextColor(TheBackbuffer->DC, 0x00000000);
	SetTextAlign(TheBackbuffer->DC, TA_TOP | TA_LEFT);
}


void Backbuffer_Resize(Backbuffer* TheBackbuffer, int Sx, int Sy, HDC Templ_Window)
{
	if (Sx != TheBackbuffer->Size_X ||
	    Sy != TheBackbuffer->Size_Y)
	{
		Backbuffer_Release(TheBackbuffer);

		if (Sx > 0 && Sy > 0)
		{
			Backbuffer_Create(TheBackbuffer, Sx, Sy, Templ_Window);
		}
	}
}


// --------------------------------------------------------------------


BOOL Backbuffer_Set_Font(Backbuffer* TheBackbuffer, int FontIndex, DWORD FontColor, FontCollection* TheFontCollection)
{
	if (FontIndex >= 0 && FontIndex < TheFontCollection->nFonts)
	{
		if (TheBackbuffer->Current_Font_Index != FontIndex)
		{
			SelectObject(TheBackbuffer->DC, TheFontCollection->aFonts[FontIndex]);
			TheBackbuffer->Current_Font_Index = FontIndex;
		}	
		
		if (TheBackbuffer->Current_Font_Color != FontColor)
		{
			SetTextColor(TheBackbuffer->DC, FontColor);
			TheBackbuffer->Current_Font_Color = FontColor;
		}
		
		return TRUE;
	}
	return FALSE;
}



