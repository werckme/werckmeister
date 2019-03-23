require "solvers/asolver"
require "com/globals"

VoiceLeadSolver = ASolver:new()

function VoiceLeadSolver:getPitchWithMinDistance(pitch, pitches)
    local min = math.maxinteger
    local minPitch = nil
    local relPitch = pitch.pitch % 12
    for i, refpitch in pairs(pitches) do
        local diff = relpitchdiff(refpitch.pitch, relPitch)
        diff = math.abs(diff)
        if (diff < min) then
            minPitch = refpitch
            min = diff
        end
    end
    return minPitch
end

function VoiceLeadSolver:toPitchNextTo(pitch, targetPitch)
    if targetPitch == nil then
        return pitch
    end
    local diff = pitch.pitch - targetPitch.pitch
    pitch.octave = targetPitch.octave
    --print(pitch.pitch, targetPitch.pitch, diff)
    if (diff > 6) then
        pitch.octave = pitch.octave - 1
    end
    if (diff < -6) then
        pitch.octave = pitch.octave + 1
    end
    return pitch
end

function VoiceLeadSolver:adjustOctaves(pitches, reference)
    for i, pitch in pairs(pitches) do
        local min = self:getPitchWithMinDistance(pitch, reference) 
        self:toPitchNextTo(pitch, min)
    end
    return pitches
end

function VoiceLeadSolver:_solveImpl(chord, intervals, args)
    local pitches = {}
    for degree, interval in pairs(intervals)
    do
        if isnumber(degree)
        then
            pitches[degree] = self:createPitch(chord, interval, interval.octave)
        end
    end
    if self.previous ~= nil 
    then
        pitches = self:adjustOctaves(pitches, self.previous)
    end
    self.previous = pitches
    return pitches
end