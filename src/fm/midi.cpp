#include "midi.hpp"
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <algorithm>

namespace fm {
	namespace midi {

		const MetaValue NoMetaValue = "not_a_meta_value";

		size_t variableLengthRequiredSize(Ticks value)
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
			throw std::runtime_error("invalid tick length");
		}
		size_t variableLengthWrite(Ticks value, Byte *outval, size_t size)
		{
			if (value > 0x0FFFFFFF)
			{
				return 0;
			}
			if (value < 128) {
				outval[0] = static_cast<Byte>(value);
				return 1;
			}
			// according to http://midi.teragonaudio.com/tech/midifile/vari.htm
			Ticks buffer = value & 0x7F;
			while ((value >>= 7))
			{
				buffer <<= 8;
				buffer |= ((value & 0x7F) | 0x80);
			}
			int c = 0;
			while (true) {
				if (c >= (int)size) {
					throw std::runtime_error("buffer too small");
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
		Ticks variableLengthRead(const Byte *inval, size_t maxSize, size_t *outReadBytes)
		{
			// http://midi.teragonaudio.com/tech/midifile/vari.htm
			Ticks value;
			Byte c;
			int idx = 0;
			if ((value = inval[idx++]) & 0x80) {
				value &= 0x7F;
				do {
					if (idx >= 4) {
						throw std::runtime_error("invalid midi stream");
					}
					if (idx >= (int)maxSize) {
						throw std::runtime_error("buffer too small");
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
				throw std::runtime_error("buffer to small");
			}
			size_t c = 0;
			absPosition(variableLengthRead(bytes, maxByteSize, &c) + deltaOffset);
			eventType(static_cast<EventType>((bytes[c] & 0xF0) >> 4));
			channel((bytes[c++] & 0xF));
			parameter1(bytes[c++]);
			if (payloadSize() > 2) {
				parameter2(bytes[c++]);
			}
			return c;
		}
		size_t Event::write(Ticks deltaOffset, Byte *bytes, size_t maxByteSize) const
		{
			size_t length = byteSize(deltaOffset);
			if (length > maxByteSize) {
				throw std::runtime_error("buffer to small");
			}
			auto relOff = relDelta(deltaOffset);
			size_t c = variableLengthWrite(relOff, bytes, maxByteSize);
			writePayload(&bytes[c], maxByteSize - c);
			return length;
		}
		size_t Event::writePayload(Byte *bytes, size_t maxByteSize) const
		{
			if (maxByteSize < 2) {
				throw std::runtime_error("buffer to small");
			}
			(*bytes++) = (eventType() << 4) | channel();
			(*bytes++) = parameter1();
			if (payloadSize() > 2) {
				if (maxByteSize < 3) {
					throw std::runtime_error("buffer to small");
				}
				(*bytes++) = parameter2();
				return 3;
			}
			return 2;
		}
		size_t Event::payloadSize() const
		{
			switch (eventType())
			{
			case ProgramChange: return 2;
			case ChannelAftertouch: return 2;
			default:break;
			}
			return 3;
		}
		size_t Event::byteSize(Ticks deltaOffset) const
		{
			return variableLengthRequiredSize(relDelta(deltaOffset)) + payloadSize();
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
		bool Event::equals(const Event&b) const
		{
			return absPosition() == b.absPosition()
				&& eventType() == b.eventType()
				&& channel() == b.channel()
				&& parameter1() == b.parameter1()
				&& parameter2() == b.parameter2();
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
		void EventContainer::add(const Event &event)
		{
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
		size_t EventContainer::write(Byte *bff, size_t maxByteSize) const
		{
			if (maxByteSize < byteSize()) {
				throw std::runtime_error("buffer to small");
			}
			size_t written = 0;
			Ticks offset = 0;
			for (const auto& ev : _container) {
				auto numBytes = ev.write(offset, bff, maxByteSize - written);
				offset = ev.absPosition();
				written += numBytes;
				bff += numBytes;
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
			throw std::runtime_error("not yet implemented");
		}
		size_t Track::write(Byte *bff, size_t maxByteSize) const
		{
			size_t wrote = 0;
			if (maxByteSize < byteSize()) {
				throw std::runtime_error("buffer to small");
			}
			Header header;
			header.chunkSize = static_cast<DWord>(events().byteSize());
			if (isLittleEndian()) {
				endswap(&header.chunkSize);
			}
			::memcpy(bff, &header, HeaderSize);
			bff += HeaderSize;
			wrote += HeaderSize;
			wrote += events().write(bff, maxByteSize - wrote);
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
			throw std::runtime_error("not yet implemented");
		}
		size_t Midi::write(Byte *bff, size_t maxByteSize) const
		{
			size_t wrote = 0;
			if (maxByteSize < byteSize()) {
				throw std::runtime_error("buffer to small");
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
			_container.push_back(track);
		}
		TrackPtr Midi::createTrack() const
		{
			return std::make_shared<Track>();
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
	}
}
