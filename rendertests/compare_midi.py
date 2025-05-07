#!/usr/bin/env python3
"""
    compares two midifiles.
    compares timepoint, channel, pitch and velocity.
    all other information will be ignored.
"""

TAG_SKIP_PLAUSIBILLITY_CHECK = 'skip-event-plausibility-test'
import copy
import sys
from mido import MidiFile  # tick2second seems not to work properly
from os import name as OSNAME

MAX_TIME_DIFF = 0.019 

_tempo = 120
_acceptedEventTypes = ['note_on', 'note_off', 'control_change', 'sysex', 'cue_marker']

fticks_2_seconds = None

def createKey(trackIdx, event, absTicks, cc_counter: map):
    if event.type == 'cue_marker':
        return (event.type, fticks_2_seconds(absTicks), event.text)
    if event.type == 'sysex':
        return (event.type, tuple(event.data), fticks_2_seconds(absTicks))
    if event.type == 'control_change':
        key = (event.type, event.channel, event.control, event.value, fticks_2_seconds(absTicks))
        if key in cc_counter:
            cc_counter[key] = cc_counter[key] + 1
        else:
            cc_counter[key] = 0
        return key + (cc_counter[key],)
    return (event.type, event.channel, event.note, event.velocity, fticks_2_seconds(absTicks))

def addEvent(trackIdx, container, event, absTicks, cc_counter: map):
    key = createKey(trackIdx, event, absTicks, cc_counter)
    if key in container:
        raise RuntimeError("event(%s, time=%f) duplicate" % (str(event), fticks_2_seconds(absTicks)))
    container.add(key)

def get_events(midifile):
    for trackIdx, track in enumerate(midifile.tracks):
        absTicks = 0
        for event in track:
            if hasattr(event, 'time'):
                absTicks += event.time
            if event.type in _acceptedEventTypes:
                yield trackIdx, event, absTicks



def check_for_event_order_issues(midifile):
    """
        an order issue could be if a note-on and a note-off event, with the same pitch, 
        happens on the same time, and the note-on is before the note off.
        So the note will never be heard. This is the case if two equal notes are written in a row: `c c`
    """
    ev_time = -1
    ev = None
    for trackIdx, event, absTicks in get_events(midifile):
        prev_ev_time = ev_time
        prev_ev = ev
        ev_time = absTicks
        ev = event
        if prev_ev_time != ev_time:
            continue
        if prev_ev.note != ev.note:
            continue
        if prev_ev.type == 'note_on':
            raise RuntimeError(f'Order issue notes: {prev_ev} AND {ev} appears in the wrong order')

def check_for_plausibility(midifile):
    key = lambda ev: (ev.note, ev.channel) 
    for track in midifile.tracks:
        noteon_by_pitches = {}
        absTicks = 0
        for event in track:
            if hasattr(event, 'time'):
                absTicks += event.time       
            if event.type == 'note_on':
                noteon_by_pitches[key(event)] = event
            if event.type == 'note_off':
                if key(event) not in noteon_by_pitches:
                    raise RuntimeError(f'{event} at {absTicks} missing corresponding noteon')
                noteon_by_pitches.pop(key(event))
        if len(noteon_by_pitches.values()) > 0:
            raise RuntimeError(f'{", ".join([str(x) for x in noteon_by_pitches.values()])} missing corresponding noteoff')

def is_same_event_except_for_time(a, b):
    a = copy.deepcopy(a)
    b = copy.deepcopy(b)
    a.time = 0
    b.time = 0
    return str(a) == str(b)

def compare(file1, file2, test_tags):
    global fticks_2_seconds
    events_a = set()
    midi_a = MidiFile(file1)
    midi_b = MidiFile(file2)
    if midi_a.ticks_per_beat != midi_b.ticks_per_beat:
        raise RuntimeError("ticks per beat dosen't match")
    fticks_2_seconds = lambda ticks:  60.0 * ticks / (_tempo * midi_a.ticks_per_beat)

    cc_counter_file_a = {}
    for trackIdx, event, absTicks in get_events(midi_a):
        addEvent(trackIdx, events_a, event, absTicks, cc_counter_file_a)
    cc_counter_file_b = {}
    eventindex = 0
    for trackIdx, event, absTicks in get_events(midi_b):
        key = createKey(trackIdx, event, absTicks, cc_counter_file_b)
        if key in events_a:
            events_a.remove(key)
        else:
            try:
                neighborTrackIdx, neighbourEvent, neighbourAbsTicks = list(get_events(midi_a))[eventindex]
                neighbourKey=createKey(neighborTrackIdx, neighbourEvent, neighbourAbsTicks, cc_counter_file_b)
                if  is_same_event_except_for_time(event, neighbourEvent):
                    timediff = abs(fticks_2_seconds(neighbourAbsTicks) - fticks_2_seconds(absTicks))
                    if (timediff > MAX_TIME_DIFF):
                        raise Exception(f"fallback compare failed: time diff = {timediff}")
                    events_a.remove(neighbourKey)
                else:
                    raise Exception("fallback compare failed")
            except Exception as ex:
                print(str(ex))
                raise RuntimeError("%s's event(%s, time=%f) not found in %s -> %s" % (file2, str(event), fticks_2_seconds(absTicks), file1, str(ex)))
        eventindex += 1

    if len(events_a) > 0:
        raise RuntimeError("%s has %d more events than %s" % (file1, len(events_a), file2))
    
    if TAG_SKIP_PLAUSIBILLITY_CHECK in test_tags:
        return
    try:
        check_for_plausibility(midi_b)
    except RuntimeError as ex:
        raise RuntimeError(f'{file2}: {ex}')        
    try:
        check_for_plausibility(midi_a)
    except RuntimeError as ex:
        raise RuntimeError(f'{file1}: {ex}')



if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("usage: %s file1 file2" % (sys.argv[0]))
        exit(1)
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    try:
        compare(file1, file2, [])
    except RuntimeError as ex:
        print(str(ex))
        exit(1)
