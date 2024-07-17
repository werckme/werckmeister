-- <command name="integra7" where="mod" using="lua/devices/integra7/integra7.lua">
-- creates integra7 parameter sysex messages
-- </command>
-- <param name="parameterId" optional="0" type="string">the parameter (node) id. As placeholder for the partId can `xxx` be used. For exampe `PRM-_PRF-_FPxxx-NEFP_OUT_ASGN`</param>
-- <param name="value" optional="0" type="number|string">the value</param>
-- <param name="partId" optional="1" type="0..15">specifies the part id. If not set, will be determined using its related instrument channel(s), assuming that the channel never changes.</param>


require "/lua/devices/integra7/_model"
require "lua/com/com"

parameters = {
    { name="parameterId"},
    { name="value"},
    { name="partId", default=-1 },
}

Parameter_PartId_Placeholder = "xxx"

local function get_partids(params, context)
    if isnumber(params.partId) then
        return { tonumber(params.partId) }
    end
    local part_ids = {}
    local function get_id_from_instrument(instrument)
        if instrument.children ~= nil then
            for _, child in pairs(instrument.children) do
                get_id_from_instrument(child)
            end
            return
        end
        table.insert(part_ids, instrument.midiChannel + 1)
    end
    local instrument = context:getCurrentInstrument()
    get_id_from_instrument(instrument)
    return part_ids
end

function execute(params, timeinfo, context)
    local part_ids = get_partids(params, context)
    local messages = {}
    local value = tonumber(params.value)
    local node_id_template = params.parameterId
    for _, part_id in pairs(part_ids) do
        local node_id = string.gsub(node_id_template, Parameter_PartId_Placeholder, part_id)
        local nodeinfo = Get_Node(node_id)
        if nodeinfo == nil then
            error("no node found for id:" .. node_id)
        end
        nodeinfo.node:setvalue(value)
        local sysex = Create_SysexMessage(nodeinfo)
        table.insert(messages, {
            ["type"] = "sysex",
            ["sysexData"] = sysex,
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