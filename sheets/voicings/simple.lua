require "com/com"

function createPitch(chord, interval)
    return { ["pitch"]= chord.rootPitch + interval.interval , ["octave"]=interval.octave }
end

function solve(chord, intervals)
    pitches = {}
    for degree, interval in pairs(intervals)
    do
        if isnumber(degree)
        then
            pitches[degree] = createPitch(chord, interval)
        end
    end
    return pitches
end