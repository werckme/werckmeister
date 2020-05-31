require "lua/com/com"
require "events"


parameters = {
    -- @positional
    { name="directionKey",    default="direction" },
    -- @positional
    { name="directionValue",  default="up" },
    -- @positional
    { name="styleKey",        default="style" },
    -- @positional
    { name="styleValue",      default="normal" },
    -- @named @alternative
    { name="style",           default="normal" },
    -- @named @alternative
    { name="direction",      default="up" },    
}

function perform(eventsOrigin, params, timeinfo)
    --dump(params)
    local event     = eventsOrigin[1]
    local events    = { }
    local comparer  = pitchCompare
    local direction = params.direction ~= nil and params.directionValue
    local style     = params.style ~= nil and params.styleValue
    --print(params.direction, params.style)

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