require "com"
local inspect = require "inspect"


function createPitch(chord, interval, octave)
    return { ["pitch"]= chord.rootPitch + (interval.interval % 12), ["octave"]=octave }
end


function solve(chord, intervals, args)
    local i = intervals
    if chord:has7() and chord:has9() and chord:has11() and chord:has13()
    then
        return {
            createPitch(chord, i[I  ], -1)
           ,createPitch(chord, i[IV ], -1)
           ,createPitch(chord, i[VII], -1)
           ,createPitch(chord, i[II ],  0)
           ,createPitch(chord, i[VI ],  0)
       }
    end   
    if chord:has7() and chord:has9() and chord:has11()
    then
        return {
            createPitch(chord, i[I  ], -1)
           ,createPitch(chord, i[IV ], -1)
           ,createPitch(chord, i[VII], -1)
           ,createPitch(chord, i[II ],  0)
           ,createPitch(chord, i[V  ],  0)
       }
    end    
    if chord:has7() and chord:has9()
    then
        return {
            createPitch(chord, i[I  ], -1)
           ,createPitch(chord, i[III], -1)
           ,createPitch(chord, i[VII], -1)
           ,createPitch(chord, i[II ],  0)
           ,createPitch(chord, i[V  ],  0)
       }
    end 
    if chord:has7()
    then
        return {
            createPitch(chord, i[I  ], -1)
           ,createPitch(chord, i[V  ], -1)
           ,createPitch(chord, i[VII], -1)
           ,createPitch(chord, i[III],  0)
           ,createPitch(chord, i[V  ],  0)
           ,createPitch(chord, i[I  ],  1)
       }
    end
    return {
        createPitch(chord, i[I  ], -1)
       ,createPitch(chord, i[V  ], -1)
       ,createPitch(chord, i[I  ],  0)
       ,createPitch(chord, i[III],  0)
       ,createPitch(chord, i[V  ],  0)
       ,createPitch(chord, i[I  ],  1)
    }
end