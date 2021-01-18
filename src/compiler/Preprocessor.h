#ifndef COMPILER_PREPROCESSOR_H
#define COMPILER_PREPROCESSOR_H

#include <fm/common.hpp>
#include "IPreprocessor.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <fm/IDefinitionsServer.h>
#include <forward.hpp>
#include "ISheetNavigator.h"

namespace sheet {
	struct Track;
	namespace compiler {
		class Preprocessor : public IPreprocessor {
		private:
			IContextPtr 			_context;
			ASheetEventRendererPtr _renderer;
			fm::IDefinitionsServerPtr _definitionServer;
			ISheetNavigatorPtr		_sheetNavigator;
		public:
			Preprocessor(IContextPtr context, 
				ASheetEventRendererPtr renderer, 
				fm::IDefinitionsServerPtr definitionServer,
				ISheetNavigatorPtr		sheetNavigator
			)
				: _context(context)
				, _renderer(renderer)
				, _definitionServer(definitionServer)
				, _sheetNavigator(sheetNavigator)
			{}
			Preprocessor(const Preprocessor&) = delete;
			Preprocessor & operator=(const Preprocessor&) = delete;
			virtual void process(Track &track);
			virtual void preprocessChordTrack(Track &sheetTrack);
			virtual void preprocess(DocumentPtr document);
			virtual ~Preprocessor() = default;
		};
	}
}

#endif