#include "Compiler.h"
#include "documentModel/Document.h"
#include <documentModel/objects/Event.h>
#include <type_traits>
#include <algorithm>
#include <list>
#include "metaCommands.h"
#include "error.hpp"
#include <boost/exception/get_error_info.hpp>
#include <com/tools.h>
#include <functional>

namespace compiler
{
	void Compiler::compile(documentModel::DocumentPtr document)
	{
		this->compilerVisitorPtr_->beginCompile();
		this->document_ = document;
		try
		{
			std::list<documentModel::DocumentConfig> priorisedDocumentConfigs;
			for (auto &conf : document->sheetDef.documentConfigs)
			{
				if (conf.type != documentModel::DocumentConfig::TypeConfigDef) 
				{
					continue;
				}
				if (conf.name == SHEET_META__SET_DEVICE)
				{ // device commands first
					priorisedDocumentConfigs.push_front(conf);
					continue;
				}
				priorisedDocumentConfigs.push_back(conf);
			}
			std::vector<documentModel::DocumentConfig> configs(priorisedDocumentConfigs.begin(), priorisedDocumentConfigs.end());
			sheetEventRenderer()->handleMetaEvents(configs,
			[](const auto &x)
			{
				documentModel::Event metaEvent;
				metaEvent.type = documentModel::Event::Meta;
				metaEvent.stringValue = x.name;
				metaEvent.metaArgs = x.args;
				return metaEvent;
			});
		}
		catch (com::Exception &ex)
		{
			ex << ex_sheet_document(document);
			throw;
		}
		logger_->babble(WMLogLambda(log << "preprocess '" << document->path << "'"));
		try
		{
			preprocessor_->preprocess(document);
			renderAccompTrack();
			renderTracks();
		}
		catch (const com::Exception &ex)
		{
			ex << ex_sheet_document(document);
			throw;
		}
		catch (...)
		{
			throw;
		}
		this->compilerVisitorPtr_->endCompile();
	}

	void Compiler::renderTracks()
	{
		auto ctx = context();
		auto renderer = sheetEventRenderer();
		auto document = document_.lock();
		for (auto &track : document->sheetDef.tracks)
		{
			com::String type = com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs).value;
			bool isNoteEventTrack = type.empty();
			if (!isNoteEventTrack)
			{
				continue;
			}
			auto trackId = ctx->createTrack();
			ctx->setTrack(trackId);
			renderer->handleMetaEvents(track.trackConfigs,
									   [](const auto &x)
									   {
										   documentModel::Event metaEvent;
										   metaEvent.type = documentModel::Event::Meta;
										   metaEvent.stringValue = x.name;
										   metaEvent.metaArgs = x.args;
										   return metaEvent;
									   });
			for (const auto &voice : track.voices)
			{
				auto voiceId = ctx->createVoice();
				ctx->setTarget(trackId, voiceId);
				for (const auto &ev : voice.events)
				{
					if (ev.type == documentModel::Event::Phrase) 
					{
						phraseRenderer_->renderPhrase(ev);
						continue;
					}
					renderer->addEvent(ev);
				}
			}
		}
	}

	namespace
	{
		void consumeChords(documentModel::Track *sheetTrack, ASheetEventRenderer *sheetEventRenderer, IContext *ctx)
		{
			using namespace com;
			auto voice = sheetTrack->voices.begin();
			std::list<documentModel::Event *> barEvents;
			ctx->setChordTrackTarget();
			auto voiceMetaData = ctx->voiceMetaData();
			// since we process the chord track a second time later, we reset all vital values
			voiceMetaData->barPosition = 0;
			voiceMetaData->position = 0;
			voiceMetaData->tempoFactor = 1;
			voiceMetaData->signatureNumerator = 4;
			voiceMetaData->signatureDenominator = 4;
			for (const auto &ev : voice->events)
			{
				sheetEventRenderer->addEvent(ev);
			}
		}

		bool isAccompTrack(const documentModel::Track &track)
		{
			return com::getFirstMetaArgumentForKey(SHEET_META__TRACK_META_KEY_TYPE, track.trackConfigs)
					   .value == SHEET_META__TRACK_META_VALUE_TYPE_ACCOMP;
		}
	}

	void Compiler::renderAccompTrack()
	{
		auto ctx = context();
		auto document = document_.lock();
		for (documentModel::Track &track : document->sheetDef.tracks)
		{
			if (!isAccompTrack(track))
			{
				continue;
			}
			if (track.voices.empty())
			{
				return;
			}
			consumeChords(&track, sheetEventRenderer().get(), ctx.get());
			sheetTemplateRenderer_->render(&track);
		}
	}

	ASheetEventRendererPtr Compiler::sheetEventRenderer()
	{
		return this->sheetEventRenderer_;
	}
}