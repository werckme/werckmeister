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
  WERCKM_EXPORT int wm_initSynth(WmSession, const char *libFluidPath, int sampleRate);
  WERCKM_EXPORT int wm_copyCompiledMidiDataToSynth(WmSession);
  WERCKM_EXPORT int wm_synthRender(WmSession, int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr);
  WERCKM_EXPORT int wm_writeToFile(WmSession, const char* outputPath);
  WERCKM_EXPORT int wm_setSoundFontHome(WmSession, const char *sfHomePath);
  WERCKM_EXPORT double wm_getSongPositionSeconds(WmSession);
  WERCKM_EXPORT int wm_getNumMidiCuePoints(WmSession);
  WERCKM_EXPORT const char * wm_getMidiCuePointText(WmSession, int index);
  WERCKM_EXPORT unsigned int wm_getMidiCuePositionMillis(WmSession, int index);
  WERCKM_EXPORT int wm_addMidiEvent(WmSession, double tickPos, const unsigned char* data, unsigned int length);
  WERCKM_EXPORT int wm_addMidiFileData(WmSession, const unsigned char* data, unsigned int length);
  WERCKM_EXPORT int wm_setCC(WmSession, int ch, int cc, int value);
}