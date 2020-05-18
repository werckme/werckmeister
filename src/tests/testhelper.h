
#ifndef SHEET_TEST_HELPER_H
#define SHEET_TEST_HELPER_H

#include <sheet/objects/Event.h>
#include <sheet/Argument.h>

bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    sheet::PitchDef::Pitch pitch = sheet::PitchDef::NoPitch,
    sheet::PitchDef::Octave octave = sheet::PitchDef::DefaultOctave,
    sheet::Event::Duration duration = sheet::Event::NoDuration);

bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    const fm::String &alias,
    sheet::Event::Duration duration = sheet::Event::NoDuration);


bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    const sheet::Event::Pitches &pitches,
    sheet::Event::Duration duration = sheet::Event::NoDuration);

bool checkChord(const sheet::Event &ev, fm::String chordName);

bool checkMetaEvent(const sheet::Event &ev, const fm::String &command, const sheet::Event::Args &args);

sheet::Argument makeArg(fm::String value);
sheet::Argument makeArg(fm::String name, fm::String value);


#endif