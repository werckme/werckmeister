#include <com/common.hpp>
#include <vector>
#include <tuple>

namespace com
{
    typedef std::vector<std::tuple<com::String, int>> GmInstruments;
    extern const GmInstruments gmInstruments;
}