require "lua/com/com"
require "events"

local direction = "up"

function perform(eventsOrigin, args, timeinfo)
    args = tokeyvalue(args)
    local event = eventsOrigin[1]
    local events = { }
    local comparer = pitchCompare
    if direction == "down" then
        comparer = pitchCompareReversed
        direction = "up"
    else
        comparer = pitchCompare
        direction = "down"
    end
    local value = 64
    if args.value ~= nil then
        value = tonumber(args.value)
    end
    local duration = 1 / value
    table.sort( event.pitches, comparer)
    for i, pitch in pairs(event.pitches) do
        local note = Note:new()
        note.offset = (i-1) * duration
        note.duration = event.duration - note.offset
        note.isTied = event.isTied
        note.velocity = event.velocity
        note:addPitch(pitch.pitch, pitch.octave)
        table.insert(events, note)   
    end
    return events
end