#pragma once

#include <com/common.hpp>
#include "IPreprocessor.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <com/IDefinitionsServer.h>
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
		com::IDefinitionsServerPtr _definitionServer;
		ISheetNavigatorPtr _sheetNavigator;

	public:
		Preprocessor(IContextPtr context,
					 ASheetEventRendererPtr renderer,
					 com::IDefinitionsServerPtr definitionServer,
					 ISheetNavigatorPtr sheetNavigator)
			: _context(context), _renderer(renderer), _definitionServer(definitionServer), _sheetNavigator(sheetNavigator)
		{
		}
		Preprocessor(const Preprocessor &) = delete;
		Preprocessor &operator=(const Preprocessor &) = delete;
		virtual void process(Track &track);
		virtual void preprocessChordTrack(Track &sheetTrack);
		virtual void preprocess(DocumentPtr document);
		virtual ~Preprocessor() = default;
	};
}
