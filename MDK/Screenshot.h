#pragma once

#include <Windows.h>
#include <GDIPlus.h>
#include <fstream>

#pragma comment(lib, "Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

class Screenshot
{
	public:
		Screenshot();
		~Screenshot();
		string getPath();
		void startScreenshot();

		static DWORD WINAPI screenshotThread(PVOID instance);
		HANDLE startThread();
};

