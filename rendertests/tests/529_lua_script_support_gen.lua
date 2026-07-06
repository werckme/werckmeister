require "./lua/com/com"

parameters = {
    { name="t1", default=0 },
}


function execute(params, timeinfo, context)
    local template1 = params.t1
    return {
        {
            type = "werckmeisterCommand",
            command = {
                id = "template",
                args = {
                   {
                        value = template1
                   }
                }
            }
        },
        {
            type = "chord",
            chordName = "C-",
            duration = 4
        },
        {
            type = "bar",
        },
        {
            type = "chord",
            chordName = "C-7",
            duration = 4
        },
        {
            type = "bar",
        },
        {
            type = "chord",
            chordName = "A#",
            duration = 4
        },
        {
            type = "bar",
        }
    }
end