require "/lua/com/com"

local function checkinstrument (instrument)
    if instrument == nil then
        return false
    end
    if instrument.name == nil then
        return false
    end
    if instrument.children ~= nil then
        for _, child in pairs(instrument.children) do
            return checkinstrument(child)
        end
    end
    if instrument.midiChannel == nil then
        return false
    end
    return true
end

function perform(events, params, timeinfo)
    if checkinstrument(events[1].instrument) == false then
        return {}
    end
    return events
end