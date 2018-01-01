
#include "StdInc.h"
#include "Core.h"


// ============================================================================
// GDIPLUS Header
// ============================================================================

#ifdef _MSC_VER
#pragma comment (lib, "gdiplus.lib")
#endif
#define    PixelFormatIndexed         0x00010000 // Indexes into a palette
#define    PixelFormatGDI             0x00020000 // Is a GDI-supported format
#define    PixelFormatAlpha           0x00040000 // Has an alpha component
#define    PixelFormatPAlpha          0x00080000 // Pre-multiplied alpha
#define    PixelFormatExtended        0x00100000 // Extended color 16 bits/channel
#define    PixelFormatCanonical       0x00200000
#define    PixelFormatUndefined       0
#define    PixelFormatDontCare        0
#define    PixelFormat1bppIndexed     (1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat4bppIndexed     (2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat8bppIndexed     (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat16bppGrayScale  (4 | (16 << 8) | PixelFormatExtended)
#define    PixelFormat16bppRGB555     (5 | (16 << 8) | PixelFormatGDI)
#define    PixelFormat16bppRGB565     (6 | (16 << 8) | PixelFormatGDI)
#define    PixelFormat16bppARGB1555   (7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
#define    PixelFormat24bppRGB        (8 | (24 << 8) | PixelFormatGDI)
#define    PixelFormat32bppRGB        (9 | (32 << 8) | PixelFormatGDI)
#define    PixelFormat32bppARGB       (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
#define    PixelFormat32bppPARGB      (11 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
#define    PixelFormat48bppRGB        (12 | (48 << 8) | PixelFormatExtended)
#define    PixelFormat64bppARGB       (13 | (64 << 8) | PixelFormatAlpha  | PixelFormatCanonical | PixelFormatExtended)
#define    PixelFormat64bppPARGB      (14 | (64 << 8) | PixelFormatAlpha  | PixelFormatPAlpha | PixelFormatExtended)
#define    PixelFormatMax             15

typedef enum {
	Ok 				           = 0,
	GenericError 	           = 1,
	InvalidParameter           = 2,
	OutOfMemory 	           = 3,
	ObjectBusy 		           = 4,
	InsufficientBuffer         = 5,
	NotImplemented             = 6,
	Win32Error 		           = 7,
	WrongState 		           = 8,
	Aborted 		           = 9,
	FileNotFound 	           = 10,
	ValueOverflow 	           = 11,
	AccessDenied 	           = 12,
	UnknownImageFormat         = 13,
	FontFamilyNotFound         = 14,
	FontStyleNotFound          = 15,
	NotTrueTypeFont            = 16,
	UnsupportedGdiplusVersion  = 17,
	GdiplusNotInitialized      = 18,
	PropertyNotFound           = 19,
	PropertyNotSupported       = 20,
	ProfileNotFound            = 21
} GpStatus;

typedef enum {
    eImageLockModeRead        = 0x0001,
    eImageLockModeWrite       = 0x0002,
    eImageLockModeUserInputBuf= 0x0004
} GpImageLockMode;

typedef enum {
    eRotateNoneFlipNone = 0,
    eRotate90FlipNone   = 1,
    eRotate180FlipNone  = 2,
    eRotate270FlipNone  = 3,

    eRotateNoneFlipX    = 4,
    eRotate90FlipX      = 5,
    eRotate180FlipX     = 6,
    eRotate270FlipX     = 7,

    eRotateNoneFlipY    = eRotate180FlipX,
    eRotate90FlipY      = eRotate270FlipX,
    eRotate180FlipY     = eRotateNoneFlipX,
    eRotate270FlipY     = eRotate90FlipX,

    eRotateNoneFlipXY   = eRotate180FlipNone,
    eRotate90FlipXY     = eRotate270FlipNone,
    eRotate180FlipXY    = eRotateNoneFlipNone,
    eRotate270FlipXY    = eRotate90FlipNone
} GpRotateFlipType;

typedef struct {
	UINT32   GdiplusVersion;
	void*    DebugEventCallback;
	BOOL     SuppressBackgroundThread;
	BOOL     SuppressExternalCodecs;
} GdiplusStartupInput;

typedef struct {
    int      unused;
} GP_GPBITMAP, GP_GPIMAGE;

typedef struct {
    int       X;
    int       Y;
    int       Width;
    int       Height;
} GPRECTI;

typedef struct {
    UINT      Width;
    UINT      Height;
    INT       Stride;
    INT       GpPixelFormat;
    VOID*     Scan0;
    UINT_PTR  Reserved;
} BITMAPDATA;


// ============================================================================
// GDIPLUS Interface
// ============================================================================

#ifdef _GDIP_LINK_DIRECT

GpStatus __stdcall GdiplusStartup(ULONG_PTR* token, const GdiplusStartupInput* input, void* output);
VOID     __stdcall GdiplusShutdown(ULONG_PTR token);
GpStatus __stdcall GdipCreateBitmapFromFile(const WCHAR* filename, GP_GPBITMAP** bitmap);
GpStatus __stdcall GdipGetImageWidth(GP_GPIMAGE* image, UINT* width);
GpStatus __stdcall GdipGetImageHeight(GP_GPIMAGE* image, UINT* height);
GpStatus __stdcall GdipBitmapLockBits(GP_GPBITMAP* bitmap, const GPRECTI* rect, GpImageLockMode flags, INT format, BITMAPDATA* lockedBitmapData);
GpStatus __stdcall GdipBitmapUnlockBits(GP_GPBITMAP* bitmap, BITMAPDATA* lockedBitmapData);
GpStatus __stdcall GdipDisposeImage(GP_GPIMAGE* image);
GpStatus __stdcall GdipImageRotateFlip(GP_GPIMAGE* image, GpRotateFlipType rfType);

#else

GpStatus (__stdcall *GdiplusStartup)           (ULONG_PTR*, const GdiplusStartupInput*, void*)                    = NULL;
VOID     (__stdcall *GdiplusShutdown)          (ULONG_PTR)                                                        = NULL;
GpStatus (__stdcall *GdipCreateBitmapFromFile) (const WCHAR*, GP_GPBITMAP**)                                      = NULL;
GpStatus (__stdcall *GdipGetImageWidth)        (GP_GPIMAGE*, UINT*)                                               = NULL;
GpStatus (__stdcall *GdipGetImageHeight)       (GP_GPIMAGE*, UINT*)                                               = NULL;
GpStatus (__stdcall *GdipBitmapLockBits)       (GP_GPBITMAP*, const GPRECTI*, GpImageLockMode, INT, BITMAPDATA*)  = NULL;
GpStatus (__stdcall *GdipBitmapUnlockBits)     (GP_GPBITMAP*, BITMAPDATA*)                                        = NULL;
GpStatus (__stdcall *GdipDisposeImage)         (GP_GPIMAGE*)                                                      = NULL;
GpStatus (__stdcall *GdipImageRotateFlip)      (GP_GPIMAGE*, GpRotateFlipType)                                    = NULL;

#define BITMAP_GDIP_DLL_IMPORT(name)    name = (void*)GetProcAddress(__static_Gdip_Dll, #name); \
                                        if (name == NULL) Return_Success = FALSE;

static BOOL Bitmap_GDIP_Dll_Init(BOOL Load)
{
    static HINSTANCE  __static_Gdip_Dll  = NULL;
    BOOL              Return_Success     = TRUE;

    if (Load)
    {
        if (__static_Gdip_Dll == NULL)
        {
            __static_Gdip_Dll = LoadLibrary("GdiPlus.dll");
            if (__static_Gdip_Dll == NULL)
            {
                Return_Success = FALSE;
            }
            else
            {
#ifdef _MSC_VER
#pragma warning(disable: 4054)
#pragma warning(disable: 4152)
#endif
                BITMAP_GDIP_DLL_IMPORT(GdiplusStartup)
                BITMAP_GDIP_DLL_IMPORT(GdiplusShutdown)
                BITMAP_GDIP_DLL_IMPORT(GdipCreateBitmapFromFile)
                BITMAP_GDIP_DLL_IMPORT(GdipGetImageWidth)
                BITMAP_GDIP_DLL_IMPORT(GdipGetImageHeight)
                BITMAP_GDIP_DLL_IMPORT(GdipBitmapLockBits)
                BITMAP_GDIP_DLL_IMPORT(GdipBitmapUnlockBits)
                BITMAP_GDIP_DLL_IMPORT(GdipDisposeImage)
                BITMAP_GDIP_DLL_IMPORT(GdipImageRotateFlip)
#ifdef _MSC_VER
#pragma warning(default: 4054)
#pragma warning(default: 4152)
#endif
            }
        }
    }

    if (Load == FALSE || Return_Success == FALSE)
    {
        if (__static_Gdip_Dll)
        {
            FreeLibrary(__static_Gdip_Dll);
            __static_Gdip_Dll = NULL;
        }
    }
    return Return_Success;
}

#endif /* _GDIP_LINK_DIRECT */


BOOL Picture_GDIP_Load(const char* FileName, Picture* Pic, BOOL Do_Flip_Y)
{
    BOOL                 Return_Status     = FALSE;
    ULONG_PTR            gdiplusToken      = 0;
	GdiplusStartupInput  gdipStartupInput  = {1, NULL, FALSE, FALSE};
    wchar_t              PicPathName[512];
    GpStatus             LastResult;
    GP_GPBITMAP*         bitmap            = NULL;
    UINT                 SizeValX;
    UINT                 SizeValY;
    GPRECTI              rect;
    BITMAPDATA           lockedBitmapData;
    DWORD                NumBytes;


    if (Bitmap_GDIP_Dll_Init(TRUE) == FALSE) return FALSE;

	GdiplusStartup(&gdiplusToken, &gdipStartupInput, NULL);

	MultiByteToWideChar(CP_ACP, 0, FileName, -1, PicPathName, 512);

    LastResult = GdipCreateBitmapFromFile(PicPathName, &bitmap);
    if (LastResult == Ok)
    {
        GdipImageRotateFlip(bitmap, eRotateNoneFlipY);
		GdipGetImageWidth(bitmap,  &SizeValX);
		GdipGetImageHeight(bitmap, &SizeValY);

        rect.X      = 0;
        rect.Y      = 0;
        rect.Width  = SizeValX;
        rect.Height = SizeValY;
        NumBytes    = SizeValX * SizeValY * 4;

        memset(&lockedBitmapData, 0, sizeof(BITMAPDATA));
        LastResult = GdipBitmapLockBits(bitmap, &rect, eImageLockModeRead, PixelFormat32bppRGB, &lockedBitmapData);
        if (LastResult == Ok)
        {
			if (Do_Flip_Y)
			{
				Picture_Flip_Y_Pixels(Pic, lockedBitmapData.Scan0, SizeValX, SizeValY);
			}
			else
			{
				Pic->Size_X         = SizeValX;
				Pic->Size_Y         = SizeValY;
				Pic->Pixels         = (Pixel*)malloc(NumBytes);
				memcpy(Pic->Pixels, lockedBitmapData.Scan0, NumBytes);
			}

			Return_Status = TRUE;
            GdipBitmapUnlockBits(bitmap, &lockedBitmapData);
        }
        GdipDisposeImage(bitmap);
    }

    GdiplusShutdown(gdiplusToken);

    Bitmap_GDIP_Dll_Init(FALSE);

    return Return_Status;
}



