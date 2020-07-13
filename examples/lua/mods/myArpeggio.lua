-- <command name="myArpeggio" where="mod" using="lua/mod/myArpeggio.lua">
-- <![CDATA[
-- A simple arpeggio implementation.
-- **Example**
-- ```language=Werckmeister\n
--using "lua/mods/myArpeggio.lua";\n
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:  piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--instrument: piano;\n
--{\n
-- /do: myArpeggio/ \n
-- <c e g b>2 \n
-- /do: myArpeggio _direction=down/ \n
-- <c eb g bb>2 \n
--}\n
--]\n
-- ```
-- ]]>
-- </command>
-- <param name="style" optional="1" type="[normal,legato]">Performs the arpeggio normal or legato</param>
-- <param name="direction" optional="1" type="[up,down]">the direction of the aprgeggio. (Default=up)</param>

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