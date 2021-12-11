#include "testhelper.h"

#include <com/literals.hpp>
#include <com/units.hpp>
#include "com/common.hpp"

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
    const com::String &alias,
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

bool checkChord(const sheet::Event &ev, com::String chordName)
{
    return ev.type == sheet::Event::Chord && ev.stringValue == chordName;
}

bool checkMetaEvent(const sheet::Event &ev, const com::String &command, const sheet::Event::Args &args)
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
        if (it1->name != it2->name || it1->value != it2->value) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

sheet::Argument makeArg(com::String value) {
    sheet::Argument argument;
    argument.value = value;
    argument.name = "";
    return argument;
}
sheet::Argument makeArg(com::String name, com::String value) {
    auto argument = makeArg(value);
    argument.name = name;
    return argument;
}	