require "lua/com/com"

local defaultAmountValue = 50

function perform(events, args, timeinfo) 
    inspect(events)
    return events
    -- args = tokeyvalue(args)
    -- local amount = defaultAmountValue
    -- if args.amount ~= nil then
    --     amount = tonumber(args.amount)
    -- end
    -- local factor = 100 - ((amount/100) * 90)
    -- factor = factor / 100
    -- for i, event in pairs(events) do
    --     event.duration = event.duration * factor
    -- end
    -- return events
end