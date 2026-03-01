
#pragma once

#include "FluidSynthWrapper.h"
#include <unordered_map>
#include <com/ILogger.h>
#include <map>
#include <vector>
#include <functional>
#include "midiProvider.h"
#include "lua/FluidWriterPerformer.h"

namespace app
{
    class FluidSynthWriter : public FluidSynth
    {
    public:
        typedef FluidSynth Base;
        typedef int TickPosition;
        typedef std::function<void(const com::midi::Event*)> VisitEventFunction;
        FluidSynthWriter(com::ILoggerPtr logger) : Base(false), _logger(logger) {}
        virtual ~FluidSynthWriter() = default;
        virtual void initSynth() override;
        virtual SoundFontId addSoundFont(const DeviceId& deviceId, const std::string &soundFontPath) override;
        bool addEvent(const com::midi::Event& event);
        void render(int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr);
        void libPath(const com::String &path) { _libPath = path; }
        com::String libPath() const { return _libPath; }
        void sampleRate(const double &sampleRate) { _sampleRate = sampleRate; }
        double sampleRate() const { return _sampleRate; }
        virtual void tearDownSynth() override;
        double tempo() const { return _tempo; }
        double getSongPositionSeconds() const;
        void setSongPositionSeconds(double songPosSeconds);
        void renderToFile(const std::string &outputPath, double seconds);
        void setMidiFileData(const unsigned char* data, size_t length, VisitEventFunction visitEventFunction = nullptr);
        void onTickEventCallback(int tick);
        void onPlaybackCallback(fluid_midi_event_t *event);
        void setPerformerScriptPath(const com::String &path);
        void stop();
        void play();
    protected:
        void sendNow(const com::midi::Event &ev, fluid_event_t* target = nullptr);
        void initScriptIfReady();
        lua::FluidWriterPerformerPtr performerScript;
        MidiProvider midiProvider;
        bool handlePresetEvent(const com::midi::Event& event, bool sendToFluidSynth = true);
        void parseMidiData(const unsigned char* data, size_t length, VisitEventFunction visitEventFunction);
        com::ILoggerPtr _logger;
        com::String _libPath;
        com::String _scriptPath;
        double _sampleRate = 44100.0f;
        virtual std::string findFluidSynthLibraryPath() const override;
        virtual void handleMetaEvent(const com::midi::Event& event);
        SoundFontId lastSoundFontId = -1;
        double _tempo = 120.0;
        fluid_player_t*  player = nullptr;
        int _sfIdPerChannel[16] = {0};
        int getSfId(int channel);
    };
    typedef std::shared_ptr<FluidSynthWriter> FluidSynthWriterPtr;
}