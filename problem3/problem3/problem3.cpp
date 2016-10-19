// problem3.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "problem3.h"
#include <string>

/*

// 这是导出变量的一个示例
PROBLEM3_API int nproblem3=0;

// 这是导出函数的一个示例。
PROBLEM3_API int fnproblem3(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 problem3.h

CPlugin::CPlugin()
{
	return;
}
*/

PROBLEM3_API const WCHAR * __stdcall GetPluginName(void)
{
	return L"PIMG_ENCRYPT";
}

PROBLEM3_API int __stdcall Process(LPCWSTR pszFile)
{
	//MessageBox(NULL, pszFile, L"test", MB_OK);
	std::wstring s = pszFile;
	s = s.substr(0, s.length() - 4) + L"_processing.bmp";
	CopyFile(pszFile, s.c_str(), FALSE);
	HANDLE hFile = CreateFile(s.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Cannot open file", L"Error", MB_ICONERROR);
		return 0;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMap == INVALID_HANDLE_VALUE || hFileMap == NULL)
	{
		CloseHandle(hFile);
		MessageBox(NULL, L"Cannot create mapping", L"Error", MB_ICONERROR);
		return 0;
	}
	const LPVOID vbptr = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (vbptr == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hFileMap);
		MessageBox(NULL, L"Mapping Error", L"Error", MB_ICONERROR);
		return 0;
	}
	PBITMAPFILEHEADER pFileHeader = (PBITMAPFILEHEADER)vbptr;
	if (pFileHeader->bfType == 0x4d42)
	{
		PBITMAPINFOHEADER pInfoHeader = (PBITMAPINFOHEADER)(pFileHeader + 1);
		PRGBTRIPLE pPixels = (PRGBTRIPLE)(reinterpret_cast<BYTE*> (vbptr) + pFileHeader->bfOffBits);
		if (pInfoHeader->biBitCount == 24)
		{

			RAWBITMAPINFO raw;
			raw.height = pInfoHeader->biHeight;
			raw.width = pInfoHeader->biWidth;
			raw.rawPixels = pPixels;
			std::vector<SLIDE> slides;
			SLIDE s(0, raw.height - 1, 0, raw.width - 1);
			slides.push_back(s);
			Transform_Split(&raw, 5, 6, slides);
			Transform_Xor(pPixels, pInfoHeader->biSizeImage);
			slides.push_back(s);
			Transform_Split(&raw,2,3, slides,1);

		}
	}
	std::wstring ss = s.substr(0, s.length() - 7) + L"ed.bmp";
	UnmapViewOfFile(vbptr);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
	MoveFileEx(s.c_str(), ss.c_str(), MOVEFILE_REPLACE_EXISTING);
	return 1;
}

void Transform_Xor(PRGBTRIPLE pPixels, LONGLONG imgSize)
{
	LONGLONG pixelCount = imgSize / 3;
	for (LONGLONG i(0);i < pixelCount;i++, pPixels++)
	{

		pPixels->rgbtRed = pPixels->rgbtRed + 91;//shift brightness
		pPixels->rgbtGreen = pPixels->rgbtGreen + 91;
		pPixels->rgbtBlue = pPixels->rgbtBlue + 91;

		int mixWay = i % 3;
		switch (mixWay)
		{
		case 0://red reserved
			pPixels->rgbtGreen ^= pPixels->rgbtRed;
			pPixels->rgbtBlue ^= pPixels->rgbtGreen;
			break;
		case 1://green reserved
			pPixels->rgbtRed ^= pPixels->rgbtGreen;
			pPixels->rgbtBlue ^= pPixels->rgbtRed;
			break;
		case 2://blue reserved
			pPixels->rgbtRed ^= pPixels->rgbtBlue;
			pPixels->rgbtGreen ^= pPixels->rgbtRed;
			break;
		}
		//continue;
		if (i % 8 == 0)
		{
			PPIXELSGROUP pGroup = (PPIXELSGROUP)pPixels;
			if (i < pixelCount - 8)
			{

				for (int j(0);j < 8;j++)
				{
					pGroup->group[j].rgbtRed ^= XorTableSmall[j];
					pGroup->group[j].rgbtGreen ^= XorTableSmall[j + 1];
					pGroup->group[j].rgbtBlue ^= XorTableSmall[j + 2];
				}
			}
			else
			{
				for (LONGLONG j(i);j < pixelCount - i;j++)
				{
					pGroup->group[j].rgbtRed ^= XorTableSmall[j];
					pGroup->group[j].rgbtGreen ^= XorTableSmall[j + 1];
					pGroup->group[j].rgbtBlue ^= XorTableSmall[j + 2];
				}
			}
		}
		if (i % 127 == 0)
		{
			if (i < pixelCount - 127)
			{
				for (int j(0);j < 127;j++)
				{
					pPixels[j].rgbtBlue ^= XorTableLarge[j];
					pPixels[j].rgbtGreen ^= XorTableLarge[j];
					pPixels[j].rgbtRed ^= XorTableLarge[j];
				}
			}
			else
			{
				for (LONGLONG j(i);j < pixelCount - i;j++)
				{
					pPixels[j].rgbtBlue ^= XorTableLarge[127 - j];
					pPixels[j].rgbtGreen ^= XorTableLarge[127 - j];
					pPixels[j].rgbtRed ^= XorTableLarge[127 - j];
				}
			}
		}
	}

}


void Transform_Split(PRAWBITMAPINFO pRaw)
{
	std::vector<SLIDE> slides;
	SLIDE s(0, pRaw->height - 1, 0, pRaw->width - 1);
	slides.push_back(s);
	Transform_Split(pRaw, 5, 6, slides);
}

void Transform_Split(PRAWBITMAPINFO pRaw, int slideRowCount, int slideColCount, std::vector<SLIDE> &slides, int maxdeep, int mode)
{
	LONG height = SlideHeight(slides.back());
	LONG width = SlideWidth(slides.back());
	if (slideRowCount >= height && height <= 3)slideRowCount = height;
	if (slideColCount >= width && width <= 3)slideColCount = width;
	LONG slideCount = slideRowCount*slideColCount;
	LONG nextHeight = height / slideRowCount;
	LONG nextWidth = width / slideColCount;

	SLIDE slideCurrent = slides.back();//save current slides
	slides.pop_back();//pop it from slides list


	if (slideCount < 2 || maxdeep <= 0 || (height <= 1 && width <= 1))
		return;

	for (int i(0), k(0);i < slideRowCount;i++)//split slides for next turn
	{
		for (int j(0);j < slideColCount;j++, k++)
		{
			SLIDE tslide;
			tslide.rowStart = slideCurrent.rowStart + i*nextHeight;
			tslide.colStart = slideCurrent.colStart + j*nextWidth;
			if (i < slideRowCount - 1)
				tslide.rowEnd = slideCurrent.rowStart + (i + 1)*nextHeight - 1;
			else
				tslide.rowEnd = slideCurrent.rowEnd;
			if (j < slideColCount - 1)
				tslide.colEnd = slideCurrent.colStart + (j + 1)*nextWidth - 1;
			else
				tslide.colEnd = slideCurrent.colEnd;
			slides.push_back(tslide);
		}
	}

	auto ritSlides = slides.rbegin();//reverse iterator to access the last (slidecount)slides

	//LONGLONG buffSize = SlideWidth(&(slides[slideCount - 1]))*SlideHeight(&(slides[slideCount - 1]));
	//PRGBTRIPLE buff = new RGBTRIPLE[buffSize];
	for (int i(slideCount / 2 - 1);i >= 0;--i)
	{
		if (mode == 1)
			ExchangeSlide(pRaw, nullptr/*buff*/, ritSlides[i], ritSlides[slideCount - i - 1]);
	}
	//delete[] buff;
	for (int i(0);i < slideCount && !slides.empty();i++)//process every slide
	{
		int nextSlideRowCount = slideRowCount - 1;
		int nextSlideColCount = slideColCount - 1;
		if (nextSlideRowCount <= 1) nextSlideRowCount = 5;
		if (nextSlideColCount <= 1) nextSlideColCount = 5;
		LONG h = SlideHeight(slides.back());
		LONG w = SlideWidth(slides.back());

		if (h <= 15)
		{
			if (h == 15) nextSlideRowCount = 5;
			else if (h >= 12) nextSlideRowCount = 4;
			else if (h >= 9) nextSlideRowCount = 3;
			else if (h >= 6) nextSlideRowCount = 2;
			else nextSlideRowCount = 1;
			if (w >= 20) w = 5;
		}
		if (w <= 15)
		{
			if (w == 15) nextSlideColCount = 5;
			else if (w == 12) nextSlideColCount = 4;
			else if (w > 9) nextSlideColCount = 3;
			else if (w > 6) nextSlideColCount = 2;
			else nextSlideColCount = 1;
			if (h >= 20)h = 5;
		}
		//if (slides.size() == 9) DebugBreak();
		Transform_Split(pRaw, nextSlideRowCount, nextSlideColCount, slides, maxdeep - 1, ~mode);

	}
}

void ExchangeSlide(PRAWBITMAPINFO pRaw, PRGBTRIPLE buff, SLIDE &slide1, SLIDE &slide2)
{
	if (!SlideHeight(slide1) || !SlideHeight(slide2) || !SlideWidth(slide1) || !SlideWidth(slide2))
	{
		return;
	}
	bool isInternalBuff = false;
	if (!buff)
	{
		buff = new RGBTRIPLE[MIN(SlideHeight(slide1), SlideHeight(slide2))*MIN(SlideWidth(slide1), SlideWidth(slide2))];
		isInternalBuff = true;
	}

	LONG h = MIN(SlideHeight(slide1), SlideHeight(slide2));
	LONG wsize = MIN(SlideWidth(slide1), SlideWidth(slide2))*sizeof(RGBTRIPLE);

	for (LONG i(0);i < h;i++)//copy by rows
	{
		LONG rowSize = pRaw->width * 3;
		if (rowSize % 4) rowSize += 4 - (rowSize % 4);

		PBYTE p = reinterpret_cast<PBYTE>(pRaw->rawPixels) + (slide1.rowStart + i)*rowSize;
		PRGBTRIPLE pSlide1Base = reinterpret_cast<PRGBTRIPLE>(p) + slide1.colStart;

		p = reinterpret_cast<PBYTE>(pRaw->rawPixels) + (slide2.rowStart + i)*rowSize;
		PRGBTRIPLE pSlide2Base = reinterpret_cast<PRGBTRIPLE>(p) + slide2.colStart;


		memcpy_s(buff, h*wsize, pSlide2Base, wsize);
		memcpy_s(pSlide2Base, wsize, pSlide1Base, wsize);
		memcpy_s(pSlide1Base, wsize, buff, wsize);
	}
	if (isInternalBuff) delete buff;
}

LONG SlideHeight(PSLIDE s)
{
	return s->rowEnd - s->rowStart + 1;
}

LONG SlideHeight(SLIDE &s)
{
	return s.rowEnd - s.rowStart + 1;
}

LONG SlideWidth(PSLIDE s)
{
	return s->colEnd - s->colStart + 1;
}

LONG SlideWidth(SLIDE &s)
{
	return s.colEnd - s.colStart + 1;
}
