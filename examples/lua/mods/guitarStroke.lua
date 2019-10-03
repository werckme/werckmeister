require "lua/com/com"
require "events"

local direction = "up"
local mode = "alternate"

function perform(eventsOrigin, args, timeinfo)
    args = tokeyvalue(args)
    if #eventsOrigin > 2 then
        return eventsOrigin
    end
    local event = eventsOrigin[1]
    local events = { }
    local comparer = pitchCompare
    if args.mode ~=nil then
        mode = args.mode
    end
    if args.direction ~=nil then
        direction = args.direction
    end
    if direction == "down" then
        comparer = pitchCompareReversed
        if mode == "alternate" then
            direction = "up"
        end
    else
        comparer = pitchCompare
        if mode == "alternate" then
            direction = "down"
        end
    end
    local value = 64
    if args.value ~= nil then
        value = tonumber(args.value)
    end
    local duration = 1 / value
    table.sort( event.pitches, comparer)
    for i, pitch in pairs(event.pitches) do
        local note = Note:new()
        local newOffset = event.offset + (i-1) * duration
        note.offset = newOffset
        note.duration = event.duration - newOffset
        note.isTied = event.isTied
        note.velocity = event.velocity
        note:addPitch(pitch.pitch, pitch.octave)
        table.insert(events, note)   
    end
    return events
end