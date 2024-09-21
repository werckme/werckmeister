
local date_key = "count"

parameters = {
    {name="when", default="somewhen"}
}

function execute(params, timeinfo, context)
    local count = context:getDate(date_key)
    if count == nil then
        count = 0
    else
        count = tonumber(count)
    end
    if params.when == "finito" then
        assert(count == 1)
        return {}
    end
    count = count + 1
    context:setDate(date_key, count)
    return {}
end