require "_model"

local function iterate_node(node)
    for _, value in pairs(node) do
        print(value:tostring())
        if value.children ~= nil then
            iterate_node(value.children)
        end
    end
end

--iterate_node(I7ROOT_NODE)
local noderesult = Get_Node("PRM-_FPART1")
assert(noderesult ~= nil)
print(noderesult.node.addr)
print(Nibble(0x19000000))
assert(noderesult.node.addr == Nibble(0x19000000));
assert(noderesult.node.desc == "Temporary Tone (Studio Mode Part 1)");