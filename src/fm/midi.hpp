#ifndef FM_MIDI_HPP
#define FM_MIDI_HPP

#include <cstddef>
#include "units.hpp"
#include <set>
#include <vector>
#include <unordered_map>
#include <boost/cstdint.hpp>
#include <ostream>
#include "common.hpp"
#include <memory>

namespace fm {
	namespace midi {
		class Track;
		typedef std::shared_ptr<Track> TrackPtr;
		class Midi;
		typedef std::shared_ptr<Midi> MidiPtr;
		typedef std::string MetaKey;
		typedef std::string MetaValue;
		extern const MetaValue NoMetaValue;
		typedef std::unordered_map<MetaKey, MetaValue> MetaDataContainer;
		size_t variableLengthRequiredSize(Ticks value);
		size_t variableLengthWrite(Ticks value, Byte *outval, size_t maxSize);
		Ticks variableLengthRead(const Byte *inval, size_t maxSize, size_t *outReadBytes = nullptr);
		typedef Byte Channel;
		typedef Byte Pitch;
		typedef Byte Velocity;
		typedef boost::uint32_t DWord;
		static_assert(sizeof(DWord) == 4, "wrong DWORD size");
		typedef boost::uint16_t Word;
		static_assert(sizeof(Word) == 2, "wrong WORD size");

		enum {
			MaxTickValue = 0x0FFFFFFF,
			MaxChannel = 0xF,
			MinEventSize = 3,
			MaxEventSize = 7
		};
		enum EventType {
			UndefinedEvent = 0,
			NoteOff = 0x8,
			NoteOn = 0x9,
			NoteAftertouch = 0xA,
			Controller = 0xB,
			ProgramChange = 0xC,
			ChannelAftertouch = 0xD,
			PitchBend = 0xE,
			MetaEvent = 0xFF
		};
		enum MetaEventType {
			UndefinedMetaEvent = 0,
			EndOfTrack = 0x2F,
		};
		struct Event {
			Ticks relDelta(Ticks deltaOffset) const;
			void absPosition(Ticks ticks) { _deltaTime = ticks; }
			Ticks absPosition() const { return _deltaTime; }
			void eventType(EventType eventType) { _type = eventType; }
			EventType eventType() const { return _type; }
			void channel(Channel channel) { _ch = channel; }
			Channel channel() const;
			void parameter1(Byte val) { _p1 = val; }
			Byte parameter1() const { return _p1; }
			void parameter2(Byte val) { _p2 = val; }
			Byte parameter2() const { return _p2; }
			size_t read(Ticks deltaOffset, const Byte *, size_t maxByteSize);
			size_t write(Ticks deltaOffset, Byte *, size_t maxByteSize) const;
			size_t byteSize(Ticks deltaOffset) const;
			/**
				@return the event size excluding offset
			*/
			size_t payloadSize() const;
			/**
				writes the event excluding offset
			*/
			size_t writePayload(Byte *, size_t maxByteSize) const;
			static Event NoteOn(Channel, Ticks, Pitch, Velocity);
			static Event NoteOff(Channel, Ticks, Pitch);
			bool equals(const Event&) const;
			bool operator==(const Event &b) const { return equals(b); }
			bool operator!=(const Event &b) const { return !(*this == b); }
			mutable bool __muted__ = false;
		private:
			Ticks _deltaTime = 0;
			EventType _type = UndefinedEvent;
			int _ch = 0;
			Byte _p1 = 0;
			Byte _p2 = 0;
		};
		struct EventCompare {
			bool operator() (const Event& a, const Event& b) const;
		};
		class EventContainer {
		public:
			typedef std::multiset<Event, EventCompare> TContainer;
			typedef TContainer::iterator Iterator;
			typedef TContainer::const_iterator ConstIterator;
			void add(const Event &event);
			void remove(const Event &event);
			void addNote(Channel, Ticks absPosition, Pitch, Velocity, Ticks length);
			ConstIterator begin() const;
			ConstIterator end() const;
			ConstIterator from(Ticks absTicks) const;
			ConstIterator to(Ticks absTicks) const;
			size_t read(const Byte *, size_t length);
			size_t write(Byte *, size_t maxByteSize) const;
			size_t byteSize() const;
			size_t numEvents() const { return _container.size(); }
			bool contains(const Event &event) const;
			const TContainer & container() const { return _container; }
			TContainer & container() { return _container; }
		private:
			TContainer _container;
		};

		class Track {
		public:
			struct Header {
				char chunkID[4] = { 'M', 'T', 'r', 'k' };
				DWord chunkSize = 0;
			};
			static const MetaKey MetaKeyRelatedVertexId;
			static const MetaKey MetaKeyRelatedSheetId;
			static const MetaKey MetaKeyMidiChannel;
			enum { HeaderSize = 8 };
			const EventContainer & events() const { return _container; }
			EventContainer & events() { return _container; }
			size_t read(const Byte *, size_t length);
			size_t write(Byte *, size_t maxByteSize) const;
			size_t byteSize() const { return _container.byteSize() + sizeof(Header); }
			/**
				values will not be written to midi file
			*/
			void setMetaData(const MetaKey &key, const MetaValue &val);
			MetaValue getMetaData(const MetaKey &key) const;
		private:
			MetaDataContainer _metaDataContainer;
			EventContainer _container;
		};

		class Midi {
		public:
			typedef std::vector<TrackPtr> TrackContainer;
			typedef TrackContainer::iterator TrackIterator;
			typedef TrackContainer::const_iterator ConstTrackIterator;
			struct Header {
				char chunkID[4] = { 'M', 'T', 'h', 'd' };
				int chunkSize = 6;
				Word formatType = 1;
				Word numberOfTracks = 0;
				Word timeDivision = 0;
			};
			enum { HeaderSize = 14 };
			Midi(Ticks ppq = 480);
			size_t read(const Byte *, size_t length);
			size_t write(Byte *, size_t maxByteSize) const;
			size_t byteSize() const;
			void addTrack(TrackPtr track);
			TrackPtr createTrack() const;
			const TrackContainer & tracks() const { return _container; }
			TrackContainer & tracks() { return _container; }
			void write(const char* filename) const;
			void write(std::ostream&) const;
			Ticks duration() const;
		private:
			Ticks _ppq = 0;
			TrackContainer _container;
		};
	}
}

#endif
