#pragma once

#include <com/common.hpp>
#include "IPreprocessor.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <compiler/IDefinitionsServer.h>
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

	public:
		Preprocessor(IContextPtr context,
					 ASheetEventRendererPtr renderer,
					 compiler::IDefinitionsServerPtr definitionServer,
					 ISheetNavigatorPtr sheetNavigator)
			: _context(context), _renderer(renderer), _definitionServer(definitionServer), _sheetNavigator(sheetNavigator)
		{
		}
		Preprocessor(const Preprocessor &) = delete;
		Preprocessor &operator=(const Preprocessor &) = delete;
		virtual void process(documentModel::Track &track);
		virtual void preprocessChordTrack(documentModel::Track &sheetTrack);
		virtual void preprocessPhraseDefs(documentModel::DocumentConfig::Events &events);
		virtual void preprocess(documentModel::DocumentPtr document);
		virtual ~Preprocessor() = default;
	};
}
