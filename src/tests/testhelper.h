#pragma once

#include <documentModel/objects/Event.h>
#include <documentModel/Argument.h>

bool checkNote(const documentModel::Event &ev,
               documentModel::Event::Type type,
               documentModel::PitchDef::Pitch pitch = documentModel::PitchDef::NoPitch,
               documentModel::PitchDef::Octave octave = documentModel::PitchDef::DefaultOctave,
               documentModel::Event::Duration duration = documentModel::Event::NoDuration);

bool checkNote(const documentModel::Event &ev,
               documentModel::Event::Type type,
               const com::String &alias,
               documentModel::Event::Duration duration = documentModel::Event::NoDuration);

bool checkNote(const documentModel::Event &ev,
               documentModel::Event::Type type,
               const documentModel::Event::Pitches &pitches,
               documentModel::Event::Duration duration = documentModel::Event::NoDuration);

bool checkChord(const documentModel::Event &ev, com::String chordName);

bool checkMetaEvent(const documentModel::Event &ev, const com::String &command, const documentModel::Event::Args &args);

documentModel::Argument makeArg(com::String value);
documentModel::Argument makeArg(com::String name, com::String value);
