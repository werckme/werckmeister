require "lua/com/com"
require "lua/com/globals"
require "solvers/guitar"

local solver = GuitarSolver:new()

function solve(chord, degrees, args, timeinfo)
    return solver:solve(chord, degrees, args)
end