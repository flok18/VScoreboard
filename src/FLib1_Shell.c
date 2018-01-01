
#include "StdInc.h"
#include "FLib1.h"

#include <commdlg.h>


static void _SH_FD_ErgaenzeExtension(char* StringBuffer, size_t StringBufferSize, char* FilterString, int FilterIndex)
{
    char*     PointerToLastPoint;
    char*     PointerToLastBackSlash;
	size_t    StringBufferLen;
    int       CurrentBlock;
    int       SearchBlock;


    // Allgemeine Tests
    if (FilterString == NULL) return;
    if (FilterIndex  <= 0)    return;

    // Schauen ob Extension angegeben wurde...
    PointerToLastPoint     = strrchr(StringBuffer, '.');
    PointerToLastBackSlash = strrchr(StringBuffer, '\\');
    if (PointerToLastPoint)
    {
        if (PointerToLastBackSlash)
        {
            if (PointerToLastPoint > PointerToLastBackSlash) return;
        }
        else
        {
            return;
        }
    }

	// Ans Ende Springen
	StringBufferLen   = strlen(StringBuffer);
	StringBuffer     += StringBufferLen;
	StringBufferSize -= StringBufferLen;
	if (StringBufferSize < 2) return;


	// Im Filter zur gesuchten Position springen
    CurrentBlock   = 1;
    SearchBlock    = FilterIndex * 2;
	for (;;)
	{
		if (*FilterString == '\0')
		{
			++FilterString;
			++CurrentBlock;

			// Ende und nicht gefunden (String ist Doppel-Terminiert)
			if (*FilterString == '\0') return;

			// Gefunden
			if (CurrentBlock == SearchBlock) break;
		}
		++FilterString;
	}

	// '*' Ueberspringen
	while (*FilterString == '*' ||
	       *FilterString == ' ')
	{
		++FilterString;
	}

	// Illegalee Zeichen suchen (wegen *.*)
	if (strchr(FilterString, '*')) return;


	// String Kopieren
	while (*FilterString != '\0' &&
	       *FilterString != ';'  &&
	       StringBufferSize > 1)
	{
		*StringBuffer = *FilterString;

		++FilterString;
		++StringBuffer;
		--StringBufferSize;
	}

	// Terminieren
	*StringBuffer = '\0';
}

static char* _SH_FD_ConvertFilterString(const char* FilterString)
{
	size_t            Buffer_Size;
	char*             Buffer           = NULL;
	char*             StrPos;

	Buffer_Size = strlen(FilterString) + 2;
	Buffer      = (char*)malloc(Buffer_Size * sizeof(char));

	StrPos = Buffer;
	while (*FilterString)
	{
		if (*FilterString == '|')
		{
			*StrPos = '\0';
		}
		else
		{
			*StrPos = *FilterString;
		}

		++StrPos;
		++FilterString;
	}

	*StrPos = '\0';
	++StrPos;
	*StrPos = '\0';

	return Buffer;
}

BOOL Shell_Dialog_GetFile_Open_hInst(HINSTANCE ModHandle, HWND ParentHwnd, const char* WinTitle, const char* FilterString,
                                     char* Ret_FileName, size_t Ret_FileName_Size, int* Ret_FilterIndex)
{
	BOOL              Return_Val         = FALSE;
    char*             Cvt_Filter_String  = NULL;
	OPENFILENAME      OFName;

    ZeroMemory(&OFName, sizeof(OFName));

	OFName.lStructSize     = sizeof(OFName);
	OFName.hwndOwner       = ParentHwnd;
	OFName.hInstance       = ModHandle;
	OFName.lpstrFile       = Ret_FileName;
	OFName.nMaxFile        = (DWORD)Ret_FileName_Size;
	OFName.lpstrTitle      = WinTitle;
	OFName.Flags           = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;

    if (FilterString)
    {
		Cvt_Filter_String = _SH_FD_ConvertFilterString(FilterString);
        OFName.lpstrFilter = Cvt_Filter_String;
    }

	if (Ret_FilterIndex)
	{
		OFName.nFilterIndex = *Ret_FilterIndex;
	}

	// Laut MSDN nicht noetig, aber in einigen Foren wird's empfohlen
	// CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

RestartOpenDialog:

	// Show the 'Open File'-dialog
    if (GetOpenFileName(&OFName))
    {
        _SH_FD_ErgaenzeExtension(Ret_FileName, Ret_FileName_Size, Cvt_Filter_String, OFName.nFilterIndex);

        if (IfFileExist(Ret_FileName) == 0)
        {
            MessageBox(ParentHwnd, "Datei existiert nicht..." , WinTitle, MB_OK | MB_ICONEXCLAMATION);
            goto RestartOpenDialog;
        }

        if (Ret_FilterIndex)
        {
            *Ret_FilterIndex = OFName.nFilterIndex;
        }

		Return_Val = TRUE;
    }

	// CoUninitialize();

	if (Cvt_Filter_String)
	{
		free(Cvt_Filter_String);
	}

	return Return_Val;
}


BOOL Shell_Dialog_GetFile_Save_hInst(HINSTANCE ModHandle, HWND ParentHwnd, const char* WinTitle, const char* FilterString,
                                     char* Ret_FileName, size_t Ret_FileName_Size, int* Ret_FilterIndex)
{
	BOOL              Return_Val         = FALSE;
    char*             Cvt_Filter_String  = NULL;
	OPENFILENAME      OFName;

    ZeroMemory(&OFName, sizeof(OFName));

	OFName.lStructSize     = sizeof(OFName);
	OFName.hwndOwner       = ParentHwnd;
	OFName.hInstance       = ModHandle;
	OFName.lpstrFile       = Ret_FileName;
	OFName.nMaxFile        = (DWORD)Ret_FileName_Size;
	OFName.lpstrTitle      = WinTitle;
	OFName.Flags           = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

    if (FilterString)
    {
		Cvt_Filter_String = _SH_FD_ConvertFilterString(FilterString);
        OFName.lpstrFilter = Cvt_Filter_String;
    }

	if (Ret_FilterIndex)
	{
		OFName.nFilterIndex = *Ret_FilterIndex;
	}


	// Laut MSDN nicht noetig, aber in einigen Foren wird's empfohlen
	// CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

RestartSaveDialog:

	// Show the 'SaveAs File'-dialog
    if (GetSaveFileName(&OFName))
    {
        _SH_FD_ErgaenzeExtension(Ret_FileName, Ret_FileName_Size, Cvt_Filter_String, OFName.nFilterIndex);

        if (IfFileExist(Ret_FileName))
        {
            if (MessageBox(ParentHwnd, "Datei existiert bereits...\nUeberschreiben ???", WinTitle, MB_YESNO | MB_ICONQUESTION) != IDYES)
            {
                goto RestartSaveDialog;
            }
        }

        if (Ret_FilterIndex)
        {
            *Ret_FilterIndex = OFName.nFilterIndex;
        }

		Return_Val = TRUE;
    }

	// CoUninitialize();

	if (Cvt_Filter_String)
	{
		free(Cvt_Filter_String);
	}

	return Return_Val;
}

