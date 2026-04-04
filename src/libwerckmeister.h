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
  WERCKM_EXPORT int wm_releaseSession(WmSession);
  WERCKM_EXPORT int wm_initSynth(WmSession, const char *libFluidPath, int sampleRate, int numChannelGroups);
  WERCKM_EXPORT int wm_synthRender(WmSession, int len, int numOut, float *out[]);
  WERCKM_EXPORT int wm_setSoundFontHome(WmSession, const char *sfHomePath);
  /**
    LevelDebug = 0,
    LevelBabble,
    LevelWarn,
    LevelError,
    LevelNone,
  */
  WERCKM_EXPORT int wm_setLogLevel(WmSession, int logLevel);
  WERCKM_EXPORT double wm_getSongPositionSeconds(WmSession);
  WERCKM_EXPORT double wm_setSongPositionSeconds(WmSession, double songPosSeconds);
  WERCKM_EXPORT int wm_getNumMidiCuePoints(WmSession);
  WERCKM_EXPORT const char * wm_getMidiCuePointText(WmSession, int index);
  WERCKM_EXPORT unsigned int wm_getMidiCuePositionMillis(WmSession, int index);
  WERCKM_EXPORT int wm_addMidiFileData(WmSession, const unsigned char* data, unsigned int length);
  WERCKM_EXPORT int wm_setPerformerScriptPath(WmSession, const char* path, unsigned int length);
  WERCKM_EXPORT int wm_setCC(WmSession, int ch, int cc, int value);
  WERCKM_EXPORT int wm_sendCustomController(WmSession, int controllerNumber, int value);  
  WERCKM_EXPORT int wm_play(WmSession);
  WERCKM_EXPORT int wm_stop(WmSession);
}