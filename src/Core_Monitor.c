

#include "StdInc.h"
#include "Core.h"



typedef struct {
	int     Count;
	BOOL    Has_Primary;
	BOOL    Has_Secondary;
	RECT    Primary_Rect;
	RECT    Secondary_Rect;
	POINT   Screen_Size;
} Struct_MonitorData;

static Struct_MonitorData   Monitor_Base_Data;



void Monitor_Set_Primary_Pos(HWND TargetWindow)
{
    RECT    AkRect;
    POINT   AkSize;
	POINT   NewPos;

    GetWindowRect(TargetWindow, &AkRect);
    AkSize.x = AkRect.right  - AkRect.left;
    AkSize.y = AkRect.bottom - AkRect.top;

    ScreenToClient(GetParent(TargetWindow), (LPPOINT)&AkRect);
	AkRect.right  = AkRect.left + AkSize.x;
	AkRect.bottom = AkRect.top  + AkSize.y;

	if (Monitor_Base_Data.Count > 1 && Monitor_Base_Data.Has_Secondary == TRUE)
	{
		NewPos.x = (Monitor_Base_Data.Primary_Rect.right  + Monitor_Base_Data.Primary_Rect.left) / 2 - AkSize.x / 2;
		NewPos.y = (Monitor_Base_Data.Primary_Rect.bottom + Monitor_Base_Data.Primary_Rect.top)  / 2 - AkSize.y / 2;
	}
	else if (Monitor_Base_Data.Count > 0 && Monitor_Base_Data.Has_Primary == TRUE)
	{
		NewPos.x = 12;
		NewPos.y = (Monitor_Base_Data.Primary_Rect.bottom + Monitor_Base_Data.Primary_Rect.top) / 2 - AkSize.y / 2;
	}
	else
	{
		NewPos.x = 4;
		NewPos.y = Monitor_Base_Data.Screen_Size.y / 2 - AkSize.y / 2;
	}

	MoveWindow(TargetWindow, NewPos.x, NewPos.y, AkSize.x, AkSize.y, FALSE);
}


void Monitor_Get_Secondary_Rect(POINT* Ret_Win_Pos, POINT* Ret_Win_Size)
{
	if (Monitor_Base_Data.Count > 1 && Monitor_Base_Data.Has_Secondary == TRUE)
	{
		Ret_Win_Pos->x   = Monitor_Base_Data.Secondary_Rect.left;
		Ret_Win_Pos->y   = Monitor_Base_Data.Secondary_Rect.top;
		Ret_Win_Size->x  = Monitor_Base_Data.Secondary_Rect.right  - Monitor_Base_Data.Secondary_Rect.left;
		Ret_Win_Size->y  = Monitor_Base_Data.Secondary_Rect.bottom - Monitor_Base_Data.Secondary_Rect.top;
	}
	else if (Monitor_Base_Data.Count > 0 && Monitor_Base_Data.Has_Primary == TRUE)
	{
		Ret_Win_Size->x  = 1024;
		Ret_Win_Size->y  = 768;
		Ret_Win_Pos->x   = Monitor_Base_Data.Primary_Rect.right - Ret_Win_Size->x;
		Ret_Win_Pos->y   = 0;
	}
	else
	{
		Ret_Win_Size->x  = 640;
		Ret_Win_Size->y  = 480;
		Ret_Win_Pos->x   = Monitor_Base_Data.Screen_Size.x - Ret_Win_Size->x;
		Ret_Win_Pos->y   = 0;
	}
}


static BOOL CALLBACK Monitor_Enum_Proc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO                  MInfo;
	Struct_MonitorData*          MonitorDataPtr;


	MonitorDataPtr = (Struct_MonitorData*)dwData;
	++(MonitorDataPtr->Count);

	ZeroMemory(&MInfo, sizeof(MONITORINFO));
	MInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &MInfo);

	if (MInfo.dwFlags & MONITORINFOF_PRIMARY)
	{
		if (MonitorDataPtr->Has_Primary == FALSE)
		{
			MonitorDataPtr->Has_Primary = TRUE;
			memcpy(&MonitorDataPtr->Primary_Rect, lprcMonitor, sizeof(RECT));
		}
	}
	else
	{
		if (MonitorDataPtr->Has_Secondary == FALSE)
		{
			MonitorDataPtr->Has_Secondary = TRUE;
			memcpy(&MonitorDataPtr->Secondary_Rect, lprcMonitor, sizeof(RECT));
		}
	}

	return TRUE;
}

void Monitor_Init(void)
{
	ZeroMemory(&Monitor_Base_Data, sizeof(Struct_MonitorData));
	EnumDisplayMonitors(NULL, NULL, Monitor_Enum_Proc, (LPARAM)&Monitor_Base_Data);

	Monitor_Base_Data.Screen_Size.x = GetSystemMetrics(SM_CXSCREEN);
	Monitor_Base_Data.Screen_Size.y = GetSystemMetrics(SM_CYSCREEN);
}

