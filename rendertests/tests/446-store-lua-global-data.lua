require "lua/com/com"

parameters = {
    {name="value", default="nil"}
}

local key = "myValue"

function perform(events, param, timeinfo, context)
    context:setDate(key, param.value)
    return events
end

function execute(param, timeinfo, context)
    local value = context:getDate(key)
    assert(value == "101")
    return {}
end