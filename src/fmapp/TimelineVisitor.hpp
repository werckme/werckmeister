#ifndef FMAPP_TIMELINECONTEXT_H
#define FMAPP_TIMELINECONTEXT_H


#include <forward.hpp>
#include <fm/midi.hpp>
#include <map>
#include <vector>
#include <memory>
#include <boost/icl/interval_map.hpp>
#include <sheet/objects/ASheetObject.h>
#include <compiler/ICompilerVisitor.h>
#include <compiler/context/MidiContext.h>
#include <fm/ILogger.h>

namespace fmapp {
    /*
    * a timeline tells you at what time, which events are used
    * this context creates such timeline
    */
    template<class TIntervalContainer>
    class TimelineVisitor : public sheet::compiler::ICompilerVisitor {
    private:
        fm::ILoggerPtr _logger;
    public:
        TimelineVisitor(fm::ILoggerPtr logger) : _logger(logger) {}
        typedef typename TIntervalContainer::value_type::second_type TSet;
        typedef typename TIntervalContainer::interval_type IntervalType;
        typedef typename TSet::value_type EventInfo;
        typedef sheet::compiler::IContext::TrackId TrackId;
        virtual ~TimelineVisitor() = default;
        TIntervalContainer &intervalContainer() { return intervalContainer_; }
        const TIntervalContainer &intervalContainer() const { return intervalContainer_; }
        virtual void beginCompile() override { intervalContainer_.clear(); }
        virtual void endCompile() override {}
        virtual void visit(sheet::compiler::IContext *context, const sheet::Event &ev) override;
        virtual void visit(sheet::compiler::IContext *context, const fm::midi::Event &ev, TrackId trackId) override;    
    private:
        TIntervalContainer intervalContainer_;
        std::shared_ptr<EventInfo> currentEventInfo_;
    };
    

    typedef int SourceId;
    struct EventInfo {
        int beginPosition = -1;
        int endPosition = -1;
        sheet::Event::SourceId sourceId = sheet::Event::UndefinedSource;
        int eventNr = -1;
        std::vector<int> pitches;
        int channel = -1;
        int instrumentId = -1;
        bool operator<(const EventInfo &b) const { return this->beginPosition < b.beginPosition;  }
        bool operator==(const EventInfo &b) const { return this->beginPosition == b.beginPosition;  }
    };

    typedef std::vector<EventInfo> EventInfos;
    typedef std::set<EventInfo> TextPositionSet;
    typedef boost::icl::interval_map<fm::Ticks, TextPositionSet> EventTimeline;
    typedef boost::icl::interval<fm::Ticks> TicksInterval;
    typedef TimelineVisitor<EventTimeline> DefaultTimeline;
    typedef std::shared_ptr<DefaultTimeline> DefaultTimelinePtr;

    ///////////////////////////////////////////////////////////////////////////  
    template<class TIntervalContainer>
    void TimelineVisitor<TIntervalContainer>::visit(sheet::compiler::IContext *ctx, const sheet::Event &ev)
    {
        if (!ev.isTimeConsuming()) {
            return;
        }
        if (ev.type == sheet::Event::Group) {
            return;
        }
        auto meta = ctx->voiceMetaData();
        auto evStartPos = meta->position * meta->tempoFactor;
        auto evEndPos = (meta->position + ev.duration) * meta->tempoFactor;
        this->currentEventInfo_ = std::make_shared<EventInfo>();
		currentEventInfo_->beginPosition = ev.sourcePositionBegin;
        if (ev.sourcePositionEnd != sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
            currentEventInfo_->endPosition = ev.sourcePositionEnd;
        }
		currentEventInfo_->sourceId = ev.sourceId;
		currentEventInfo_->eventNr = intervalContainer_.size();
        TSet value = { *currentEventInfo_ };
        intervalContainer_ += std::make_pair(IntervalType::right_open(evStartPos, evEndPos), value);
    }

    template<class TIntervalContainer>
    void TimelineVisitor<TIntervalContainer>::visit(sheet::compiler::IContext *ctx, const fm::midi::Event &ev, TrackId trackId)
    {
        if (ev.eventType() != fm::midi::NoteOn) {
            return;
        }
        if (!this->currentEventInfo_) {
            return;
        }
        this->currentEventInfo_->pitches.push_back(ev.parameter1());
        this->currentEventInfo_->channel = ev.channel();
        typedef sheet::compiler::MidiContext::TrackMetaData MidiTrackMetaData;
        auto trackMeta = std::dynamic_pointer_cast<MidiTrackMetaData>(ctx->trackMetaData(trackId));
        if (trackMeta) {
            this->currentEventInfo_->instrumentId = trackMeta->instrument.id;
        }
    }
}

#endif