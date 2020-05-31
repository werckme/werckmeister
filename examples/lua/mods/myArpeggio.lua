require "lua/com/com"
require "events"


parameters = {
    -- @positional @legacy
    { name="directionKey",    default="direction" },
    -- @positional @legacy
    { name="directionValue",  default="up" },
    -- @positional @legacy
    { name="styleKey",        default="style" },
    -- @positional @legacy
    { name="styleValue",      default="normal" },
    -- @named
    { name="style",           default=NamedAlternativeWithDefultNoValue },
    -- @named
    { name="direction",      default=NamedAlternativeWithDefultNoValue }    
}

function perform(eventsOrigin, params, timeinfo)
    local event     = eventsOrigin[1]
    local events    = { }
    local comparer  = pitchCompare
    local direction = getNamedAlternative(params, "direction", "directionValue")
    local style     = getNamedAlternative(params, "style"    , "styleValue"    )
    print(direction, style)

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