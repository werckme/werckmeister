require "lua/com/com"
require "solvers/simple"

local solver = Simple:new()

function solve(chord, degrees, args)
    return solver:solve(chord, degrees, args)
end