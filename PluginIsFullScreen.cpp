#include <Windows.h>
#include <Psapi.h>
#include <string>
#include "../../API/RainmeterAPI.h"

struct Measure
{
	std::wstring processName;
	Measure() {}
};

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;
	measure->processName.clear();

	double foundFullScreen = 0.0;
	RECT appBounds = { 0 };
	RECT screenBounds = { 0 };
	HWND foregroundHandle = GetForegroundWindow();

	GetWindowRect(GetDesktopWindow(), &screenBounds);

	if (foregroundHandle != GetDesktopWindow() && foregroundHandle != GetShellWindow())
	{
		GetWindowRect(foregroundHandle, &appBounds);
		if (EqualRect(&appBounds, &screenBounds))
		{
			DWORD processPID;
			GetWindowThreadProcessId(foregroundHandle, &processPID);

			HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processPID);
			if (processHandle)
			{
				WCHAR processName[MAX_PATH];
				if (0 != GetModuleBaseName(processHandle, NULL, processName, MAX_PATH))
				{
					measure->processName = processName;
					foundFullScreen = 1.0;
				}

				CloseHandle(processHandle);
			}
		}
	}

	return foundFullScreen;
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
	Measure* measure = (Measure*)data;
	return measure->processName.c_str();
}

//PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
//{
//   Measure* measure = (Measure*)data;
//}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	delete measure;
}
