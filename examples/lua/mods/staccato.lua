-- <command name="staccato" where="mod" using="lua/mod/staccato.lua">
-- Performs every note staccato. It is also possible to tag single notes.
-- If a tag is given, only the tagged notes are performed staccato.
-- ```
-- /mod: staccato _forTag="stac"/\n
-- c d e "stac"@f -- only f will be performed staccato\n
-- ```
-- </command>
-- <param name="forTag" optional="1" type="word">Specifies a tag name. If set only events with this tag name will be affected by the staccato mod.</param>
-- <param name="amount" optional="1" type="0..100">The ammount of the staccato</param>

require "lua/com/com"


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
            event.duration = event.duration * factor
        end
    end
    return events
end