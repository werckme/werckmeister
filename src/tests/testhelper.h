
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
    const com::String &alias,
    sheet::Event::Duration duration = sheet::Event::NoDuration);


bool checkNote(const sheet::Event &ev,
    sheet::Event::Type type,
    const sheet::Event::Pitches &pitches,
    sheet::Event::Duration duration = sheet::Event::NoDuration);

bool checkChord(const sheet::Event &ev, com::String chordName);

bool checkMetaEvent(const sheet::Event &ev, const com::String &command, const sheet::Event::Args &args);

sheet::Argument makeArg(com::String value);
sheet::Argument makeArg(com::String name, com::String value);


#endif