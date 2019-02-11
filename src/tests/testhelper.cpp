#include "testhelper.h"

#include <fm/literals.hpp>
#include <fm/units.hpp>
#include "fm/common.hpp"

bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    sheet::PitchDef::Pitch pitch,
    sheet::PitchDef::Octave octave,
    sheet::Event::Duration duration)
{
    if (ev.pitches.empty() && pitch != sheet::PitchDef::NoPitch) {
        return false;
    }
    if (pitch != sheet::PitchDef::NoPitch) {
        auto first = ev.pitches.begin();
        if (first->pitch != pitch || first->octave != octave) {
            return false;
        }
    }
    return ev.type == type && ev.duration == duration;
}

bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    const fm::String &alias,
    sheet::Event::Duration duration)
{
    if (ev.pitches.empty()) {
        return false;
    }
    
    auto first = ev.pitches.begin();
    if (first->alias != alias) {
        return false;
    }
    return ev.type == type && ev.duration == duration;
}



bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    const sheet::Event::Pitches &pitches,
    sheet::Event::Duration duration)
{
    bool pre = ev.type == type && ev.duration == duration;
    if (!pre) {
        return false;
    }
    if (ev.pitches.size() != pitches.size()) {
        return false;
    }
    auto it1 = ev.pitches.begin();
    auto it2 = pitches.begin();
    while (it1 != ev.pitches.end())
    {
        if (it1->pitch != it2->pitch || it1->octave != it2->octave) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

bool checkChord(const sheet::Event &ev, fm::String chordName)
{
    return ev.type == sheet::Event::Chord && ev.stringValue == chordName;
}

bool checkMetaEvent(const sheet::Event &ev, const fm::String &command, const sheet::Event::Args &args)
{
    bool pre = ev.type == sheet::Event::Meta && ev.stringValue == command;
    if (!pre) {
        return false;
    }
    if (ev.metaArgs.size() != args.size()) {
        return false;
    }
    auto it1 = ev.metaArgs.begin();
    auto it2 = args.begin();
    while (it1 != ev.metaArgs.end())
    {
        if (*it1 != *it2) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}
