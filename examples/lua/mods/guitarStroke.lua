-- note: the alternate mode works only if this mod is set via instrumentConf.
-- e.g.
-- instrumentConf: myInstrument
--     _set=mod _use=guitarStroke _mode=alternate
-- ;

require "lua/com/com"
require "events"


parameters = {
    -- @positional @legacy
    { name="directionKey",    default="direction" },
    -- @positional @legacy
    { name="directionValue",  default="up" },
    -- @positional @legacy
    { name="modeKey",         default="mode" },
    -- @positional @legacy
    { name="modeValue",       default="alternate" },
    -- @positional @legacy
    { name="valueKey",        default="value" },
    -- @positional @legacy
    { name="valueValue",      default="64" },
    -- @named
    { name="direction",       default=NamedAlternativeWithDefultNoValue },
    -- @named
    { name="value",           default=NamedAlternativeWithDefultNoValue },
    -- @named
    { name="mode",            default=NamedAlternativeWithDefultNoValue }

}


local direction = nil
local mode = nil

function perform(eventsOrigin, params, timeinfo)
    if #eventsOrigin > 2 then
        return eventsOrigin
    end

    local directionParam = getNamedAlternative(params, "direction", "directionValue")
    local modeParam      = getNamedAlternative(params, "mode"     , "modeValue"    )
    local value          = getNamedAlternative(params, "value"    , "valueValue"    )
    value = tonumber(value)
    print(value)
    local event = eventsOrigin[1]
    local events = { }
    local comparer = pitchCompare
    
    if mode == nil then
        mode = modeParam
    end

    if direction ==nil then
        direction = directionParam
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