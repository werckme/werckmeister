#pragma once

#if _WIN32
  #define WM_EXPORT __declspec(dllexport)
#else
  #define WM_EXPORT
#endif

extern "C" WM_EXPORT void hello();