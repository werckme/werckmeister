require "com/com"
require "com/globals"
require "solvers/guitar"

local solver = GuitarSolver:new()

function solve(chord, degrees, args, timeinfo)
    inspect(timeinfo)
    return solver:solve(chord, degrees, args)
end