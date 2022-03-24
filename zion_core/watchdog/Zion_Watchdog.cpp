// Spd_Client_Watchdog.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <climits>
#include <memory>
#include <string>
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <wtsapi32.h>
#include <tlhelp32.h> 
#include <Shlwapi.h>
#include <string>
#include <psapi.h>

#pragma comment(lib,"wtsapi32.lib")

static const int32_t MAX_PROCESS_LIST_COUNT = 1024;

int32_t find_number_of_process(const char* name)
{
	int32_t numberOfProcess = 0;
	DWORD	processes[MAX_PROCESS_LIST_COUNT];
	DWORD	rbytes;

	if (!EnumProcesses(processes, sizeof(processes), &rbytes))
		return -1;

	int32_t count = rbytes / sizeof(DWORD);
	for (int32_t i = 0; i < count; ++i)
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_ALL_ACCESS, FALSE, processes[i]);

		if (hProcess)
		{
			char szImagePath[MAX_PATH] = { 0 };
			if (GetProcessImageFileNameA(hProcess, szImagePath, sizeof(szImagePath)))
			{
				std::string proc_path = szImagePath;
				std::string proc_name = proc_path.substr(proc_path.find_last_of("/\\") + 1);

				if (proc_name.compare(name) == 0)
					numberOfProcess++;
			}
			CloseHandle(hProcess);
		}
	}
	return numberOfProcess;
}

BOOL fork(const char* executable, const char* path, const char* arguments, unsigned long* pid)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	memset(&si, 0x00, sizeof(si));
	memset(&pi, 0x00, sizeof(pi));
	si.cb = sizeof(STARTUPINFO);

	char module_path[MAX_PATH] = { 0 };
	char* module_name = module_path;
	module_name += GetModuleFileNameA(NULL, module_name, (sizeof(module_path) / sizeof(*module_path)) - (module_name - module_path));
	if (module_name != module_path)
	{
		CHAR* slash = strrchr(module_path, '\\');
		if (slash != NULL)
		{
			module_name = slash + 1;
			_strset_s(module_name, strlen(module_name) + 1, 0);
		}
		else
		{
			_strset_s(module_path, strlen(module_path) + 1, 0);
		}
	}

	char real_executable[MAX_PATH] = { 0 };
	char real_path[MAX_PATH] = { 0 };
	_snprintf_s(real_executable, sizeof(real_executable), sizeof(real_executable), "%s%s", module_path, executable);
	_snprintf_s(real_path, sizeof(real_path), sizeof(real_path), "%s%s", module_path, path);

	BOOL result = FALSE;
	std::string command = real_executable;
	if (arguments && strlen(arguments) > 0)
	{
		command += " ";
		command += arguments;
	}

	//result = ::CreateProcessA(NULL, (LPSTR)(LPCSTR)command.c_str(), NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP, NULL, real_path, &si, &pi);
	result = ::CreateProcessA(NULL, (LPSTR)(LPCSTR)command.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (!result)
	{
		DWORD err = GetLastError();
	}
	*pid = pi.dwProcessId;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return result ? TRUE : FALSE;
}

void kill(const TCHAR * filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (_tcscmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

int main()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE; //Remove quick edit mode
	mode &= ~ENABLE_INSERT_MODE; //Remove insert mode
	mode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, mode);

	while (1)
	{
		std::string process_name = "CMd.exe";
		int32_t numberOfProcess = find_number_of_process(process_name.c_str());
		if (numberOfProcess < 1)
		{
			printf("process is dead, create new one..\n");
			unsigned long pid = 0;
			fork(process_name.c_str(), NULL, NULL, &pid);
		}
		Sleep(10);
	}
	return 0;
}