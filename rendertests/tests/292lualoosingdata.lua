require "lua/com/com"
Velocity = 0.1

function perform(events, params, timeinfo)
    events[1].velocity = Velocity
    Velocity = Velocity + 0.1
    return events
end