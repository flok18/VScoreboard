
#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"



void FileList_Init(FileList* TheFileList)
{
	TheFileList->FolderWithBackslash[0] = '\0';
	TheFileList->aFiles                 = NULL;
	TheFileList->nFiles                 = 0;
	TheFileList->mFiles                 = 0;
}

void FileList_Release(FileList* TheFileList)
{
	TheFileList->FolderWithBackslash[0] = '\0';

	if (TheFileList->aFiles)
	{
		free(TheFileList->aFiles);
	}
	TheFileList->aFiles = NULL;
	TheFileList->nFiles = 0;
	TheFileList->mFiles = 0;
}


// ============================================================================


int FileList_Get_Count(FileList* TheFileList)
{
	return TheFileList->nFiles;
}

FileData* FileList_Get_File(FileList* TheFileList, int Index)
{
	if (Index >= 0 &&
	    Index < TheFileList->nFiles)
	{
		return TheFileList->aFiles + Index;
	}
	return NULL;
}


// ============================================================================

void FileList_Swap(FileList* TheFileList, int Index1, int Index2)
{
	FileData   TmpBuf;
	FileData*  Buf1;
	FileData*  Buf2;

	if (Index1 != Index2)
	{
		Buf1 = FileList_Get_File(TheFileList, Index1);
		Buf2 = FileList_Get_File(TheFileList, Index2);

		if (Buf1 && Buf2)
		{
			memcpy(&TmpBuf, Buf1, sizeof(FileData));
			memcpy(Buf1,    Buf2, sizeof(FileData));
			memcpy(Buf2, &TmpBuf, sizeof(FileData));
		}
	}
}


static FileData* FileList_Add_Struct(FileList* TheFileList)
{
	int  LastIndex;

	LastIndex = TheFileList->nFiles;

	++(TheFileList->nFiles);
	if (TheFileList->nFiles > TheFileList->mFiles)
	{
		TheFileList->mFiles += 32;
		TheFileList->aFiles = ReAllocMem(TheFileList->aFiles, TheFileList->mFiles * sizeof(FileData));
	}
	return TheFileList->aFiles + LastIndex;
}


static void FileList_Add_Simple(FileList* TheFileList, const char* NewFileName)
{
    FileData*     ListEntry;
	char*         FoundPos;

	if (NewFileName)
	{
		ListEntry = FileList_Add_Struct(TheFileList);

		strcpy_ss(ListEntry->FullFileName, MAX_STR_SIZE_FILE, NewFileName);

		FoundPos = strrchr(ListEntry->FullFileName, '\\');
		if (FoundPos)
		{
			++FoundPos;
			ListEntry->FileNameOffset = (int)(FoundPos - ListEntry->FullFileName);
		}
		else
		{
			ListEntry->FileNameOffset = 0;
		}
	}
}

static void FileList_Wild_Enum_CB(WIN32_FIND_DATA* AkFindResult, char* Folder,  int Count, void* UserData)
{
	char  FullPath[MAX_STR_SIZE_FILE];

	strcpy_ss(FullPath, sizeof(FullPath), Folder);
	strcat_ss(FullPath, sizeof(FullPath), AkFindResult->cFileName);

	FileList_Add_Simple(UserData, FullPath);
}


void FileList_Create(FileList* TheFileList, const char* SrcFolder)
{
	char  TmpPattern[MAX_STR_SIZE_FILE];

	if (SrcFolder)
    {
        if (*SrcFolder)
        {
			if (DirectoryExists(SrcFolder))
			{
				strcpy_ss(TmpPattern, sizeof(TmpPattern), SrcFolder);
				SetFolderBackslash(TmpPattern, sizeof(TmpPattern));

				strcpy_ss(TheFileList->FolderWithBackslash, sizeof(MAX_STR_SIZE_FILE), TmpPattern);

				strcat_ss(TmpPattern, sizeof(TmpPattern), "*");
				Execute_FileSearch_Simple(TmpPattern, FileList_Wild_Enum_CB, TheFileList);
			}
        }
    }
}


// ============================================================================


void FileList_Save(FileList* TheFileList, FILE* TheFP)
{
	fwrite(TheFileList->FolderWithBackslash,  sizeof(char), MAX_STR_SIZE_FILE, TheFP);

	fwrite(&TheFileList->nFiles, sizeof(int), 1, TheFP);

	if (TheFileList->nFiles > 0 &&
	    TheFileList->aFiles != NULL)
	{
		fwrite(TheFileList->aFiles, sizeof(FileData), TheFileList->nFiles, TheFP);
	}
}

void FileList_Load(FileList* TheFileList, FILE* TheFP)
{
	fread(TheFileList->FolderWithBackslash,  sizeof(char), MAX_STR_SIZE_FILE, TheFP);

	fread(&TheFileList->nFiles, sizeof(int), 1, TheFP);
	if (TheFileList->nFiles > 0)
	{
		TheFileList->mFiles = TheFileList->nFiles;
		TheFileList->aFiles = malloc(TheFileList->mFiles * sizeof(FileData));

		fread(TheFileList->aFiles, sizeof(FileData), TheFileList->nFiles, TheFP);
	}
	else
	{
		TheFileList->nFiles = 0;
		TheFileList->mFiles = 0;
		TheFileList->aFiles = NULL;
	}
}


// ============================================================================


