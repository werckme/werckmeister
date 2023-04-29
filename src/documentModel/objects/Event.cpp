#include "Event.h"
#include <exception>
#include <com/common.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <locale>
#include <com/exception.hpp>

namespace documentModel
{

	namespace
	{
		const std::unordered_map<wchar_t, int> _name2pitch = {
			{FM_CHAR('c'), com::notes::C}, {FM_CHAR('d'), com::notes::D}, {FM_CHAR('e'), com::notes::E}, {FM_CHAR('f'), com::notes::F}, {FM_CHAR('g'), com::notes::G}, {FM_CHAR('a'), com::notes::A}, {FM_CHAR('b'), com::notes::B}};
	}

	/*
		failed to get a proper parser to working which can parse
		a chordname and the options in one step. The problem was that it is needed to
		have the two parser elements in one lexeme. But this seems to consume only one property
		of a struct. May be there is a solution with sematic actions etc. But in sake of moving forward,
		I decided to parse on the fly.
	*/
	Event::ChordElements Event::chordElements() const
	{
		PitchDef::Pitch pitch = 0;
		auto nameLower = stringValue;
		if (nameLower.length() == 0)
		{
			FM_THROW(com::Exception, "empty chord");
		}
		boost::algorithm::to_lower(nameLower);
		com::String::const_iterator it = nameLower.begin();
		auto pitchIt = _name2pitch.find(*it);
		if (pitchIt == _name2pitch.end())
		{
			FM_THROW(com::Exception, "ivalid chord: " + stringValue);
		}
		pitch = pitchIt->second;
		++it;
		// check for sharp or flat
		if (nameLower.length() >= 2)
		{
			if (*(it) == FM_CHAR('#'))
			{
				pitch += 1;
				it += 1;
			}
			else if (*(it) == FM_CHAR('b'))
			{
				pitch -= 1;
				it += 1;
			}
		}
		auto idxOptionsStart = it - nameLower.begin();
		return std::make_tuple(pitch, Options(stringValue.begin() + idxOptionsStart, stringValue.end()));
	}

	com::String Event::phraseName() const 
	{
		if (type != Phrase) 
		{
			FM_THROW(com::Exception, "expecting phrase type");
		}
		if (tags.size() < 1) 
		{
			FM_THROW(com::Exception, "expecting phrase name");
		}
		return *tags.begin();
	}

	com::String Event::chordDefName() const
	{
		std::locale loc;
		auto elements = chordElements();
		com::String::const_iterator it = stringValue.begin();
		if (std::isupper(*it, loc))
		{
			return FM_STRING("X") + std::get<1>(elements);
		}
		else
		{
			FM_THROW(com::Exception, "lowercase chords are not allowed: " + stringValue);
		}
	}

	com::String Event::toString() const
	{
		com::StringStream ss;
		switch (type)
		{
		case Rest:
			ss << "Rest";
			break;
		case Degree:
			ss << "Degree";
			break;
		case TiedDegree:
			ss << "TiedDegree";
			break;
		case Note:
			ss << "Note";
			break;
		case TiedNote:
			ss << "TiedNote";
			break;
		case Chord:
			ss << "Chord";
			break;
		case EOB:
			ss << "EOB";
			break;
		case Meta:
			ss << "Meta";
			break;
		case Expression:
			ss << "Expression";
			break;
		case Unknown:
		default:
			ss << "Unknown";
			break;
		}
		ss << "(" << duration << ")";
		return ss.str();
	}

	void Event::isTied(bool val)
	{
		if (type != Note && type != Degree && type != TiedNote && type != TiedDegree)
		{
			throw std::runtime_error("set isTied property failed: event is not a note nor a degree type");
		}
		if (val)
		{
			type = isAbsoluteNote() ? TiedNote : TiedDegree;
		}
		else
		{
			type = isAbsoluteNote() ? Note : Degree;
		}
	}
}