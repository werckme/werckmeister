require "lua/com/com"



function perform(events, args, timeinfo)
    args = tokeyvalue(args)
    for i, event in pairs(events) do
        event.duration = event.duration * 0.25
    end
    return events
end