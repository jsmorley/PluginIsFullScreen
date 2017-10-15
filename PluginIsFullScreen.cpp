#include <Windows.h>
#include "../../API/RainmeterAPI.h"

RECT appBounds;
RECT screenBounds;
HWND foregroundHandle;

struct Measure
{
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

	foregroundHandle = GetForegroundWindow();

	GetWindowRect(GetDesktopWindow(), &screenBounds);

	if (foregroundHandle != GetDesktopWindow() && foregroundHandle != GetShellWindow())
	{
		GetWindowRect(foregroundHandle, &appBounds);
		if (EqualRect(&appBounds, &screenBounds))
		{
			return 1.0;
		}
	}

	return 0.0;
}

//PLUGIN_EXPORT LPCWSTR GetString(void* data)
//{
//	Measure* measure = (Measure*)data;
//	return L"";
//}

//PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
//{
//	Measure* measure = (Measure*)data;
//}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	delete measure;
}
