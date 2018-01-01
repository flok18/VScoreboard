
#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"


#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif
FILE* File_IO_Open(const char* FileName, const char* Mode)
{
	return fopen(FileName, Mode);
}
#ifdef _MSC_VER
#pragma warning (default: 4996)
#endif


void _dbg_print(const char* format, ...)
{
    va_list  argptr;

    va_start(argptr, format);
	vprintf(format, argptr);
    va_end(argptr);
}


void* ReAllocMem(void* MemPtr, size_t MemSize)
{
	if (MemSize)
	{
		if (MemPtr)
		{
			MemPtr = realloc(MemPtr, MemSize);
		}
		else
		{
			MemPtr = malloc(MemSize);
		}
	}
	else
	{
		if (MemPtr)
		{
			free(MemPtr);
			MemPtr = NULL;
		}
	}
	return MemPtr;
}


BOOL Has_File_Extension(const char* File, const char* Ext)
{
	BOOL     RetVal   = FALSE;
	char*    Found;
	size_t   ExtLen;

	ExtLen = strlen(Ext);

	for (;;)
	{
		Found = _stristr(File, Ext);
		if (Found == NULL) break;

		Found += ExtLen;
		if (*Found == '\0')
		{
			RetVal = TRUE;
			break;
		}

		File = Found;
	}
	return RetVal;
}


char* Split_Command(char* Command, char Splitter)
{
	while (*Command)
	{
		if (*Command == Splitter)
		{
			*Command = '\0';
			++Command;
			break;
		}
		++Command;
	}
	return Command;
}

void Deflate_Command(char* Command)
{
	char* Target = Command;

	while (*Command)
	{
		if (*Command != ' ' &&
		    *Command != '\t')
		{
			if (Target < Command)
			{
				*Target = *Command;
			}
			++Target;
		}
		++Command;
	}
	if (Target < Command)
	{
		*Target = '\0';
	}
}

void Lower_Command(char* Command)
{
	while (*Command)
	{
		if (*Command >= 'A' &&
		    *Command <= 'Z')
		{
			*Command += ('a' - 'A');
		}
		++Command;
	}
}

void Trim_Command(char* Command)
{
	char*    StringOrig    = Command;
	char*    TrimPos       = Command;

	// Links Trimmen
	while (*Command == ' '  ||
		   *Command == '\t' ||
		   *Command == '\n' ||
		   *Command == '\r')
	{
		++Command;
	}

	// Falls Getrimmt, alle Zeichen Uebertragen
	if (Command > TrimPos)
	{
		while (*Command)
		{
			*TrimPos = *Command;
			++TrimPos;
			++Command;
		}
		*TrimPos = '\0';
	}
	else
	{
		while (*TrimPos)
		{
			++TrimPos;
		}
	}

	// Rechts Trimmen
	while (TrimPos > StringOrig)
	{
		--TrimPos;

		if (*TrimPos == ' '  ||
			*TrimPos == '\t' ||
			*TrimPos == '\n' ||
			*TrimPos == '\r')
		{
			*TrimPos = '\0';
		}
		else
		{
			++TrimPos;
			break;
		}
	}
}


int Get_Listbox_Selection(HWND ListBox, BOOL** Ret_Array)
{
	int     NumTotal;
	BOOL*   SelArray;
	int     ii;

	NumTotal = (int)SendMessage(ListBox, LB_GETCOUNT, 0, 0);
	if (NumTotal > 0)
	{
		SelArray = malloc(NumTotal * sizeof(BOOL));

		for (ii = 0; ii < NumTotal; ++ii)
		{
			if (SendMessage(ListBox, LB_GETSEL, ii, 0))
			{
				SelArray[ii] = TRUE;
			}
			else
			{
				SelArray[ii] = FALSE;
			}
		}

		*Ret_Array = SelArray;
	}
	else
	{
		NumTotal = 0;
	}
	return NumTotal;
}


void Set_Listbox_Selection(HWND ListBox, BOOL* Sel_Array, int Sel_Array_Max)
{
	int     NumTotal;
	int     ii;

	NumTotal = (int)SendMessage(ListBox, LB_GETCOUNT, 0, 0);
	for (ii = 0; ii < NumTotal && ii < Sel_Array_Max; ++ii)
	{
		SendMessage(ListBox, LB_SETSEL, Sel_Array[ii], ii);
	}
}

void Swap_In_Bool_Array(BOOL* Array, int Index1, int Index2)
{
	BOOL  Mem;

	if (Index1 != Index2)
	{
		Mem           = Array[Index1];
		Array[Index1] = Array[Index2];
		Array[Index2] = Mem;
	}
}

BOOL Is_Path_Absolute(const char* PathString)
{
	if (*PathString)
	{
		++PathString;
		if (*PathString == ':')
		{
			return TRUE;
		}
	}
	return FALSE;
}


