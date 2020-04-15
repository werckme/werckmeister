require "lua/com/com"

local defaultAmountValue = 50

function perform(events, args, timeinfo) 
    args = tokeyvalue(args)
    local tag = args.forTag
    local amount = defaultAmountValue
    if args.amount ~= nil then
        amount = tonumber(args.amount)
    end
    local factor = 100 - ((amount/100) * 90)
    factor = factor / 100
    for i, event in pairs(events) do
        if tag == nil or contains(event.tags, tag) then
            event.duration = event.duration * factor
        end
    end
    return events
end