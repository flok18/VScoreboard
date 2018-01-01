

#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"



// --------------------------------------------------------------------


void FontCollection_Init(FontCollection* TheFontCollection)
{
	memset(TheFontCollection, 0, sizeof(FontCollection));
}

void FontCollection_Release(FontCollection* TheFontCollection)
{
	int  ii;

	if (TheFontCollection->aFonts)
	{
		for (ii = 0; ii < TheFontCollection->nFonts; ++ii)
		{
			DeleteObject(TheFontCollection->aFonts[ii]);
		}
		free(TheFontCollection->aFonts);
	}
	TheFontCollection->aFonts = NULL;
	TheFontCollection->nFonts = 0;
}


// --------------------------------------------------------------------


int FontCollection_New_Font(FontCollection* TheFontCollection, const char* Name, int Height, int Weight, int Angle)
{
	int      ii;
	LOGFONT  FontInfo;

	if (*Name == '\0' || Height < 1) return INVALID_INDEX;

	for (ii = 0; ii < TheFontCollection->nFonts; ++ii)
	{
		if (GetObject(TheFontCollection->aFonts[ii], sizeof(LOGFONT), &FontInfo) == sizeof(LOGFONT))
		{
			if (FontInfo.lfHeight      == Height &&
			    FontInfo.lfWeight      == Weight &&
			    FontInfo.lfEscapement  == Angle)
			{
				if (_stricmp(FontInfo.lfFaceName, Name) == 0)
				{
					return ii;
				}
			}
		}
	}

	++(TheFontCollection->nFonts);
	TheFontCollection->aFonts = ReAllocMem(TheFontCollection->aFonts, TheFontCollection->nFonts * sizeof(HFONT));

	memset(&FontInfo, 0, sizeof(LOGFONT));
	FontInfo.lfHeight     = Height;
	FontInfo.lfWeight     = Weight;
	FontInfo.lfEscapement = Angle;
	strcpy_ss(FontInfo.lfFaceName, LF_FACESIZE, Name);

	TheFontCollection->aFonts[ii] = CreateFontIndirect(&FontInfo);
	if (TheFontCollection->aFonts[ii] == NULL)
	{
		--(TheFontCollection->nFonts);
		return INVALID_INDEX;
	}

	return ii;
}


// --------------------------------------------------------------------


void FontCollection_Save(FontCollection* TheFontCollection, FILE* TheFP)
{
	int      ii;
	LOGFONT  FontInfo;
	BOOL     FontOK;

	fwrite(&TheFontCollection->nFonts, sizeof(int), 1, TheFP);
	for (ii = 0; ii < TheFontCollection->nFonts; ++ii)
	{
		FontOK = FALSE;
		if (TheFontCollection->aFonts[ii])
		{
			if (GetObject(TheFontCollection->aFonts[ii], sizeof(LOGFONT), &FontInfo) == sizeof(LOGFONT))
			{
				FontOK = TRUE;
			}
		}
		if (FontOK == FALSE)
		{
			ZeroMemory(&FontInfo, sizeof(LOGFONT));
		}
		fwrite(&FontInfo, sizeof(LOGFONT), 1, TheFP);
	}

}

void FontCollection_Load(FontCollection* TheFontCollection, FILE* TheFP)
{
	int      ii;
	LOGFONT  FontInfo;

	fread(&TheFontCollection->nFonts, sizeof(int), 1, TheFP);
	if (TheFontCollection->nFonts > 0)
	{
		TheFontCollection->aFonts = malloc(TheFontCollection->nFonts * sizeof(HFONT));

		for (ii = 0; ii < TheFontCollection->nFonts; ++ii)
		{
			fread(&FontInfo, sizeof(LOGFONT), 1, TheFP);

			if (FontInfo.lfHeight > 0)
			{
				TheFontCollection->aFonts[ii] = CreateFontIndirect(&FontInfo);
			}
			else
			{
				TheFontCollection->aFonts[ii] = NULL;
			}
		}
	}
	else
	{
		TheFontCollection->nFonts = 0;
		TheFontCollection->aFonts = NULL;
	}
}





