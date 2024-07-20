require "_model"

local parameter_partId_placeholder = "xxx"

function Create_Sysex_Message_For(part_id, parameter_node_id_template, value)
    local node_id = string.gsub(parameter_node_id_template, parameter_partId_placeholder, part_id)
    local nodeinfo = Get_Node(node_id)
    if nodeinfo == nil then
        error("no node found for id:" .. node_id)
    end
    nodeinfo.node:setvalue(value)
    local sysex = Create_SysexMessage(nodeinfo)
    return sysex
end

