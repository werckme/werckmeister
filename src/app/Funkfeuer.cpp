#include "Funkfeuer.h"
#include <iostream>
#include <time.h>
#include "UdpSender.hpp"

namespace app {
    void Funkfeuer::visit(com::Ticks elapsed) 
    {
        std::stringstream ss;
        lastUpdateTimestamp = (unsigned long)time(NULL);
        const auto &timeline = _timelineVisitor->intervalContainer();
        auto elapsedQuarter = elapsed / (double)com::PPQ;
        auto ev = timeline.find(elapsed);
        EventInfos eventInfos;
        if (ev == lastTimelineEvent || ev == timeline.end()) {
            // send time update without infos
            eventInfoToJSON(ss, elapsedQuarter, lastUpdateTimestamp, eventInfos);
            auto bff = ss.str();
            _sender->send(bff.data(), bff.size());
            return;
        }
        lastTimelineEvent = ev;
        eventInfos.reserve(ev->second.size());
        _logger->debug(WMLogLambda(log << "funkfeuer " << ev->second.size() << " events at " << elapsedQuarter));
        for (const auto &x : ev->second) {
            eventInfos.push_back(x);
        }
        eventInfoToJSON(ss, elapsedQuarter, lastUpdateTimestamp, eventInfos);
        auto bff = ss.str();
        _sender->send(bff.data(), bff.size());
    }  
}