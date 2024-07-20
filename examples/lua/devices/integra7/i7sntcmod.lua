-- <command name="i7sntcmod" where="voicingStrategy" using="lua/voicings/inversion.lua">
-- set a mod value for a sna tone type instrument
-- </command>
-- <param name="partId" optional="-1" type="1..16" type="number">the part number, will be detected via instrument channel if not set.</param>
-- <param name="modNr" type="1..32" type="number">the mod number.</param>
-- <param name="value" type="number">the value</param>
-- <param name="deviceId" optional="1" type="number">an alternative device id</param>

require "lua/com/com"
local i7p = require "i7parameter"

parameters = {
    { name="partId", default=-1 },
    { name="modNr"},
    { name="value"},
    { name="deviceId", default=17 },
}

local node_id_template = "PRM-_FPARTxxx-_SNTONE-_SNTC-SNTC_MOD_PRM%i"

function execute(params, timeinfo, context)
    local modNr = tonumber(params.modNr)
    if modNr < 1 or modNr > 32 then
        error("modNr out of bounds " .. modNr)
    end
    params.parameterId = string.format(node_id_template, modNr)
    return i7p.execute(params, timeinfo, context)
end

function perform(events, params, timeinfo, context)
    local messages = execute(params, timeinfo, context)
    for _, message in pairs(messages) do
        table.insert(events, message)
    end
    return events
end

return {execute = execute}