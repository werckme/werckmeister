ASolver = {}

function ASolver:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function ASolver:createPitch(chord, interval, octave)
    if interval == nil
    then
        print("warning: missing interval for: " .. chord.strBase .. chord.strOptions)
        return { ["pitch"]= 0, ["octave"]=0 }
    end
    return { ["pitch"]= self:intervalToPitch(interval, chord), ["octave"]=octave }
end

function ASolver:intervalToPitch(interval, chord)
    return chord.rootPitch + interval.interval
end

function ASolver:getAbsolutePitch(pitch)
    return pitch.pitch + (pitch.octave * 12)
end

function ASolver:solve(chord, intervals, args)
    return self:_solveImpl(chord, intervals, args)
end

function ASolver:_solveImpl(chord, intervals, args)
    error("this is an abstract function")
end