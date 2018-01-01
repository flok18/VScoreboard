
#include "StdInc.h"
#include "Core.h"


/*
	Alpha = 0  ->  Bild Farbe
	Alpha = 1  ->  Schwarzer Rahmen
	Alpha = 2  ->  Transparent
*/


void Pixel_Erase_Background(Pixel* To_Bitmap, int To_Size_X, int To_Size_Y, unsigned int Color_Value)
{
	int  Num;

	Num = To_Size_X * To_Size_Y;
	while (Num > 0)
	{
		To_Bitmap->iValue = Color_Value;
		
		++To_Bitmap;
		--Num;
	}
}


void Pixel_Erase_Rectangle(Pixel* To_Bitmap, int To_Size_X, int To_Size_Y, unsigned int Color_Value,
						   int Rect_To_X, int Rect_To_Y, int Rect_Size_X, int Rect_Size_Y)
{
	int   ii_x;
	int   ii_y;

	if (To_Bitmap)
	{
		if (Rect_To_X < 0)
		{
			Rect_Size_X += Rect_To_X;
			Rect_To_X    = 0;
		}
		if (Rect_To_Y < 0)
		{
			Rect_Size_Y += Rect_To_Y;
			Rect_To_Y    = 0;
		}
		if (Rect_To_X + Rect_Size_X > To_Size_X)
		{
			Rect_Size_X = To_Size_X - Rect_To_X;
		}
		if (Rect_To_Y + Rect_Size_Y > To_Size_Y)
		{
			Rect_Size_Y = To_Size_Y - Rect_To_Y;
		}

		if (Rect_Size_X > 0 && Rect_Size_Y > 0)
		{
			To_Bitmap += (Rect_To_Y * To_Size_X + Rect_To_X);

			for (ii_y = 0; ii_y < Rect_Size_Y; ++ii_y)
			{
				for (ii_x = 0; ii_x < Rect_Size_X; ++ii_x)
				{
					To_Bitmap->iValue = Color_Value;
					
					++To_Bitmap;
				}

				To_Bitmap += (To_Size_X - Rect_Size_X);
			}
		}		
	}
}



void Pixel_Transfer_Copy(Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                         Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
						 int Rect_To_X, int Rect_To_Y,
						 int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y)
{
	int   ii_x;
	int   ii_y;

	if (To_Bitmap && From_Bitmap)
	{
		if (Rect_From_X < 0)
		{
			Rect_Size_X += Rect_From_X;
			Rect_To_X   -= Rect_From_X;
			Rect_From_X  = 0;
		}
		if (Rect_From_Y < 0)
		{
			Rect_Size_Y += Rect_From_Y;
			Rect_To_Y   -= Rect_From_Y;
			Rect_From_Y  = 0;
		}
		if (Rect_From_X + Rect_Size_X > From_Size_X)
		{
			Rect_Size_X = From_Size_X - Rect_From_X;
		}
		if (Rect_From_Y + Rect_Size_Y > From_Size_Y)
		{
			Rect_Size_Y = From_Size_Y - Rect_From_Y;
		}

		if (Rect_To_X < 0)
		{
			Rect_From_X -= Rect_To_X;
			Rect_Size_X += Rect_To_X;
			Rect_To_X    = 0;
		}
		if (Rect_To_Y < 0)
		{
			Rect_From_Y -= Rect_To_Y;
			Rect_Size_Y += Rect_To_Y;
			Rect_To_Y    = 0;
		}
		if (Rect_To_X + Rect_Size_X > To_Size_X)
		{
			Rect_Size_X = To_Size_X - Rect_To_X;
		}
		if (Rect_To_Y + Rect_Size_Y > To_Size_Y)
		{
			Rect_Size_Y = To_Size_Y - Rect_To_Y;
		}


		if (Rect_Size_X > 0 && Rect_Size_Y > 0)
		{
			From_Bitmap += (Rect_From_Y * From_Size_X + Rect_From_X);
			To_Bitmap   += (Rect_To_Y   * To_Size_X   + Rect_To_X);

			for (ii_y = 0; ii_y < Rect_Size_Y; ++ii_y)
			{
				for (ii_x = 0; ii_x < Rect_Size_X; ++ii_x)
				{
					To_Bitmap->iValue = From_Bitmap->iValue;
					To_Bitmap->RGBA.A = 0;
					
					++From_Bitmap;
					++To_Bitmap;
				}

				From_Bitmap += (From_Size_X - Rect_Size_X);
				To_Bitmap   += (To_Size_X   - Rect_Size_X);
			}
		}		
	}
}

void Pixel_Transfer_Draw(Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                         Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
					     int Rect_To_X, int Rect_To_Y,
						 int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y)
{
	int   ii_x;
	int   ii_y;


	if (To_Bitmap && From_Bitmap)
	{
		if (Rect_From_X < 0)
		{
			Rect_Size_X += Rect_From_X;
			Rect_To_X   -= Rect_From_X;
			Rect_From_X  = 0;
		}
		if (Rect_From_Y < 0)
		{
			Rect_Size_Y += Rect_From_Y;
			Rect_To_Y   -= Rect_From_Y;
			Rect_From_Y  = 0;
		}
		if (Rect_From_X + Rect_Size_X > From_Size_X)
		{
			Rect_Size_X = From_Size_X - Rect_From_X;
		}
		if (Rect_From_Y + Rect_Size_Y > From_Size_Y)
		{
			Rect_Size_Y = From_Size_Y - Rect_From_Y;
		}

		if (Rect_To_X < 0)
		{
			Rect_From_X -= Rect_To_X;
			Rect_Size_X += Rect_To_X;
			Rect_To_X    = 0;
		}
		if (Rect_To_Y < 0)
		{
			Rect_From_Y -= Rect_To_Y;
			Rect_Size_Y += Rect_To_Y;
			Rect_To_Y    = 0;
		}
		if (Rect_To_X + Rect_Size_X > To_Size_X)
		{
			Rect_Size_X = To_Size_X - Rect_To_X;
		}
		if (Rect_To_Y + Rect_Size_Y > To_Size_Y)
		{
			Rect_Size_Y = To_Size_Y - Rect_To_Y;
		}


		if (Rect_Size_X > 0 && Rect_Size_Y > 0)
		{
			From_Bitmap += (Rect_From_Y * From_Size_X + Rect_From_X);
			To_Bitmap   += (Rect_To_Y   * To_Size_X   + Rect_To_X);

			for (ii_y = 0; ii_y < Rect_Size_Y; ++ii_y)
			{
				for (ii_x = 0; ii_x < Rect_Size_X; ++ii_x)
				{
					if (From_Bitmap->RGBA.A == 0)
					{
						To_Bitmap->iValue = From_Bitmap->iValue;
					}
					else if (From_Bitmap->RGBA.A == 1)
					{
						To_Bitmap->iValue = 0;
					}

					++From_Bitmap;
					++To_Bitmap;
				}

				From_Bitmap += (From_Size_X - Rect_Size_X);
				To_Bitmap   += (To_Size_X   - Rect_Size_X);
			}
		}
	}
}

