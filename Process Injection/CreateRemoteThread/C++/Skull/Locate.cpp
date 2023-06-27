

#include <windows.h>
#include <psapi.h>
#include <stdio.h>

int Locate(const char* processName) {
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