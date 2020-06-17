#include "PlayerTimePrinter.h"
#include <boost/format.hpp>
#include <iostream>
#include <fm/config.hpp>

namespace fmapp {
    void PlayerTimePrinter::visit(fm::Ticks elapsed)
    {
        using boost::format;
        using boost::str;
        using boost::io::group;

        std::string strOut = "[" + str(format("%.3f") % (elapsed / (double)fm::PPQ)) + "]";
        for (std::size_t i=0; i<_lastOutput.size(); ++i) {
            std::cout << "\b";
        }
        std::cout 
            << strOut 
            << std::flush;
        _lastOutput = strOut;
    }
}
