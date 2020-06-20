#include "Funkfeuer.h"
#include <iostream>

namespace fmapp {
    void Funkfeuer::visit(fm::Ticks elapsed) 
    {
        const auto &timeline = _timelineVisitor->intervalContainer();
        auto elapsedQuarter = elapsed / (double)fm::PPQ;
        auto ev = timeline.find(elapsed);
        if (ev == lastTimelineEvent || ev == timeline.end()) {
            // std::string bff = jsonWriter.funkfeuerToJSON(elapsedQuarter, lastUpdateTimestamp);
            // funkfeuer->send(bff.data(), bff.size());
            return;
        }
        lastTimelineEvent = ev;
        EventInfos eventInfos;
        eventInfos.reserve(ev->second.size());
        _logger->debug(WMLogLambda(log << "funkfeuer " << ev->second.size() << " events at " << elapsedQuarter));
        for (const auto &x : ev->second) {
            eventInfos.push_back(x);
        }
        eventInfoToJSON(std::cout, elapsedQuarter, 0, eventInfos);
        // funkfeuer->send(bff.data(), bff.size());
    }  
}