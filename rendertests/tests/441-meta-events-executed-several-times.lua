
local count = 0

function execute(params, timeinfo)
    count = count + 1
    print(count)
    return {}
end