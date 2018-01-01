

#include "StdInc.h"
#include "FLib1.h"
#include "Core.h"


#include <mmsystem.h>


static char   Sound_File_Short[MAX_STR_SIZE_FILE];
static char   Sound_File_Long[MAX_STR_SIZE_FILE];


void Sound_Init(void)
{
	char*  Found;

	GetModuleFileName(NULL, Sound_File_Short, MAX_STR_SIZE_FILE);
	Found = strrchr(Sound_File_Short, '\\');
	if (Found)
	{
		++Found;
		*Found = '\0';
	}
	else
	{
		*Sound_File_Short = '\0';
	}

	strcpy_ss(Sound_File_Long,  MAX_STR_SIZE_FILE, Sound_File_Short);

	strcat_ss(Sound_File_Short, MAX_STR_SIZE_FILE, "Sound1.wav");
	strcat_ss(Sound_File_Long,  MAX_STR_SIZE_FILE, "Sound2.wav");

	if (!IfFileExist(Sound_File_Short))
	{
		*Sound_File_Short = '\0';
	}
	if (!IfFileExist(Sound_File_Long))
	{
		*Sound_File_Long = '\0';
	}
}

void Sound_Play_Long(void)
{
	if (*Sound_File_Long)
	{
		PlaySound(Sound_File_Long, NULL, SND_ASYNC);
	}
}

void Sound_Play_Short(void)
{
	if (*Sound_File_Short)
	{
		sndPlaySound(Sound_File_Short, SND_ASYNC);
	}
}

void Sound_Abort(void)
{
	sndPlaySound(NULL, SND_ASYNC);
}



