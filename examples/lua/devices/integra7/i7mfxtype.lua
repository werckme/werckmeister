-- <command name="i7mfxtype" where="voicingStrategy" using="lua/voicings/inversion.lua">
-- set the mfx type for a part.
-- </command>
-- <param name="partId" optional="-1" type="1..16" type="number">the part number, will be detected via instrument channel if not set.</param>
-- <param name="typeNr" type="0..67" type="number">the mfx type</param>
-- <param name="toneType" type="[sna, sns, snd, pcms, pcmd]">the type of the tone on the target part</param>
-- <param name="deviceId" optional="1" type="16..31">an alternative device id</param>

require "lua/com/com"
require "_model"
require "_integra7"
local i7p = require "i7parameter"

parameters = {
    { name="partId", default=-1 },
    { name="typeNr" },
    { name="toneType" },
    { name="deviceId", default=16 },
}

local toneTypeMap = {
    sna = Mfx_Types.SNA,
    sns = Mfx_Types.SNS,
    snd = Mfx_Types.SND,
    pcms = Mfx_Types.PCMS,
    pcmd = Mfx_Types.PCMD
}


function execute(params, timeinfo, context)
    local part_ids = i7p.get_partids(params, context)
    local mfxType = toneTypeMap[params.toneType]
    if mfxType == nil then
        error("invalid tone type " .. params.toneType)
    end
    local typeNr = tonumber(params.typeNr)
    if typeNr < 0 or typeNr > 67 then
        error("typeNr out of bounds " .. typeNr)
    end
    local device_id = tonumber(params.deviceId)
    if device_id == nil or device_id < 16 or device_id > 31 then
        error("invalid device id: " .. device_id)
    end
    local messages = {}
    for _, part_id in pairs(part_ids) do
        local sysex = Get_Set_Mfx_Type_Messages(mfxType, part_id, typeNr, device_id)
        table.insert(messages, { -- set type
        ["type"] = "sysex",
        ["sysexData"] = sysex[1],
    })
    table.insert(messages, { -- set default values
        ["type"] = "sysex",
        ["sysexData"] = sysex[2],
    })
    end
    return messages
end

function perform(events, params, timeinfo, context)
    local messages = execute(params, timeinfo, context)
    for _, message in pairs(messages) do
        table.insert(events, message)
    end
    return events
end

return {execute = execute}