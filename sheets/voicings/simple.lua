require "com/com"
require "solvers/simple"

local solver = Simple:new()

function solve(chord, intervals, args)
    return solver:solve(chord, intervals, args)
end