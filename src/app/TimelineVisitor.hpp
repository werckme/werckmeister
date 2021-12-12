#pragma once

#include <forward.hpp>
#include <com/midi.hpp>
#include <map>
#include <vector>
#include <memory>
#include <boost/icl/interval_map.hpp>
#include <documentModel/objects/ASheetObject.h>
#include <compiler/ICompilerVisitor.h>
#include <compiler/context/MidiContext.h>
#include <com/ILogger.h>

namespace app
{
    /*
    * a timeline tells you at what time, which events are used
    * this context creates such timeline
    */
    template <class TIntervalContainer>
    class TimelineVisitor : public documentModel::compiler::ICompilerVisitor
    {
    private:
        com::ILoggerPtr _logger;

    public:
        TimelineVisitor(com::ILoggerPtr logger) : _logger(logger) {}
        typedef typename TIntervalContainer::value_type::second_type TSet;
        typedef typename TIntervalContainer::interval_type IntervalType;
        typedef typename TSet::value_type EventInfo;
        typedef documentModel::compiler::IContext::TrackId TrackId;
        virtual ~TimelineVisitor() = default;
        TIntervalContainer &intervalContainer() { return intervalContainer_; }
        const TIntervalContainer &intervalContainer() const { return intervalContainer_; }
        virtual void beginCompile() override { intervalContainer_.clear(); }
        virtual void endCompile() override {}
        virtual void visit(documentModel::compiler::IContext *context, const documentModel::Event &ev) override;
        virtual void visit(documentModel::compiler::IContext *context, const com::midi::Event &ev, TrackId trackId) override;

    private:
        TIntervalContainer intervalContainer_;
        std::shared_ptr<EventInfo> currentEventInfo_;
    };

    typedef int SourceId;
    struct EventInfo
    {
        int beginPosition = -1;
        int endPosition = -1;
        documentModel::Event::SourceId sourceId = documentModel::Event::UndefinedSource;
        int eventNr = -1;
        std::vector<int> pitches;
        int channel = -1;
        int instrumentId = -1;
        bool operator<(const EventInfo &b) const { return this->beginPosition < b.beginPosition; }
        bool operator==(const EventInfo &b) const { return this->beginPosition == b.beginPosition; }
    };

    typedef std::vector<EventInfo> EventInfos;
    typedef std::set<EventInfo> TextPositionSet;
    typedef boost::icl::interval_map<com::Ticks, TextPositionSet> EventTimeline;
    typedef boost::icl::interval<com::Ticks> TicksInterval;
    typedef TimelineVisitor<EventTimeline> DefaultTimeline;
    typedef std::shared_ptr<DefaultTimeline> DefaultTimelinePtr;

    ///////////////////////////////////////////////////////////////////////////
    template <class TIntervalContainer>
    void TimelineVisitor<TIntervalContainer>::visit(documentModel::compiler::IContext *ctx, const documentModel::Event &ev)
    {
        if (!ev.isTimeConsuming())
        {
            return;
        }
        if (ev.type == documentModel::Event::Group)
        {
            return;
        }
        auto meta = ctx->voiceMetaData();
        auto evStartPos = meta->position * meta->tempoFactor;
        auto evEndPos = (meta->position + ev.duration) * meta->tempoFactor;
        this->currentEventInfo_ = std::make_shared<EventInfo>();
        currentEventInfo_->beginPosition = ev.sourcePositionBegin;
        if (ev.sourcePositionEnd != documentModel::ASheetObjectWithSourceInfo::UndefinedPosition)
        {
            currentEventInfo_->endPosition = ev.sourcePositionEnd;
        }
        currentEventInfo_->sourceId = ev.sourceId;
        currentEventInfo_->eventNr = (int)intervalContainer_.size();
        TSet value = {*currentEventInfo_};
        intervalContainer_ += std::make_pair(IntervalType::right_open(evStartPos, evEndPos), value);
    }

    template <class TIntervalContainer>
    void TimelineVisitor<TIntervalContainer>::visit(documentModel::compiler::IContext *ctx, const com::midi::Event &ev, TrackId trackId)
    {
        // TODO: #89
        //if (ev.eventType() != com::midi::NoteOn) {
        //    return;
        //}
        //if (!this->currentEventInfo_) {
        //    return;
        //}
        //this->currentEventInfo_->pitches.push_back(ev.parameter1());
        //this->currentEventInfo_->channel = ev.channel();
        //typedef documentModel::compiler::MidiContext::TrackMetaData MidiTrackMetaData;
        //auto trackMeta = std::dynamic_pointer_cast<MidiTrackMetaData>(ctx->trackMetaData(trackId));
        //if (trackMeta) {
        //    this->currentEventInfo_->instrumentId = trackMeta->instrument.id;
        //}
    }
}
