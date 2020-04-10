#ifndef FMAPP_TIMELINECONTEXT_H
#define FMAPP_TIMELINECONTEXT_H

#include <compiler/context/MidiContext.h>
#include <compiler/sheetEventRenderer.h>
#include <forward.hpp>
#include <fm/midi.hpp>
#include <map>
#include <vector>
#include <memory>
#include <boost/icl/interval_map.hpp>
#include <sheet/ASheetObject.hpp>

namespace fmapp {
    /*
    * a timeline tells you at what time, which events are used
    * this context creates such timeline, while creating the midi data.
    * this class combines renderer and context to get this information.
    */
    template<class TIntervalContainer>
    class MidiAndTimeline : public sheet::compiler::MidiContext, public sheet::compiler::SheetEventRenderer {
    public:
        typedef sheet::compiler::MidiContext Context;
        typedef sheet::compiler::SheetEventRenderer Renderer;
        typedef typename TIntervalContainer::value_type::second_type TSet;
        typedef typename TIntervalContainer::interval_type IntervalType;
        typedef typename TSet::value_type EventInfo;        
        MidiAndTimeline();
        virtual ~MidiAndTimeline();
        virtual void addEvent(const sheet::Event &ev) override;
        virtual void addEvent(const fm::midi::Event &ev, TrackId trackId) override;
        TIntervalContainer *intervalContainer() { return intervalContainer_; }
        void intervalContainer(TIntervalContainer *container) { intervalContainer_ = container; }
    private:
        TIntervalContainer *intervalContainer_ = nullptr;
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
    typedef MidiAndTimeline<EventTimeline> MidiAndTimelineContext;

    ///////////////////////////////////////////////////////////////////////////
    template<class TIntervalContainer>
    MidiAndTimeline<TIntervalContainer>::MidiAndTimeline() : sheet::compiler::SheetEventRenderer(this)
    {

    }
    template<class TIntervalContainer>
    MidiAndTimeline<TIntervalContainer>::~MidiAndTimeline()
    {

    }    
    template<class TIntervalContainer>
    void MidiAndTimeline<TIntervalContainer>::addEvent(const sheet::Event &ev)
    {
        if (!ev.isTimeConsuming()) {
            Renderer::addEvent(ev);
            return;
        }
        if (ev.type == sheet::Event::Group) {
            Renderer::addEvent(ev);
            return;
        }
        auto meta = voiceMetaData();
        auto evStartPos = meta->position * meta->tempoFactor;
        this->currentEventInfo_ = std::make_shared<EventInfo>();
        Renderer::addEvent(ev);
        auto evEndPos = meta->position * meta->tempoFactor;
		currentEventInfo_->beginPosition = ev.sourcePositionBegin;
        if (ev.sourcePositionEnd != sheet::ASheetObjectWithSourceInfo::UndefinedPosition) {
            currentEventInfo_->endPosition = ev.sourcePositionEnd;
        }
		currentEventInfo_->sourceId = ev.sourceId;
		currentEventInfo_->eventNr = intervalContainer_->size();
        TSet value = { *currentEventInfo_ };
        (*intervalContainer_) += std::make_pair(IntervalType::right_open(evStartPos, evEndPos), value);
    }

    template<class TIntervalContainer>
    void MidiAndTimeline<TIntervalContainer>::addEvent(const fm::midi::Event &ev, TrackId trackId)
    {
        Context::addEvent(ev, trackId);
        if (ev.eventType() != fm::midi::NoteOn) {
            return;
        }
        if (!this->currentEventInfo_) {
            return;
        }
        this->currentEventInfo_->pitches.push_back(ev.parameter1());
        this->currentEventInfo_->channel = ev.channel();
        auto trackMeta = trackMetaData<Context::TrackMetaData>(trackId);
        if (trackMeta) {
            this->currentEventInfo_->instrumentId = trackMeta->instrument.id;
        }
    }
}

#endif