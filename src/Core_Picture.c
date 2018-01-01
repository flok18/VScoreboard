

#include "StdInc.h"
#include "Core.h"


void Picture_Init(Picture* Pic)
{
    Pic->Size_X = 0;
    Pic->Size_Y = 0;
    Pic->Pixels = NULL;
}

void Picture_Release(Picture* Pic)
{
    if (Pic->Pixels)
    {
        free(Pic->Pixels);
    }
    Picture_Init(Pic);
}

void Picture_Move(Picture* From_Pic, Picture* To_Pic)
{
	Picture_Release(To_Pic);
	memcpy(To_Pic, From_Pic, sizeof(Picture));
	Picture_Init(From_Pic);
}

// --------------------------------------------------------------------

void Picture_Resize(Picture* Pic, int Ideal_Size_X, int Ideal_Size_Y)
{
	int       New_Size_X;
	int       New_Size_Y;
	Picture   Resized_Pic;

	if (Pic->Size_X  > 0 &&
	    Pic->Size_Y  > 0 &&
	    Ideal_Size_X > 0 &&
	    Ideal_Size_Y > 0)
	{
		if (Pic->Size_X < Ideal_Size_X &&
			Pic->Size_Y < Ideal_Size_Y)
		{
			New_Size_X = Ideal_Size_X;
			New_Size_Y = Pic->Size_Y * Ideal_Size_X / Pic->Size_X;

			if (New_Size_Y > Ideal_Size_Y)
			{
				New_Size_X = Pic->Size_X * Ideal_Size_Y / Pic->Size_Y;
				New_Size_Y = Ideal_Size_Y;
			}

			Picture_Init(&Resized_Pic);
			Picture_Resize_Bilinear(Pic, &Resized_Pic, New_Size_X, New_Size_Y);
			Picture_Move(&Resized_Pic, Pic);
		}
		else if (Pic->Size_X > Ideal_Size_X ||
				 Pic->Size_Y > Ideal_Size_Y)
		{
			New_Size_X = Pic->Size_X;
			New_Size_Y = Pic->Size_Y;

			if (New_Size_X > Ideal_Size_X)
			{
				New_Size_Y = MulDiv(New_Size_Y, Ideal_Size_X, New_Size_X);
				New_Size_X = Ideal_Size_X;
			}
			if (New_Size_Y > Ideal_Size_Y)
			{
				New_Size_X = MulDiv(New_Size_X, Ideal_Size_Y, New_Size_Y);
				New_Size_Y = Ideal_Size_Y;
			}

			Picture_Init(&Resized_Pic);
			Picture_Shrink(Pic, &Resized_Pic, New_Size_X, New_Size_Y);
			Picture_Move(&Resized_Pic, Pic);
		}
	}
}

BOOL Picture_Load(Picture* Pic, const char* PicFileName)
{
	BOOL  Load_Ret  = FALSE;

	// First Try WIC Interface
	Load_Ret = Picture_WIC_Load(PicFileName, Pic, FALSE);
	if (Load_Ret == FALSE)
	{
		// Then Try GDI-Plus
		Load_Ret = Picture_GDIP_Load(PicFileName, Pic, TRUE);
	}
    return Load_Ret;
}


