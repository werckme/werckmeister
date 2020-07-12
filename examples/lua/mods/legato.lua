-- <command name="legato" where="mod" using="lua/mod/legato.lua">
-- Performs every note legato. It is also possible to tag single notes.
-- If a tag is given, only the tagged notes are performed legato.
-- ```language=Werckmeister\n
--using "lua/mods/legato.lua";\n
--tempo: 140;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--instrument: piano;\n
--{\n
--   /mod: legato _forTag=leg _amount=100/\n
--   c "leg"@d e f\n
--}\n
--]\n
-- ```
-- *(only the `d` will be performed legato.)* 
-- </command>
-- <param name="forTag" optional="1" type="word">Specifies a tag name. If set only events with this tag name will be affected by the legato mod.</param>
-- <param name="amount" optional="1" type="0..100">The ammount of the legato effect</param>

require "lua/com/com"

local defaultAmountValue = 50

parameters = {
    -- specify a tag name. If set only events with this tag name will be affected by the staccato mod. 
    { name="forTag", default=ParameterValueNoTag },
    -- the ammount of the staccato mod. Can be between 0 and 100
    { name="amount",           default="50" }
}

function perform(events, params, timeinfo) 
    checkLegacyNamedParams(params, "forTag", "amount")
    local tag = params.forTag
    if tag == ParameterValueNoTag then
        tag = nil
    end
    local amount = tonumber(params.amount)
    local factor = 100 - ((amount/100) * 90)
    factor = factor / 100
    for i, event in pairs(events) do
        if tag == nil or contains(event.tags, tag) then
            event.duration = event.duration / factor
        end
    end
    return events
end