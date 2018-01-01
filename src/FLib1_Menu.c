
#include "StdInc.h"
#include "FLib1.h"


#define         _FloLib1_Menu_MaxStufe                             5       // [0-4]
static int      _FloLib1_Menu_AkStufe                            = -1;
static HWND     _FloLib1_Menu_AkWindow                           = NULL;
static HMENU    _FloLib1_Menu_MemHMENU[_FloLib1_Menu_MaxStufe];

HMENU Menu_Start(HWND WindowHandle)
{
    _FloLib1_Menu_AkStufe  = 0;
    _FloLib1_Menu_MemHMENU[0]  = CreateMenu();
    _FloLib1_Menu_AkWindow     = WindowHandle;
    return _FloLib1_Menu_MemHMENU[0];
}

HMENU Menu_Begin(const char* MenuName)
{
    if (_FloLib1_Menu_AkStufe > _FloLib1_Menu_MaxStufe - 2) return 0;

    if (_FloLib1_Menu_AkStufe < 0)    // Ohne Fenster -> Benutzerdefiniertes Popup Menu
    {
        _FloLib1_Menu_AkStufe  = 0;
        _FloLib1_Menu_MemHMENU[0]  = CreatePopupMenu();
        _FloLib1_Menu_AkWindow     = NULL;
        return _FloLib1_Menu_MemHMENU[0];
    }
    else                            // Normales Untermenu
    {
        ++_FloLib1_Menu_AkStufe;
        _FloLib1_Menu_MemHMENU[_FloLib1_Menu_AkStufe] = CreatePopupMenu();
        AppendMenu(_FloLib1_Menu_MemHMENU[_FloLib1_Menu_AkStufe - 1], MF_STRING|MF_POPUP, (UINT_PTR)_FloLib1_Menu_MemHMENU[_FloLib1_Menu_AkStufe], MenuName);
        return _FloLib1_Menu_MemHMENU[_FloLib1_Menu_AkStufe];
    }
}

void Menu_Add(const char* MenuName, int MenuID)
{
    if (_FloLib1_Menu_AkStufe < 0) return;

    if (MenuID == 0 || MenuName == NULL)
    {
        AppendMenu(_FloLib1_Menu_MemHMENU[_FloLib1_Menu_AkStufe], MF_SEPARATOR, 0, NULL);
    }
    else
    {
        AppendMenu(_FloLib1_Menu_MemHMENU[_FloLib1_Menu_AkStufe], MF_STRING, (UINT)MenuID, MenuName);
    }
}

void Menu_End(void)
{
    if (_FloLib1_Menu_AkStufe < 0) return;

    if (_FloLib1_Menu_AkStufe == 0)
    {
        if (_FloLib1_Menu_AkWindow != NULL)
        {
            SetMenu(_FloLib1_Menu_AkWindow, _FloLib1_Menu_MemHMENU[0]);
            _FloLib1_Menu_AkWindow = NULL;
        }
        _FloLib1_Menu_AkStufe = -1;
    }
    else
    {
        --_FloLib1_Menu_AkStufe;
    }
}

int Menu_ShowPopup(HWND WindowHandle, HMENU MenuHandle)  // Mit Menu-ID als Rückgabewert
{
    POINT CursorPos;
    GetCursorPos(&CursorPos);
    return TrackPopupMenuEx(MenuHandle, TPM_RETURNCMD | TPM_NONOTIFY, CursorPos.x, CursorPos.y, WindowHandle, NULL);
}
int Menu_ShowPopupForWindow(HWND WindowHandle, HMENU MenuHandle)  // Menu-Prozedur wird aufgerufen
{
    POINT CursorPos;
    GetCursorPos(&CursorPos);
    return TrackPopupMenuEx(MenuHandle, 0, CursorPos.x, CursorPos.y, WindowHandle, NULL);
}


void Menu_CheckItem(HMENU MenuHandle, int SubMenu_ID, int New_Status)
{
	UINT  Menu_Flags  = MF_BYCOMMAND;

	if (New_Status)
	{
		Menu_Flags |= MF_CHECKED;
	}

	CheckMenuItem(MenuHandle, SubMenu_ID, Menu_Flags);
}



