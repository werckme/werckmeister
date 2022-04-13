require "lua/com/com"
Velocity = 0.1

function perform(events, params, timeinfo)
    --print(events[1].voiceId .. " " .. Velocity .. " " .. events[1].pitches[1].pitch)
    events[1].velocity = Velocity
    Velocity = Velocity + 0.1
    return events
end