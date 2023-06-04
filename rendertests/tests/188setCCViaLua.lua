parameters = {
    -- specify a tag name. If set only events with this tag name will be affected by the staccato mod. 
    { name="modulationValue", default=0 },
    { name="offset", default=0 },
}


function perform(events, params, timeinfo)
    local value = tonumber(params["modulationValue"])
    local offset = tonumber(params["offset"])
    table.insert(events, {
        ["type"] = "cc",
        ["ccNr"] = 1,
        ["ccValue"] = value,
        ["offset"] = offset
    })
    return events
end