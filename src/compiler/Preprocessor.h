#ifndef COMPILER_PREPROCESSOR_H
#define COMPILER_PREPROCESSOR_H

#include <fm/common.hpp>
#include "IPreprocessor.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include <fm/IDefinitionsServer.h>
#include <forward.hpp>

namespace sheet {
	struct Track;
	namespace compiler {
		class Preprocessor : public IPreprocessor {
		private:
			IContextPtr 			_context;
			ASheetEventRendererPtr _renderer;
			fm::IDefinitionsServerPtr _definitionServer;
		public:
			Preprocessor(IContextPtr context, 
				ASheetEventRendererPtr renderer, 
				fm::IDefinitionsServerPtr definitionServer
			)
				: _context(context)
				, _renderer(renderer)
				, _definitionServer(definitionServer)
			{}
			Preprocessor(const Preprocessor&) = delete;
			Preprocessor & operator=(const Preprocessor&) = delete;
			virtual void process(Track &track);
			virtual void preprocessSheetTrack(Track &sheetTrack);
			virtual void prepareTemplateDefinitions(DocumentPtr document);
		};
	}
}

#endif