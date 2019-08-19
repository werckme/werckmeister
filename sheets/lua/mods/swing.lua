require "lua/com/com"

function isAccent(timeinfo)
    local q = timeinfo.quarterPosition
    return q - math.floor(q) == 0.5
end

function perform(event, args, timeinfo)
    inspect(event)
    if isAccent(timeinfo) then
        event.offset = 0.2
    end
    return { event }
end