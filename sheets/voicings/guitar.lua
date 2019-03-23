require "com/com"
require "com/globals"
require "solvers/guitar"

local solver = GuitarSolver:new()

function solve(chord, intervals, args)
    return solver:solve(chord, intervals, args)
end