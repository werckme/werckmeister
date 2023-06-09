"""
    [#188] check the correct order of the insrtument def init sequence
"""
from mido import MidiFile 

def _assert(a, b, context):
    if a != b:
        raise RuntimeError(f"{context}: expecting {b} but was {a}")

def execute(testdata: dict):
    midifile = testdata["midiFile"]
    midiFile = MidiFile(midifile)
    track1 = midiFile.tracks[1]

    _assert(track1[4].type,     'control_change',   '1th cc')
    _assert(track1[4].channel,  0,                  '1th cc ch')
    _assert(track1[4].control,  0,                  '1th cc nr')
    _assert(track1[4].value,    2,                  '1th cc value')
    _assert(track1[4].time,     0,                  '1th cc time')

    _assert(track1[5].type,     'control_change',   '2nd cc')
    _assert(track1[5].channel,  0,                  '2nd cc ch')
    _assert(track1[5].control,  32,                 '2nd cc nr')
    _assert(track1[5].value,    1,                  '2nd cc value')
    _assert(track1[5].time,     0,                  '2nd cc time')

    _assert(track1[6].type,     'program_change',   '1th pc')
    _assert(track1[6].channel,  0,                  '1th pc ch')
    _assert(track1[6].program,  0,                  '1th pc value')
    _assert(track1[6].time,     0,                  '1th pc time')

    _assert(track1[7].type,     'control_change',   '3th cc')
    _assert(track1[7].channel,  0,                  '3th cc ch')
    _assert(track1[7].control,  1,                  '3th cc nr')
    _assert(track1[7].value,    127,                '3th cc value')
    _assert(track1[7].time,     0,                  '3th cc time')

    _assert(track1[8].type,     'control_change',   '4th cc')
    _assert(track1[8].channel,  0,                  '4th cc ch')
    _assert(track1[8].control,  7,                  '4th cc nr')
    _assert(track1[8].value,    50,                 '4th cc value')
    _assert(track1[8].time,     0,                  '4th cc time')

    _assert(track1[9].type,     'control_change',   '5th cc')
    _assert(track1[9].channel,  0,                  '5th cc ch')
    _assert(track1[9].control,  10,                 '5th cc nr')
    _assert(track1[9].value,    63,                 '5th cc value')
    _assert(track1[9].time,     0,                  '5th cc time')

    _assert(track1[10].type,     'control_change',   '6th cc')
    _assert(track1[10].channel,  0,                  '6th cc ch')
    _assert(track1[10].control,  11,                 '6th cc nr')
    _assert(track1[10].value,    127,                '6th cc value')
    _assert(track1[10].time,     0,                  '6th cc time')

    _assert(track1[11].type,     'note_on',         '1th note on')
