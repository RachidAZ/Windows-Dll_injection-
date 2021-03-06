// InjectorEXE.cpp : Defines the entry point for the console application.
//    

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>


typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsProcess32(HANDLE ProcessHandle) {

	BOOL bIsWow64 = FALSE;

		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

		if (NULL == fnIsWow64Process) return TRUE;   // 32-bit OS doenst have this funtion
		

		fnIsWow64Process(ProcessHandle, &bIsWow64);


	return bIsWow64;



}


DWORD getProccId(std::wstring ProcName) {

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	DWORD res = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{

			if (wcscmp (entry.szExeFile, L"OUTLOOK.EXE") == 0)
			{

				res= entry.th32ProcessID;
				return res;

				
			}
		}
	}

	CloseHandle(snapshot);

	return res;

}

int main()
{

	printf("Process injection starting ..\n");

	LPCSTR DllPath = "C:\\Users\\razgaou\\source\\repos\\Dll_injected\\Debug\\Dll_injected.dll";


	// Open a handle to target process

	DWORD ProcessID = getProccId(L"explorer.exe");

	if (ProcessID == 0) {

		printf("This process is not found ! \n");
		getchar();
		return 1;


	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);


	if (IsProcess32(hProcess) == FALSE) {

		printf("This process is not 32 ! \n");	
		getchar();
		return 1;

	}


	
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	
	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);


	if (hLoadThread == NULL) {
		printf("Error  in CreateRemoteThread..\n");
		printf("Return code error : %x", GetLastError());
		getchar();
		return 1;
	}


	// Wait for the execution of our loader thread to finish
	WaitForSingleObject(hLoadThread, INFINITE);

	std::cout << "Dll path allocated at: " << std::hex << pDllPath << std::endl;
	std::cin.get();

	// Free the memory allocated for our dll path
	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);



	getchar();

    return 0;
}


	

