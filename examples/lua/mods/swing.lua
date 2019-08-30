require "lua/com/com"

function compareQuarters(a, b)
    return math.abs(a-b) <= 0.1
end

function isLaidBack(timeinfo, args)
    local q = timeinfo.quarterPosition
    q = q - math.floor(q)
    if tonumber(args.grid) == 16 then
        return compareQuarters(q, 0.25) or compareQuarters(q, 0.75)
    end
    return compareQuarters(q, 0.5)
end

function offset(args)
    local offset = tonumber(args.offset)
    if offset == nil then
        offset = 100
    end
    offset = offset / 100
    local max = 0.25
    if tonumber(args.grid) == 16 then
        max = 0.09
    end
    return offset * max
end

function perform(events, args, timeinfo)
    args = tokeyvalue(args)
    for i, event in pairs(events) do
        if isLaidBack(timeinfo, args) then
            event.offset = offset(args)
            event.velocity = event.velocity
        end
    end
    return events
end