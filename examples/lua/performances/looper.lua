--[[
    # Looper Script Example Implementation

    This script implements an automatic loop sequencer that cycles through 
    predefined loop sections marked with cue points in your MIDI sheet.

    ## Setup and Usage

    1. Mark loop sections in your sheet using cue points e.g. `/cue:"loop_1"/`, `/cue:"loop_2"/`, 
    etc. Each loop needs a start and end cue point with the same name.
    
    2. Set `midiInputName` to match your MIDI controller device.

    3. During playback:
    - start the playback: `sheetp --script=<<path_to_this_file>> <<path_to_your_sheet_file>>
    - The script starts with the first loop
    - Send MIDI events from your controller to advance to the next loop
    - Playback automatically jumps back to the loop start when reaching the end
    - After the last loop, playback returns to the beginning

    This is useful for practicing sections repeatedly or creating dynamic live 
    performances with controlled progression through your arrangement.
--]]

require "lua/com/com"
require "com"

local midiInputName = "MY_MIDI_INPUT_DEVICE_NAME"
local loops = {}
local loopNames = {}
local currentLoopIdx = 1

local function onMidiInput(midiEvent)
    DumpEvent(midiEvent)
    if currentLoopIdx >= (#loopNames + 1) then
        JumpToPosition(0)
    end
    currentLoopIdx = currentLoopIdx + 1
end

local function getCueText(event)
    if event.type ~= MetaEvent or event.metaType ~= MetaCuePoint then
        return nil
    end
    local cue = event.data
    if not BeginsWith(cue, "loop_") then
        return nil
    end
    return cue
end

local function handleCue(event)
    local cue = getCueText(event)
    if cue == nil then
        return
    end
    if loops[cue] == nil then
        loops[cue] = { event.position }
        table.insert(loopNames, cue)
        return
    end
    table.insert(loops[cue], event.position)
end

function Init()
    local midiEvents = GetMidiEvents()
    for _, track in ipairs(midiEvents) do
        for _, event in ipairs(track.events) do
            handleCue(event)
        end
    end
    local midiInputs = GetMidiInputs()
    local input = FindInput(midiInputs, midiInputName)
    if input == nil then
        local availableInputs = ""
        for _, inp in ipairs(midiInputs) do
            availableInputs = availableInputs .. inp.name .. "\n" 
        end
        error("input '" .. midiInputName .. "' not found. Available Inputs are:\n" .. availableInputs)
    end
    AddMidiInputListener(input.id, onMidiInput)
end

function OnMidiEvent(event)
    local cue = getCueText(event)
    local currentLoop = loopNames[currentLoopIdx]
    if cue == nil or cue ~= currentLoop then
        return event
    end
    local loop = loops[cue]
    local loopBegin = loop[1]
    if event.position == loopBegin then
        return event
    end
    JumpToPosition(loopBegin)
    return event
end