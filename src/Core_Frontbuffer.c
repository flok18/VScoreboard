

#include "StdInc.h"
#include "Core.h"


static const char*   Frontbuffer_Class_Name       = "ScoreBoard_Window_Class";
static ATOM          Frontbuffer_Win_Class        = 0;
static HINSTANCE     Frontbuffer_Win_hInst        = NULL;


// --------------------------------------------------------------------


void Frontbuffer_Init(Frontbuffer* TheFrontbuffer)
{
	memset(TheFrontbuffer, 0, sizeof(Frontbuffer));
}

void Frontbuffer_Release(Frontbuffer* TheFrontbuffer)
{
	if (TheFrontbuffer->Window)
	{
		if (TheFrontbuffer->DC)
		{
			ReleaseDC(TheFrontbuffer->Window, TheFrontbuffer->DC);
		}
		DestroyWindow(TheFrontbuffer->Window);
	}
	TheFrontbuffer->Window        = NULL;
	TheFrontbuffer->DC            = NULL;
	TheFrontbuffer->Size_X        = 0;
	TheFrontbuffer->Size_Y        = 0;
	TheFrontbuffer->Paint_Source  = NULL;
}


// --------------------------------------------------------------------


static BOOL Frontbuffer_Paint(HWND hwnd)
{
	Frontbuffer*   TargetDisplay;
    HDC            DevCtx;
    PAINTSTRUCT    ps;


	TargetDisplay = (Frontbuffer*)GetWindowLongPtr(hwnd, 0);

	if (TargetDisplay)
	{
		if (TargetDisplay->Paint_Source)
		{
			if (TargetDisplay->Paint_Source->DC &&
			    TargetDisplay->Paint_Source->Mem)
			{
				DevCtx = BeginPaint(hwnd, &ps);

				if (TargetDisplay->Paint_Source->Size_X == TargetDisplay->Size_X &&
				    TargetDisplay->Paint_Source->Size_Y == TargetDisplay->Size_Y)
				{
					BitBlt(DevCtx, 0, 0, TargetDisplay->Size_X, TargetDisplay->Size_Y,
						   TargetDisplay->Paint_Source->DC, 0, 0, SRCCOPY);
				}
				else
				{

					StretchBlt(DevCtx, 0, 0, TargetDisplay->Size_X, TargetDisplay->Size_Y,
					           TargetDisplay->Paint_Source->DC, 0, 0,
							   TargetDisplay->Paint_Source->Size_X, TargetDisplay->Paint_Source->Size_Y, SRCCOPY);
				}

				EndPaint(hwnd, &ps);
				return TRUE;
			}
		}
	}
	return FALSE;
}


#ifdef _MSC_VER
#pragma warning (disable: 4306)
#endif

static void Frontbuffer_Window_Escape(HWND hwnd)
{
	LONG_PTR  ExStyles;

	ExStyles = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

	if (ExStyles & WS_EX_TOPMOST)
	{
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
					 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
					 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_NOSIZE);
	}
}

#ifdef _MSC_VER
#pragma warning (default: 4306)
#endif


static LRESULT CALLBACK Frontbuffer_Window_Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
		case WM_PAINT:  // WM_NCPAINT
			if (Frontbuffer_Paint(hwnd)) return 0;
			break;

		case WM_KEYUP:
			if (wParam == VK_ESCAPE) Frontbuffer_Window_Escape(hwnd);
			break;
	}
    return DefWindowProc(hwnd, message, wParam, lParam);
}


BOOL Frontbuffer_Create_Window(Frontbuffer* TheFrontbuffer, Backbuffer* TheBackbuffer,
                               int Px, int Py, int Sx, int Sy)
{
	TheFrontbuffer->Window = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
					  					    Frontbuffer_Class_Name, NULL,
											WS_VISIBLE | WS_POPUP,
											Px, Py, Sx, Sy,
											NULL, NULL, Frontbuffer_Win_hInst, NULL);
	if (TheFrontbuffer->Window)
	{
		SetWindowLongPtr(TheFrontbuffer->Window, 0, (LONG_PTR)TheFrontbuffer);

		TheFrontbuffer->DC            = GetDC(TheFrontbuffer->Window);  // GetWindowDC(...)
		TheFrontbuffer->Size_X        = Sx;
		TheFrontbuffer->Size_Y        = Sy;
		TheFrontbuffer->Paint_Source  = TheBackbuffer;

		/*
			STRETCH_ANDSCANS,    BLACKONWHITE  => Schlecht
			STRETCH_DELETESCANS, COLORONCOLOR  => Gut
			STRETCH_HALFTONE,    HALFTONE      => Gut
			STRETCH_ORSCANS,     WHITEONBLACK  => Schlecht
		*/
		SetStretchBltMode(TheFrontbuffer->DC, STRETCH_DELETESCANS);

		return TRUE;
	}
	return FALSE;
}

void Frontbuffer_Update(Frontbuffer* TheFrontbuffer)
{
	if (TheFrontbuffer->Window)
	{
		InvalidateRect(TheFrontbuffer->Window, NULL, FALSE);
	}
}

// --------------------------------------------------------------------


void Frontbuffer_Register_Class(HINSTANCE My_hInst)
{
    WNDCLASSEX       wndcls;

	if (Frontbuffer_Win_Class == 0 &&
    	Frontbuffer_Win_hInst == NULL)
	{
		ZeroMemory(&wndcls, sizeof(WNDCLASSEX));
		wndcls.cbSize                  = sizeof(WNDCLASSEX);
		wndcls.style                   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndcls.lpfnWndProc             = Frontbuffer_Window_Proc;
		wndcls.cbWndExtra              = sizeof(void*);
		wndcls.hInstance               = My_hInst;
		wndcls.lpszClassName           = Frontbuffer_Class_Name;

		Frontbuffer_Win_Class = RegisterClassEx(&wndcls);
		Frontbuffer_Win_hInst = My_hInst;
	}
}

void Frontbuffer_Unregister_Class(void)
{
	if (Frontbuffer_Win_Class != 0 &&
    	Frontbuffer_Win_hInst != NULL)
	{
		UnregisterClass(Frontbuffer_Class_Name, Frontbuffer_Win_hInst);
		Frontbuffer_Win_Class  = 0;
		Frontbuffer_Win_hInst  = NULL;
	}
}


