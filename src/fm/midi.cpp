#include "midi.hpp"
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <fm/exception.hpp>
#include <math.h>

namespace fm {
	namespace midi {

		const MetaValue NoMetaValue = "not_a_meta_value";

		size_t variableLengthRequiredSize(MidiLong value)
		{
			if (value <= 0x7F) {
				return 1;
			}
			if (value <= 0x3fff) {
				return 2;
			}
			if (value <= 0x1fffff) {
				return 3;
			}
			if (value <= 0x0FFFFFFF) {
				return 4;
			}
			FM_THROW(fm::Exception, "invalid tick length");
		}
		size_t variableLengthWrite(MidiLong value, Byte *outval, size_t size)
		{
			if (value > 0x0FFFFFFF)
			{
				return 0;
			}
			if (size == 0) {
				FM_THROW(fm::Exception, "buffer too small");
			}
			if (value < 128) {
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
			while (true) {
				if (c >= (int)size) {
					FM_THROW(fm::Exception, "buffer too small");
				}
				outval[c++] = (Byte)buffer;
				if (buffer & 0x80) {
					buffer >>= 8;
				}
				else {
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
			if ((value = inval[idx++]) & 0x80) {
				value &= 0x7F;
				do {
					if (idx >= 4) {
						FM_THROW(fm::Exception, "invalid midi stream");
					}
					if (idx >= (int)maxSize) {
						FM_THROW(fm::Exception, "buffer too small");
					}
					c = inval[idx++];
					value = (value << 7) + (c & 0x7F);
				} while (c & 0x80);
			}
			if (outReadBytes) {
				*outReadBytes = idx;
			}
			return value;
		}

		Ticks Event::relDelta(Ticks deltaOffset) const
		{
			return absPosition() - deltaOffset;
		}

		Channel Event::channel() const {
			return _ch % MaxChannel;
		}
		///////////////////////////////////////////////////////////////////////////
		// Event
		size_t Event::read(Ticks deltaOffset, const Byte *bytes, size_t maxByteSize)
		{
			if (maxByteSize < MinEventSize) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			size_t c = 0;
			absPosition(variableLengthRead(bytes, maxByteSize, &c) + deltaOffset);
			return c + readPayload(bytes + c, maxByteSize - c);
		}
		size_t Event::readPayload(const Byte *bytes, size_t maxByteSize)
		{
			
			if (*bytes != MetaEvent) {
				return readPayloadDefault(bytes, maxByteSize);
			}
			eventType(MetaEvent);
			size_t eventTypeByteSize = 1;
			return readPayloadMeta(bytes + eventTypeByteSize, maxByteSize - eventTypeByteSize) + eventTypeByteSize;
		}
		size_t Event::readPayloadDefault(const Byte *bytes, size_t maxByteSize)
		{
			size_t c = 0;
			eventType(static_cast<EventType>((*(bytes) & 0xF0) >> 4));
			channel((bytes[c++] & 0xF));
			parameter1(bytes[c++]);
			if (payloadSize() > 2) {
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
			bytes+=vlengthBytes;

			if ((maxByteSize-vlengthBytes-metaTypeByteSize) < _metaDataSize) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			if (_metaDataSize >= MaxVarLength) {
				FM_THROW(fm::Exception, "meta data bytes overflow");
			}
			_metaData = Bytes(new Byte[_metaDataSize]);
			::memcpy(_metaData.get(), bytes, _metaDataSize);
			return metaTypeByteSize + vlengthBytes + _metaDataSize;
		}
		size_t Event::write(Ticks deltaOffset, Byte *bytes, size_t maxByteSize) const
		{
			size_t length = byteSize(deltaOffset);
			if (length > maxByteSize) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			auto relOff = relDelta(deltaOffset);
			size_t c = variableLengthWrite(relOff, bytes, maxByteSize);
			writePayload(&bytes[c], maxByteSize - c);
			return length;
		}
		size_t Event::writePayload(Byte *bytes, size_t maxByteSize) const
		{
			if (eventType()==MetaEvent) {
				return writePayloadMeta(bytes, maxByteSize);
			}
			return writePayloadDefault(bytes, maxByteSize);
		}
		size_t Event::writePayloadDefault(Byte *bytes, size_t maxByteSize) const
		{
			if (maxByteSize < 2) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			(*bytes++) = (eventType() << 4) | channel();
			(*bytes++) = parameter1();
			if (payloadSize() > 2) {
				if (maxByteSize < 3) {
					FM_THROW(fm::Exception, "buffer to small");
				}
				(*bytes++) = parameter2();
				return 3;
			}
			return 2;
		}

		size_t Event::writePayloadMeta(Byte *bytes, size_t maxByteSize) const
		{
			if (maxByteSize < payloadSize()) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			(*bytes++) = eventType();
			(*bytes++) = metaEventType();
			size_t vlengthBytes = variableLengthWrite(metaDataSize(), bytes, maxByteSize - 2);
			bytes += vlengthBytes;
			::memcpy(bytes, _metaData.get(), metaDataSize());
			return 3 + metaDataSize();
		}
		size_t Event::payloadSize() const
		{
			switch (eventType())
			{
			case ProgramChange: return 2;
			case ChannelAftertouch: return 2;
			case MetaEvent: return 2 + variableLengthRequiredSize(_metaDataSize) + _metaDataSize;
			default:break;
			}
			return 3;
		}
		size_t Event::byteSize(Ticks deltaOffset) const
		{
			return variableLengthRequiredSize(relDelta(deltaOffset)) + payloadSize();
		}
		Event Event::CCPan(Channel channel, Byte val)
		{
			auto ev = fm::midi::Event();
			ev.eventType(fm::midi::Controller);
			ev.channel(channel);
			ev.parameter1(0xA);
			ev.parameter2(std::max(std::min(val, (Byte)MaxMidiValue), (Byte)0));
			return ev;
		}		
		Event Event::CCVolume(Channel channel, Byte volume)
		{
			auto ev = fm::midi::Event();
			ev.eventType(fm::midi::Controller);
			ev.channel(channel);
			ev.parameter1(0xB);
			ev.parameter2(std::max(std::min(volume, (Byte)MaxMidiValue), (Byte)0));
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
			int ivalue = static_cast<int>( (value * (double)MaxPitchbend) );
			Event ev;
			ev.channel(channel);
			ev.absPosition(absPos);
			ev.parameter2( static_cast<Byte>(ivalue >> 7) );
			ev.parameter1( static_cast<Byte>(ivalue & 0x7f) );
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
			denominator = static_cast<Byte>( log(denominator) / log2);
			std::vector<Byte> bytes = {nominator, denominator, clocksBetweenMetronomeClick, nth32PerQuarter };
			ev.metaData(TimeSignature, bytes.data(), bytes.size());
			return ev;
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
		void Event::metaData(MetaEventType type, Byte *data, size_t numBytes)
		{
			if (numBytes >= MaxVarLength) {
				FM_THROW(fm::Exception, "meta data bytes overflow");
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
			for (size_t i=1; i<=numBytes; ++i) {
				auto byte = value & 0xFF;
				result[numBytes-i] = byte;
				value = value >> 8;
			}
			return result;
		}
		int Event::MetaGetIntValue(const Byte *data, size_t length)
		{
			if (length > sizeof(int)) {
				FM_THROW(fm::Exception, "to many bytes for meta data int value");
			}
			int result = 0;
			size_t idx = 0;
			while(true) {
				result |= data[idx++];
				if (idx >= length) {
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
			if (custom.type == CustomMetaData::Undefined) {
				FM_THROW(fm::Exception, "invalid meta custom data");
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
			if (length == 0) {
				FM_THROW(fm::Exception, "invalid meta custom data");
			}
			CustomMetaData result;
			result.type = static_cast<CustomMetaData::Type>(data[0]);
			if (length == 1) {
				return result;
			}
			result.data = CustomMetaData::Data(&data[1], &data[length]);
			return result;
		}
		bool Event::equals(const Event&b) const
		{
			bool res = absPosition() == b.absPosition()
				&& eventType() == b.eventType()
				&& channel() == b.channel()
				&& parameter1() == b.parameter1()
				&& parameter2() == b.parameter2();
			if (eventType() != MetaEvent) {
				return res;
			}
			if (metaEventType() != b.metaEventType()) {
				return false;
			}
			if (metaDataSize() != b.metaDataSize()) {
				return false;
			}
			return ::memcmp(metaData(), b.metaData(), metaDataSize()) == 0;
		}


		///////////////////////////////////////////////////////////////////////////
		// EventCompare
		// events should be ordered by following order:
		// (1)control changes, (2)program change then note-on / note-off 

		bool EventCompare::operator() (const Event& a, const Event& b) const
		{
			auto pos1 = a.absPosition();
			auto pos2 = b.absPosition();
			if (pos1 != pos2) {
				return pos1 < pos2;
			}
			int t1 = a.eventType();
			int t2 = b.eventType();
			if (t1==MetaEvent && t2!=MetaEvent ) { // could be custom meta: change device
				return true;
			}
			if (t2==MetaEvent && t1!=MetaEvent) { // could be custom meta: change device
				return false;
			}
			bool isNoteEvent1 = t1 == NoteOn || t1 == NoteOff;
			bool isNoteEvent2 = t2 == NoteOn || t2 == NoteOff;
			if (isNoteEvent1) {
				return false; // note1 < other
			}
			if (isNoteEvent2) {
				return true; // other > note2
			}
			return (int)t1<(int)t2;
		}


		///////////////////////////////////////////////////////////////////////////
		// EventContainer
		const MidiConfig * EventContainer::midiConfig() const
		{
			if (!this->_midiConfig) {
				FM_THROW(fm::Exception, "midiconfig == null");
			}
			return this->_midiConfig;
		}
		void EventContainer::add(const Event &event)
		{
			if (midiConfig()->skipMetaEvents && event.eventType() == MetaEvent) {
				return;
			}
			if (contains(event)) {
				return;
			}
			_container.insert(event);
		}
		void EventContainer::remove(const Event &event)
		{
			ConstIterator it = _container.begin();
			ConstIterator end = _container.end();
			for (; it != end; ++it) {
				if (*it == event) {
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
		EventContainer::ConstIterator EventContainer::from(Ticks absTicks) const
		{
			return _container.lower_bound(Event::NoteOff(0, absTicks, 0));
		}
		EventContainer::ConstIterator EventContainer::to(Ticks absTicks) const
		{
			return _container.upper_bound(Event::NoteOff(0, absTicks, 0));
		}
		size_t EventContainer::read(const Byte *bff, size_t byteSize)
		{
			Ticks offset = 0;
			size_t startSize = byteSize;
			while (true) {
				Event ev;
				auto numBytes = ev.read(offset, bff, byteSize);
				offset = ev.absPosition();
				_container.insert(ev);
				bff += numBytes;
				byteSize -= numBytes;
				if (((int)byteSize - (int)MinEventSize) <= 0) {
					break;
				}
			}
			return startSize - byteSize;
		}
		size_t EventContainer::write(Byte *bff, size_t maxByteSize, fm::Ticks *outWrittenDuration) const
		{
			if (maxByteSize < byteSize()) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			size_t written = 0;
			Ticks offset = 0;
			for (const auto& ev : _container) {
				auto numBytes = ev.write(offset, bff, maxByteSize - written);
				offset = ev.absPosition();
				written += numBytes;
				bff += numBytes;
			}
			if (outWrittenDuration) {
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
			for (const auto& ev : _container) {
				result += ev.byteSize(offset);
				offset = ev.absPosition();
			}
			return result;
		}
		bool EventContainer::contains(const Event &event) const
		{
			TContainer::const_iterator s, e;
			std::tie(s, e) = _container.equal_range(event);
			return std::any_of(s, e, [event](const Event &x) { return x.equals(event); });
		}
		///////////////////////////////////////////////////////////////////////////
		// Track

		const MetaKey Track::MetaKeyRelatedVertexId = "related_vertexid";
		const MetaKey Track::MetaKeyRelatedSheetId = "related_sheetid";
		const MetaKey Track::MetaKeyMidiChannel = "midi_channel";

		size_t Track::read(const Byte *, size_t length)
		{
			FM_THROW(fm::Exception, "not yet implemented");
		}
		size_t Track::write(Byte *bff, size_t maxByteSize) const
		{
			size_t wrote = 0;
			if (maxByteSize < byteSize()) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			Header header;
			header.chunkSize = static_cast<DWord>(events().byteSize() + EoTSize);
			if (isLittleEndian()) {
				endswap(&header.chunkSize);
			}
			::memcpy(bff, &header, HeaderSize);
			bff += HeaderSize;
			wrote += HeaderSize;
			fm::Ticks writtenDuration = 0;
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
			if (it == _metaDataContainer.end()) {
				return NoMetaValue;
			}
			return it->second;
		}
		///////////////////////////////////////////////////////////////////////////
		// MIDI
		Midi::Midi(Ticks ppq) {
			_ppq = ppq;
		}
		size_t Midi::read(const Byte *, size_t length)
		{
			FM_THROW(fm::Exception, "not yet implemented");
		}
		size_t Midi::write(Byte *bff, size_t maxByteSize) const
		{
			size_t wrote = 0;
			if (maxByteSize < byteSize()) {
				FM_THROW(fm::Exception, "buffer to small");
			}
			Header header;
			header.timeDivision = static_cast<Word>(0x7FFF) & static_cast<Word>(_ppq);
			header.numberOfTracks = static_cast<Word>(tracks().size());
			if (isLittleEndian()) {
				endswap(&header.chunkSize);
				endswap(&header.formatType);
				endswap(&header.numberOfTracks);
				endswap(&header.timeDivision);
			}
			::memcpy(bff, &header, HeaderSize);
			bff += HeaderSize;
			wrote += HeaderSize;
			for (const auto &track : _container) {
				size_t bytes = track->write(bff, maxByteSize - wrote);
				bff += bytes;
				wrote += bytes;
			}
			return wrote;
		}
		size_t Midi::byteSize() const
		{
			size_t result = HeaderSize;
			for (const auto &track : _container) {
				result += track->byteSize();
			}
			return result;
		}
		void Midi::addTrack(TrackPtr track)
		{
			track->events().midiConfig(&this->midiConfig);
			_container.push_back(track);
		}
		TrackPtr Midi::createTrack() const
		{
			auto result = std::make_shared<Track>();
			return result;
		}
		void Midi::write(const char* filename) const
		{
			std::fstream stream(filename, std::ios::out | std::ios::trunc | std::ios::binary);
			write(stream);

			stream.close();
		}
		void Midi::write(std::ostream &os) const
		{
			size_t size = byteSize();
			Byte *bff = new Byte[size];
			write(bff, size);
			os.write(reinterpret_cast<char*>(bff), size);
			os.flush();
			delete[] bff;
		}

		Ticks Midi::duration() const
		{
			Ticks duration = 0;
			for (auto track : tracks()) {
				auto &events = track->events();
				if (events.container().size() == 0) {
					continue;
				}
				auto last = --(events.end());
				duration = std::max(duration, last->absPosition());
			}
			return duration;
		}

		void Midi::clear() {
			bpm_ = fm::DefaultTempo;
			_container.clear();
		}
	}
}
