#pragma once

#include <com/common.hpp>
#include "IPreprocessor.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <compiler/IDefinitionsServer.h>
#include <documentModel/objects/Track.h>
#include <forward.hpp>
#include "ISheetNavigator.h"

namespace documentModel
{
	struct Track;
}
namespace compiler
{
	class Preprocessor : public IPreprocessor
	{
	private:
		IContextPtr _context;
		ASheetEventRendererPtr _renderer;
		compiler::IDefinitionsServerPtr _definitionServer;
		ISheetNavigatorPtr _sheetNavigator;

		virtual void preprocessChordTrack(documentModel::Track &sheetTrack);
		virtual void preprocessPhraseDefs(documentModel::DocumentConfig::Events &events);
		virtual void preprocess(documentModel::DocumentPtr document);

	public:
		Preprocessor(IContextPtr context,
					 ASheetEventRendererPtr renderer,
					 compiler::IDefinitionsServerPtr definitionServer,
					 ISheetNavigatorPtr sheetNavigator);
		Preprocessor(const Preprocessor &) = delete;
		Preprocessor &operator=(const Preprocessor &) = delete;
		virtual void process(documentModel::Track &track);
		virtual ~Preprocessor() = default;
	};
}
