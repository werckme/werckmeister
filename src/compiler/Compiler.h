#ifndef COMPILER_H
#define COMPILER_H

#include "ICompiler.h"

namespace sheet {
	struct Event;
	namespace compiler {
		class SheetTemplateRenderer;
		class SheetEventRenderer;
		typedef std::shared_ptr<SheetEventRenderer> SheetEventRendererPtr;
		class Compiler : public ICompiler {
		public:
			Compiler();
			Compiler(const Compiler&) = delete;
			Compiler & operator=(const Compiler&) = delete;
			void context(AContextPtr context) { context_ = context; }
			AContextPtr context() const { return context_; }
			virtual void compile(DocumentPtr document) override;
			virtual ~Compiler();
			SheetEventRendererPtr sheetEventRenderer();
			void sheetEventRenderer(SheetEventRendererPtr);
		protected:
			void renderTracks();
			void renderChordTrack();
		private:
			AContextPtr context_;
			DocumentPtr document_;
			SheetTemplateRenderer *currentSheetTemplateRenderer_ = nullptr;
			SheetEventRendererPtr sheetEventRenderer_;
		};
	}
}

#endif