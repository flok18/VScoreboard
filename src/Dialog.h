


#define  TIMER_EV_ID_1  1
#define  TIMER_EV_ID_2  2



typedef struct {
	Scoreboard*  RefScoreBoard;
	HWND         TimerWindow;
	UINT_PTR     EventID;

	BOOL         Is_Active;
	UINT_PTR     TimerID;

	int          TeamNr;
	DWORD        Duration;
	DWORD        Last_Timestamp;

} Timeout_Timer;

typedef struct {
	Scoreboard*  RefScoreBoard;
	HWND         TimerWindow;
	UINT_PTR     EventID;

	BOOL         Is_Active;
	UINT_PTR     TimerID;

} Picture_Timer;




int Dialog_Game_Show(HWND Parent, Game* TheGame);

int Dialog_Description_Show(HWND Parent, Scoreboard* TheScoreBoard);

void Dialog_Control_Update_Game_Status(HWND hwndDlg);
void Dialog_Control_Fill_Layout_Combo(HWND hwndDlg);
void Dialog_Control_Fill_Picture_List(HWND hwndDlg);
void Dialog_Control_Show(HWND Parent, Scoreboard* TheScoreBoard);

void Timeout_Timer_Init(Timeout_Timer* TheTimer, Scoreboard* TheScoreboard, HWND TimerWin, UINT_PTR TheEventID);
void Timeout_Timer_Stop(Timeout_Timer* TheTimer, BOOL Use_Auto_Sound);
void Timeout_Timer_Event(Timeout_Timer* TheTimer, WPARAM wParam);
void Timeout_Timer_Update(Timeout_Timer* TheTimer);
void Timeout_Timer_Start(Timeout_Timer* TheTimer, int TimerTeamNr, DWORD TimerDuration_ms);


void Picture_Timer_Go_Next_Pic(HWND hwndDlg, Scoreboard* TheScoreboard);
void Picture_Timer_Init(Picture_Timer* TheTimer, Scoreboard* TheScoreboard, HWND TimerWin, UINT_PTR TheEventID);
void Picture_Timer_Stop(Picture_Timer* TheTimer);
void Picture_Timer_Event(Picture_Timer* TheTimer, WPARAM wParam);
void Picture_Timer_Start(Picture_Timer* TheTimer, DWORD TimerInterval_ms);

BOOL Check_Button_Timestamp(HWND hwndDlg, int Button_Type_Index);
void Dialog_Control_Do_Manual_Update(HWND hwndDlg);
void Dialog_Control_Set_Timestamp_Warning(HWND hwndDlg);
void Dialog_Control_Set_Vertical_Summary(HWND hwndDlg);
void Dialog_Control_Show_Edit_Description(HWND hwndDlg);
void Dialog_Control_Show_Edit_Game_Status(HWND hwndDlg);
void Dialog_Control_Menu_File_Save(HWND hwndDlg);
void Dialog_Control_Menu_File_Load(HWND hwndDlg);
void Dialog_Control_Menu_Layout_Load(HWND hwndDlg);
void Dialog_Control_Menu_Score_Save(HWND hwndDlg);
void Dialog_Control_Menu_Score_Load(HWND hwndDlg);

void Dialog_Control_Button_Checkbox_Set_Timer(HWND hwndDlg);
void Dialog_Control_Button_Layout_Change(HWND hwndDlg);
void Dialog_Control_Auto_Layout_Change(HWND hwndDlg, int LayoutIndex, BOOL Reset_Pic_Index);
void Dialog_Control_Button_Picture_Set_Next(HWND hwndDlg);
void Dialog_Control_Button_Picture_Set_Current(HWND hwndDlg);
void Dialog_Control_Button_Picture_Move_Down(HWND hwndDlg);
void Dialog_Control_Button_Picture_Move_Up(HWND hwndDlg);
void Dialog_Control_Button_Abort_Timeout(HWND hwndDlg);
void Dialog_Control_Button_Tech_Timeout(HWND hwndDlg);
void Dialog_Control_Button_Timeout_Sub(HWND hwndDlg, int TeamNr);
void Dialog_Control_Button_Timeout_Add(HWND hwndDlg, int TeamNr);
void Dialog_Control_Button_PlayerChange_Edit(HWND hwndDlg, int TeamNr, int Val);
void Dialog_Control_Button_Score_End_Set(HWND hwndDlg, BOOL Update_Board);
void Dialog_Control_Button_Score_Sub(HWND hwndDlg, int TeamNr);
void Dialog_Control_Button_Score_Add(HWND hwndDlg, int TeamNr);


extern HMENU          Dialog_Control_Settings_Menu;
extern BOOL           Dialog_Control_Is_Vertical_Summary;

extern Picture_Timer  Dialog_Control_Picture_Timer;
extern Timeout_Timer  Dialog_Control_Timeout_Timer;

extern Scoreboard*    Dialog_Control_ScoreBoard;
extern HINSTANCE      Dialog_Control_hInst;




