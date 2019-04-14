local _inspect = require "com/inspect"

local MidiSchluesselCOffset = 60;

function istable(t) return type(t) == 'table' end
function isnumber(t) return type(t) == 'number' end
function inspect(x) return _inspect(x) end
function dump(x) print(inspect(x)) end

-- returns the minimum amount of semitones between the relative pitches x and y.
-- e.g.: from c(0) to b(11) = -1
function relpitchdiff(x, y)
    x = x % 12
    y = y % 12
    if math.abs(x-y) < 6 then
        return -(x-y)
    end
    if x < 6 then 
        x = x + 12 
    end
    if y < 6 then
        y = y + 12
    end
    return -(x-y)
end

-- [a, 1, b, 2, c, 3] => {a=1, b=2, c=3}
function tokeyvalue(args)
    assert(#args % 2 == 0)
    local result = {}
    for i=1, #args, 2 do
        result[ args[i] ] = args[i+1]
    end
    return result
end

function toMidiPitch(pitch)
    return MidiSchluesselCOffset + pitch.pitch + (pitch.octave * 12)
end
    