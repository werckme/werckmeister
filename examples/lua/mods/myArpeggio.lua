require "lua/com/com"
require "events"

function perform(eventsOrigin, args, timeinfo)
    local event = eventsOrigin[1]
    local events = { }
    args = tokeyvalue(args)
    local comparer = pitchCompare
    if args.direction == "down" then
        comparer = pitchCompareReversed
    end
    local duration = event.duration / #event.pitches
    table.sort( event.pitches, comparer)
    for i, pitch in pairs(event.pitches) do
        local note = Note:new()
        note.duration = duration
        if args.style == "legato" then
            note.duration = event.duration
        end
        note.velocity = event.velocity * i * (event.velocity / #event.pitches)
        note.offset = (i-1) * duration
        note.isTied = event.isTied
        note:addPitch(pitch.pitch, pitch.octave)
        table.insert(events, note)   
    end
    return events
end