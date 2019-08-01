require "solvers/asolver"
require "com/globals"

-- https://www.8notes.com 
--ChordMatrix = {
--    ["1234567"] = nil,
--    ["1010100"] = [I, V, I, III, I]
--}


GuitarSolver = ASolver:new()

function GuitarSolver:degreeToPitch(degreeDef, chord)
    return chord.rootPitch + (degreeDef.degreeValue % 12)
end

function GuitarSolver:_solveImpl(chord, degrees, args)
    local d = function(degreeValue) return self:getDefaultDegreeDef(degreeValue, degrees) end
    if chord:has7() and chord:has9() and chord:has11() and chord:has13()
    then
        return {
            self:createPitch(chord, d(I  ), -1)
           ,self:createPitch(chord, d(IV ), -1)
           ,self:createPitch(chord, d(VII), -1)
           ,self:createPitch(chord, d(II ),  0)
           ,self:createPitch(chord, d(VI ),  0)
       }
    end   
    if chord:has7() and chord:has9() and chord:has11()
    then
        return {
            self:createPitch(chord, d(I  ), -1)
           ,self:createPitch(chord, d(IV ), -1)
           ,self:createPitch(chord, d(VII), -1)
           ,self:createPitch(chord, d(II ),  0)
           ,self:createPitch(chord, d(V  ),  0)
       }
    end    
    if chord:has7() and chord:has9()
    then
        return {
            self:createPitch(chord, d(I  ), -1)
           ,self:createPitch(chord, d(III), -1)
           ,self:createPitch(chord, d(VII), -1)
           ,self:createPitch(chord, d(II ),  0)
           ,self:createPitch(chord, d(V  ),  0)
       }
    end 
    if chord:has7()
    then
        return {
            self:createPitch(chord, d(I  ), -1)
           ,self:createPitch(chord, d(V  ), -1)
           ,self:createPitch(chord, d(VII), -1)
           ,self:createPitch(chord, d(III),  0)
           ,self:createPitch(chord, d(V  ),  0)
           ,self:createPitch(chord, d(I  ),  1)
       }
    end
    return {
        self:createPitch(chord, d(I  ), -1)
       ,self:createPitch(chord, d(V  ), -1)
       ,self:createPitch(chord, d(I  ),  0)
       ,self:createPitch(chord, d(III),  0)
       ,self:createPitch(chord, d(V  ),  0)
       ,self:createPitch(chord, d(I  ),  1)
    }
end
