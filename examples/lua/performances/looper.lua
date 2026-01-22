print("tach")

function OnMidiEvent(event)
    if event.position >=4.1 then
        JumpToPosition(0)
    end
end