#include "MidiFileWriter.h"
#include <fstream>
#include <fm/midi.hpp>
#include <boost/filesystem.hpp>

namespace fmapp {
    void MidiFileWriter::write(sheet::DocumentPtr)
    {
        using namespace std;
        std::string outfile = "unnamed.mid";
		if (_programOptions->isOutputSet()) {
			outfile = _programOptions->getOutput();
		}        
        else if (_programOptions->isInputSet()) {
            outfile = boost::filesystem::path(_programOptions->getInput()).filename().string() + ".mid";
        }
        ofstream fstream(outfile.c_str(), std::ios::binary);
	    _midifile->write(fstream);
	    fstream.flush();
        _logger->babble(WMLogLambda(log << "wrote " << _midifile->byteSize() << " bytes"));
    }
}

