
#include "StdInc.h"
#include "Core.h"



#define BYTES_PER_PIXEL  4


void Picture_Flip_Y_Pixels(Picture* Pic, BYTE* Source, UINT SizeX, UINT SizeY)
{
    DWORD  StrideSize;
    DWORD  ImageSize;
    BYTE*  SourcePos;
    BYTE*  TargetPos;

    StrideSize = SizeX * sizeof(DWORD);
    ImageSize  = StrideSize * SizeY;

    Pic->Size_X = SizeX;
    Pic->Size_Y = SizeY;
    Pic->Pixels = (Pixel*)malloc(ImageSize);

    SourcePos = Source + ImageSize;
    TargetPos = (BYTE*)Pic->Pixels;

    while (SourcePos > Source)
    {
        SourcePos -= StrideSize;

        memcpy(TargetPos, SourcePos, StrideSize);

        TargetPos += StrideSize;
    }
}




// http://www.codeguru.com/cpp/g-m/bitmap/specialeffects/article.php/c1779/Win32-C-Function-for-HighQuality-Bitmap-Shrinking.htm

void Picture_Shrink(Picture* FromPic, Picture* ToPic, int Resize_To_X, int Resize_To_Y)
{
	DWORD    New_Size_X;
	DWORD    New_Size_Y;
	DWORD    Old_Size_X;
	DWORD    Old_Size_Y;
	BYTE*    uy;
	BYTE*    ux;
	DWORD    i;
	DWORD    x;
	DWORD    y;
	DWORD    nx;
	DWORD    ny;
	DWORD    df;
	DWORD    nf;
	DWORD    j;
	float    k;
	float    qx[2];
	float    qy[2];
	float    q[4];
	float*   f;

	New_Size_X = (DWORD)Resize_To_X;
	New_Size_Y = (DWORD)Resize_To_Y;
	Old_Size_X = (DWORD)FromPic->Size_X;
	Old_Size_Y = (DWORD)FromPic->Size_Y;

	ToPic->Size_X = Resize_To_X;
	ToPic->Size_Y = Resize_To_Y;
	ToPic->Pixels = malloc(Resize_To_X * Resize_To_Y * BYTES_PER_PIXEL);

	uy = (BYTE*)FromPic->Pixels;
	ny = 0;
	df = BYTES_PER_PIXEL * New_Size_X;
	nf = df * New_Size_Y;

	f  = (float*)malloc(nf * sizeof(float));
	memset(f, 0, nf * sizeof(float));

	j = 0;

	for (y = 0; y < Old_Size_Y; ++y)
	{
		ux =  uy;
		uy += (Old_Size_X * BYTES_PER_PIXEL);
		nx =  0;
		ny += New_Size_Y;

		if (ny > Old_Size_Y)
		{
			qy[0] = 1.0f - (qy[1] = (float)(ny - Old_Size_Y) / (float)New_Size_Y);

			for (x = 0; x < Old_Size_X; ++x)
			{
				nx += New_Size_X;

				if (nx > Old_Size_X)
				{
					qx[0] = 1.0f - (qx[1] = (float)(nx - Old_Size_X) / (float)New_Size_X);

					for (i = 0; i < 4; ++i)
					{
						q[i] = qx[i & 1] * qy[i >> 1];
					}

					for (i = 0; i < 3; ++i)
					{
						f[j]                        += (*ux) * q[0];
						f[j + BYTES_PER_PIXEL]      += (*ux) * q[1];
						f[j + df]                   += (*ux) * q[2];
						f[j + df + BYTES_PER_PIXEL] += (*ux) * q[3];
						++ux;
						++j;
					}
					++ux; // BYTES_PER_PIXEL == 4
					++j;
				}
				else
				{
					for (i = 0; i < 3; ++i)
					{
						f[j + i]      += (*ux) * qy[0];
						f[j + df + i] += (*ux) * qy[1];
						++ux;
					}
					++ux; // BYTES_PER_PIXEL == 4
				}
				if (nx >= Old_Size_X) nx -= Old_Size_X;
				if (!nx) j += BYTES_PER_PIXEL;
			}
		}
		else
		{
			for (x = 0; x < Old_Size_X; ++x)
			{
				nx += New_Size_X;

				if (nx > Old_Size_X)
				{
					qx[0] = 1.0f - (qx[1] = (float)(nx - Old_Size_X) / (float)New_Size_X);

					for (i = 0; i < 3; ++i)
					{
						f[j]                   += (*ux) * qx[0];
						f[j + BYTES_PER_PIXEL] += (*ux) * qx[1];
						++j;
						++ux;
					}
					++ux; // BYTES_PER_PIXEL == 4
					++j;
				}
				else
				{
					for (i = 0; i < 3; ++i)
					{
						f[j + i] += *ux;
						++ux;
					}
					++ux; // BYTES_PER_PIXEL == 4
				}

				if (nx >= Old_Size_X) nx -= Old_Size_X;
				if (!nx) j += BYTES_PER_PIXEL;
			}
			if (ny < Old_Size_Y) j -= df;
		}
		if (ny >= Old_Size_Y) ny -= Old_Size_Y;
	}

	nf = 0;
	k  = (float)(New_Size_X * New_Size_Y) / (float)(Old_Size_X * Old_Size_Y);
	uy = (BYTE*)ToPic->Pixels;

	for (y = 0; y < New_Size_Y; ++y)
	{
		for (j = 0; j < df; ++j)
		{
			uy[j] = (BYTE)(f[nf] * k + 0.5f);
			++nf;
		}
		uy += (BYTES_PER_PIXEL * New_Size_X);
	}

	free(f);
}


void Picture_Resize_Bilinear(Picture* FromPic, Picture* ToPic, int Resize_To_X, int Resize_To_Y)
{
    int            a, b, c, d, x, y, index;
    float          x_ratio, y_ratio;
    float          x_diff, y_diff, blue, red, green;
	int            w1, h1, w2, h2;
	unsigned int*  From_Pixels;
	unsigned int*  To_Pixels;
	int            offset, i, j;

	ToPic->Size_X = Resize_To_X;
	ToPic->Size_Y = Resize_To_Y;
	ToPic->Pixels = malloc(Resize_To_X * Resize_To_Y * BYTES_PER_PIXEL);

	w1 = FromPic->Size_X;
	h1 = FromPic->Size_Y;
	w2 = ToPic->Size_X;
	h2 = ToPic->Size_Y;

	From_Pixels = (unsigned int*)FromPic->Pixels;
	To_Pixels   = (unsigned int*)ToPic->Pixels;

	x_ratio = ((float)(w1 - 1)) / w2;
	y_ratio = ((float)(h1 - 1)) / h2;

	offset = 0;

    for (i = 0; i < h2; ++i)
	{
        for (j = 0; j < w2; ++j)
		{
            x = (int)(x_ratio * j);
            y = (int)(y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            index = (y * w1 + x);

            a = From_Pixels[index];
            b = From_Pixels[index + 1];
            c = From_Pixels[index + w1];
            d = From_Pixels[index + w1 + 1];

            // blue element
            // Yb = Ab(1-w1)(1-h1) + Bb(w1)(1-h1) + Cb(h1)(1-w1) + Db(wh)
            blue = (a & 0xff) * (1.0f - x_diff) * (1.0f - y_diff) +
			       (b & 0xff) *  x_diff         * (1.0f - y_diff) +
                   (c & 0xff) *  y_diff         * (1.0f - x_diff) +
				   (d & 0xff) *  x_diff         *  y_diff;

            // green element
            // Yg = Ag(1-w1)(1-h1) + Bg(w1)(1-h1) + Cg(h1)(1-w1) + Dg(wh)
            green = ((a >> 8) & 0xff) * (1.0f - x_diff) * (1.0f - y_diff) +
			        ((b >> 8) & 0xff) *  x_diff         * (1.0f - y_diff) +
                    ((c >> 8) & 0xff) *  y_diff         * (1.0f - x_diff) +
					((d >> 8) & 0xff) *  x_diff         *  y_diff;

            // red element
            // Yr = Ar(1-w1)(1-h1) + Br(w1)(1-h1) + Cr(h1)(1-w1) + Dr(wh)
            red = ((a >> 16) & 0xff) * (1.0f - x_diff) * (1.0f - y_diff) +
			      ((b >> 16) & 0xff) *  x_diff         * (1.0f - y_diff) +
                  ((c >> 16) & 0xff) *  y_diff         * (1.0f - x_diff) +
				  ((d >> 16) & 0xff) *  x_diff         *  y_diff;

            To_Pixels[offset++] =
                    0x00000000 | // hardcode alpha
                    ((((unsigned int)red)   << 16) & 0xff0000) |
                    ((((unsigned int)green) << 8)  & 0xff00) |
                    ((unsigned int)blue) ;
        }
    }
}


void Picture_Resize_Nearest_Neighbor(Picture* FromPic, Picture* ToPic, int Resize_To_X, int Resize_To_Y)
{
    int            x_ratio, y_ratio;
    int            x2, y2;
    int            i, j;
	int            w1, h1, w2, h2;
	unsigned int*  From_Pixels;
	unsigned int*  To_Pixels;

	ToPic->Size_X = Resize_To_X;
	ToPic->Size_Y = Resize_To_Y;
	ToPic->Pixels = malloc(Resize_To_X * Resize_To_Y * BYTES_PER_PIXEL);

	w1 = FromPic->Size_X;
	h1 = FromPic->Size_Y;
	w2 = ToPic->Size_X;
	h2 = ToPic->Size_Y;

	From_Pixels = (unsigned int*)FromPic->Pixels;
	To_Pixels   = (unsigned int*)ToPic->Pixels;

    // EDIT: added +1 to account for an early rounding problem
    x_ratio = (int)((w1 << 16) / w2) + 1;
    y_ratio = (int)((h1 << 16) / h2) + 1;

    for (i = 0; i < h2; i++)
	{
        for (j = 0; j < w2; j++)
		{
            x2 = ((j * x_ratio) >> 16);
            y2 = ((i * y_ratio) >> 16);
            To_Pixels[(i * w2) + j] = From_Pixels[(y2 * w1) + x2];
        }
    }
}



