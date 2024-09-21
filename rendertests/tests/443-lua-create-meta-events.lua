
parameters = {
    { name="text" },
}

function execute(params, timeinfo)
    return {
        {
            type="meta",
            metaType="cuePoint",
            metaValue=params.text
        }
    }
end