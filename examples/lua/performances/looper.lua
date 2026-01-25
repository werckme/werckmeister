print("tach")

function OnMidiEvent(event)
    if event.position >=4 then
        JumpToPosition(0)
    end
end