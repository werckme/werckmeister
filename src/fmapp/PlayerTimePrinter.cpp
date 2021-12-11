#include "PlayerTimePrinter.h"
#include <boost/format.hpp>
#include <iostream>
#include <com/config.hpp>

namespace fmapp {
    void PlayerTimePrinter::visit(com::Ticks elapsed)
    {
        using boost::format;
        using boost::str;
        using boost::io::group;

        std::string strOut = "[" + str(format("%.3f") % (elapsed / (double)com::PPQ)) + "]";
        clearLine();
        std::cout 
            << strOut 
            << std::flush;
        _lastOutput = strOut;
    }

    void PlayerTimePrinter::clearLine()
    {
        for (std::size_t i=0; i<_lastOutput.size()+1; ++i) {
            std::cout << "\b";
        }
    }

    void PlayerTimePrinter::loopEnd()
    {
        clearLine();
    }
}
