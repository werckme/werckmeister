require "com/com"
require "voicings/solvers/simple"

local solver = Simple:new()

PitchOffset = 0

function solve(chord, degrees, args)
    local result = solver:solve(chord, degrees, args)
    result[1].pitch = result[1].pitch + PitchOffset
    PitchOffset = PitchOffset + 1
    return result
end