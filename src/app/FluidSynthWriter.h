
#pragma once

#include "FluidSynthWrapper.h"
#include <unordered_map>
#include <com/ILogger.h>

namespace app
{
    class FluidSynthWriter : public FluidSynth
    {
    public:
        typedef int SoundFontId;
        typedef FluidSynth Base;
        FluidSynthWriter(com::ILoggerPtr logger) : _logger(logger) {}
        virtual ~FluidSynthWriter() = default;
        virtual void initSynth(const std::string &soundFondPath) override;
        SoundFontId addSoundFont(const std::string &soundFondPath);
        bool addEvent(const com::midi::Event& event);
        void render(int len, float* lout, int loff, int lincr, float* rout, int roff, int rincr);
        void libPath(const com::String &path) { _libPath = path; }
        com::String libPath() const { return _libPath; }
        void sampleRate(const double &sampleRate) { _sampleRate = sampleRate; }
        double sampleRate() const { return _sampleRate; }
        virtual void tearDownSynth() override;
        double tempo() const { return _tempo; }
        double getSongPositionSeconds() const;
        void renderToFile(const std::string &outputPath, double seconds);
    protected:
        com::ILoggerPtr _logger;
        com::String _libPath;
        double _sampleRate = 44100.0f;
        virtual std::string findFluidSynthLibraryPath() const override;
        void handleMetaEvent(const com::midi::Event& event);
        double _tempo = 120.0;
        typedef std::unordered_map<com::String, SoundFontId> SoundFontIdMap;
        SoundFontIdMap soundFontIds;
    };
    typedef std::shared_ptr<FluidSynthWriter> FluidSynthWriterPtr;
}