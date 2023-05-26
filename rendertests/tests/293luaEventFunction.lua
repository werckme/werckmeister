require "lua/com/com"

parameters = {
    { name="octave", default=0 },
}

function CreateNote(octave, pitch, duration, tags)
    return {
        ["type"] = "note",
        ["velocity"] = 100,
        ["duration"] = duration,
        ["pitches"] = {
            {
                ["octave"] = octave,
                ["pitch"] = pitch,
            }
        },
        ["tags"] = tags
    }
end


function execute(params, timeinfo)
    local octave = params["octave"]
    return {
        CreateNote(octave, 1, 1, {"leise"}),
        CreateNote(octave, 2, 1, {"leise", "legato"}),
        CreateNote(octave, 3, 1, {}),
        {
            ["type"] = "rest",
            ["duration"] = 1,
        }
    }
end