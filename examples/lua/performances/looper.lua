require "lua/com/com"
require "com"

local midiInputName = "Ampero"
local looping = true

local function onMidiInput(midiEvent)
    DumpEvent(midiEvent)
    looping = false
end


function Init()
    local midiEvents = GetMidiEvents()
    for _, track in ipairs(midiEvents) do
        --print(track.name, #track.events)
    end

    local midiInputs = GetMidiInputs()
    local input = FindInput(midiInputs, midiInputName)
    if input == nil then
        error("input '" .. midiInputName .. "' not found")
    end
    AddMidiInputListener(input.id, onMidiInput)
end

function OnMidiEvent(event)
    if event.type == NoteOn then
        print(event.position)
    end
    if event.type == MetaEvent and event.metaType == MetaCuePoint and looping then
        JumpToPosition(0)
    end
end