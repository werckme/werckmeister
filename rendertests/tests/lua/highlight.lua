parameters = {
    {name="instrument", default=""},
    {name="value", default="high"}
}


function execute(params, timeinfo, context)
    local text = "t:" .. params.instrument
    if params.value == "high" then
        text = text .. "_1"
    else
        text = text .. "_0"
    end
    return {
        {
            type = "meta",
            metaType = "cuePoint",
            metaValue = text
        }
    }
end