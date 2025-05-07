parameters = {
    {name="instrument", default=""},
}


function execute(params, timeinfo, context)
    local textHigh = "t:" .. params.instrument .. "_1"
    local textLow = "t:" .. params.instrument .. "_0"
    return {
        {
            type = "meta",
            metaType = "cuePoint",
            metaValue = textHigh
        },
        {
            type = "meta",
            metaType = "cuePoint",
            metaValue = textLow,
            offset = 4
        }
    }
end