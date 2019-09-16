require "simple"
require "lua/com/globals"

VoiceLeadSolver = Simple:new()

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

-- changes the octave of pitch, so that the interval
-- between pitch and targetPitch is minimal
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


-- compare pitches with a reference and adjust octaves
function VoiceLeadSolver:adjustOctaves(pitches, reference)
    for i, pitch in pairs(pitches) do
        local min = self:getPitchWithMinDistance(pitch, reference) 
        self:toPitchNextTo(pitch, min)
    end
    return pitches
end

function VoiceLeadSolver:_solveImpl(chord, degrees, args)
    local pitches = Simple:_solveImpl(chord, degrees, args)
    if #pitches < 2
    then
        return pitches
    end
    if self.previous ~= nil 
    then
        pitches = self:adjustOctaves(pitches, self.previous)
    end
    return pitches
end

function VoiceLeadSolver:solve(chord, degrees, args)
    args = tokeyvalue(args)
    local result = self:_solveImpl(chord, degrees, args)
    if args.range ~=nil then
        self:_keepRange(result, args.range)
    end
    self.previous = result
    return result
end