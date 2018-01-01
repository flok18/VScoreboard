
#include "StdInc.h"
#include "FLib1.h"

char* _stristr(const char* QuellString, const char* SuchString)
{
    int   FoundCount        = 0;

    // Strings Ueberpruefungen
    if (QuellString == NULL) return NULL;
    if (SuchString  == NULL) return NULL;

    // String in String suchen (Gross/Klein Egal)
	if (*SuchString)
	{
		while(*QuellString)
		{
			if (tolower(*QuellString) == tolower(*SuchString))
			{
				++SuchString;
				++QuellString;
				++FoundCount;
				if (!(*SuchString))
				{
					return (char*)(QuellString - FoundCount);
				}
			}
			else
			{
				if(FoundCount > 0)
				{
					SuchString -= FoundCount;
					FoundCount = 0;
				}
				else
				{
					++QuellString;
				}
			}
		}
	}
    return NULL;
}



int sprintf_ss(char* Buffer, size_t BufferLen, const char* format, ...)
{
    int       retValue   = 0;
    va_list   argptr;

	if (Buffer)
	{
		if (BufferLen > 0)
		{
			va_start(argptr, format);
#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif
			retValue = _vsnprintf(Buffer, BufferLen, format, argptr);
#ifdef _MSC_VER
#pragma warning (default: 4996)
#endif
			va_end(argptr);

			if (retValue < 0)
			{
				Buffer[0] = '\0';
				retValue = 0;
			}
			else if (retValue >= (int)BufferLen)
			{
				Buffer[BufferLen - 1] = '\0';
				retValue = (int)(BufferLen - 1);
			}
		}
	}
    return retValue;
}

int strcpy_ss(char* dst, size_t max, const char* src)
{
	if (dst && max)
	{
		if (src)
		{
			while(max > 1)
			{
				*dst = *src;
				if (*src == '\0') return 0;
				++src;
				++dst;
				--max;
			}			
		}
		*dst = '\0';
	}
	return 0;
}

int strcat_ss(char* dst, size_t max, const char* src)
{
	if (dst && max && src)
	{
		while(*dst)
		{
			++dst;
			--max;
		}
		while(max > 1)
		{
			*dst = *src;
			if (*src == '\0') return 0;
			++src;
			++dst;
			--max;
		}
		*dst = '\0';
	}
	return 0;
}


