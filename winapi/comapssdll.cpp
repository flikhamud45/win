#include "pch.h"
#define DLL_EXPORT
#include <stdio.h>

#include "compassdll.h"

#define FUNCTION_DIFFERENCE 17893

typedef void (*PFUNC) (void);

typedef PIMAGE_NT_HEADERS(WINAPI* IMAGE_NT_HEADER_FUNC)(PVOID);


extern "C"
{
	DECLDIR void run()
	{
		printf("injected!\n");

		HMODULE hModule = GetModuleHandle(NULL); 
		if (!hModule) {
			printf("error number %d in GetModuleHandle.\n", GetLastError());
			return;
		}

		HMODULE hImageHlp = LoadLibraryA("ImageHlp.dll");
		if (!hImageHlp) {
			printf("error number %d in LoadLibraryA.\n", GetLastError());
			return;
		}

		IMAGE_NT_HEADER_FUNC ImageNtHeader = (IMAGE_NT_HEADER_FUNC)GetProcAddress(hImageHlp, "ImageNtHeader");

		PIMAGE_NT_HEADERS ntHeaders = ImageNtHeader(hModule);

		
		DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;

		void* entryPointAddr = (BYTE*)hModule + entryPointRVA;

		
		PFUNC func = (PFUNC)((char*)entryPointAddr + FUNCTION_DIFFERENCE);
		
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
		run();
		break;
	case DLL_THREAD_ATTACH:
		
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
