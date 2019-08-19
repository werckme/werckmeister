require "lua/com/com"

function isLaidBack(timeinfo, args)
    local q = timeinfo.quarterPosition
    q = q - math.floor(q)
    if tonumber(args.grid) == 16 then
        return q == 0.25 or q == 0.75
    end
    return q == 0.5
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

function perform(event, args, timeinfo)
    args = tokeyvalue(args)
    if isLaidBack(timeinfo, args) then
        event.offset = offset(args)
        event.velocity = event.velocity * 0.5
    end
    return { event }
end