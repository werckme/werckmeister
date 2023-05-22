-- <command name="voicelead" where="voicingStrategy" using="lua/voicings/voicelead.lua">
-- <![CDATA[
-- Tries to simulates a voice leading approach:
-- the octaves of a chord are rearranged, so that the actual pitch distance between the previous chord is kept minimal.
-- ```language=Werckmeister\n
--using "lua/voicings/voicelead.lua";\n
--using "chords/default.chords";
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--type: template;\n
--name: myTemplate;\n
--instrument: piano;\n
--{\n
--   /voicingStrategy: voicelead/\n
--   <I II III IV V VI VII>1\n
--}\n
--]\n
--[\n
--type: accomp;
--{\n
--  C | F | G | C |\n
--}\n
--]\n
-- ```
-- ]]>
-- </command>
-- <param name="range" optional="1" type="[contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano]">If given the strategy tries to keeps the range by shifting the octave of the chord.</param>

require "lua/com/com"
require "lua/com/globals"
require "com"

parameters = {
    table.unpack(SolverDefaultParameter)
}

local previousResult = nil

local function GetPitchWithMinDistance(pitch, pitches)
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
local function ChangeOctaveToPitchNextTo(pitch, targetPitch)
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
local function AdjustOctaves(pitches, reference)
    for _, pitch in pairs(pitches) do
        local min = GetPitchWithMinDistance(pitch, reference)
        ChangeOctaveToPitchNextTo(pitch, min)
    end
    return pitches
end

local function SolveImpl(pitches)
    if #pitches < 2
    then
        return pitches
    end
    if previousResult ~= nil
    then
        pitches = AdjustOctaves(pitches, previousResult)
    end
    return pitches
end

function solve(chord, pitches, parameters)
    CheckForLegacyParameters(parameters)
    SolveImpl(pitches)
    KeepRange(pitches, parameters.range)
    previousResult = pitches
    return pitches
end