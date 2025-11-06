#pragma once

#if _WIN32
  #define WERCKM_EXPORT __declspec(dllexport)
#else
  #define WERCKM_EXPORT __attribute__((visibility("default")))
#endif

#define WmSession void*

#define WERCKM_OK 0
#define WERCKM_ERR -1

extern "C"  
{
  WERCKM_EXPORT const char * wm_getStrVersion();
  WERCKM_EXPORT WmSession wm_createSession();
  WERCKM_EXPORT int wm_compile(WmSession, const char * sourcePath);
  WERCKM_EXPORT bool wm_iscompiled(WmSession);
  WERCKM_EXPORT int wm_releaseSession(WmSession);
  WERCKM_EXPORT int wm_initSynth(WmSession);
  WERCKM_EXPORT int wm_copyMidiDataToSynth(WmSession);
  WERCKM_EXPORT int wm_synthRender(WmSession, int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr);
}