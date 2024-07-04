require "_model"

local function iterate_node(node)
    for _, value in pairs(node) do
        print(value:tostring())
        if value.children ~= nil then
            iterate_node(value.children)
        end
    end
end

iterate_node(I7ROOT_NODE)