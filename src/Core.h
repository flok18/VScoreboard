
#define INVALID_INDEX  (-1)
#define CRLF  "\r\n"

#define MAX_STR_SIZE_NAME    64
#define MAX_STR_SIZE_FILE   288  // MAX_PATH

#define  GAME_WIN_SETS               3
#define  GAME_MAX_SETS               5
#define  GAME_NORMAL_WIN_POINTS     25
#define  GAME_TIEBREAK_WIN_POINTS   15

#define  GAME_MAX_TIMEOUTS           2
#define  GAME_MAX_CHANGES            6
#define  TIMEOUT_DURATION_NORMAL    30900
#define  TIMEOUT_DURATION_TECH      60900

#define ELEMENT_ID_SCORE_TEAM_1                 0
#define ELEMENT_ID_SCORE_TEAM_2                 1
#define ELEMENT_ID_CURRENT_SET_NUM              2
#define ELEMENT_ID_SETS_TEAM_1                  3
#define ELEMENT_ID_SETS_TEAM_2                  4
#define ELEMENT_ID_SET_1_RESULT_TEAM_1          5
#define ELEMENT_ID_SET_1_RESULT_TEAM_2          6
#define ELEMENT_ID_SET_2_RESULT_TEAM_1          7
#define ELEMENT_ID_SET_2_RESULT_TEAM_2          8
#define ELEMENT_ID_SET_3_RESULT_TEAM_1          9
#define ELEMENT_ID_SET_3_RESULT_TEAM_2         10
#define ELEMENT_ID_SET_4_RESULT_TEAM_1         11
#define ELEMENT_ID_SET_4_RESULT_TEAM_2         12
#define ELEMENT_ID_SET_5_RESULT_TEAM_1         13
#define ELEMENT_ID_SET_5_RESULT_TEAM_2         14
#define ELEMENT_ID_TIMEOUT_TIMER               15
#define ELEMENT_ID_TIMEOUTS_TEAM_1             16
#define ELEMENT_ID_TIMEOUTS_TEAM_2             17
#define ELEMENT_ID_TIMEOUT_TEXT                18
#define ELEMENT_ID_SERVICE_LED_TEAM_1          19
#define ELEMENT_ID_SERVICE_LED_TEAM_2          20
#define ELEMENT_ID_PLAYER_CHANGE_TEAM_1        21
#define ELEMENT_ID_PLAYER_CHANGE_TEAM_2        22
#define ELEMENT_ID_GAME_TITLE                  23
#define ELEMENT_ID_NAME_TEAM_1                 24
#define ELEMENT_ID_NAME_TEAM_2                 25
#define ELEMENT_ID_RESERVE                     26
#define NUM_SCORE_ELEMENTS                     27

#define ELEMENT_ORIENTATION_HORIZONTAL   0
#define ELEMENT_ORIENTATION_VERTICAL     1


// ============================================================================


typedef union {
    unsigned int iValue;
	struct {
		char R;
		char G;
		char B;
		char A;
	} RGBA;
} Pixel;

typedef struct {
	short      Score_Team_1;
	short      Score_Team_2;
	short      Timeouts_Team_1;
	short      Timeouts_Team_2;
	short      Player_Changes_Team_1;
	short      Player_Changes_Team_2;
} SetResult;

typedef struct {

	int         Score_Team_1;
	int         Score_Team_2;
	int         Service_Team_Nr;     // Base 1 (0 = Ende)
	int         Current_Set_Nr;      // Base 1
	int         Sets_Team_1;
	int         Sets_Team_2;

	SetResult   Set_1_Result;
	SetResult   Set_2_Result;
	SetResult   Set_3_Result;
	SetResult   Set_4_Result;
	SetResult   Set_5_Result;

	int         Timeouts_Team_1;
	int         Timeouts_Team_2;

	int         Player_Changes_Team_1;
	int         Player_Changes_Team_2;

} Game;

typedef struct {

	int        Pos_X;
	int        Pos_Y;

	int        Size_X;
	int        Size_Y;

	int        Orientation;

	BOOL       Is_Configured;

	int        Font_Index;
	DWORD      Font_Color;

} Element;

typedef struct {
	char       FullFileName[MAX_STR_SIZE_FILE];
	int        FileNameOffset;
} FileData;

typedef struct {
	char         FolderWithBackslash[MAX_STR_SIZE_FILE];
	FileData*    aFiles;
	int          nFiles;
	int          mFiles;
} FileList;

typedef struct {

	char          Name[MAX_STR_SIZE_NAME];

	char          Background_File[MAX_STR_SIZE_FILE];

	Element       Score_Elements[NUM_SCORE_ELEMENTS];

	FileList      Picture_List;
	Element       Picture_Box;
	int           Current_Picture;
	int           Picture_Timer_Interval;

} Layout;

typedef struct {
    int              Size_X;
    int              Size_Y;
    Pixel*           Pixels;
} Picture;

typedef struct {

	int                 Size_X;
	int                 Size_Y;

	HDC                 DC;
	HBITMAP             Bmp;
	Pixel*              Mem;

	HBITMAP             Orig_Bmp;
	HFONT               Orig_Font;
	HBRUSH              Orig_Brush;
	HPEN                Orig_Pen;

	int                 Current_Font_Index;
	DWORD               Current_Font_Color;

} Backbuffer;

typedef struct {
	HWND                Window;
	HDC                 DC;
	int                 Size_X;
	int                 Size_Y;
	Backbuffer*         Paint_Source;
} Frontbuffer;

typedef struct {
	HFONT*              aFonts;
	int                 nFonts;
} FontCollection;

typedef struct {

	Frontbuffer         Target_Display;
	Backbuffer          Target_Backbuffer;
	Picture             Target_Picture;

	char                Title[MAX_STR_SIZE_NAME];
	char                Team_1[MAX_STR_SIZE_NAME];
	char                Team_2[MAX_STR_SIZE_NAME];

	FontCollection      Fonts;

	Layout*             aLayout;
	int                 nLayout;
	Layout*             CurrentLayout;

	Game                Game_Status;

} Scoreboard;


// ============================================================================


void Scoreboard_Init(Scoreboard* TheScoreboard);
void Scoreboard_Release(Scoreboard* TheScoreboard);
void Scoreboard_Clear_Layouts(Scoreboard* TheScoreboard);
void Scoreboard_Move_Data(Scoreboard* FromScoreboard, Scoreboard* ToScoreboard, BOOL Include_Game);
void Scoreboard_Save(Scoreboard* TheScoreboard, FILE* TheFP);
void Scoreboard_Load(Scoreboard* TheScoreboard, FILE* TheFP);
BOOL Scoreboard_Create_Window(Scoreboard* TheScoreboard);
int Scoreboard_New_Layout(Scoreboard* TheScoreboard, const char* Name);
int Scoreboard_Get_Layout_Index(Scoreboard* TheScoreboard);
void Scoreboard_Select_Layout(Scoreboard* TheScoreboard, int Index, BOOL Reset_Pic_Index);
void Scoreboard_Update_Static_Text(Scoreboard* TheScoreboard);
void Scoreboard_Update_Game_Status(Scoreboard* TheScoreboard);
void Scoreboard_Update_Picture_Box(Scoreboard* TheScoreboard);
void Scoreboard_Update_Timer(Scoreboard* TheScoreboard, int Value);
void Scoreboard_Update_TimeoutText(Scoreboard* TheScoreboard, int TeamNr);

void Pixel_Erase_Background(Pixel* To_Bitmap, int To_Size_X, int To_Size_Y, unsigned int Color_Value);
void Pixel_Erase_Rectangle(Pixel* To_Bitmap, int To_Size_X, int To_Size_Y, unsigned int Color_Value,
						   int Rect_To_X, int Rect_To_Y, int Rect_Size_X, int Rect_Size_Y);
void Pixel_Transfer_Copy(Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                         Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
						 int Rect_To_X, int Rect_To_Y,
						 int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y);
void Pixel_Transfer_Draw(Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                         Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
					     int Rect_To_X, int Rect_To_Y,
						 int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y);

void Monitor_Set_Primary_Pos(HWND TargetWindow);
void Monitor_Get_Secondary_Rect(POINT* Ret_Win_Pos, POINT* Ret_Win_Size);
void Monitor_Init(void);

void Layout_Init(Layout* TheLayout);
void Layout_Release(Layout* TheLayout);
void Layout_Save(Layout* TheLayout, FILE* TheFP);
void Layout_Load(Layout* TheLayout, FILE* TheFP);
void Layout_Draw_Game_Status(Layout* TheLayout, Game* TheGame, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection);
void Layout_Draw_Picture_Box(Layout* TheLayout, Picture* TheBackPic, Backbuffer* TheBackbuffer);
void Layout_Draw_Timer(Layout* TheLayout, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection, int TimeValue);
void Layout_Draw_Timout_Text(Layout* TheLayout, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection, const char* TeamText);

void Sound_Init(void);
void Sound_Play_Long(void);
void Sound_Play_Short(void);
void Sound_Abort(void);

void SetResult_Reset(SetResult* TheSetResult);
void Game_Reset(Game* TheGame);
void Game_Copy(Game* FromGame, Game* ToGame);
void Game_Save(Game* TheGame, FILE* TheFP);
void Game_Load(Game* TheGame, FILE* TheFP);
SetResult* Game_Get_Any_Set_Result(Game* TheGame, int SetNumber);
SetResult* Game_Get_Current_Set_Result(Game* TheGame);
void Game_Build_Summary_Horizontal(Game* TheGame, char* Out_Buffer, size_t Buffer_Size);
void Game_Build_Summary_Vertical(Game* TheGame, char* Out_Buffer, size_t Buffer_Size);

void Frontbuffer_Init(Frontbuffer* TheFrontbuffer);
void Frontbuffer_Release(Frontbuffer* TheFrontbuffer);
BOOL Frontbuffer_Create_Window(Frontbuffer* TheFrontbuffer, Backbuffer* TheBackbuffer,
                               int Px, int Py, int Sx, int Sy);
void Frontbuffer_Update(Frontbuffer* TheFrontbuffer);
void Frontbuffer_Register_Class(HINSTANCE My_hInst);
void Frontbuffer_Unregister_Class(void);

void FontCollection_Init(FontCollection* TheFontCollection);
void FontCollection_Release(FontCollection* TheFontCollection);
int FontCollection_New_Font(FontCollection* TheFontCollection, const char* Name, int Height, int Weight, int Angle);
void FontCollection_Save(FontCollection* TheFontCollection, FILE* TheFP);
void FontCollection_Load(FontCollection* TheFontCollection, FILE* TheFP);

void FileList_Init(FileList* TheFileList);
void FileList_Release(FileList* TheFileList);
int FileList_Get_Count(FileList* TheFileList);
FileData* FileList_Get_File(FileList* TheFileList, int Index);
void FileList_Swap(FileList* TheFileList, int Index1, int Index2);
void FileList_Create(FileList* TheFileList, const char* SrcFolder);
void FileList_Save(FileList* TheFileList, FILE* TheFP);
void FileList_Load(FileList* TheFileList, FILE* TheFP);

void Element_Init(Element* TheLayout);
void Element_Release(Element* TheLayout);
void Element_Save(Element* TheElement, FILE* TheFP);
void Element_Load(Element* TheElement, FILE* TheFP);
void Element_Erase_Background(Element* TheElement, Picture* TheBackPic, Backbuffer* TheBackbuffer);
void Element_Draw_Simple_Text(Element* TheElement, const char* TextToDraw, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection);
void Element_Draw_Simple_Number(Element* TheElement, int NumValue, Picture* TheBackPic, Backbuffer* TheBackbuffer, FontCollection* TheFontCollection);
void Element_Draw_Simple_Led_On(Element* TheElement, DWORD Color, Picture* TheBackPic, Backbuffer* TheBackbuffer);
void Element_Draw_Simple_Led_Off(Element* TheElement, Picture* TheBackPic, Backbuffer* TheBackbuffer);
void Element_Draw_Simple_Picture(Element* TheElement, Picture* FrontPic, Picture* TheBackPic, Backbuffer* TheBackbuffer);

void Backbuffer_Init(Backbuffer* TheBackbuffer);
void Backbuffer_Release(Backbuffer* TheBackbuffer);
void Backbuffer_Release_Font(Backbuffer* TheBackbuffer);
void Backbuffer_Create(Backbuffer* TheBackbuffer, int Sx, int Sy, HDC Templ_Window);
void Backbuffer_Resize(Backbuffer* TheBackbuffer, int Sx, int Sy, HDC Templ_Window);
BOOL Backbuffer_Set_Font(Backbuffer* TheBackbuffer, int FontIndex, DWORD FontColor, FontCollection* TheFontCollection);

BOOL Import_Layout_From_File(Scoreboard* TheScoreboard, const char* Layout_File);
BOOL Scoreboard_Save_Layouts_To_File(Scoreboard* TheScoreboard, const char* Scoreboard_File);
BOOL Scoreboard_Load_Layouts_From_File(Scoreboard* TheScoreboard, const char* Scoreboard_File);
BOOL Game_Save_Score_To_File(Game* TheGame, const char*  Game_File);
BOOL Game_Load_Score_From_File(Game* TheGame, const char* Game_File);

void Picture_Init(Picture* Pic);
void Picture_Release(Picture* Pic);
void Picture_Move(Picture* From_Pic, Picture* To_Pic);
void Picture_Resize(Picture* Pic, int Ideal_Size_X, int Ideal_Size_Y);
BOOL Picture_Load(Picture* Pic, const char* PicFileName);

BOOL Picture_GDIP_Load(const char* FileName, Picture* Pic, BOOL Do_Flip_Y);

BOOL Picture_WIC_Load(const char* FileName, Picture* Pic, BOOL Do_Flip_Y);

void Picture_Flip_Y_Pixels(Picture* Pic, BYTE* Source, UINT SizeX, UINT SizeY);
void Picture_Shrink(Picture* FromPic, Picture* ToPic, int Resize_To_X, int Resize_To_Y);
void Picture_Resize_Bilinear(Picture* FromPic, Picture* ToPic, int Resize_To_X, int Resize_To_Y);
void Picture_Resize_Nearest_Neighbor(Picture* FromPic, Picture* ToPic, int Resize_To_X, int Resize_To_Y);



// ============================================================================


FILE* File_IO_Open(const char* FileName, const char* Mode);
void _dbg_print(const char* format, ...);
void* ReAllocMem(void* MemPtr, size_t MemSize);
BOOL Has_File_Extension(const char* File, const char* Ext);
char* Split_Command(char* Command, char Splitter);
void Deflate_Command(char* Command);
void Lower_Command(char* Command);
void Trim_Command(char* Command);
int Get_Listbox_Selection(HWND ListBox, BOOL** Ret_Array);
void Set_Listbox_Selection(HWND ListBox, BOOL* Sel_Array, int Sel_Array_Max);
void Swap_In_Bool_Array(BOOL* Array, int Index1, int Index2);
BOOL Is_Path_Absolute(const char* PathString);


