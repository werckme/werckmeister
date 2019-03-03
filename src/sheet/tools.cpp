#include "tools.h"
#include "Event.h"

namespace sheet {

    namespace toolsimpl {

        const std::vector<fm::String> & getMetaArgs(const Event &metaEvent)
        {
            return metaEvent.metaArgs;
        }
        const fm::String & getMetaCommand(const Event &metaEvent)
        {
            return metaEvent.stringValue;
        }
    }

    fm::String pitchToString(fm::Pitch pitch)
    {
        switch (pitch) {
			case fm::notes::C   :  return FM_STRING("C");
			case fm::notes::CIS :  return FM_STRING("C#");
			case fm::notes::D   :  return FM_STRING("D");
			case fm::notes::DIS :  return FM_STRING("D#");
			case fm::notes::E   :  return FM_STRING("E");
			case fm::notes::F   :  return FM_STRING("F");
			case fm::notes::FIS :  return FM_STRING("F#");
			case fm::notes::G   :  return FM_STRING("G");
			case fm::notes::GIS :  return FM_STRING("G#");
			case fm::notes::A   :  return FM_STRING("A");
			case fm::notes::AIS :  return FM_STRING("A#");
			case fm::notes::B   :  return FM_STRING("B");
            default : return FM_STRING("?"); 
        }
    }
}