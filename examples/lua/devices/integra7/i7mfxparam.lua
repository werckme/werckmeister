-- <command name="i7mfxtype" where="voicingStrategy" using="lua/voicings/inversion.lua">
-- set the value for a mfx parameter.
-- </command>
-- <param name="partId" optional="-1" type="1..16" type="number">the part number, will be detected via instrument channel if not set.</param>
-- <param name="paramNr" type="1..32" type="number">the mfx parameter number</param>
-- <param name="toneType" type="[sna, sns, snd, pcms, pcmd]">the type of the tone on the target part</param>
-- <param name="value" type="number">the value</param>
-- <param name="deviceId" optional="1" type="16..31">an alternative device id</param>

require "lua/com/com"
require "_model"
require "_integra7"
local i7p = require "i7parameter"

parameters = {
    { name="partId", default=-1 },
    { name="paramNr" },
    { name="toneType" },
    { name="value" },
    { name="deviceId", default=16 },
}

local toneTypeMap = {
    sna = Mfx_Types.SNA,
    sns = Mfx_Types.SNS,
    snd = Mfx_Types.SND,
    pcms = Mfx_Types.PCMS,
    pcmd = Mfx_Types.PCMD
}

local mfx_param_templates = {
    SNA = Mfx_Ids[Mfx_Types.SNA] .. "-_SNTF-SNTF_MFX_PRM%i",
    SNS = Mfx_Ids[Mfx_Types.SNS] .. "-_SHPF-SHPF_MFX_PRM%i",
    SND = Mfx_Ids[Mfx_Types.SND] .. "-_KF-SDKF_MFX_PRM%i",
    PCMS = Mfx_Ids[Mfx_Types.PCMS] .. "-RFPF_MFX_PRM%i",
    PCMD = Mfx_Ids[Mfx_Types.PCMD] .. "-RFRF_MFX_PRM%i"
}


function execute(params, timeinfo, context)
    local mfxType = toneTypeMap[params.toneType]
    if mfxType == nil then
        error("invalid tone type " .. params.toneType)
    end
    local parameterNr = tonumber(params.paramNr)
    if parameterNr == nil or parameterNr < 1 or parameterNr > 32 then
        error("invalid parameterNr " .. params.paramNr)
    end
    params.parameterId = string.format(mfx_param_templates[mfxType], parameterNr)
    local events = i7p.execute(params, timeinfo, context)
    return events
end

function perform(events, params, timeinfo, context)
    local messages = execute(params, timeinfo, context)
    for _, message in pairs(messages) do
        table.insert(events, message)
    end
    return events
end

return {execute = execute}