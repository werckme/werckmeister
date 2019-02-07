#include "Event.h"
#include <exception>
#include <fm/common.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <locale>
#include <fm/exception.hpp>

namespace sheet {

	namespace {
		const std::unordered_map<wchar_t, int> _name2pitch = {
			 { FM_CHAR('c'), fm::notes::C }
			,{ FM_CHAR('d'), fm::notes::D }
			,{ FM_CHAR('e'), fm::notes::E }
			,{ FM_CHAR('f'), fm::notes::F }
			,{ FM_CHAR('g'), fm::notes::G }
			,{ FM_CHAR('a'), fm::notes::A }
			,{ FM_CHAR('b'), fm::notes::B }
		};
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
		if (nameLower.length() == 0) {
			FM_THROW(fm::Exception, "empty chord");
		}
		boost::algorithm::to_lower(nameLower);
		fm::String::const_iterator it = nameLower.begin();
		auto pitchIt = _name2pitch.find(*it);
		if (pitchIt == _name2pitch.end()) {
			FM_THROW(fm::Exception, "ivalid chord: " + fm::to_string(stringValue));
		}
		pitch = pitchIt->second;
		++it;
		// check for is or es
		if (nameLower.length() >= 2) {
			if (pitch == fm::notes::E && *(it) == FM_CHAR('s')) {
				pitch -= 1;
				it += 1;
			}
			else if (pitch == fm::notes::A && *(it) == FM_CHAR('s')) {
				pitch -= 1;
				it += 1;
			}
			else if (*(it) == FM_CHAR('i') && *(it + 1) == FM_CHAR('s')) {
				pitch += 1;
				it += 2;
			}
			else if (*(it) == FM_CHAR('e') && *(it + 1) == FM_CHAR('s')) {
				pitch -= 1;
				it += 2;
			}
		}
		auto idxOptionsStart = it - nameLower.begin();
		return std::make_tuple(pitch, Options(stringValue.begin() + idxOptionsStart,  stringValue.end()));
	}

	fm::String Event::chordDefName() const 
	{
		std::locale loc;
		auto elements = chordElements();
		fm::String::const_iterator it = stringValue.begin();
		if (std::isupper(*it, loc)) {
			return FM_STRING("X") + std::get<1>(elements);
		}
		else {
			FM_THROW(fm::Exception, "lowercase chords are not allowed: " + fm::to_string(stringValue));
		}
	}
}