
#include "StdInc.h"
#include "FLib1.h"

size_t InputStringToBuffer(char* buf, size_t bsize, FILE *fp)
{
	size_t   i   = 0;
	int      c;
	
	// Tests
	if (buf == NULL || bsize == 0 || fp == NULL)
	{
		return 0;
	}
	
	// '\0' braucht auch Platz
	--bsize;
	
	while (i < bsize)
	{
		c = fgetc(fp);
		if (c == EOF)
		{
			if (i == 0)
			{
				return 0;
			}
			break;
		}
		
		if (c == '\r') continue;
		if (c == '\n') break;

		buf[i] = (char)c;
		++i;
	}
	
	buf[i] = '\0';
	++i;
	
	return i;
}


void SetFolderBackslash(char* FolderString, size_t FolderStringSize)
{
	char*    TmpPtr;

	if (FolderString && FolderStringSize)
	{
		TmpPtr = strchr(FolderString, '\0');
		if (TmpPtr)
		{
			if (TmpPtr > FolderString &&
			    TmpPtr < FolderString + FolderStringSize - 1)
			{
				--TmpPtr;
				if (*TmpPtr != '\\')
				{
					++TmpPtr;
					*TmpPtr = '\\';
					++TmpPtr;
					*TmpPtr = '\0';
				}
			}
		}
	}
}

void RemoveFileFromFullPath(char* FullPath)
{
	char*    Pos;

	if (FullPath)
	{
		Pos = strrchr(FullPath, '\\');
		if (Pos)
		{
			Pos[1] = '\0';
		}
		else
		{
			FullPath[0] = '\0';
		}
	}
}



int Execute_FileSearch_Simple(const char* Pattern, void(*CallBackFnc)(WIN32_FIND_DATA*, char*,  int, void*), void* AnyPtr)
{
	int                FindCounter                    = 0;
	char               FolderOfPattern[MAX_PATH];
	char*              PosBS;
	size_t             NumCpy;
	HANDLE             AkSearchHandle;
	WIN32_FIND_DATA    AkFindResult;
	
	
	*FolderOfPattern = '\0';
	PosBS = strrchr(Pattern, '\\');
	if (PosBS)
	{
		NumCpy = PosBS - Pattern;
		++NumCpy;
		if (NumCpy < sizeof(FolderOfPattern))
		{
			memcpy(FolderOfPattern, Pattern, NumCpy);
			FolderOfPattern[NumCpy] = '\0';
		}
	}

	// Erster Eintrag suchen (wahrscheinlich "." oder "..")
	AkSearchHandle = FindFirstFile(Pattern, &AkFindResult);

	// Falls was gefunden...
	if (AkSearchHandle != INVALID_HANDLE_VALUE)
	{
		// Testen kein Verzeichnis
		if ((AkFindResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			++FindCounter;
			if (CallBackFnc)
			{
				CallBackFnc(&AkFindResult, FolderOfPattern, FindCounter, AnyPtr);
			}
		}

		// Weitere Einträge abrufen
		while (FindNextFile(AkSearchHandle, &AkFindResult))
		{
			// Testen kein Verzeichnis
			if ((AkFindResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				++FindCounter;
				if (CallBackFnc)
				{
					CallBackFnc(&AkFindResult, FolderOfPattern, FindCounter, AnyPtr);
				}
			}
		}

		// Suche Schliessen
		FindClose(AkSearchHandle);
	}
	
	return FindCounter;
}


int IfFileExist(const char* FullFilePath)
{
	if (FullFilePath == NULL) return 0;
	if (GetFileAttributes(FullFilePath) == INVALID_FILE_ATTRIBUTES)
	{
    	return 0;
	}
	else
	{
	    return 1;
	}
}

BOOL DirectoryExists(const char* dirName)
{
	DWORD attribs = GetFileAttributes(dirName);

	if (attribs == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}



