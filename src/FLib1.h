

int ComboBox_AddItem(HWND WinHnd, const char* NewText);
void ComboBox_Clear(HWND WinHnd);
void ComboBox_SetIndex(HWND WinHnd, int SelIndex);
int ComboBox_GetIndex(HWND WinHnd);
int ListBox_AddItem(HWND WinHnd, const char* NewText);
void ListBox_Clear(HWND WinHnd);
void ListBox_SetIndex(HWND WinHnd, int SelIndex);
int ListBox_GetIndex(HWND WinHnd);

void ListBox_SetTopIndex(HWND WinHnd, int TopIndex);
int ListBox_GetTopIndex(HWND WinHnd);
LPARAM ListBox_GetData(HWND WinHnd, int SelIndex);
void ListBox_SetData(HWND WinHnd, int SelIndex, LPARAM NewData);
void ListBox_SetSelectStatus(HWND WinHnd, int SelIndex, BOOL NewStatus);
BOOL ListBox_GetSelectStatus(HWND WinHnd, int SelIndex);

void Window_SoftClose(HWND WinHnd);
void Window_SetIcon_hInst(HWND WinHnd, int IconID, HINSTANCE ThisAppInst);

size_t InputStringToBuffer(char* buf, size_t bsize, FILE *fp);
void SetFolderBackslash(char* FolderString, size_t FolderStringSize);
void RemoveFileFromFullPath(char* FullPath);
int Execute_FileSearch_Simple(const char* Pattern, void(*CallBackFnc)(WIN32_FIND_DATA*, char*,  int, void*), void* AnyPtr);
int IfFileExist(const char* FullFilePath);
BOOL DirectoryExists(const char* dirName);

HMENU Menu_Start(HWND WindowHandle);
HMENU Menu_Begin(const char* MenuName);
void Menu_Add(const char* MenuName, int MenuID);
void Menu_End(void);
int Menu_ShowPopup(HWND WindowHandle, HMENU MenuHandle);
int Menu_ShowPopupForWindow(HWND WindowHandle, HMENU MenuHandle);
void Menu_CheckItem(HMENU MenuHandle, int SubMenu_ID, int New_Status);

BOOL Shell_Dialog_GetFile_Open_hInst(HINSTANCE ModHandle, HWND ParentHwnd, const char* WinTitle, const char* FilterString,
                                     char* Ret_FileName, size_t Ret_FileName_Size, int* Ret_FilterIndex);
BOOL Shell_Dialog_GetFile_Save_hInst(HINSTANCE ModHandle, HWND ParentHwnd, const char* WinTitle, const char* FilterString,
                                     char* Ret_FileName, size_t Ret_FileName_Size, int* Ret_FilterIndex);

char* _stristr(const char* QuellString, const char* SuchString);
int sprintf_ss(char* Buffer, size_t BufferLen, const char* format, ...);
int strcpy_ss(char* dst, size_t max, const char* src);
int strcat_ss(char* dst, size_t max, const char* src);

BOOL SetApplicationRunning(char* Global_MutexName);
size_t Get_CommandLine_File(const char* CmdLne, char* TargetBuffer, size_t TargetBufferSize);
char* Get_Win32_CommandLine(void);
void MsgBox_Info(HWND ParentWinHand, const char* MsgText);
BOOL MsgBox_Frage(HWND ParentWinHand, const char* MsgText);
void MsgBox_Fehler(HWND ParentWinHand, const char* MsgText);


