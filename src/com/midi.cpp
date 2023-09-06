#include "midi.hpp"
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <com/exception.hpp>
#include <math.h>
#include <sstream>
#include <boost/functional/hash.hpp>

namespace 
{
	unsigned int MidiIdCounter = 0;
}

namespace com
{
	namespace midi
	{

		const MetaValue NoMetaValue = "not_a_meta_value";

		// pc
		// cc _bankMsb(0)  
		// cc _bankLsb(32) 
		bool isDeviceConfigEvent(const Event& ev)
		{
			return ev.eventType() == ProgramChange
				|| 
				(
					ev.eventType() == Controller
					&& (ev.parameter1() == 0 || ev.parameter1() == 32)
				)
			;
		}

		size_t variableLengthRequiredSize(MidiLong value)
		{
			if (value <= 0x7F)
			{
				return 1;
			}
			if (value <= 0x3fff)
			{
				return 2;
			}
			if (value <= 0x1fffff)
			{
				return 3;
			}
			if (value <= 0x0FFFFFFF)
			{
				return 4;
			}
			FM_THROW(com::Exception, "invalid tick length");
		}
		size_t variableLengthWrite(MidiLong value, Byte *outval, size_t size)
		{
			if (value > 0x0FFFFFFF)
			{
				return 0;
			}
			if (size == 0)
			{
				FM_THROW(com::Exception, "buffer too small");
			}
			if (value < 128)
			{
				outval[0] = static_cast<Byte>(value);
				return 1;
			}
			// according to http://midi.teragonaudio.com/tech/midifile/vari.htm
			MidiLong buffer = value & 0x7F;
			while ((value >>= 7))
			{
				buffer <<= 8;
				buffer |= ((value & 0x7F) | 0x80);
			}
			int c = 0;
			while (true)
			{
				if (c >= (int)size)
				{
					FM_THROW(com::Exception, "buffer too small");
				}
				outval[c++] = (Byte)buffer;
				if (buffer & 0x80)
				{
					buffer >>= 8;
				}
				else
				{
					break;
				}
			}
			return c;
		}
		MidiLong variableLengthRead(const Byte *inval, size_t maxSize, size_t *outReadBytes)
		{
			// http://midi.teragonaudio.com/tech/midifile/vari.htm
			MidiLong value;
			Byte c;
			int idx = 0;
			if ((value = inval[idx++]) & 0x80)
			{
				value &= 0x7F;
				do
				{
					if (idx >= 4)
					{
						FM_THROW(com::Exception, "invalid midi stream");
					}
					if (idx >= (int)maxSize)
					{
						FM_THROW(com::Exception, "buffer too small");
					}
					c = inval[idx++];
					value = (value << 7) + (c & 0x7F);
				} while (c & 0x80);
			}
			if (outReadBytes)
			{
				*outReadBytes = idx;
			}
			return value;
		}

		Ticks Event::relDelta(Ticks deltaOffset) const
		{
			return absPosition() - deltaOffset;
		}

		Channel Event::channel() const
		{
			return _ch % MaxChannel;
		}
		///////////////////////////////////////////////////////////////////////////
		// Event
		size_t Event::read(Ticks deltaOffset, const Byte *bytes, size_t maxByteSize)
		{
			if (maxByteSize < MinEventSize)
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			size_t c = 0;
			absPosition(variableLengthRead(bytes, maxByteSize, &c) + deltaOffset);
			return c + readPayload(bytes + c, maxByteSize - c);
		}
		size_t Event::readPayload(const Byte *bytes, size_t maxByteSize)
		{
			if (*bytes == midi::Sysex)
			{
				return readPayloadSysex(bytes + 1, maxByteSize - 1);
			}
			if (*bytes != MetaEvent)
			{
				return readPayloadDefault(bytes, maxByteSize);
			}
			eventType(MetaEvent);
			size_t eventTypeByteSize = 1;
			return readPayloadMeta(bytes + eventTypeByteSize, maxByteSize - eventTypeByteSize) + eventTypeByteSize;
		}
		size_t Event::readPayloadDefault(const Byte *bytes, size_t maxByteSize)
		{
			size_t c = 0;
			eventType(static_cast<EventType>((*(bytes)&0xF0) >> 4));
			channel((bytes[c++] & 0xF));
			parameter1(bytes[c++]);
			if (payloadSize() > 2)
			{
				parameter2(bytes[c++]);
			}
			return c;
		}
		size_t Event::readPayloadMeta(const Byte *bytes, size_t maxByteSize)
		{
			_metaEventType = static_cast<MetaEventType>(*(bytes++));
			size_t vlengthBytes = 0;
			size_t metaTypeByteSize = 1;
			_metaDataSize = variableLengthRead(bytes, maxByteSize - metaTypeByteSize, &vlengthBytes);
			bytes += vlengthBytes;

			if ((maxByteSize - vlengthBytes - metaTypeByteSize) < _metaDataSize)
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			if (_metaDataSize >= MaxVarLength)
			{
				FM_THROW(com::Exception, "meta data bytes overflow");
			}
			_metaData = Bytes(new Byte[_metaDataSize]);
			::memcpy(_metaData.get(), bytes, _metaDataSize);
			return metaTypeByteSize + vlengthBytes + _metaDataSize;
		}
		size_t Event::readPayloadSysex(const Byte* bytes, size_t maxByteSize)
		{
			eventType(midi::Sysex);
			const Byte* end = bytes + maxByteSize;
			size_t vlengthBytes = 0;
			//																	  - F7
			_metaDataSize = variableLengthRead(bytes, maxByteSize, &vlengthBytes) - 1;
			bytes += vlengthBytes;

			if ((maxByteSize - vlengthBytes) < _metaDataSize)
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			if (_metaDataSize >= MaxVarLength)
			{
				FM_THROW(com::Exception, "meta data bytes overflow");
			}
			_metaData = Bytes(new Byte[_metaDataSize]);
			::memcpy(_metaData.get(), bytes, _metaDataSize);
			//		F0 + varLength + dataBytes + F7
			return  1  + vlengthBytes + _metaDataSize + 1;
		}
		size_t Event::write(Ticks deltaOffset, Byte *bytes, size_t maxByteSize) const
		{
			size_t length = byteSize(deltaOffset);
			if (length > maxByteSize)
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			MidiLong relOff = (MidiLong)::nearbyint(relDelta(deltaOffset));
			size_t c = variableLengthWrite(relOff, bytes, maxByteSize);
			writePayload(&bytes[c], maxByteSize - c);
			return length;
		}
		size_t Event::writePayload(Byte *bytes, size_t maxByteSize, MidiEventTarget target) const
		{
			if (eventType() == MetaEvent)
			{
				return writePayloadMeta(bytes, maxByteSize);
			}
			if (eventType() == midi::Sysex)
			{
				return writePayloadSysex(bytes, maxByteSize, target);
			}
			return writePayloadDefault(bytes, maxByteSize);
		}
		size_t Event::writePayloadDefault(Byte *bytes, size_t maxByteSize) const
		{
			if (maxByteSize < 2)
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			(*bytes++) = (eventType() << 4) | channel();
			(*bytes++) = parameter1();
			if (payloadSize() > 2)
			{
				if (maxByteSize < 3)
				{
					FM_THROW(com::Exception, "buffer to small");
				}
				(*bytes++) = parameter2();
				return 3;
			}
			return 2;
		}

		size_t Event::writePayloadMeta(Byte *bytes, size_t maxByteSize) const
		{
			if (maxByteSize < payloadSize())
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			(*bytes++) = eventType();
			(*bytes++) = metaEventType();
			size_t vlengthBytes = variableLengthWrite(metaDataSize(), bytes, maxByteSize - 2);
			bytes += vlengthBytes;
			::memcpy(bytes, _metaData.get(), metaDataSize());
			return 3 + metaDataSize();
		}

		/*
		* https://www.cs.cmu.edu/~music/cmsip/readings/Standard-MIDI-file-format-updated.pdf
		* A normal complete system exclusive message is stored in a MIDI File in this way:
		* F0 <length> <bytes to be transmitted after F0. The length is stored as a variable-length quantity. 
		* It specifies the number of bytes which follow it, not including the F0 or the length itself.
		*/
		size_t Event::writePayloadSysex(Byte* bytes, size_t maxByteSize, MidiEventTarget target) const
		{
			if (maxByteSize < payloadSize(target))
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			(*bytes++) = 0xF0;
			size_t vlengthBytes = 0;
			if (target == MidiEventTargetFile)
			{
				vlengthBytes = variableLengthWrite(metaDataSize() + 1, bytes, maxByteSize - 1); // including F7
				bytes += vlengthBytes;
			}
			::memcpy(bytes, _metaData.get(), metaDataSize());
			bytes += metaDataSize();
			(*bytes) = 0xF7;
			//     f0  vlength       data
			return 1 + vlengthBytes + metaDataSize();
		}
		size_t Event::payloadSize(MidiEventTarget target) const
		{
			switch (eventType())
			{
			case ProgramChange:
				return 2;
			case ChannelAftertouch:
				return 2;
			case MetaEvent:
				return 2 + variableLengthRequiredSize(_metaDataSize) + _metaDataSize;
			case midi::Sysex: 
			{
				if (target == MidiEventTargetDevice)
				{
					return 1 + _metaDataSize + 1;
				}
				//     F0  vlength                                     data		   	  F7
				return 1 + variableLengthRequiredSize(_metaDataSize) + _metaDataSize + 1;
			}
			default:
				break;
			}
			return 3;
		}
		size_t Event::byteSize(Ticks deltaOffset) const
		{
			return variableLengthRequiredSize(relDelta(deltaOffset)) + payloadSize();
		}
		Event Event::CCPan(Channel channel, Byte val)
		{
			auto ev = com::midi::Event();
			ev.eventType(com::midi::Controller);
			ev.channel(channel);
			ev.parameter1(0xA);
			ev.parameter2(std::max(std::min(val, (Byte)MaxMidiValue), (Byte)0));
			return ev;
		}
		Event Event::CCVolume(Channel channel, Byte volume)
		{
			auto ev = com::midi::Event();
			ev.eventType(com::midi::Controller);
			ev.channel(channel);
			ev.parameter1(0xB);
			ev.parameter2(std::max(std::min(volume, (Byte)MaxMidiValue), (Byte)0));
			return ev;
		}
		Event Event::CCValue(Channel channel, Byte ccNr, Byte value)
		{
			auto ev = com::midi::Event();
			ev.eventType(com::midi::Controller);
			ev.channel(channel);
			ev.parameter1(ccNr);
			ev.parameter2(std::max(std::min(value, (Byte)MaxMidiValue), (Byte)0));
			return ev;
		}
		Event Event::NoteOn(Channel channel, Ticks absPos, Pitch pitch, Velocity velocity)
		{
			Event ev;
			ev.channel(channel);
			ev.absPosition(absPos);
			ev.parameter1(pitch);
			ev.parameter2(velocity);
			ev.eventType(midi::NoteOn);
			return ev;
		}
		Event Event::NoteOff(Channel channel, Ticks absPos, Pitch pitch)
		{
			Event ev;
			ev.channel(channel);
			ev.absPosition(absPos);
			ev.parameter1(pitch);
			ev.parameter2(0);
			ev.eventType(midi::NoteOff);
			return ev;
		}
		Event Event::PitchBend(Channel channel, Ticks absPos, double value)
		{
			int ivalue = static_cast<int>((value * (double)MaxPitchbend));
			Event ev;
			ev.channel(channel);
			ev.absPosition(absPos);
			ev.parameter2(static_cast<Byte>(ivalue >> 7));
			ev.parameter1(static_cast<Byte>(ivalue & 0x7f));
			ev.eventType(midi::PitchBend);
			return ev;
		}
		double Event::pitchBend() const
		{
			int ivalue = (parameter2() << 7) | parameter1();
			return ivalue / (double)MaxPitchbend;
		}
		Event Event::MetaTempo(double bpm)
		{
			bpm = std::max(bpm, 1.0);
			auto ev = Event();
			auto bytes = MetaCreateIntData(static_cast<int>(MicrosecondsPerMinute / bpm), 3);
			ev.metaData(Tempo, bytes.data(), bytes.size());
			return ev;
		}
		Event Event::MetaSignature(Byte nominator, Byte denominator, Byte clocksBetweenMetronomeClick, Byte nth32PerQuarter)
		{
			auto ev = Event();
			static const double log2 = 0.6931471805599453;
			denominator = static_cast<Byte>(log(denominator) / log2);
			std::vector<Byte> bytes = {nominator, denominator, clocksBetweenMetronomeClick, nth32PerQuarter};
			ev.metaData(TimeSignature, bytes.data(), bytes.size());
			return ev;
		}
		std::pair<Byte, Byte> Event::MetaGetSignatureValue(const Byte *data, size_t length)
		{
			if (length != 4)
			{
				throw std::runtime_error("invalid byte size for meta time signature");
			}
			static const double log2 = 0.6931471805599453;
			Byte nominator = data[0];
			Byte denominator = Byte(::nearbyint(exp(double(data[1] * log2))));
			return std::make_pair(nominator, denominator);
		}
		Event Event::MetaInstrument(const std::string &name)
		{
			auto ev = Event();
			auto bytes = MetaCreateStringData(name);
			ev.metaData(InstrumentName, bytes.data(), bytes.size());
			return ev;
		}
		Event Event::MetaTrack(const std::string &name)
		{
			auto ev = Event();
			auto bytes = MetaCreateStringData(name);
			ev.metaData(SequenceOrTrackName, bytes.data(), bytes.size());
			return ev;
		}
		Event Event::MetaCue(const std::string &name)
		{
			auto ev = Event();
			auto bytes = MetaCreateStringData(name);
			ev.metaData(CuePoint, bytes.data(), bytes.size());
			return ev;
		}
		void Event::metaData(MetaEventType type, Byte *data, size_t numBytes)
		{
			if (numBytes >= MaxVarLength)
			{
				FM_THROW(com::Exception, "meta data bytes overflow");
			}
			eventType(MetaEvent);
			_metaEventType = type;
			_metaDataSize = numBytes;
			_metaData = Bytes(new Byte[numBytes]);
			::memcpy(_metaData.get(), data, numBytes);
		}
		std::vector<Byte> Event::MetaCreateIntData(int value, size_t numBytes)
		{
			std::vector<Byte> result(numBytes);
			for (size_t i = 1; i <= numBytes; ++i)
			{
				auto byte = value & 0xFF;
				result[numBytes - i] = byte;
				value = value >> 8;
			}
			return result;
		}
		int Event::MetaGetIntValue(const Byte *data, size_t length)
		{
			if (length > sizeof(int))
			{
				FM_THROW(com::Exception, "to many bytes for meta data int value");
			}
			int result = 0;
			size_t idx = 0;
			while (true)
			{
				result |= data[idx++];
				if (idx >= length)
				{
					break;
				}
				result = result << 8;
			}
			return result;
		}
		std::vector<Byte> Event::MetaCreateStringData(const std::string &string)
		{
			return std::vector<Byte>(string.begin(), string.end());
		}
		std::string Event::MetaGetStringValue(const Byte *data, size_t length)
		{
			return std::string(data, data + length);
		}
		Event Event::MetaCustom(const CustomMetaData &custom)
		{
			if (custom.type == CustomMetaData::Undefined)
			{
				FM_THROW(com::Exception, "invalid meta custom data");
			}
			std::vector<Byte> bff(custom.data.size() + 1);
			bff[0] = static_cast<Byte>(custom.type);
			std::copy(custom.data.begin(), custom.data.end(), bff.begin() + 1);
			Event result;
			result.metaData(CustomMetaEvent, bff.data(), bff.size());
			return result;
		}
		CustomMetaData Event::MetaGetCustomData(const Byte *data, size_t length)
		{
			if (length == 0)
			{
				FM_THROW(com::Exception, "invalid meta custom data");
			}
			CustomMetaData result;
			result.type = static_cast<CustomMetaData::Type>(data[0]);
			if (length == 1)
			{
				return result;
			}
			result.data = CustomMetaData::Data(&data[1], &data[length]);
			return result;
		}
		Event Event::Sysex(const Byte* sysexData, size_t numBytes)
		{
			Event event;
			event.eventType(midi::Sysex);
			event._metaDataSize = numBytes;
			event._metaData = Bytes(new Byte[numBytes]);
			::memcpy(event._metaData.get(), sysexData, numBytes);
			return event;
		}
		bool Event::equals(const Event &b) const
		{
			bool res = absPosition() == b.absPosition() && eventType() == b.eventType() && channel() == b.channel() && parameter1() == b.parameter1() && parameter2() == b.parameter2();
			if (eventType() != MetaEvent)
			{
				return res;
			}
			if (metaEventType() != b.metaEventType())
			{
				return false;
			}
			if (metaDataSize() != b.metaDataSize())
			{
				return false;
			}
			return ::memcmp(metaData(), b.metaData(), metaDataSize()) == 0;
		}
		com::String Event::toString() const
		{
			std::stringstream ss;
			ss << "pos(" << (int)absPosition() << ") " << "ch(" << (int)channel() << ") p1(" << (int)parameter1() << ") p2(" << (int)parameter2() << ")"; 
			return ss.str();
		}

		///////////////////////////////////////////////////////////////////////////
		// EventCompare
		// events on the same position should be ordered as follows:
		// 1: cc _bankMsb(0)  
		// 2: cc _bankLsb(32) 
		// 3: program change 
		// 4: other cc
		// 5: then note-on / note-off

		bool EventCompare::operator()(const Event &a, const Event &b) const
		{
			auto pos1 = a.absPosition();
			auto pos2 = b.absPosition();
			if (pos1 != pos2)
			{
				return pos1 < pos2;
			}
			int t1 = a.eventType();
			int t2 = b.eventType();
			if (t1 == MetaEvent && t2 != MetaEvent)
			{ // could be custom meta: change device
				return true;
			}
			if (t2 == MetaEvent && t1 != MetaEvent)
			{ // could be custom meta: change device
				return false;
			}
			if (t1 == Controller && t2 == Controller)
			{
				if (a.parameter1() == 0) 
				{
					return true;
				}
				if (b.parameter1() == 0) 
				{
					return false;
				}
				if (a.parameter1() == 32) 
				{
					return true;
				}
				if (b.parameter1() == 32) 
				{
					return false;
				}
				return a.parameter1() < b.parameter1();
			}
			if (t1 == Controller && t2 == ProgramChange)
			{
				if (a.parameter1() == 0 || a.parameter1() == 32) 
				{
					return true;
				}
				return false;
			}
			if (t1 == ProgramChange && t2 == Controller)
			{
				if (b.parameter1() == 0 || b.parameter1() == 32) 
				{
					return false;
				}
				return true;
			}
			bool isNoteEvent1 = t1 == NoteOn || t1 == NoteOff;
			bool isNoteEvent2 = t2 == NoteOn || t2 == NoteOff;
			if (isNoteEvent1 && !isNoteEvent2)
			{
				return false; // note1 < other
			}
			if (isNoteEvent2 && !isNoteEvent1)
			{
				return true; // other > note2
			}
			return (int)t1 < (int)t2;
		}

		size_t EventHasher::operator()(const Event &ev) const
		{
			using boost::hash_value;
			using boost::hash_combine;
			size_t seed = 0;
			hash_combine(seed, hash_value(ev.absPosition()));
			hash_combine(seed, hash_value(ev.channel()));
			hash_combine(seed, hash_value(ev.eventType()));
			hash_combine(seed, hash_value(ev.parameter1()));
			hash_combine(seed, hash_value(ev.parameter2()));
			hash_combine(seed, hash_value(ev.payloadSize()));
			return seed;
		}

		///////////////////////////////////////////////////////////////////////////
		// EventContainer
		void EventContainer::sort()
		{
			std::sort(_container.begin(), _container.end(), EventCompare());
		}
		const MidiConfig *EventContainer::midiConfig() const
		{
			if (!this->_midiConfig)
			{
				FM_THROW(com::Exception, "midiconfig == null");
			}
			return this->_midiConfig;
		}
		void EventContainer::add(const Event &event)
		{
			if (midiConfig()->skipMetaEvents && event.eventType() == MetaEvent)
			{
				return;
			}
			_container.push_back(event);
		}
		void EventContainer::remove(const Event &event)
		{
			ConstIterator it = _container.begin();
			ConstIterator end = _container.end();
			for (; it != end; ++it)
			{
				if (*it == event)
				{
					_container.erase(it);
					break;
				}
			}
		}
		EventContainer::ConstIterator EventContainer::begin() const
		{
			return _container.begin();
		}
		EventContainer::ConstIterator EventContainer::end() const
		{
			return _container.end();
		}
		size_t EventContainer::read(const Byte *bff, size_t byteSize)
		{
			Ticks offset = 0;
			size_t startSize = byteSize;
			while (true)
			{
				Event ev;
				auto numBytes = ev.read(offset, bff, byteSize);
				offset = ev.absPosition();
				_container.push_back(ev);
				bff += numBytes;
				byteSize -= numBytes;
				if (((int)byteSize - (int)MinEventSize) <= 0)
				{
					break;
				}
			}
			return startSize - byteSize;
		}
		size_t EventContainer::write(Byte *bff, size_t maxByteSize, com::Ticks *outWrittenDuration) const
		{
			if (maxByteSize < byteSize())
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			size_t written = 0;
			Ticks offset = 0;
			for (const auto &ev : _container)
			{
				auto numBytes = ev.write(offset, bff, maxByteSize - written);
				offset = ::nearbyint(ev.absPosition());
				written += numBytes;
				bff += numBytes;
			}
			if (outWrittenDuration)
			{
				*outWrittenDuration = offset;
			}
			return written;
		}
		void EventContainer::addNote(Channel channel, Ticks absPosition, Pitch pitch, Velocity velocity, Ticks length)
		{
			add(Event::NoteOn(channel, absPosition, pitch, velocity));
			add(Event::NoteOff(channel, absPosition + length, pitch));
		}
		size_t EventContainer::byteSize() const
		{
			size_t result = 0;
			Ticks offset = 0;
			for (const auto &ev : _container)
			{
				result += ev.byteSize(offset);
				offset = ev.absPosition();
			}
			return result;
		}
		///////////////////////////////////////////////////////////////////////////
		// Track

		const MetaKey Track::MetaKeyRelatedVertexId = "related_vertexid";
		const MetaKey Track::MetaKeyRelatedSheetId = "related_sheetid";
		const MetaKey Track::MetaKeyMidiChannel = "midi_channel";

		size_t Track::read(const Byte *, size_t length)
		{
			FM_THROW(com::Exception, "not yet implemented");
		}
		size_t Track::write(Byte *bff, size_t maxByteSize)
		{
			size_t wrote = 0;
			if (maxByteSize < byteSize())
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			Header header;
			header.chunkSize = static_cast<DWord>(events().byteSize() + EoTSize);
			if (isLittleEndian())
			{
				endswap(&header.chunkSize);
			}
			::memcpy(bff, &header, HeaderSize);
			bff += HeaderSize;
			wrote += HeaderSize;
			com::Ticks writtenDuration = 0;
			auto eventBytesWritten = events().write(bff, maxByteSize - wrote, &writtenDuration);
			wrote += eventBytesWritten;
			bff += eventBytesWritten;
			auto eot = Event();
			eot.metaData(EndOfTrack, nullptr, 0);
			eot.absPosition(writtenDuration);
			wrote += eot.write(writtenDuration, bff, maxByteSize - wrote);
			return wrote;
		}
		void Track::setMetaData(const MetaKey &key, const MetaValue &val)
		{
			_metaDataContainer[key] = val;
		}

		MetaValue Track::getMetaData(const MetaKey &key) const
		{
			auto it = _metaDataContainer.find(key);
			if (it == _metaDataContainer.end())
			{
				return NoMetaValue;
			}
			return it->second;
		}
		///////////////////////////////////////////////////////////////////////////
		// MIDI
		Event::Event() : id(MidiIdCounter++)
		{

		}
		Midi::Midi(Ticks ppq)
		{
			_ppq = ppq;
		}
		size_t Midi::read(const Byte *, size_t length)
		{
			FM_THROW(com::Exception, "not yet implemented");
		}
		size_t Midi::write(Byte *bff, size_t maxByteSize) const
		{
			if (!_sealed)
			{
				throw std::runtime_error("seal midi file before writing");
			}
			size_t wrote = 0;
			if (maxByteSize < byteSize())
			{
				FM_THROW(com::Exception, "buffer to small");
			}
			Header header;
			header.timeDivision = static_cast<Word>(0x7FFF) & static_cast<Word>(_ppq);
			header.numberOfTracks = static_cast<Word>(tracks().size());
			if (isLittleEndian())
			{
				endswap(&header.chunkSize);
				endswap(&header.formatType);
				endswap(&header.numberOfTracks);
				endswap(&header.timeDivision);
			}
			::memcpy(bff, &header, HeaderSize);
			bff += HeaderSize;
			wrote += HeaderSize;
			for (const auto &track : _container)
			{
				size_t bytes = track->write(bff, maxByteSize - wrote);
				bff += bytes;
				wrote += bytes;
			}
			return wrote;
		}
		size_t Midi::byteSize() const
		{
			if (!_sealed)
			{
				throw std::runtime_error("seal midi file before obtaining byte size");
			}
			size_t result = HeaderSize;
			for (const auto &track : _container)
			{
				result += track->byteSize();
			}
			return result;
		}
		void Midi::addTrack(TrackPtr track)
		{
			if (_sealed)
			{
				throw std::runtime_error("midi file is sealed");
			}
			track->events().midiConfig(&this->midiConfig);
			_container.push_back(track);
		}

		Midi::TrackContainer &Midi::tracks()
		{
			if (_sealed)
			{
				throw std::runtime_error("midi file is sealed");
			}
			return _container;
		}

		TrackPtr Midi::createTrack() const
		{
			auto result = std::make_shared<Track>();
			return result;
		}
		void Midi::write(const char *filename)
		{
			std::fstream stream(filename, std::ios::out | std::ios::trunc | std::ios::binary);
			write(stream);

			stream.close();
		}
		void Midi::write(std::ostream &os)
		{
			size_t size = byteSize();
			Byte *bff = new Byte[size];
			write(bff, size);
			os.write(reinterpret_cast<char *>(bff), size);
			os.flush();
			delete[] bff;
		}

		Ticks Midi::duration() const
		{
			Ticks duration = 0;
			for (auto track : tracks())
			{
				auto &events = track->events();
				if (events.container().empty())
				{
					continue;
				}
				auto last = --(events.end());
				duration = std::max(duration, last->absPosition());
			}
			return duration;
		}

		void Midi::clear()
		{
			bpm_ = com::DefaultTempo;
			_container.clear();
		}

		void Midi::seal()
		{
			for (auto track : tracks())
			{
				track->events().sort();
			}
			_sealed = true;
		}

		void Midi::crop(com::Ticks begin, com::Ticks end)
		{
			for (auto &track : _container)
			{
				auto &originalContainer = track->events().container();
				com::midi::EventContainer::TContainer copy;
				copy.reserve(originalContainer.size());
				auto it = originalContainer.begin();
				auto itEnd = originalContainer.end();
				for (; it != itEnd; ++it)
				{
					auto midiEvent = *it;
					com::Ticks midiEventPosition = midiEvent.absPosition();
					auto eventType = midiEvent.eventType();
					bool isConfigEvent = midiEventPosition == 0 && (eventType == com::midi::ProgramChange 
						|| eventType == com::midi::MetaEvent
						|| eventType == com::midi::Sysex);
					bool isInRange = midiEventPosition >= begin && midiEventPosition < end;
					if(!isInRange && !isConfigEvent) 
					{
						continue;
					}
					midiEvent.absPosition(midiEvent.absPosition() - begin);
					copy.push_back(midiEvent);
					if (eventType == com::midi::NoteOn) 
					{
						auto correspondingNoteOff = 
							std::find_if(it, itEnd, [midiEvent](auto &ev) 
							{ 
								if (ev.eventType() != com::midi::NoteOff) 
								{
									return false;
								}
								if (ev.parameter1() != midiEvent.parameter1()) 
								{
									return false;
								}
								return true;
							 });
						if (correspondingNoteOff != itEnd) 
						{
							auto noteOffCopy = *correspondingNoteOff;
							noteOffCopy.absPosition(noteOffCopy.absPosition() - begin);
							copy.push_back(noteOffCopy);
						}
					}
				}
				originalContainer.swap(copy);
			}
		}
	}
}
