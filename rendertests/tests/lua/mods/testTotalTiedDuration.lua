require "lua/com/com"

local tiedTotal = 4

function perform(events, params, timeinfo)
    assert(events[1].isTied == true)
    assert(events[1].totalTiedDuration == 4)
    assert(events[1].tiedDuration == tiedTotal)
    tiedTotal = tiedTotal - events[1].duration
    return events
end