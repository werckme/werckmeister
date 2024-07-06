require "_model"

local nodeinfo = Get_Node("xxx")
assert(nodeinfo == nil)

local nodeinfo = Get_Node("PRM-_FPART1")
assert(nodeinfo ~= nil)
assert(Nibble(0x19000000) == 52428800)
assert(nodeinfo.node.desc == "Temporary Tone (Studio Mode Part 1)");
assert(nodeinfo.node.addr == Nibble(0x19000000));

nodeinfo = Get_Node("PRM-_FPART1-_SNTONE-_SNTC-SNTC_MOD_PRM14");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.addr == Nibble(0x002F));
assert(nodeinfo.node.desc == "Modify Parameter 14");

nodeinfo = Get_Node("PRM-_SETUP-_STP-_RC2-NESTP_SND_MODE_SD1");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "Side 1 Sound Mode");

nodeinfo = Get_Node("PRM-_FPART1-_SNTONE-_SNTC-SNTC_TONE_LEVEL");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "Tone Level");
nodeinfo.node:setvalue(127)
local sysex = Create_SysexMessage(nodeinfo)