#ifndef COMPILER_H
#define COMPILER_H

#include "ICompiler.h"
#include "ISheetTemplateRenderer.h"
#include "ASheetEventRenderer.h"
#include "IPreprocessor.h"
#include <compiler/context/IContext.h>

namespace sheet {
	struct Event;
	namespace compiler {
		class Compiler : public ICompiler {
		private:
			IContextPtr context_;
			DocumentWPtr document_;
			ASheetEventRendererPtr sheetEventRenderer_;
			ISheetTemplateRendererPtr sheetTemplateRenderer_;
			IPreprocessorPtr preprocessor_;
		public:
			Compiler(
				IContextPtr context, 
				ASheetEventRendererPtr sheetEventRenderer,
				ISheetTemplateRendererPtr sheetTemplateRenderer,
				IPreprocessorPtr preprocessor
			) : context_(context),
				sheetEventRenderer_(sheetEventRenderer),
				sheetTemplateRenderer_(sheetTemplateRenderer),
				preprocessor_(preprocessor)
			{
			}
			Compiler(const Compiler&) = delete;
			Compiler & operator=(const Compiler&) = delete;
			IContextPtr context() const { return context_; }
			virtual void compile(DocumentPtr document) override;
			virtual ~Compiler() = default;
			ASheetEventRendererPtr sheetEventRenderer();
		protected:
			void renderTracks();
			void renderChordTrack();
		};
	}
}

#endif