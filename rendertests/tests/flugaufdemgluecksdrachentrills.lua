require "lua/com/com"

parameters = {}
-- http://www.bachmusicology.com/?page_id=159

local t = 0

function perform(events, params, timeinfo)
    local result = {}
    local oneWholeNote = 4 -- quarters
    local oneTrillNoteLength = oneWholeNote / 32 -- 1 == one quarter
    local event = events[1]
    local hasTrillTag = contains(event.tags, "tr")
    if hasTrillTag == false then
        return events
    end
    local isTiedNote = event.totalTiedDuration > 0
    local isNotAtTieBegin = isTiedNote and event.totalTiedDuration ~= event.tiedDuration
    if isNotAtTieBegin then
        return {}
    end
    local c = 0
    local eventDuration = event.duration
    if isTiedNote then
        eventDuration = event.totalTiedDuration
        event.isTied = false
    end
    for offset=0, eventDuration, oneTrillNoteLength do
        local isUpper = c % 2 == 0
        local newEvent = deepcopy(event)
        newEvent.duration = oneTrillNoteLength
        newEvent.offset = offset
        newEvent.velocity = newEvent.velocity - 0.1 - (0.1 * math.sin(t*0.5))
        if isUpper then
        local hasHalfToneTag = contains(event.tags, "1/2")
        local upperValue = 2
        if hasHalfToneTag then
            upperValue = 1
        end
        newEvent.pitches[1].pitch = newEvent.pitches[1].pitch + upperValue
        end
        table.insert(result, newEvent)
        c = c + 1
        t = t + 0.1
    end
    return result
end