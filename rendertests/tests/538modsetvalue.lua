parameters = {
    {name="x", default="0"}
}

function perform(events, params, timeinfo)
    local x = tonumber(params.x)
    events[1].pitches[1].pitch = events[1].pitches[1].pitch + x
    return events
end