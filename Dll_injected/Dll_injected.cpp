// Dll_injected.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{

	if (fdwReason == DLL_PROCESS_ATTACH) {

		MessageBox(NULL, L"Hi from dll", L"rachid", MB_ICONINFORMATION);


	}

	return TRUE;


}




