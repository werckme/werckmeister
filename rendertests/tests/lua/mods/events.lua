
Note = {}

function Note:new()
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.duration = 0
    o.isTied = false
    o.offset = 0
    o.pitches = {}
    o.type = "note"
    o.velocity = 0
    return o 
end

function Note:addPitch(pitch, octave)
    table.insert(self.pitches, {pitch=pitch, octave=octave})
end

function pitchCompare(a, b)
    a = a.pitch + (a.octave * 12)
    b = b.pitch + (b.octave * 12)
    return a < b
end

function pitchCompareReversed(a, b)
    a = a.pitch + (a.octave * 12)
    b = b.pitch + (b.octave * 12)
    return a > b
end
