#pragma once

#if _WIN32
  #define WM_EXPORT __declspec(dllexport)
#else
  #define WM_EXPORT __attribute__((visibility("default")))
#endif

#define WmSession void*

extern "C"  
{
  WM_EXPORT const char * wm_getStrVersion();
  WM_EXPORT WmSession wm_createSession(const char * sourcePath);
  WM_EXPORT int wm_releaseSession(WmSession);
}