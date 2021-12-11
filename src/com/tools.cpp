#include "tools.h"
#include <sheet/objects/Event.h>
#include <sheet/Document.h>
#include <com/werckmeister.hpp>


namespace com {

        namespace {
			auto _lineAndPos(const com::String &file, unsigned int sourcePos)
			{
				if (sourcePos == sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
					sourcePos = 0;
				}
				try {
					auto source = com::getWerckmeister().openResource(file);
					com::StreamBuffIterator end;
					com::StreamBuffIterator begin(*source.get());
					com::String sourceStr(begin, end);
					auto result = getLineAndPosition(sourceStr, sourcePos, false);
					auto lineNr = getNumberOfLines(sourceStr, sourcePos);
					return std::make_tuple(std::get<0>(result), std::get<1>(result), lineNr);
				} catch(...) {
					return std::make_tuple(com::String(), -1, -1);
				}
			}
		}

    void argumentsToParameters(const std::vector<sheet::Argument> arguments, IHasParameter::ParametersByNames &outParameters)
    {
        if (arguments.empty()) {
            return;
        }
        for (int position = 0; position < (int)arguments.size(); ++position)
        {
            const auto &argument = arguments[position];
            bool byPosition = argument.name.empty();
            bool byName = !byPosition;
            IHasParameter::ParametersByNames::iterator parameterIt;
            if (byName) {
                parameterIt = std::find_if(outParameters.begin(), outParameters.end(), [position, &argument](const auto &p) {
                    return argument.name == p.second.name();
                });
            }
            if (byPosition) {
                parameterIt = std::find_if(outParameters.begin(), outParameters.end(), [position, &argument](const auto &p) {
                    return p.second.position() == position;
                });
            }
            if (parameterIt == outParameters.end()) {
                continue;
            }
            parameterIt->second.strValue(argument.value);
        }

    }

    RowAndColumn InvalidRowAndColumn = RowAndColumn(-1, -1);

    namespace toolsimpl {

        const std::vector<sheet::Argument> & getMetaArgs(const sheet::Event &metaEvent)
        {
            return metaEvent.metaArgs;
        }
        const com::String & getMetaCommand(const sheet::Event &metaEvent)
        {
            return metaEvent.stringValue;
        }
    }

    com::String pitchToString(com::Pitch pitch)
    {
        switch (pitch) {
			case com::notes::C   :  return FM_STRING("C");
			case com::notes::CIS :  return FM_STRING("C#");
			case com::notes::D   :  return FM_STRING("D");
			case com::notes::DIS :  return FM_STRING("D#");
			case com::notes::E   :  return FM_STRING("E");
			case com::notes::F   :  return FM_STRING("F");
			case com::notes::FIS :  return FM_STRING("F#");
			case com::notes::G   :  return FM_STRING("G");
			case com::notes::GIS :  return FM_STRING("G#");
			case com::notes::A   :  return FM_STRING("A");
			case com::notes::AIS :  return FM_STRING("A#");
			case com::notes::B   :  return FM_STRING("B");
            default : return FM_STRING("?"); 
        }
    } 

    std::ostream & documentMessageWhere(std::ostream &ss, const std::string filename, int line)
    {
        ss << "in file " << filename;
        if (line > 0) {
            ss << ":" << line;
        } 
        return ss;
    }

    std::ostream & documentMessageWhat(std::ostream &ss, const std::string &what)
    {
        ss << what;
        return ss;
    }

    std::ostream & documentMessage(std::ostream &ss, 
        const std::shared_ptr<sheet::Document> document, 
        sheet::ASheetObjectWithSourceInfo::SourceId sourceId,
        unsigned int sourcePosition,
        const std::string &message)
    {
        auto sheetfile = document->findSourcePath(sourceId);
        com::String errorLine;
        int errorPosition = -1;
        int lineNr = -1;
        std::tie(errorLine, errorPosition, lineNr) = _lineAndPos(sheetfile, sourcePosition);
        std::string arrowLine(std::max(errorPosition-1, 0), ' ');
        arrowLine += "^~~~~";
        documentMessageWhere(ss, sheetfile, lineNr+1) << std::endl;
        documentMessageWhat(ss, message) << std::endl
            << errorLine << std::endl
            << arrowLine;
        return ss;
    }
}