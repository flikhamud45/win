#include "pch.h"
#define DLL_EXPORT
#include <stdio.h>

#include "epipendll.h"

#define FUNCTION "my_epic_function"

typedef void (*PFUNC) (void);

extern "C"
{
	DECLDIR void run()
	{
		
		PFUNC func = (PFUNC)GetProcAddress(
			GetModuleHandleA(NULL), // the current exe
			FUNCTION
		);
		if (func == NULL)
		{
			printf("error number %d in GetProcAddress\n", GetLastError());
		}
		func();

	}
	
}

BOOL APIENTRY DllMain(HANDLE hModule, // Handle to DLL module
	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// A process is loading the DLL.
		break;
	case DLL_THREAD_ATTACH:
		run();
		// A process is creating a new thread.
		break;
	case DLL_THREAD_DETACH:
		// A thread exits normally.
		break;
	case DLL_PROCESS_DETACH:
		// A process unloads the DLL.
		break;
	}
	return TRUE;
}
