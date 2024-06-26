#pragma once

// Define EXPORTS symbol to export functions from DLL on Windows
#ifdef _WIN32
#ifdef ENGINE_DLL_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#endif
