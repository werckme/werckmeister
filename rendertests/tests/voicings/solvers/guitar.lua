require "solvers/asolver"
require "com/globals"

GuitarSolver = ASolver:new()

function GuitarSolver:degreeToPitch(degreeDef, chord)
    return chord.rootPitch + (degreeDef.degreeValue % 12)
end

function GuitarSolver:_solveImpl(chord, degrees, args)
    local i = degrees
    if chord:has7() and chord:has9() and chord:has11() and chord:has13()
    then
        return {
            self:createPitch(chord, i[I  ], -1)
           ,self:createPitch(chord, i[IV ], -1)
           ,self:createPitch(chord, i[VII], -1)
           ,self:createPitch(chord, i[II ],  0)
           ,self:createPitch(chord, i[VI ],  0)
       }
    end   
    if chord:has7() and chord:has9() and chord:has11()
    then
        return {
            self:createPitch(chord, i[I  ], -1)
           ,self:createPitch(chord, i[IV ], -1)
           ,self:createPitch(chord, i[VII], -1)
           ,self:createPitch(chord, i[II ],  0)
           ,self:createPitch(chord, i[V  ],  0)
       }
    end    
    if chord:has7() and chord:has9()
    then
        return {
            self:createPitch(chord, i[I  ], -1)
           ,self:createPitch(chord, i[III], -1)
           ,self:createPitch(chord, i[VII], -1)
           ,self:createPitch(chord, i[II ],  0)
           ,self:createPitch(chord, i[V  ],  0)
       }
    end 
    if chord:has7()
    then
        return {
            self:createPitch(chord, i[I  ], -1)
           ,self:createPitch(chord, i[V  ], -1)
           ,self:createPitch(chord, i[VII], -1)
           ,self:createPitch(chord, i[III],  0)
           ,self:createPitch(chord, i[V  ],  0)
           ,self:createPitch(chord, i[I  ],  1)
       }
    end
    return {
        self:createPitch(chord, i[I  ], -1)
       ,self:createPitch(chord, i[V  ], -1)
       ,self:createPitch(chord, i[I  ],  0)
       ,self:createPitch(chord, i[III],  0)
       ,self:createPitch(chord, i[V  ],  0)
       ,self:createPitch(chord, i[I  ],  1)
    }
end