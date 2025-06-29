#include "pch.h"
#include <stdio.h>

// path to the dll to be injected
#define LIBRARY "C:\\Users\\User\\source\\repos\\winapi\\winapi\\mydll.dll"
// the PID of the injected process (notepad)
#define PID 37096 

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
		exit(1);
	}

	LPVOID dll_path = VirtualAllocEx(
		hProcess,
		NULL,
		strlen(LIBRARY) + 1,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE

	);

	if (dll_path == NULL)
	{
		printf("error number %d in VirtualAllocEx\n", GetLastError());
		CloseHandle(hProcess);
		exit(1);
	}

	BOOL write = WriteProcessMemory(
		hProcess,
		dll_path,
		LIBRARY,
		strlen(LIBRARY) + 1,
		NULL
	);
	if (not write)
	{
		printf("error number %d in WriteProcessMemory\n", GetLastError());
		CloseHandle(hProcess);
		VirtualFree(dll_path, strlen(LIBRARY) + 1, MEM_DECOMMIT);
		VirtualFree(dll_path, 0, MEM_RELEASE);
		exit(1);
	}


	
	LPTHREAD_START_ROUTINE LoadLibraryAddress = (LPTHREAD_START_ROUTINE)&LoadLibraryA;

	
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
		CloseHandle(hProcess);
		VirtualFree(dll_path, strlen(LIBRARY) + 1, MEM_DECOMMIT);
		VirtualFree(dll_path, 0, MEM_RELEASE);
		exit(1);
	}

	WaitForSingleObject(
		hThread,
		INFINITE
	);

	CloseHandle(hProcess);
	VirtualFree(dll_path, strlen(LIBRARY) + 1, MEM_DECOMMIT);
	VirtualFree(dll_path, 0, MEM_RELEASE);
	CloseHandle(hThread);

	return 0;
}