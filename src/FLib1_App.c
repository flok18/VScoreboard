
#include "StdInc.h"
#include "FLib1.h"


//                         z.B. "Global\\MyApp01"
BOOL SetApplicationRunning(char* Global_MutexName)
{
	static HANDLE       __static_FloLib1_Mutex    = NULL;
	BOOL                Must_Close_Mutex          = FALSE;
	BOOL                Return_Status             = FALSE;
	
	
	if (Global_MutexName)
	{
		if (__static_FloLib1_Mutex == NULL)
		{
			__static_FloLib1_Mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, Global_MutexName);
			if (__static_FloLib1_Mutex)
			{
				Must_Close_Mutex = TRUE;
			}
			else
			{
				__static_FloLib1_Mutex = CreateMutex(NULL, FALSE, Global_MutexName);
				if (__static_FloLib1_Mutex)
				{
					if (GetLastError() == ERROR_ALREADY_EXISTS)
					{
						Must_Close_Mutex = TRUE;
					}
					else
					{
						Return_Status = TRUE;
					}
				}
			}
		}
	}
	else
	{
		if (__static_FloLib1_Mutex)
		{
			Must_Close_Mutex = TRUE;
			Return_Status = TRUE;
		}
	}
	
	if (Must_Close_Mutex)
	{
		CloseHandle(__static_FloLib1_Mutex);
		__static_FloLib1_Mutex = NULL;		
	}
	return Return_Status;
}



size_t Get_CommandLine_File(const char* CmdLne, char* TargetBuffer, size_t TargetBufferSize)
{
	size_t   CmdStrLen  = 0;
	BOOL     TrimAtEnd  = FALSE;
	char*    FoundPos;
	
	
	*TargetBuffer = '\0';
	if (CmdLne == NULL) return 0;
	
	
	while (*CmdLne == ' ')
	{
		++CmdLne;
	}
	
	if (*CmdLne == '"')
	{
		++CmdLne;
		
		FoundPos = strchr(CmdLne, '"');
		if (FoundPos)
		{
			CmdStrLen = (FoundPos - CmdLne);
		}
		else
		{
			CmdStrLen = strlen(CmdLne);
			TrimAtEnd = TRUE;
		}
	}
	else
	{
		CmdStrLen = strlen(CmdLne);
		TrimAtEnd = TRUE;
	}

	
	if (TrimAtEnd)
	{
		while (CmdStrLen)
		{
			if (CmdLne[CmdStrLen - 1] == ' ')
			{
				--CmdStrLen;
			}
			else
			{
				break;
			}
		}
	}

	
	if (CmdStrLen >= TargetBufferSize)
	{
		CmdStrLen = 0;
	}

	
	if (CmdStrLen)
	{
		memcpy(TargetBuffer, CmdLne, CmdStrLen);
		TargetBuffer[CmdStrLen] = '\0';
	}

	return CmdStrLen;
}


char* Get_Win32_CommandLine(void)
{
	char*   lpszCommandLine;


	lpszCommandLine = GetCommandLine();
	if (lpszCommandLine == NULL) return NULL;
	
	if (*lpszCommandLine == '"')
	{
		++lpszCommandLine;
		
		while (*lpszCommandLine)
		{
			if (*lpszCommandLine == '"')
			{
				++lpszCommandLine;
				break;
			}
			++lpszCommandLine;
		}
	}
	else
	{
		while (*lpszCommandLine > ' ')
		{
			++lpszCommandLine;
		}
	}

	while (*lpszCommandLine != '\0' && 
	       *lpszCommandLine <= ' ')
	{
		++lpszCommandLine;
	}

	return lpszCommandLine;
}

void MsgBox_Info(HWND ParentWinHand, const char* MsgText)
{
    MessageBox(ParentWinHand, MsgText, " Info...", MB_ICONASTERISK|MB_OK) ;
}

BOOL MsgBox_Frage(HWND ParentWinHand, const char* MsgText)
{
    if (MessageBox(ParentWinHand, MsgText, " Frage...", MB_ICONQUESTION|MB_YESNO) == IDYES)
    {
        return TRUE;
    }
    return FALSE;
}

void MsgBox_Fehler(HWND ParentWinHand, const char* MsgText)
{
    MessageBox(ParentWinHand, MsgText, " Fehler...", MB_ICONHAND|MB_OK) ;
}

