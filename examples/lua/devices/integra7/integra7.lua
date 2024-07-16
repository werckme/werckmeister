-- <command name="integra7" where="mod" using="lua/devices/integra7/integra7.lua">
-- creates integra7 parameter sysex messages
-- </command>
-- <param name="parameterId" optional="0" type="string">the parameter (node) id. As placeholder for the partId can `xxx` be used. For exampe `PRM-_PRF-_FPxxx-NEFP_OUT_ASGN`</param>
-- <param name="value" optional="0" type="number|string">the value</param>
-- <param name="partId" optional="1" type="0..15">specifies the part id. If not set, will be determined using its related instrument channel(s), assuming that the channel never changes.</param>


require "/lua/devices/integra7/_model"
require "lua/com/com"

parameters = {
    -- { name="parameterId"},
    -- { name="value"},
    -- { name="partId", default=-1 },
}

local function get_partids(params)
    if params.partIdsFromInstrument ~= nil then
        return params.partIdsFromInstrument
    end
    if isnumber(params.partId) then
        return { tonumber(params.partId) }
    end
    local function get_id_from_instrument()
        
    end
end

function execute(params, timeinfo)
    local part_ids = get_partids(params);
    local value = tonumber(params.value)
    local node_id = string.format(NODE_ID_TEMPLATE, part_nr)

    local nodeinfo = Get_Node(node_id)
    if nodeinfo == nil then
        error("no node found for id:" .. node_id)
    end
    nodeinfo.node:setvalue(value)
    local sysex = Create_SysexMessage(nodeinfo)
    return {
        {
            ["type"] = "sysex",
            ["sysexData"] = sysex,
        }
    }
end

local function get_partids_from_instrument(events)
    
end

function perform(events, params, timeinfo)
    params.partIdsFromInstrument = get_partids_from_instrument(events)
    local midiMessage = execute(params, timeinfo)
    table.insert(events, midiMessage[1])
    return events
end