require "lua/com/com"
require "_events"


parameters = {
    -- can be legato, normal
    { name="style",           default="normal" },
    -- can be up, down
    { name="direction",       default="up" }
}

function perform(eventsOrigin, params, timeinfo)
    checkLegacyNamedParams(params, "direction", "style")
    local event     = eventsOrigin[1]
    local events    = { }
    local comparer  = pitchCompare
    local direction = params.direction
    local style     = params.style

    if direction == "down" then
        comparer = pitchCompareReversed
    end
    local duration = event.duration / #event.pitches
    table.sort( event.pitches, comparer)
    for i, pitch in pairs(event.pitches) do
        local note = Note:new()
        note.duration = duration
        if style == "legato" then
            note.duration = event.duration
        end
        note.velocity = event.velocity
        note.offset = (i-1) * duration
        note.isTied = event.isTied
        note:addPitch(pitch.pitch, pitch.octave)
        table.insert(events, note)   
    end
    return events
end