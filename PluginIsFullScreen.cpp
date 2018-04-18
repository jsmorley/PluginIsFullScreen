#include <Windows.h>
#include <Psapi.h>
#include <string>
#include "../../API/RainmeterAPI.h"

struct Measure
{
	std::wstring name;
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
	measure->name.clear();

	double found = 0.0;
	RECT appBounds = { 0 };
	RECT screenBounds = { 0 };
	HWND foregroundHandle = GetForegroundWindow();

	GetWindowRect(GetDesktopWindow(), &screenBounds);

	if (foregroundHandle != GetDesktopWindow() && foregroundHandle != GetShellWindow())
	{
		GetWindowRect(foregroundHandle, &appBounds);
		if (EqualRect(&appBounds, &screenBounds))
		{
			DWORD procId;
			GetWindowThreadProcessId(foregroundHandle, &procId);

			HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procId);
			if (process)
			{
				WCHAR name[MAX_PATH];
				if (0 != GetModuleBaseName(process, NULL, name, MAX_PATH))
				{
					measure->name = name;
					found = 1.0;
				}

				CloseHandle(process);
			}
		}
	}

	return found;
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
	Measure* measure = (Measure*)data;
	return measure->name.c_str();
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
