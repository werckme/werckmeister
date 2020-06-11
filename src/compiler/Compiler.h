#ifndef COMPILER_H
#define COMPILER_H

#include "ICompiler.h"
#include "ISheetTemplateRenderer.h"
#include "ASheetEventRenderer.h"
#include "IPreprocessor.h"

namespace sheet {
	struct Event;
	namespace compiler {
		class Compiler : public ICompiler {
		private:
			AContextPtr context_;
			DocumentPtr document_;
			ASheetEventRenderer *sheetEventRenderer_;
			ISheetTemplateRenderer *sheetTemplateRenderer_;
			IPreprocessor *preprocessor_;
		public:
			Compiler(
				AContextPtr context, 
				DocumentPtr document, 
				ASheetEventRenderer *sheetEventRenderer,
				ISheetTemplateRenderer *sheetTemplateRenderer,
				IPreprocessor *preprocessor
			) : context_(context),
				document_(document),
				sheetEventRenderer_(sheetEventRenderer),
				sheetTemplateRenderer_(sheetTemplateRenderer),
				preprocessor_(preprocessor)
			{
			}
			Compiler(const Compiler&) = delete;
			Compiler & operator=(const Compiler&) = delete;
			AContextPtr context() const { return context_; }
			virtual void compile(DocumentPtr document) override;
			virtual ~Compiler() = default;
			ASheetEventRenderer* sheetEventRenderer();
		protected:
			void renderTracks();
			void renderChordTrack();
		};
	}
}

#endif