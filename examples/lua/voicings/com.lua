require "lua/com/com"

PitchRanges = {
    contrabass   = { min=28, max=55 },
    bass         = { min=40, max=60 },
    baritone     = { min=45, max=69 },
    tenor        = { min=48, max=72 },
    alto         = { min=53, max=77 },
    mezzosoprano = { min=57, max=81 },
    soprano      = { min=60, max=84 }
}

NoRangeSet = "__noRangeSet"
NoImportantDegreesSet = "_noImportantDegreesSet"
NoNumberSet = "__noNumberSet"

SolverDefaultParameter = {
    -- can be contrabass, bass, baritone, tenor, alto, mezzosoprano, soprano
    { name="range",             default=NoRangeSet },
    { name="importantDegrees",  default=NoImportantDegreesSet },
    { name="importantDegree",   default=NoImportantDegreesSet },
}

function CheckForLegacyParameters(params)
    checkLegacyNamedParams(params, "range", "importantDegrees", "importantDegree")
end

function Transpose(pitches, numOctaves)
    for i, pitch in pairs(pitches) do
        pitch.octave = pitch.octave + numOctaves
    end
end

function KeepRange(pitches, strRange)
    if strRange == nil or strRange ==NoRangeSet then
        return;
    end
    local destinationRange = PitchRanges[strRange]
    if destinationRange == nil then
        error("strRange " .. strRange .. " not defined")
    end
    local nbUnder = 0
    local nbOver = 0
    local mindiff = math.maxinteger
    local maxdiff = math.mininteger
    for i, pitch in pairs(pitches) do
        local midipitch = toMidiPitch(pitch)
        if midipitch > destinationRange.max then
            local diff = midipitch - destinationRange.max
            if diff > maxdiff then
                maxdiff = diff
            end
            nbOver = nbOver + 1
        end
        if midipitch < destinationRange.min then
            local diff = destinationRange.min - midipitch
            if diff < mindiff then
                mindiff = diff
            end
            nbUnder = nbUnder + 1
        end
    end
    if nbUnder > nbOver then
        local octaves = math.ceil(mindiff/12)
        Transpose(pitches, octaves)
    end
    if nbUnder < nbOver then
        local octaves = -math.ceil(maxdiff/12)
        Transpose(pitches, octaves)
    end
end