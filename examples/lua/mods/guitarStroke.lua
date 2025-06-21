-- <command name="guitarStroke" where="mod" using="lua/mod/guitarStroke.lua">
-- <![CDATA[
-- Simulates guitar strokes as mini arpeggios.
-- *Note: the alternate mode works only if this mod is set via instrumentConf.*
-- ```
-- -- does not work: \n
-- [ \n
-- { \n
--   /mod: guitarStroke _mode=alternate/\n
-- } \n
-- ] \n
-- ```
-- ```
-- -- does work: \n
-- instrumentConf: myInstrument mod guitarStroke _mode=alternate; \n
-- ```
-- **Example**
-- ```language=Werckmeister\n
--using "lua/mods/guitarStroke.lua";\n
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:  piano  MyDevice  _ch=0 _pc=24;\n
--instrumentConf: piano mod guitarStroke _mode=alternate _value=16; \n
--[\n
--instrument: piano;\n
--{\n
--  <c e g b>4 <c e g b>4 <c eb g bb>2 |  <c eb g bb>2 <c eb g bb>2\n
--}\n
--]\n
-- ```
-- ]]>
-- </command>
-- <param name="direction" optional="1" type="[up,down,alternate]">Specifies the start direction of the stroke</param>
-- <param name="value" optional="1" type="[1,2,4,8,...]">the duration of one arpeggio event. (Default=64)</param>
-- <param name="mode" optional="1" type="[normal,alternate]">Deprecated: Use _direction=alternate instead. Perform only one stroke direction (normal) or alternates between up and down. (Default=normal)</param>
-- <param name="withTag" optional="1" type="tagName">if set, only events including this tag will be processed.</param>

require "lua/com/com"
require "_events"

local NO_TAG = "WM_NO_TAG"

parameters = {
    -- can be up, down
    { name="direction",       default="up" },
    -- can be 1, 2, 4, 8, 16, 32, 64, ...
    { name="value",           default="64" },
    -- can be normal, alternate
    { name="mode",            default="normal" },
    { name="withTag",            default="WM_NO_TAG" }

}

local direction = nil
local mode = nil

function perform(eventsOrigin, params, timeinfo)
    local withTag = params.withTag
    local filtered_events
    if withTag ~= NO_TAG then
        filtered_events = FilterEventsByTagName(eventsOrigin, withTag)
    else
        filtered_events = eventsOrigin
    end
    if #eventsOrigin > 2 or #filtered_events == 0 then
        return eventsOrigin
    end
    checkLegacyNamedParams(params, "direction", "mode", "value")
    local directionParam = params.direction
    local modeParam      = params.mode
    local value          = params.value
    value = tonumber(value)
    local event = eventsOrigin[1]
    if event.pitches == nil then
        return  eventsOrigin
    end
    local events = { }
    local comparer = pitchCompare
    
    if modeParam == "alternate" then
        directionParam = "alternate"
    end

    if direction ==nil then
        direction = directionParam
    end

    if direction == "down" then
        comparer = pitchCompareReversed
        if directionParam == "alternate" then
            direction = "up"
        end
    else
        comparer = pitchCompare
        if directionParam == "alternate" then
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