#include "tools.h"
#include <documentModel/objects/Event.h>
#include <documentModel/Document.h>
#include <documentModel/objects/Track.h>
#include <com/werckmeister.hpp>
#include <compiler/metaCommands.h>

namespace com
{

    namespace
    {
        auto _lineAndPos(const com::String &file, unsigned int sourcePos)
        {
            if (sourcePos == documentModel::ASheetObjectWithSourceInfo::UndefinedPosition)
            {
                sourcePos = 0;
            }
            try
            {
                auto source = com::getWerckmeister().openResource(file);
                com::StreamBuffIterator end;
                com::StreamBuffIterator begin(*source.get());
                com::String sourceStr(begin, end);
                auto result = getLineAndPosition(sourceStr, sourcePos, false);
                auto lineNr = getNumberOfLines(sourceStr, sourcePos);
                return std::make_tuple(std::get<0>(result), std::get<1>(result), lineNr);
            }
            catch (...)
            {
                return std::make_tuple(com::String(), -1, -1);
            }
        }
    }

    void argumentsToParameters(const std::vector<documentModel::Argument> arguments, IHasParameter::ParametersByNames &outParameters)
    {
        if (arguments.empty())
        {
            return;
        }
        for (int position = 0; position < (int)arguments.size(); ++position)
        {
            const auto &argument = arguments[position];
            bool byPosition = argument.name.empty();
            bool byName = !byPosition;
            IHasParameter::ParametersByNames::iterator parameterIt;
            if (byName)
            {
                parameterIt = std::find_if(outParameters.begin(), outParameters.end(), [position, &argument](const auto &p)
                                           { return argument.name == p.second.name(); });
            }
            if (byPosition)
            {
                parameterIt = std::find_if(outParameters.begin(), outParameters.end(), [position, &argument](const auto &p)
                                           { return p.second.position() == position; });
            }
            if (parameterIt == outParameters.end())
            {
                continue;
            }
            parameterIt->second.strValue(argument.value);
        }
    }

    RowAndColumn InvalidRowAndColumn = RowAndColumn(-1, -1);

    namespace toolsimpl
    {

        const std::vector<documentModel::Argument> &getMetaArgs(const documentModel::Event &metaEvent)
        {
            return metaEvent.metaArgs;
        }
        const com::String &getMetaCommand(const documentModel::Event &metaEvent)
        {
            return metaEvent.stringValue;
        }
    }

    com::String pitchToString(com::Pitch pitch)
    {
        switch (pitch)
        {
        case com::notes::C:
            return FM_STRING("C");
        case com::notes::CIS:
            return FM_STRING("C#");
        case com::notes::D:
            return FM_STRING("D");
        case com::notes::DIS:
            return FM_STRING("D#");
        case com::notes::E:
            return FM_STRING("E");
        case com::notes::F:
            return FM_STRING("F");
        case com::notes::FIS:
            return FM_STRING("F#");
        case com::notes::G:
            return FM_STRING("G");
        case com::notes::GIS:
            return FM_STRING("G#");
        case com::notes::A:
            return FM_STRING("A");
        case com::notes::AIS:
            return FM_STRING("A#");
        case com::notes::B:
            return FM_STRING("B");
        default:
            return FM_STRING("?");
        }
    }

    std::ostream &documentMessageWhere(std::ostream &ss, const std::string filename, int line)
    {
        ss << "in file " << filename;
        if (line > 0)
        {
            ss << ":" << line;
        }
        return ss;
    }

    std::ostream &documentMessageWhat(std::ostream &ss, const std::string &what)
    {
        ss << what;
        return ss;
    }

    std::ostream &documentMessage(std::ostream &ss,
                                  const std::shared_ptr<documentModel::Document> document,
                                  documentModel::ASheetObjectWithSourceInfo::SourceId sourceId,
                                  unsigned int sourcePosition,
                                  const std::string &message)
    {
        auto sheetfile = document->findSourcePath(sourceId);
        com::String errorLine;
        int errorPosition = -1;
        int lineNr = -1;
        std::tie(errorLine, errorPosition, lineNr) = _lineAndPos(sheetfile, sourcePosition);
        std::string arrowLine(std::max(errorPosition - 1, 0), ' ');
        arrowLine += "^~~~~";
        documentMessageWhere(ss, sheetfile, lineNr + 1) << std::endl;
        documentMessageWhat(ss, message) << std::endl
                                         << errorLine << std::endl
                                         << arrowLine;
        return ss;
    }

    bool isNoteEventTrack(const documentModel::Track &track)
    {
        return !isAccompEventTrack(track) && !isTemplateTrack(track);
    }
    bool isAccompEventTrack(const documentModel::Track &track)
    {
        com::String str = com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
        return str == SHEET_META__TRACK_META_VALUE_TYPE_ACCOMP;
    }
    bool isTemplateTrack(const documentModel::Track &track)
    {
        com::String str = com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TEMPLATE_TYPE, track.trackConfigs).value;
        if (!str.empty()) {
            return true;
        }
        str = getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
        return str == SHEET_META__TRACK_META_VALUE_TYPE_SHEET_TEMPLATE;
    }
    String getTrackName(const documentModel::Track& track)
    {
        auto trackname = com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TEMPLATE_TYPE, track.trackConfigs).value;
        if (!trackname.empty()) {
            return trackname;
        }
        return com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_NAME, track.trackConfigs).value;
    }

}