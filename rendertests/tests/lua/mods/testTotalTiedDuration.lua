require "lua/com/com"

-- replacing the tied note event with a note with actual total length
function replaceWithTotalLength(events)
    local event = events[1]
    local atTieBegin = event.isTied and event.tiedDuration == event.totalTiedDuration
    local inTheMiddleOfATie = event.totalTiedDuration > 0 and event.tiedDuration ~= event.totalTiedDuration
    if atTieBegin then
        event.duration = event.totalTiedDuration
        event.isTied = false
        event.velocity = 1
    end
    if inTheMiddleOfATie then
        return {}  
    end
    return events
end

function perform(events, params, timeinfo)
    return replaceWithTotalLength(events)
end