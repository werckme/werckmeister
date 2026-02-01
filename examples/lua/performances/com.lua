NoteOff = 0x8
NoteOn = 0x9
NoteAftertouch = 0xA
Controller = 0xB
ProgramChange = 0xC
ChannelAftertouch = 0xD
PitchBend = 0xE
Sysex = 0xF0
MetaEvent = 0xFF


MetaSequenceNumber = 0
MetaTextEvent = 1
MetaCopyright = 2
MetaSequenceOrTrackName = 3
MetaInstrumentName = 4
MetaLyricText = 5
MetaMarkerText = 6
MetaCuePoint = 7
MetaMIDIChannelPrefixAssignment = 0x20
MetaEndOfTrack = 0x2F
MetaTempo = 0x51
MetaSMPTEOffset = 0x54
MetaTimeSignature = 0x58
MetaKeySignature = 0x59
MetaCustomMetaEvent = 0x7F

local function bytesToHex(str)
    local hex = {}
    for i = 1, #str do
        table.insert(hex, string.format("0x%02x", string.byte(str, i)))
    end
    return table.concat(hex, " ")
end

function DumpEvent(midiEvent)
    print("{")
    print(string.format("\tposition: %f", midiEvent.position))
    print(string.format("\ttype: 0x%x", midiEvent.type))
    print(string.format("\tchannel: 0x%x", midiEvent.channel))
    print(string.format("\tparameter1: 0x%x", midiEvent.parameter1))
    print(string.format("\tparameter2: 0x%x", midiEvent.parameter2))
    print(string.format("\tmetaType: 0x%x", midiEvent.metaType))
    print(string.format("\tdata: %s", bytesToHex(midiEvent.data)))
    print("}")
end

function FindInput(midiInputs, deviceName)
    for _, input in ipairs(midiInputs) do
        if input.name:lower():match(deviceName:lower()) ~= nil then
            return input
        end
    end
end

function BeginsWith(str, needle)
   return str.sub(needle,1,string.len(needle))==needle
end