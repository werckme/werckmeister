#include <boost/test/unit_test.hpp>
#include <fm/config.hpp>
#include <fm/literals.hpp>
#include <fm/midi.hpp>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <fm/common.hpp>
#include <iostream>
#include <fm/exception.hpp>

#define TEST_MIDI_FILE "testmidi.mid"

BOOST_AUTO_TEST_CASE(literals)
{
	using namespace fm;
	Ticks ganze = PPQ * 4;
	BOOST_CHECK(ganze == 1.0_N1);
	BOOST_CHECK(ganze / 2 == 1.0_N2);
	BOOST_CHECK(ganze / 4 == 1.0_N4);
	BOOST_CHECK(ganze / 8 == 1.0_N8);
	BOOST_CHECK(ganze / 16 == 1.0_N16);
	BOOST_CHECK(ganze / 32 == 1.0_N32);
	BOOST_CHECK(ganze / 64 == 1.0_N64);
	BOOST_CHECK(ganze / 128 == 1.0_N128);

	BOOST_CHECK(1.0_N1 + 1.0_N2 == 1.0_N1p);
	BOOST_CHECK(1.0_N2 + 1.0_N4 == 1.0_N2p);
	BOOST_CHECK(1.0_N4 + 1.0_N8 == 1.0_N4p);
	BOOST_CHECK(1.0_N8 + 1.0_N16 == 1.0_N8p);
	BOOST_CHECK(1.0_N16 + 1.0_N32 == 1.0_N16p);
	BOOST_CHECK(1.0_N32 + 1.0_N64 == 1.0_N32p);
	BOOST_CHECK(1.0_N64 + 1.0_N128 == 1.0_N64p);
	BOOST_CHECK(1.0_N128 + 0.5_N128 == 1.0_N128p);

	BOOST_CHECK(2.0_N1 / 3 == 1.0_N1t);
	BOOST_CHECK(1.0_N1 / 3 == 1.0_N2t);
	BOOST_CHECK(1.0_N2 / 3 == 1.0_N4t);
	BOOST_CHECK(1.0_N4 / 3 == 1.0_N8t);
	BOOST_CHECK(1.0_N8 / 3 == 1.0_N16t);
	BOOST_CHECK(1.0_N16 / 3 == 1.0_N32t);
	BOOST_CHECK(1.0_N32 / 3 == 1.0_N64t);
	BOOST_CHECK(1.0_N64 / 3 == 1.0_N128t);
}


BOOST_AUTO_TEST_CASE(write_variable_required_size)
{
	using namespace fm;
	BOOST_CHECK(midi::variableLengthRequiredSize(0) == 1);
	BOOST_CHECK(midi::variableLengthRequiredSize(0b01111111) == 1);
	BOOST_CHECK(midi::variableLengthRequiredSize(0b10000000) == 2);
	BOOST_CHECK(midi::variableLengthRequiredSize(0b0111111111111111) == 3);
	BOOST_CHECK(midi::variableLengthRequiredSize(0b1000000000000000) == 3);
	BOOST_CHECK(midi::variableLengthRequiredSize(0b011111111111111111111111) == 4);
	BOOST_CHECK(midi::variableLengthRequiredSize(0b100000000000000000000000) == 4);
	BOOST_CHECK(midi::variableLengthRequiredSize(midi::MaxTickValue) == 4);
	BOOST_CHECK_THROW(midi::variableLengthRequiredSize(midi::MaxTickValue + 1), fm::Exception);
}

BOOST_AUTO_TEST_CASE(write_variable_length)
{
	using namespace fm;
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(1 == midi::variableLengthWrite(0, val, 4));
		BOOST_CHECK((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0));
	}
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(1 == midi::variableLengthWrite(127, val, 4));
		BOOST_CHECK((val[0] == 127 && val[1] == 0 && val[2] == 0 && val[3] == 0));
	}
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(2 == midi::variableLengthWrite(128, val, 4));
		BOOST_CHECK(val[0] == 0b10000001);
		BOOST_CHECK(val[1] == 0b00000000);
		BOOST_CHECK((val[2] == 0 && val[3] == 0));
	}
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(2 == midi::variableLengthWrite(0xC8, val, 4));
		BOOST_CHECK((val[0] == 0b10000001 && val[1] == 0b01001000 && val[2] == 0 && val[3] == 0));
	}
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(3 == midi::variableLengthWrite(0x100000, val, 4));
		BOOST_CHECK((val[0] == 0b11000000 && val[1] == 0b10000000 && val[2] == 0 && val[3] == 0));
	}
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(4 == midi::variableLengthWrite(midi::MaxTickValue, val, 4));
		BOOST_CHECK((val[0] == 0xFF && val[1] == 0xFF && val[2] == 0xFF && val[3] == 0x7F));
	}
}

BOOST_AUTO_TEST_CASE(event_heap_issue)
{
	// bug was incorrect byte size for the variable length tick value
	// so we got a corrupt heap message here (on windows VSC++ 2015)
	using namespace fm;
	midi::Event ev;
	ev.absPosition(12.0_N1);
	ev.eventType(midi::NoteOff);
	ev.channel(0);
	auto size = ev.byteSize(0);
	Byte *bff = new Byte[size];
	ev.write(0, bff, size);
	delete[] bff;
}

BOOST_AUTO_TEST_CASE(read_variable_length_fail)
{
	using namespace fm;
	{
		Byte val[] = { 0xFF, 0xFF, 0xFF, 0xFF };
		BOOST_CHECK_THROW(midi::variableLengthRead(val, 4), fm::Exception);
	}
}

BOOST_AUTO_TEST_CASE(read_variable_length)
{
	using namespace fm;
	{
		Byte val[4] = { 0 };
		BOOST_CHECK(midi::variableLengthRead(val, 4) == 0);
	}
	{
		Byte val[4] = { 127, 0, 0, 0 };
		BOOST_CHECK(midi::variableLengthRead(val, 4) == 127);
	}
	{
		Byte val[4] = { 0b10000001, 0b0000000, 0, 0 };
		BOOST_CHECK(midi::variableLengthRead(val, 4) == 128);
	}
	{
		Byte val[4] = { 0b10000001, 0b01001000, 0, 0 };
		BOOST_CHECK(midi::variableLengthRead(val, 4) == 0xC8);
	}
	{
		Byte val[4] = { 0b11000000, 0b10000000, 0, 0 };
		BOOST_CHECK(midi::variableLengthRead(val, 4) == 0x100000);
	}
	{
		Byte val[4] = { 0xFF, 0xFF, 0xFF, 0x7F };
		BOOST_CHECK(midi::variableLengthRead(val, 4) == midi::MaxTickValue);
	}
}

BOOST_AUTO_TEST_CASE(midi_event_size)
{
	using namespace fm;
	midi::Event event;
	BOOST_CHECK(event.byteSize(0) == 4);
	BOOST_CHECK(event.byteSize(-128) == 5);
	event.absPosition(midi::MaxTickValue);
	BOOST_CHECK(event.byteSize(0) == 7);
}


BOOST_AUTO_TEST_CASE(write_midi_event_fail)
{
	using namespace fm;
	midi::Event event;
	event.absPosition(midi::MaxTickValue);
	Ticks deltaOffset = 0;
	constexpr size_t size = 4; // 5 are needed
	Byte bytes[size] = { 0 };
	BOOST_CHECK_THROW(event.write(deltaOffset, &bytes[0], size), fm::Exception);
}

BOOST_AUTO_TEST_CASE(write_midi_event_0)
{
	using namespace fm;
	midi::Event event;
	event.channel(1);
	event.eventType(midi::NoteOn);
	event.parameter1(64);
	event.parameter2(100);
	event.absPosition(15);
	Ticks deltaOffset = 5;
	constexpr size_t size = 4; // event.byteSize(deltaOffset)
	Byte bytes[size];
	BOOST_CHECK(event.write(deltaOffset, &bytes[0], size) == size);
	BOOST_CHECK(bytes[0] == 10);
	BOOST_CHECK((bytes[1] & 0xF0) >> 4 == midi::NoteOn);
	BOOST_CHECK((bytes[1] & 0xF) == 1);
	BOOST_CHECK(bytes[2] == 64);
	BOOST_CHECK(bytes[3] == 100);
}

BOOST_AUTO_TEST_CASE(write_midi_event_1)
{
	using namespace fm;
	midi::Event event;
	event.channel(1);
	event.eventType(midi::NoteOn);
	event.parameter1(64);
	event.parameter2(100);
	event.absPosition(midi::MaxTickValue);
	Ticks deltaOffset = 0;
	constexpr size_t size = 7; // event.byteSize(deltaOffset)
	Byte bytes[size];
	BOOST_CHECK(event.write(deltaOffset, &bytes[0], size) == size);
	BOOST_CHECK(bytes[0] == 0xFF);
	BOOST_CHECK(bytes[1] == 0xFF);
	BOOST_CHECK(bytes[2] == 0xFF);
	BOOST_CHECK(bytes[3] == 0x7F);
	BOOST_CHECK((bytes[4] & 0xF0) >> 4 == midi::NoteOn);
	BOOST_CHECK((bytes[4] & 0xF) == 1);
	BOOST_CHECK(bytes[5] == 64);
	BOOST_CHECK(bytes[6] == 100);
}

BOOST_AUTO_TEST_CASE(write_midi_event_program_change)
{
	using namespace fm;
	midi::Event event;
	event.channel(1);
	event.eventType(midi::ProgramChange);
	event.parameter1(64);
	constexpr size_t size = 3;
	Byte bytes[size] = { 0 };
	BOOST_CHECK(event.write(0, &bytes[0], size) == size);
	BOOST_CHECK(bytes[0] == 0);
	BOOST_CHECK((bytes[1] & 0xC0) >> 4 == midi::ProgramChange);
	BOOST_CHECK((bytes[1] & 0xF) == 1);
	BOOST_CHECK(bytes[2] == 64);
}


BOOST_AUTO_TEST_CASE(write_read_event_0)
{
	using namespace fm;
	midi::Event event;
	Byte bytes[] = { 0xFF, 0xFF, 0xFF, 0x7F, 0x91, 64, 100 };
	Ticks deltaOffset = 15;
	BOOST_CHECK(event.read(deltaOffset, &bytes[0], 7) == 7);
	BOOST_CHECK(event.absPosition() == midi::MaxTickValue + deltaOffset);
	BOOST_CHECK(event.eventType() == midi::NoteOn);
	BOOST_CHECK(event.channel() == 1);
	BOOST_CHECK(event.parameter1() == 64);
	BOOST_CHECK(event.parameter2() == 100);
}

BOOST_AUTO_TEST_CASE(write_read_program_change)
{
	using namespace fm;
	midi::Event event;
	Byte bytes[] = { 0x0, 0xC1, 0x22 };
	BOOST_CHECK(event.read(0, &bytes[0], 3) == 3);
	BOOST_CHECK(event.absPosition() == 0);
	BOOST_CHECK(event.eventType() == midi::ProgramChange);
	BOOST_CHECK(event.channel() == 1);
	BOOST_CHECK(event.parameter1() == 0x22);
	BOOST_CHECK(event.parameter2() == 0x0);
}

BOOST_AUTO_TEST_CASE(write_midi_event_container)
{
	using namespace fm;
	midi::MidiConfig config;
	midi::EventContainer events;
	events.midiConfig(&config);
	BOOST_CHECK(events.numEvents() == 0);
}

BOOST_AUTO_TEST_CASE(write_midi_event_container_write_fail)
{
	using namespace fm;
	midi::MidiConfig config;
	midi::EventContainer events;
	events.midiConfig(&config);
	events.add(midi::Event::NoteOn(0, 0, 24, 100));
	events.add(midi::Event::NoteOn(0, 1.0_N4, 25, 100));
	events.add(midi::Event::NoteOn(0, 2.0_N4, 26, 100));
	events.add(midi::Event::NoteOn(0, 3.0_N4, 27, 100));
	Byte bytes[1];
	BOOST_CHECK_THROW(events.write(&bytes[0], 3), fm::Exception);
}

BOOST_AUTO_TEST_CASE(write_midi_event_container_write)
{
	using namespace fm;
	midi::MidiConfig config;
	midi::EventContainer events;
	events.midiConfig(&config);
	events.add(midi::Event::NoteOn(0, 1, 24, 100));
	events.add(midi::Event::NoteOn(0, 1, 48, 100));
	events.add(midi::Event::NoteOn(1, 50, 25, 101));
	events.add(midi::Event::NoteOn(2, 100, 26, 102));
	BOOST_CHECK(events.numEvents() == 4);
	constexpr size_t byteSize = 16;
	BOOST_CHECK_EQUAL(byteSize, events.byteSize());
	Byte bytes[byteSize];
	BOOST_CHECK_EQUAL(events.write(&bytes[0], byteSize), byteSize);
	int c = 0;
	BOOST_CHECK(bytes[c] == 1); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 0); BOOST_CHECK(bytes[c + 2] == 24); BOOST_CHECK(bytes[c + 3] == 100);
	c += 4;
	BOOST_CHECK(bytes[c] == 0); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 0); BOOST_CHECK(bytes[c + 2] == 48); BOOST_CHECK(bytes[c + 3] == 100);
	c += 4;
	BOOST_CHECK(bytes[c] == 49); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 1); BOOST_CHECK(bytes[c + 2] == 25); BOOST_CHECK(bytes[c + 3] == 101);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 2); BOOST_CHECK(bytes[c + 2] == 26); BOOST_CHECK(bytes[c + 3] == 102);
}

BOOST_AUTO_TEST_CASE(write_midi_event_container_read_n_write)
{
	using namespace fm;
	Byte *bytes;
	size_t byteSize;
	// write
	{
		midi::MidiConfig config;
		midi::EventContainer events;
		events.midiConfig(&config);
		events.add(midi::Event::NoteOn(0, 0, 24, 101));
		events.add(midi::Event::NoteOff(1, 1.0_N4, 25));
		events.add(midi::Event::NoteOn(2, 2.0_N4, 26, 103));
		events.add(midi::Event::NoteOff(3, 3.0_N4, 27));
		BOOST_CHECK(events.numEvents() == 4);
		byteSize = events.byteSize();
		bytes = new Byte[byteSize];
		BOOST_CHECK(events.write(bytes, byteSize) == byteSize);
	}
	// read
	{
		midi::MidiConfig config;
		midi::EventContainer events;
		events.midiConfig(&config);
		BOOST_CHECK(events.read(bytes, byteSize));
		BOOST_CHECK(events.numEvents() == 4);
		std::vector<midi::Event> bff(events.numEvents());
		std::copy(events.begin(), events.end(), bff.begin());
		int c = 0;
		BOOST_CHECK(bff[c].channel() == 0); BOOST_CHECK(bff[c].absPosition() == 0); BOOST_CHECK(bff[c].eventType() == midi::NoteOn);
		BOOST_CHECK(bff[c].parameter1() == 24); BOOST_CHECK(bff[c].parameter2() == 101);
		++c;
		BOOST_CHECK(bff[c].channel() == 1); BOOST_CHECK(bff[c].absPosition() == 1.0_N4); BOOST_CHECK(bff[c].eventType() == midi::NoteOff);
		BOOST_CHECK(bff[c].parameter1() == 25); BOOST_CHECK(bff[c].parameter2() == 0);
		++c;
		BOOST_CHECK(bff[c].channel() == 2); BOOST_CHECK(bff[c].absPosition() == 2.0_N4); BOOST_CHECK(bff[c].eventType() == midi::NoteOn);
		BOOST_CHECK(bff[c].parameter1() == 26); BOOST_CHECK(bff[c].parameter2() == 103);
		++c;
		BOOST_CHECK(bff[c].channel() == 3); BOOST_CHECK(bff[c].absPosition() == 3.0_N4); BOOST_CHECK(bff[c].eventType() == midi::NoteOff);
		BOOST_CHECK(bff[c].parameter1() == 27); BOOST_CHECK(bff[c].parameter2() == 0);
	}
	delete[] bytes;
}

BOOST_AUTO_TEST_CASE(write_midi_event_pc_cc_before_notes)
{
	using namespace fm;
	Byte *bytes;
	size_t byteSize;
	// write
	{
		midi::MidiConfig config;
		midi::EventContainer events;
		events.midiConfig(&config);
		events.addNote(0, 0, 100, 100, 1.0_N4);

		auto cc0 = midi::Event();
		cc0.absPosition(0);
		cc0.channel(0);
		cc0.eventType(midi::EventType::Controller);
		cc0.parameter1(0);
		cc0.parameter2(10);
		events.add(cc0);

		auto pc = midi::Event();
		pc.absPosition(0);
		pc.channel(0);
		pc.eventType(midi::EventType::ProgramChange);
		pc.parameter1(5);
		events.add(pc);


		BOOST_CHECK(events.numEvents() == 4);
		byteSize = events.byteSize();
		bytes = new Byte[byteSize];
		BOOST_CHECK(events.write(bytes, byteSize) == byteSize);
	}
	// read
	{
		midi::MidiConfig config;
		midi::EventContainer events;
		events.midiConfig(&config);
		BOOST_CHECK(events.read(bytes, byteSize));
		BOOST_CHECK(events.numEvents() == 4);
		std::vector<midi::Event> bff(events.numEvents());
		std::copy(events.begin(), events.end(), bff.begin());
		int c = 0;
		BOOST_CHECK(bff[c].channel() == 0); BOOST_CHECK(bff[c].absPosition() == 0); BOOST_CHECK(bff[c].eventType() == midi::Controller);
		BOOST_CHECK(bff[c].parameter1() == 0); BOOST_CHECK(bff[c].parameter2() == 10);
		++c;
		BOOST_CHECK(bff[c].channel() == 0); BOOST_CHECK(bff[c].absPosition() == 0); BOOST_CHECK(bff[c].eventType() == midi::ProgramChange);
		BOOST_CHECK(bff[c].parameter1() == 5);
		++c;
		BOOST_CHECK(bff[c].channel() == 0); BOOST_CHECK(bff[c].absPosition() == 0); BOOST_CHECK(bff[c].eventType() == midi::NoteOn);
		BOOST_CHECK(bff[c].parameter1() == 100); BOOST_CHECK(bff[c].parameter2() == 100);
		++c;
		BOOST_CHECK(bff[c].channel() == 0); BOOST_CHECK(bff[c].absPosition() == 1.0_N4); BOOST_CHECK(bff[c].eventType() == midi::NoteOff);
		BOOST_CHECK(bff[c].parameter1() == 100); BOOST_CHECK(bff[c].parameter2() == 0);
	}
	delete[] bytes;
}

BOOST_AUTO_TEST_CASE(write_midi_event_track_write_fail)
{
	using namespace fm;
	midi::Track track;
	midi::MidiConfig config;
	auto &events = track.events();
	events.midiConfig(&config);
	events.add(midi::Event::NoteOn(0, 0, 24, 100));
	events.add(midi::Event::NoteOn(1, 50, 25, 101));
	events.add(midi::Event::NoteOn(2, 100, 26, 102));

	constexpr size_t byteSize = 20;
	Byte bytes[byteSize];
	BOOST_CHECK_THROW(track.write(&bytes[0], byteSize - 1), fm::Exception);
}

BOOST_AUTO_TEST_CASE(write_midi_event_track_write)
{
	using namespace fm;
	midi::Track track;
	midi::MidiConfig config;
	auto &events = track.events();
	events.midiConfig(&config);
	events.add(midi::Event::NoteOn(0, 0, 24, 100));
	events.add(midi::Event::NoteOn(1, 50, 25, 101));
	events.add(midi::Event::NoteOn(2, 100, 26, 102));

	constexpr size_t byteSize = 24;
	BOOST_CHECK_EQUAL(byteSize, track.byteSize());
	Byte bytes[byteSize];
	BOOST_CHECK(track.write(&bytes[0], byteSize) == byteSize);
	int c = 0;
	BOOST_CHECK(std::string(reinterpret_cast<char*>(&bytes[c]), 4) == "MTrk");
	c += 4;
	BOOST_CHECK((bytes[c] == 0 && bytes[c + 1] == 0 && bytes[c + 2] == 0 && bytes[c + 3] == 16));
	c += 4;
	BOOST_CHECK(bytes[c] == 0); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 0); BOOST_CHECK(bytes[c + 2] == 24); BOOST_CHECK(bytes[c + 3] == 100);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 1); BOOST_CHECK(bytes[c + 2] == 25); BOOST_CHECK(bytes[c + 3] == 101);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 2); BOOST_CHECK(bytes[c + 2] == 26); BOOST_CHECK(bytes[c + 3] == 102);
	BOOST_CHECK(bytes[21] == 0xFF); 
	BOOST_CHECK(bytes[22] == 0x2F); 
	BOOST_CHECK(bytes[23] == 0x00); 
}

BOOST_AUTO_TEST_CASE(write_midi_write_fail)
{
	using namespace fm;
	midi::Midi midi(PPQ);
	midi::TrackPtr track = std::make_shared<midi::Track>();
	auto &events = track->events();
	midi::MidiConfig config;
	events.midiConfig(&config);
	events.add(midi::Event::NoteOn(0, 0, 24, 100));
	events.add(midi::Event::NoteOn(1, 50, 25, 101));
	events.add(midi::Event::NoteOn(2, 100, 26, 102));
	midi.addTrack(track);

	constexpr size_t byteSize = 14 + 8 + 4 * 3;
	Byte bytes[byteSize];
	BOOST_CHECK_THROW(midi.write(&bytes[0], byteSize - 1), fm::Exception);
}

BOOST_AUTO_TEST_CASE(write_midi_write)
{
	using namespace fm;
	midi::Midi midi(PPQ);
	{
		midi::TrackPtr track = std::make_shared<midi::Track>();
		auto &events = track->events();
		midi::MidiConfig config;
		events.midiConfig(&config);
		events.add(midi::Event::NoteOn(0, 0, 24, 100));
		events.add(midi::Event::NoteOn(1, 50, 25, 101));
		events.add(midi::Event::NoteOn(2, 100, 26, 102));
		midi.addTrack(track);
	}
	{
		midi::TrackPtr track = std::make_shared<midi::Track>();
		auto &events = track->events();
		midi::MidiConfig config;
		events.midiConfig(&config);
		events.add(midi::Event::NoteOn(3, 0, 48, 103));
		events.add(midi::Event::NoteOn(4, 50, 50, 104));
		events.add(midi::Event::NoteOn(5, 100, 52, 105));
		midi.addTrack(track);
	}
	constexpr size_t byteSize = 62;
	BOOST_CHECK(byteSize == midi.byteSize());
	Byte bytes[byteSize];
	BOOST_CHECK(midi.write(&bytes[0], byteSize) == byteSize);
	int c = 0;
	// Midi Header
	BOOST_CHECK(std::string(reinterpret_cast<char*>(&bytes[c]), 4) == "MThd");
	c += 4;
	BOOST_CHECK((bytes[c] == 0 && bytes[c + 1] == 0 && bytes[c + 2] == 0 && bytes[c + 3] == 6));
	c += 4;
	BOOST_CHECK((bytes[c] == 0 && bytes[c + 1] == 1));
	c += 2;
	BOOST_CHECK((bytes[c] == 0 && bytes[c + 1] == 2));
	c += 2;
	midi::Word ppq = static_cast<midi::Word>(0x7FFF) & static_cast<midi::Word>(PPQ);
	if (isLittleEndian()) {
		endswap(&ppq);
	}
	BOOST_CHECK((*reinterpret_cast<midi::Word*>(&bytes[c])) == ppq);
	c += 2;

	// Track 1
	BOOST_CHECK(std::string(reinterpret_cast<char*>(&bytes[c]), 4) == "MTrk");
	c += 4;
	BOOST_CHECK((bytes[c] == 0 && bytes[c + 1] == 0 && bytes[c + 2] == 0 && bytes[c + 3] == 16));
	c += 4;
	BOOST_CHECK(bytes[c] == 0); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 0); BOOST_CHECK(bytes[c + 2] == 24); BOOST_CHECK(bytes[c + 3] == 100);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 1); BOOST_CHECK(bytes[c + 2] == 25); BOOST_CHECK(bytes[c + 3] == 101);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 2); BOOST_CHECK(bytes[c + 2] == 26); BOOST_CHECK(bytes[c + 3] == 102);
	c += 5;
	// end of track sequence
	BOOST_CHECK(bytes[c] == 0xFF);
	c += 1;
	BOOST_CHECK(bytes[c] == 0x2F);
	c += 1;
	BOOST_CHECK(bytes[c] == 0x00);
	c += 1;		

	// Track 2
	BOOST_CHECK(std::string(reinterpret_cast<char*>(&bytes[c]), 4) == "MTrk");
	c += 4;
	BOOST_CHECK((bytes[c] == 0 && bytes[c + 1] == 0 && bytes[c + 2] == 0 && bytes[c + 3] == 16));
	c += 4;
	BOOST_CHECK(bytes[c] == 0); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 3); BOOST_CHECK(bytes[c + 2] == 48); BOOST_CHECK(bytes[c + 3] == 103);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 4); BOOST_CHECK(bytes[c + 2] == 50); BOOST_CHECK(bytes[c + 3] == 104);
	c += 4;
	BOOST_CHECK(bytes[c] == 50); BOOST_CHECK((bytes[c + 1] & 0xF0) >> 4 == midi::NoteOn); BOOST_CHECK((bytes[c + 1] & 0xF) == 5); BOOST_CHECK(bytes[c + 2] == 52); BOOST_CHECK(bytes[c + 3] == 105);
	c +=5;
	// end of track sequence
	BOOST_CHECK(bytes[c] == 0xFF);
	c += 1;
	BOOST_CHECK(bytes[c] == 0x2F);
	c += 1;
	BOOST_CHECK(bytes[c] == 0x00); 
}

BOOST_AUTO_TEST_CASE(write_midi_write_to_file)
{
	using namespace fm;
	midi::Midi midi(PPQ);
	{
		midi::TrackPtr track = std::make_shared<midi::Track>();
		auto &events = track->events();
		midi::MidiConfig config;
		events.midiConfig(&config);
		events.addNote(0, 0, 48, 100, 1.0_N4);
		events.addNote(0, 1.0_N4, 50, 100, 1.0_N4);
		events.addNote(0, 2.0_N4, 52, 100, 1.0_N4);
		events.addNote(0, 3.0_N4, 53, 100, 1.0_N4);
		midi.addTrack(track);
	}
	{
		midi::TrackPtr track = std::make_shared<midi::Track>();
		auto &events = track->events();
		midi::MidiConfig config;
		events.midiConfig(&config);
		events.addNote(0, 0, 24, 100, 1.0_N8);
		events.addNote(0, 1.0_N8, 48, 100, 1.0_N8);
		events.addNote(0, 2.0_N8, 24, 100, 1.0_N8);
		events.addNote(0, 3.0_N8, 48, 100, 1.0_N8);
		events.addNote(0, 4.0_N8, 24, 100, 1.0_N8);
		events.addNote(0, 5.0_N8, 48, 100, 1.0_N8);
		events.addNote(0, 6.0_N8, 24, 100, 1.0_N8);
		events.addNote(0, 7.0_N8, 48, 100, 1.0_N8);
		midi.addTrack(track);
	}
	midi.write(TEST_MIDI_FILE);
	size_t byteSize = midi.byteSize();

	std::fstream midifile(TEST_MIDI_FILE, std::ios::in | std::ios::binary | std::ios::ate);
	BOOST_CHECK((size_t)midifile.tellg() == byteSize);
	midifile.seekg(std::ios::beg);
}

BOOST_AUTO_TEST_CASE(event_container_from)
{
	using namespace fm;
	midi::Midi midi(PPQ);
	midi::TrackPtr track = std::make_shared<midi::Track>();
	auto &events = track->events();
	midi::MidiConfig config;
	events.midiConfig(&config);
	events.add(midi::Event::NoteOn(0, 0, 24, 100));
	events.add(midi::Event::NoteOn(1, 50, 25, 101));
	events.add(midi::Event::NoteOn(3, 100, 26, 102));
	events.add(midi::Event::NoteOn(4, 200, 26, 102));
	events.add(midi::Event::NoteOn(5, 300, 26, 102));

	auto it = events.to(100);
	BOOST_CHECK((*it).channel() == 4);
}

BOOST_AUTO_TEST_CASE(meta_event_write_1)
{
	using namespace fm;
	constexpr size_t eventSize = 7;
	auto tempo = midi::Event::MetaTempo(120.0);
	BOOST_CHECK( tempo.eventType() == midi::MetaEvent );
	BOOST_CHECK( tempo.metaEventType() == midi::Tempo);
	BOOST_CHECK( tempo.byteSize(0) == 7 );
	BOOST_CHECK( tempo.payloadSize() == 6);
	
	Byte bff[eventSize];
	BOOST_CHECK_THROW(tempo.write(0, &bff[0], eventSize-1), fm::Exception);
	tempo.write(0, &bff[0], eventSize);
	
	BOOST_CHECK(bff[0] == 0);
	BOOST_CHECK(bff[1] == 0xFF);
	BOOST_CHECK(bff[2] == 0x51);
	BOOST_CHECK(bff[3] == 0x3);
	BOOST_CHECK(bff[4] == 0x7);
	BOOST_CHECK(bff[5] == 0xA1);
	BOOST_CHECK(bff[6] == 0x20);
}

BOOST_AUTO_TEST_CASE(meta_event_read_1)
{
	using namespace fm;
	constexpr size_t eventSize = 7;
	auto tempo = midi::Event::MetaTempo(120.0);	
	Byte bff[eventSize];
	tempo.write(0, &bff[0], eventSize);
	
	auto readTempo = midi::Event();
	BOOST_CHECK_THROW(readTempo.read(0, &bff[0], eventSize-1), fm::Exception);
	readTempo.read(0, &bff[0], eventSize);

	BOOST_CHECK(readTempo.eventType() == midi::MetaEvent);
	BOOST_CHECK(readTempo.metaEventType() == midi::Tempo);
	BOOST_CHECK(readTempo.metaDataSize() == 3);

	int bpmMicros = midi::Event::MetaGetIntValue(readTempo.metaData(), readTempo.metaDataSize());
	BOOST_CHECK(midi::MicrosecondsPerMinute / bpmMicros == 120);
}

BOOST_AUTO_TEST_CASE(meta_event_read_write_1)
{
	using namespace fm;
	constexpr size_t eventSize = 8;
	auto metaOrg = midi::Event::MetaInstrument("bass");
	BOOST_CHECK( metaOrg.eventType() == midi::MetaEvent );
	BOOST_CHECK( metaOrg.metaEventType() == midi::InstrumentName );
	BOOST_CHECK( metaOrg.byteSize(0) == 8 );
	BOOST_CHECK( metaOrg.payloadSize() == 7);

	Byte bff[eventSize];
	metaOrg.write(0, &bff[0], eventSize);
	
	auto dst = midi::Event();
	dst.read(0, bff, eventSize);
	auto name = midi::Event::MetaGetStringValue(dst.metaData(), dst.metaDataSize());
	BOOST_CHECK(name == "bass");
}

extern const char * LongString1001;

BOOST_AUTO_TEST_CASE(meta_event_read_write_long_string)
{
	using namespace fm;
	constexpr size_t eventSize = 5 + 1001;
	auto metaOrg = midi::Event::MetaInstrument(LongString1001);
	BOOST_CHECK( metaOrg.eventType() == midi::MetaEvent );
	BOOST_CHECK( metaOrg.metaEventType() == midi::InstrumentName );
	BOOST_CHECK( metaOrg.byteSize(0) == 5 + 1001 );
	BOOST_CHECK( metaOrg.payloadSize() == 4 + 1001);

	Byte bff[eventSize];
	BOOST_CHECK_THROW(metaOrg.write(0, &bff[0], eventSize-1), fm::Exception);
	auto written = metaOrg.write(0, &bff[0], eventSize);
	BOOST_CHECK(written == eventSize);

	auto dst = midi::Event();
	BOOST_CHECK_THROW(dst.read(0, bff, eventSize-1), fm::Exception);
	size_t bytesRead = dst.read(0, bff, eventSize);
	BOOST_CHECK(written == bytesRead);
	auto name = midi::Event::MetaGetStringValue(dst.metaData(), dst.metaDataSize());
	BOOST_CHECK(name == LongString1001);
}

BOOST_AUTO_TEST_CASE(meta_event_read_write_custom_meta)
{
	using namespace fm;
	constexpr size_t dataSize = 1001 + 1;
	constexpr size_t eventSize = 5 + dataSize;
	midi::CustomMetaData data;
	data.type = midi::CustomMetaData::SetDevice;
	std::string longString(&LongString1001[0]);
	data.data = midi::CustomMetaData::Data(longString.begin(), longString.end());
	auto org = midi::Event::MetaCustom(data);

	BOOST_CHECK( org.eventType() == midi::MetaEvent );
	BOOST_CHECK( org.metaEventType() == midi::CustomMetaEvent );
	BOOST_CHECK( org.byteSize(0) == eventSize );

	Byte bff[eventSize];
	org.write(0, bff, eventSize);

	midi::Event copy;
	copy.read(0, &bff[0], eventSize);

	midi::CustomMetaData dataCopy = midi::Event::MetaGetCustomData(copy.metaData(), copy.metaDataSize());
	BOOST_CHECK( dataCopy.type == midi::CustomMetaData::SetDevice );
	std::string dataCopyAsString(dataCopy.data.begin(), dataCopy.data.end());
	BOOST_CHECK( longString == dataCopyAsString );
}

BOOST_AUTO_TEST_CASE(meta_event_read_write_custom_meta_0)
{
	using namespace fm;
	constexpr size_t dataSize = 1;
	constexpr size_t eventSize = 4 + dataSize;
	midi::CustomMetaData data;
	data.type = midi::CustomMetaData::SetDevice;
	auto org = midi::Event::MetaCustom(data);

	BOOST_CHECK( org.eventType() == midi::MetaEvent );
	BOOST_CHECK( org.metaEventType() == midi::CustomMetaEvent );
	BOOST_CHECK( org.byteSize(0) == eventSize );

	Byte bff[eventSize];
	org.write(0, bff, eventSize);

	midi::Event copy;
	copy.read(0, &bff[0], eventSize);

	midi::CustomMetaData dataCopy = midi::Event::MetaGetCustomData(copy.metaData(), copy.metaDataSize());
	BOOST_CHECK( dataCopy.type == midi::CustomMetaData::SetDevice );
	BOOST_CHECK( dataCopy.data.size() == 0 );
}