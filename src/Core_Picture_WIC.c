
#include "StdInc.h"
#include "Core.h"


#include <wincodec.h>

#define  COM_PTR_RELEASE(p)  if (p) {p->lpVtbl->Release(p); p = NULL;}

static const IID    MY_IID_IWICImagingFactory       = {0xEC5EC8A9, 0xC395, 0x4314, {0x9C, 0x77, 0x54, 0xD7, 0xA9, 0x35, 0xFF, 0x70}};
static const CLSID  MY_CLSID_WICImagingFactory      = {0xCACAF262, 0x9370, 0x4615, {0xA1, 0x3B, 0x9F, 0x55, 0x39, 0xDA, 0x4C, 0x0A}};
static const IID    MY_IID_IWICBitmapSource         = {0x00000120, 0xA8F2, 0x4877, {0xBA, 0x0A, 0xFD, 0x2B, 0x66, 0x45, 0xFB, 0x94}};
static const GUID   MY_GUID_WICPixelFormat32bppBGR  = {0x6FDDC324, 0x4E03, 0x4BFE, {0xB1, 0x85, 0x3D, 0x77, 0x76, 0x8D, 0xC9, 0x0E}};


#ifdef __PRINT_GUIDs

static void Print_GUID(const GUID* TheGUID)
{
    printf("{0x%08X, 0x%04X, 0x%04X, {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}};\n",
            TheGUID->Data1, TheGUID->Data2, TheGUID->Data3,
            TheGUID->Data4[0], TheGUID->Data4[1], TheGUID->Data4[2], TheGUID->Data4[3],
            TheGUID->Data4[4], TheGUID->Data4[5], TheGUID->Data4[6], TheGUID->Data4[7]);
}

static void Print_All_GUIDs(void)
{
    Print_GUID(&IID_IWICImagingFactory);
    Print_GUID(&CLSID_WICImagingFactory);
    Print_GUID(&IID_IWICBitmapSource);
    Print_GUID(&GUID_WICPixelFormat32bppBGR);
}

#endif // __PRINT_GUIDs



BOOL Picture_WIC_Load(const char* FileName, Picture* Pic, BOOL Do_Flip_Y)
{
    BOOL                    Return_Status     = FALSE;
    HRESULT                 hr;
    wchar_t                 FileNameU[MAX_PATH];
    IWICImagingFactory*     m_pIWICFactory    = NULL;
    IWICBitmapDecoder*      pDecoder          = NULL;
    IWICBitmapFrameDecode*  pFrame            = NULL;
    IWICBitmapSource*       pSource           = NULL;
    UINT                    uiWidth           = 0;
    UINT                    uiHeight          = 0;
    UINT                    cbStride          = 0;
    UINT                    cbImage           = 0;
    WICPixelFormatGUID      pixelFormat;
    IWICFormatConverter*    pConverter        = NULL;
    IWICBitmapSource*       pSourceRgb        = NULL;
    BYTE*                   ImagePixels       = NULL;


#ifdef __PRINT_GUIDs
    Print_All_GUIDs();
#endif

	CoInitialize(NULL);

    // Create WIC factory
    hr = CoCreateInstance(&MY_CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, &MY_IID_IWICImagingFactory, (void**)(&m_pIWICFactory));
    if (SUCCEEDED(hr))
    {
        // Create a decoder
        MultiByteToWideChar(CP_ACP, 0, FileName, -1, FileNameU, MAX_PATH);
        hr = m_pIWICFactory->lpVtbl->CreateDecoderFromFilename(m_pIWICFactory, FileNameU, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
        if (SUCCEEDED(hr))
        {
            // Retrieve the first frame of the image from the decoder
            hr = pDecoder->lpVtbl->GetFrame(pDecoder, 0, &pFrame);
            if (SUCCEEDED(hr))
            {
                // Retrieve IWICBitmapSource from the frame
                hr = pFrame->lpVtbl->QueryInterface(pFrame, &MY_IID_IWICBitmapSource, (void**)(&pSource));
                if (SUCCEEDED(hr))
                {
                    // Get The Size
                    hr = pSource->lpVtbl->GetSize(pSource, &uiWidth, &uiHeight);
                    if (SUCCEEDED(hr))
                    {
                        // Get The Pixelformat
                        hr = pSource->lpVtbl->GetPixelFormat(pSource, &pixelFormat);
                        if (SUCCEEDED(hr))
                        {
                            // Do we need to Convert...
                            if (IsEqualGUID(&pixelFormat, &MY_GUID_WICPixelFormat32bppBGR))
                            {
                                // Achtung: Bei Freigeben undbedingt Nullen bzw. auf NULL pruefen.
                                pSourceRgb = pSource;
                            }
                            else
                            {
                                // Format convert to 32bppBGR
                                hr = m_pIWICFactory->lpVtbl->CreateFormatConverter(m_pIWICFactory, &pConverter);
                                if (SUCCEEDED(hr))
                                {
                                    // Do the conversion
                                    hr = pConverter->lpVtbl->Initialize(pConverter, pSource, &MY_GUID_WICPixelFormat32bppBGR, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Store the converted bitmap as IWICBitmapSource
                                        hr = pConverter->lpVtbl->QueryInterface(pConverter, &MY_IID_IWICBitmapSource, (void**)(&pSourceRgb));
                                        if (FAILED(hr))
                                        {
                                            pSourceRgb = NULL;
                                        }
                                    }
                                    COM_PTR_RELEASE(pConverter);
                                }
                            }

                            // Is Conversion OK
                            if (pSourceRgb)
                            {
                                cbStride  = uiWidth * sizeof(DWORD);
                                cbImage   = cbStride * uiHeight;

								if (Do_Flip_Y)
								{
									// Allocate Buffer
									ImagePixels = malloc(cbImage);
									if (ImagePixels)
									{
										// Extract the image pixels
										hr = pSourceRgb->lpVtbl->CopyPixels(pSourceRgb, NULL, cbStride, cbImage, ImagePixels);
										if (SUCCEEDED(hr))
										{
											Picture_Flip_Y_Pixels(Pic, ImagePixels, uiWidth, uiHeight);
											Return_Status = TRUE;
										}
										free(ImagePixels);
									}
								}
								else
								{
									Pic->Size_X = uiWidth;
									Pic->Size_Y = uiHeight;
									Pic->Pixels = (Pixel*)malloc(cbImage);
									if (Pic->Pixels)
									{
										hr = pSourceRgb->lpVtbl->CopyPixels(pSourceRgb, NULL, cbStride, cbImage, (BYTE*)Pic->Pixels);
										if (SUCCEEDED(hr))
										{
											Return_Status = TRUE;
										}
									}
								}

                                COM_PTR_RELEASE(pSourceRgb);
                            }
                        }
                    }
                    COM_PTR_RELEASE(pSource);
                }
                COM_PTR_RELEASE(pFrame);
            }
            COM_PTR_RELEASE(pDecoder);
        }
        COM_PTR_RELEASE(m_pIWICFactory);
    }
	else
	{
		puts("ERROR: CoCreateInstance");
	}

	CoUninitialize();

    return Return_Status;
}


