#pragma once

#include <com/common.hpp>
#include <forward.hpp>
#include <com/units.hpp>
#include "compiler/voicings/VoicingStrategy.h"
#include <unordered_map>
#include <list>

namespace compiler
{
	class IContext;
	class SheetEventRenderer;
	struct AInstrumentDef : std::enable_shared_from_this<AInstrumentDef>
	{
		int id = 0;
		com::String uname;
		double volume = 100.0;
		double pan = 50.0;
		VoicingStrategyPtr voicingStrategy;
		typedef std::list<AModificationPtr> Modifications;
		Modifications modifications;
		typedef std::unordered_map<com::Expression, double> VelocityOverride;
		VelocityOverride velocityOverride;
		virtual ~AInstrumentDef() = default;
		virtual void setToContext(IContext *context) = 0;
		virtual void renderEvents(SheetEventRenderer *renderer, std::list<documentModel::Event> &events, const com::String &parentInsrtumentSectionName = com::String()) = 0;
		virtual std::shared_ptr<AInstrumentDef> activeInstrument() = 0;

	protected:
		template <class TImpl>
		std::shared_ptr<TImpl> getThis() { return std::dynamic_pointer_cast<TImpl>(shared_from_this()); }
	};
	typedef std::shared_ptr<AInstrumentDef> AInstrumentDefPtr;
	struct MidiInstrumentDef : public AInstrumentDef
	{
		com::String deviceName;
		int channel = 0;
		int bankMsb = -1;
		int bankLsb = -1;
		int pc = 0;
		virtual ~MidiInstrumentDef() = default;
		virtual void setToContext(IContext *context) override;
		virtual void renderEvents(SheetEventRenderer *renderer, std::list<documentModel::Event> &events, const com::String& parentInsrtumentSectionName) override;
		virtual std::shared_ptr<AInstrumentDef> activeInstrument() override { return shared_from_this(); }
	};

	struct InstrumentSectionDef : public AInstrumentDef
	{
		int id = 0;
		std::list<com::String> instrumentNames;
		virtual ~InstrumentSectionDef() = default;
		virtual void setToContext(IContext *context) override;
		virtual void renderEvents(SheetEventRenderer *renderer, std::list<documentModel::Event> &events, const com::String& parentInsrtumentSectionName) override;
		virtual std::shared_ptr<AInstrumentDef> activeInstrument() override { return _currentInstrument.lock(); }

	private:
		std::weak_ptr<AInstrumentDef> _currentInstrument;
	};
	typedef std::shared_ptr<InstrumentSectionDef> InstrumentSectionDefPtr;
}
