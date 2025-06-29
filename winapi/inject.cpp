#include "pch.h"
#include <stdio.h>

// path to the dll to be injected
#define LIBRARY "C:\\Users\\User\\source\\repos\\winapi\\winapi\\mydll.dll"
// the PID of the injected process (notepad)
#define PID 34268 

int main()
{
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		PID
	);

	if (hProcess == NULL)
	{
		printf("error number %d in OpenProcess\n", GetLastError());
	}

	LPVOID dll_path = VirtualAllocEx(
		hProcess,
		NULL,
		strlen(LIBRARY),
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE

	);

	if (dll_path == NULL)
	{
		printf("error number %d in VirtualAllocEx\n", GetLastError());
	}

	BOOL write = WriteProcessMemory(
		hProcess,
		dll_path,
		LIBRARY,
		strlen(LIBRARY),
		NULL
	);
	if (not write)
	{
		printf("error number %d in WriteProcessMemory\n", GetLastError());
	}


	HMODULE LoadLibraryhModule = LoadLibraryA("Kernel32.dll");

	if (LoadLibraryhModule == NULL)
	{
		printf("error number %d in LoadLibraryA\n", GetLastError());
	}

	LPTHREAD_START_ROUTINE LoadLibraryAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(
		LoadLibraryhModule,
		"LoadLibraryA"
	);

	if (LoadLibraryAddress == NULL)
	{
		printf("error number %d in GetProcAddress\n", GetLastError());
	}

	DWORD threadId;
	HANDLE hThread = CreateRemoteThread(
		hProcess,
		NULL,
		0,
		LoadLibraryAddress,
		dll_path,
		0,
		&threadId
	);

	if (hThread == NULL)
	{
		printf("error number %d in CreateRemoteThread\n", GetLastError());
	}

	WaitForSingleObject(
		hThread,
		INFINITE
	);

	return 0;
}