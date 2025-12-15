require "lua/com/com"

params = {}

local i = 0
function perform(events, params, timeinfo)
    if i <= 6  and timeinfo.barPosition == 0 then
        i = i + 1
        return events
    end
    if i > 6  and i <= 8 and timeinfo.barPosition == 4 then
        i = i + 1
        return events
    end
    if i == 9 and timeinfo.barPosition == 8 then
        i = i + 1
        return events
    end
    i = i + 1
    return {}
end

function CreateEvent(octave, pitch, duration)
    return {
        ["type"] = "note",
        ["velocity"] = 100,
        ["duration"] = duration,
        ["pitches"] = {
            {
                ["octave"] = octave,
                ["pitch"] = pitch,
            }
        }
    }
end

local j = 0
function execute(params, timeinfo)
    j = j + 1
    if j == 1 or j == 2 and timeinfo.barPosition == 0 then
        return { CreateEvent(-2, timeinfo.barPosition, 2)}
    end
    if j == 3 or j == 4 and timeinfo.barPosition == 4 then
        return { CreateEvent(-2, timeinfo.barPosition, 2)}
    end 
    if j == 5 or j == 6 and timeinfo.barPosition == 8 then
        return { CreateEvent(-2, timeinfo.barPosition, 2)}
    end        
    return {}
end