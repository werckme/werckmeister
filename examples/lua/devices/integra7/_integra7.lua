require "_model"
require "_mfx"

local parameter_partId_placeholder = "xxx"

local function create_id(parameter_node_id_template, part_id)
    return string.gsub(parameter_node_id_template, parameter_partId_placeholder, part_id)
end

function Create_Sysex_Message_For_Node(part_id, parameter_node_id_template, value, device_id)
    local node_id = create_id(parameter_node_id_template, part_id)
    local nodeinfo = Get_Node(node_id)
    if nodeinfo == nil then
        error("no node found for id:" .. node_id)
    end
    nodeinfo.node:setvalue(value)
    local sysex = Create_SysexMessage(nodeinfo, device_id)
    return sysex
end

local function get_set_mfx_default_values_sysex(mfxType, part_id, mfx_id, device_id)
    local prm = Mfx_Table[mfx_id + 1]
    local payload = {}
    local from = -1
    local nodes = Mfx_Nodes[mfxType]
    for i = 1, #nodes, 1 do
        local node = nodes[i]
        if node.pos == nil then goto continue end
        if from < 0 then from = i end
        local default_value_index = i - from + 1
        if default_value_index <= #prm.leaf then
            local default_value = prm.leaf[default_value_index].init
            local bytesize = Get_Byte_Size(node.valueByteSizeType)
            local bytes = Value_To_Bytes(default_value, bytesize)
            payload = ConcatTable(payload, bytes)
        end
    ::continue::
    end
    local idtemplate = Mfx_Ids[mfxType] .. "-" .. nodes[from].id
    local id = create_id(idtemplate, part_id)
    local addr = Get_Adress(id)
    local sysex = Create_Sysex_Message_For_Payload(addr, payload, device_id)
    return sysex
end

function Get_Set_Mfx_Type_Messages(mfxType, part_id, mfx_id, device_id)
    local nodeid_template = Mfx_Type_Ids[mfxType]
    return {
        Create_Sysex_Message_For_Node(part_id, nodeid_template, mfx_id, device_id),
        get_set_mfx_default_values_sysex(mfxType, part_id, mfx_id, device_id)
    }
end