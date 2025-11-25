#pragma once






#ifdef WIDGET_LIBRARY_DLL_EXPORT
#define WIDGET_LIBRARY_DLL_API __declspec(dllexport)
#else
#define WIDGET_LIBRARY_DLL_API __declspec(dllimport)
#endif
