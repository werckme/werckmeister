require "lua/com/com"
require "com"

local midiInputName = "Ampero"
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
    dump(loops)
    local midiInputs = GetMidiInputs()
    local input = FindInput(midiInputs, midiInputName)
    if input == nil then
        error("input '" .. midiInputName .. "' not found")
    end
    AddMidiInputListener(input.id, onMidiInput)
end

function OnMidiEvent(event)
    local cue = getCueText(event)
    local currentLoop = loopNames[currentLoopIdx]
    if cue == nil or cue ~= currentLoop then
        return
    end
    local loop = loops[cue]
    local loopBegin = loop[1]
    if event.position == loopBegin then
        return
    end
    JumpToPosition(loopBegin)
end