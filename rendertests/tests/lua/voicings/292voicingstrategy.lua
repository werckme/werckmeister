
PitchOffset = 0

function solve(chord, pitches, args)
    pitches[1].pitch = pitches[1].pitch + PitchOffset
    PitchOffset = PitchOffset + 1
    return pitches
end