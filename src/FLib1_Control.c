
#include "StdInc.h"
#include "FLib1.h"

int ComboBox_AddItem(HWND WinHnd, const char* NewText)
{
    return (int)SendMessage(WinHnd, CB_ADDSTRING, 0, (LPARAM)NewText);
}

void ComboBox_Clear(HWND WinHnd)
{
    SendMessage(WinHnd, CB_RESETCONTENT, 0, 0);
}

void ComboBox_SetIndex(HWND WinHnd, int SelIndex)
{
    if (SelIndex < 0) SelIndex = -1;
    SendMessage (WinHnd, CB_SETCURSEL, SelIndex, 0);
}

int ComboBox_GetIndex(HWND WinHnd)
{
    int RetVal;
    RetVal = (int)SendMessage(WinHnd, CB_GETCURSEL, 0, 0);
    if (RetVal < 0) RetVal = -1;
    return RetVal;
}

int ListBox_AddItem(HWND WinHnd, const char* NewText)
{
    return (int)SendMessage(WinHnd, LB_ADDSTRING, 0, (LPARAM)NewText);
}
void ListBox_Clear(HWND WinHnd)
{
    SendMessage (WinHnd, LB_RESETCONTENT, 0, 0);
}

void ListBox_SetIndex(HWND WinHnd, int SelIndex)
{
    if (SelIndex < 0) SelIndex = -1;

    SendMessage (WinHnd, LB_SETCURSEL, SelIndex, 0);
}

int ListBox_GetIndex(HWND WinHnd)
{
    LRESULT RetVal;

    RetVal = SendMessage(WinHnd, LB_GETCURSEL, 0, 0);

    if (RetVal < 0) return -1;
    return (int)RetVal;
}



void ListBox_SetTopIndex(HWND WinHnd, int TopIndex)
{
    if (TopIndex < 0) return;

    SendMessage(WinHnd, LB_SETTOPINDEX, TopIndex, 0);
}

int ListBox_GetTopIndex(HWND WinHnd)
{
    LRESULT RetVal;

    RetVal = SendMessage(WinHnd, LB_GETTOPINDEX, 0, 0);

    if (RetVal < 0) return -1;
    return (int)RetVal;
}

LPARAM ListBox_GetData(HWND WinHnd, int SelIndex)
{
    return (LPARAM)SendMessage(WinHnd, LB_GETITEMDATA, (WPARAM)SelIndex, 0);
}

void ListBox_SetData(HWND WinHnd, int SelIndex, LPARAM NewData)
{
	SendMessage(WinHnd, LB_SETITEMDATA, (WPARAM)SelIndex, NewData);
}


void ListBox_SetSelectStatus(HWND WinHnd, int SelIndex, BOOL NewStatus)
{
    if (SelIndex < 0) return;

    SendMessage(WinHnd, LB_SETSEL, NewStatus, SelIndex);
}

BOOL ListBox_GetSelectStatus(HWND WinHnd, int SelIndex)
{
    LRESULT RetVal;

    if (SelIndex < 0) return 0;

    RetVal = SendMessage(WinHnd, LB_GETSEL, SelIndex, 0);
    if (RetVal > 0)
    {
        return TRUE;
    }
    return FALSE;
}

void Window_SoftClose(HWND WinHnd)
{
    PostMessage(WinHnd,WM_CLOSE,0,0);
}

void Window_SetIcon_hInst(HWND WinHnd, int IconID, HINSTANCE ThisAppInst)
{
    HICON       RetIcon        = NULL;

    if (IconID)
	{
		// RetIcon = LoadIcon(ModuleInstance, MAKEINTRESOURCE(IconID));
		RetIcon = (HICON)LoadImage(ThisAppInst, MAKEINTRESOURCE(IconID), IMAGE_ICON, 0, 0, 0);
	}
    if (RetIcon)
    {
        SendMessage(WinHnd, WM_SETICON, ICON_SMALL, (LPARAM)RetIcon);
        SendMessage(WinHnd, WM_SETICON, ICON_BIG, (LPARAM)RetIcon);
        // SetClassLong(WinHnd, GCL_HICON, (LPARAM)RetIcon);
        DeleteObject(RetIcon);
    }
}


