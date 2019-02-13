#!/usr/bin/env python3
"""
    compares two midifiles.
    compares timepoint, channel, pitch and velocity.
    all other information will be ignored.
"""

import sys
from mido import MidiFile  # tick2second seems not to work properly

_tempo = 120
_acceptedEventTypes = ['note_on', 'note_off']

fticks_2_seconds = None

def createKey(trackIdx, event, absTicks):
    return (trackIdx, event.type, event.channel, event.note, event.velocity, absTicks)

def addEvent(trackIdx, container, event, absTicks):
    key = createKey(trackIdx, event, absTicks)
    if key in container:
        raise RuntimeError("event(%s, time=%f) duplicate" % (str(event), fticks_2_seconds(absTicks)))
    container.add(key)

def get_events(midifile):
    for track in midifile.tracks:
        absTicks = 0
        for trackIdx, event in enumerate(track):
            if not event.type in _acceptedEventTypes:
                continue
            yield trackIdx, event, absTicks
            absTicks += event.time

def compare(file1, file2):
    global fticks_2_seconds
    events_a = set()
    midi_a = MidiFile(file1)
    midi_b = MidiFile(file2)
    if midi_a.ticks_per_beat != midi_b.ticks_per_beat:
        raise RuntimeError("ticks per beat dosen't match")
    fticks_2_seconds = lambda ticks:  60.0 * ticks / (_tempo * midi_a.ticks_per_beat)
    for trackIdx, event, absTicks in get_events(midi_a):
        addEvent(trackIdx, events_a, event, absTicks)
    
    for trackIdx, event, absTicks in get_events(midi_b):
        key = createKey(trackIdx, event, absTicks)
        if key in events_a:
            events_a.remove(key)
        else:
            raise RuntimeError("%s's event(%s, time=%f) not found in %s" % (file2, str(event), fticks_2_seconds(absTicks), file1))
    if len(events_a) > 0:
        raise RuntimeError("%s has %d more events than %s" % (file1, len(events_a), file2))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("usage: %s file1 file2" % (sys.argv[0]))
        exit(1)
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    try:
        compare(file1, file2)
    except RuntimeError as ex:
        print(str(ex))
        exit(1)