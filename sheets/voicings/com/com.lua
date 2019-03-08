local _inspect = require "com/inspect"

function istable(t) return type(t) == 'table' end
function isnumber(t) return type(t) == 'number' end
function inspect(x) return _inspect(x) end
function dump(x) print(inspect(x)) end