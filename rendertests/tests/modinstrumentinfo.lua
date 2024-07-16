require "/lua/com/com"

local function checkinstrument (instrument)
    if instrument == nil then
        return false
    end
    if instrument.name == nil then
        return false
    end
    if #instrument.name == 0 then
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

function perform(events, params, timeinfo, context)
    local instrument = context:getCurrentInstrument()
    if checkinstrument(instrument) == false then
        error("instrument check failed")
    end
    return events
end

function execute(params, timeinfo, context)
    local instrument = context:getCurrentInstrument()
    if checkinstrument(instrument) == false then
        error("instrument check failed")
    end
    return {}
end