require "lua/com/com"

function offset(pitches, pitch, octave)
    for i, x in pairs(pitches) do
        x.pitch = x.pitch + pitch
        x.octave = x.octave + octave
    end
end

function perform(event, args, timeinfo)
    if event.isTied == true then
        return { event }
    end
    local copy = deepcopy(event)
    offset(copy.pitches, 1, 0)
    copy.offset = 0.5
    copy.velocity = copy.velocity * 0.9
    event.duration = event.duration / 2
    local result = { event, copy }
    return result
end