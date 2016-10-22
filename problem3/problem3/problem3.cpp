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
			std::vector<SLICE> slices;
			SLICE s(0, raw.height - 1, 0, raw.width - 1);
			slices.push_back(s);
			Transform_Split(&raw, 5, 6, slices);
			Transform_Xor(pPixels, pInfoHeader->biSizeImage);
			slices.push_back(s);
			Transform_Split(&raw,2,3, slices,1);

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
	std::vector<SLICE> slices;
	SLICE s(0, pRaw->height - 1, 0, pRaw->width - 1);
	slices.push_back(s);
	Transform_Split(pRaw, 5, 6, slices);
}

void Transform_Split(PRAWBITMAPINFO pRaw, int sliceRowCount, int sliceColCount, std::vector<SLICE> &slices, int maxdeep, int mode)
{
	LONG height = SliceHeight(slices.back());
	LONG width = SliceWidth(slices.back());
	if (sliceRowCount >= height && height <= 3)sliceRowCount = height;
	if (sliceColCount >= width && width <= 3)sliceColCount = width;
	LONG sliceCount = sliceRowCount*sliceColCount;
	LONG nextHeight = height / sliceRowCount;
	LONG nextWidth = width / sliceColCount;

	SLICE sliceCurrent = slices.back();//save current slices
	slices.pop_back();//pop it from slices list


	if (sliceCount < 2 || maxdeep <= 0 || (height <= 1 && width <= 1))
		return;

	for (int i(0), k(0);i < sliceRowCount;i++)//split slices for next turn
	{
		for (int j(0);j < sliceColCount;j++, k++)
		{
			SLICE tslice;
			tslice.rowStart = sliceCurrent.rowStart + i*nextHeight;
			tslice.colStart = sliceCurrent.colStart + j*nextWidth;
			if (i < sliceRowCount - 1)
				tslice.rowEnd = sliceCurrent.rowStart + (i + 1)*nextHeight - 1;
			else
				tslice.rowEnd = sliceCurrent.rowEnd;
			if (j < sliceColCount - 1)
				tslice.colEnd = sliceCurrent.colStart + (j + 1)*nextWidth - 1;
			else
				tslice.colEnd = sliceCurrent.colEnd;
			slices.push_back(tslice);
		}
	}

	auto ritSlices = slices.rbegin();//reverse iterator to access the last (slicecount)slices

	//LONGLONG buffSize = SliceWidth(&(slices[sliceCount - 1]))*SliceHeight(&(slices[sliceCount - 1]));
	//PRGBTRIPLE buff = new RGBTRIPLE[buffSize];
	for (int i(sliceCount / 2 - 1);i >= 0;--i)
	{
		if (mode == 1)
			ExchangeSlice(pRaw, nullptr/*buff*/, ritSlices[i], ritSlices[sliceCount - i - 1]);
	}
	//delete[] buff;
	for (int i(0);i < sliceCount && !slices.empty();i++)//process every slice
	{
		int nextSliceRowCount = sliceRowCount - 1;
		int nextSliceColCount = sliceColCount - 1;
		if (nextSliceRowCount <= 1) nextSliceRowCount = 5;
		if (nextSliceColCount <= 1) nextSliceColCount = 5;
		LONG h = SliceHeight(slices.back());
		LONG w = SliceWidth(slices.back());

		if (h <= 15)
		{
			if (h == 15) nextSliceRowCount = 5;
			else if (h >= 12) nextSliceRowCount = 4;
			else if (h >= 9) nextSliceRowCount = 3;
			else if (h >= 6) nextSliceRowCount = 2;
			else nextSliceRowCount = 1;
			if (w >= 20) w = 5;
		}
		if (w <= 15)
		{
			if (w == 15) nextSliceColCount = 5;
			else if (w == 12) nextSliceColCount = 4;
			else if (w > 9) nextSliceColCount = 3;
			else if (w > 6) nextSliceColCount = 2;
			else nextSliceColCount = 1;
			if (h >= 20)h = 5;
		}
		//if (slices.size() == 9) DebugBreak();
		Transform_Split(pRaw, nextSliceRowCount, nextSliceColCount, slices, maxdeep - 1, ~mode);

	}
}

void ExchangeSlice(PRAWBITMAPINFO pRaw, PRGBTRIPLE buff, SLICE &slice1, SLICE &slice2)
{
	if (!SliceHeight(slice1) || !SliceHeight(slice2) || !SliceWidth(slice1) || !SliceWidth(slice2))
	{
		return;
	}
	bool isInternalBuff = false;
	if (!buff)
	{
		buff = new RGBTRIPLE[MIN(SliceHeight(slice1), SliceHeight(slice2))*MIN(SliceWidth(slice1), SliceWidth(slice2))];
		isInternalBuff = true;
	}

	LONG h = MIN(SliceHeight(slice1), SliceHeight(slice2));
	LONG wsize = MIN(SliceWidth(slice1), SliceWidth(slice2))*sizeof(RGBTRIPLE);

	for (LONG i(0);i < h;i++)//copy by rows
	{
		LONG rowSize = pRaw->width * 3;
		if (rowSize % 4) rowSize += 4 - (rowSize % 4);

		PBYTE p = reinterpret_cast<PBYTE>(pRaw->rawPixels) + (slice1.rowStart + i)*rowSize;
		PRGBTRIPLE pSlice1Base = reinterpret_cast<PRGBTRIPLE>(p) + slice1.colStart;

		p = reinterpret_cast<PBYTE>(pRaw->rawPixels) + (slice2.rowStart + i)*rowSize;
		PRGBTRIPLE pSlice2Base = reinterpret_cast<PRGBTRIPLE>(p) + slice2.colStart;


		memcpy_s(buff, h*wsize, pSlice2Base, wsize);
		memcpy_s(pSlice2Base, wsize, pSlice1Base, wsize);
		memcpy_s(pSlice1Base, wsize, buff, wsize);
	}
	if (isInternalBuff) delete buff;
}

LONG SliceHeight(PSLICE s)
{
	return s->rowEnd - s->rowStart + 1;
}

LONG SliceHeight(SLICE &s)
{
	return s.rowEnd - s.rowStart + 1;
}

LONG SliceWidth(PSLICE s)
{
	return s->colEnd - s->colStart + 1;
}

LONG SliceWidth(SLICE &s)
{
	return s.colEnd - s.colStart + 1;
}
