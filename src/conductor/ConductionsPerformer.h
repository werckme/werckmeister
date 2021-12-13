#pragma once

#include "IConductionsPerformer.h"
#include <com/common.hpp>
#include <forward.hpp>
#include <memory>
#include <com/midi.hpp>
#include <vector>
#include <conductor/declarations/IDeclaration.h>
#include <conductor/selectors/ISelector.h>
#include <com/ILogger.h>
#include <compiler/IEventInformationServer.h>
namespace conductor
{
	class ConductionsPerformer : public IConductionsPerformer
	{
	public:
		typedef std::vector<EventWithMetaInfo> Events;
		typedef std::vector<IDeclarationPtr> Declarations;

	protected:
		struct EventsAndDeclarations
		{
			Declarations declarations;
			Events events;
		};
		typedef std::vector<EventsAndDeclarations> EventsAndDeclarationsCollection;
		EventsAndDeclarationsCollection selectEvents() const;

	public:
		ConductionsPerformer(com::midi::MidiPtr midifile, 
			documentModel::DocumentPtr document, 
			com::ILoggerPtr logger,
			compiler::IEventInformationServerPtr eventInformationServer) :
			 _midifile(midifile), 
			 _document(document), 
			 _logger(logger),
			 _eventInformationServer(eventInformationServer)
		{
		}
		virtual void applyConductions() override;

	private:
		bool isEventOfInterest(const com::midi::Event &) const;
		Events findMatches(const documentModel::ConductionSelector &) const;
		Events findMatches(const documentModel::ConductionSelector &, Events &) const;
		void perform(const EventsAndDeclarationsCollection &) const;
		com::midi::MidiPtr _midifile;
		documentModel::DocumentPtr _document;
		com::ILoggerPtr _logger;
		compiler::IEventInformationServerPtr _eventInformationServer;
	};
}
