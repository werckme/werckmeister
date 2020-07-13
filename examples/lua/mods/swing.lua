-- <command name="swing" where="mod" using="lua/mod/swing.lua">
-- Performs a swing groove.
-- ```language=Werckmeister\n
--using "lua/mods/swing.lua";\n
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:  piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--instrument: piano;\n
--{\n
--   /mod: swing/\n
--   c8  c   c   c   c   c   c   c
--}\n
--]\n
-- ```
-- </command>
-- <param name="grid" optional="1" type="[8, 16]">Performs the swing offset either on 8th or 16th. (Default=8)</param>
-- <param name="offset" optional="1" type="0..100">The ammount of the swing offset</param>

require "lua/com/com"

parameters = {
    -- can be 8, 16
    { name="grid",      default="8" },
    -- can be between 0 and 100
    { name="offset",    default="100" }
}


function compareQuarters(a, b)
    return math.abs(a-b) <= 0.1
end

function isLaidBack(timeinfo, grid)
    local q = timeinfo.quarterPosition
    q = q - math.floor(q)
    if grid == 16 then
        return compareQuarters(q, 0.25) or compareQuarters(q, 0.75)
    end
    return compareQuarters(q, 0.5)
end

function offset(grid, offset)
    if offset == nil then
        offset = 100
    end
    offset = offset / 100
    local max = 0.25
    if grid == 16 then
        max = 0.09
    end
    return offset * max
end

function perform(events, params, timeinfo)
    checkLegacyNamedParams(params, "grid", "offset")
    local gridValue   = tonumber(params.grid)
    local offsetValue = tonumber(params.offset)
    for i, event in pairs(events) do
        if isLaidBack(timeinfo, gridValue) then
            event.offset = offset(gridValue, offsetValue)
            event.velocity = event.velocity
        end
    end
    return events
end