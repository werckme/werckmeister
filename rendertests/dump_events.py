#!/usr/bin/env python3

import sys
from mido import MidiFile  # tick2second seems not to work properly

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("usage: %s file1" % (sys.argv[0]))
        exit(1)
    file = sys.argv[1] 
    midifile = MidiFile(file)
    absTicks = 0
    for track in midifile.tracks:
        noteon_by_pitches = {}
        for event in track:
            if hasattr(event, 'time'):
                absTicks += event.time  
            if hasattr(event, 'name'):
                print(f'::{event.name}::')
                continue 
            print(f'{absTicks}: {event}')