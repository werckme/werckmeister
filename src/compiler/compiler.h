#ifndef COMPILER_H
#define COMPILER_H

#include <fm/common.hpp>
#include "forward.hpp"

namespace sheet {
	struct Event;
	namespace compiler {
		class SheetTemplateRenderer;
		class Compiler {
		public:
			Compiler();
			Compiler(const Compiler&) = delete;
			Compiler & operator=(const Compiler&) = delete;
			void context(AContextPtr context) { context_ = context; }
			AContextPtr context() const { return context_; }
			void compile(DocumentPtr document);
			~Compiler();
		protected:
			void renderTracks();
			void renderChordTrack();
		private:
			bool metaEventHandler(const Event &metaEvent);
			AContextPtr context_;
			DocumentPtr document_;
			void switchSheetTemplate(SheetTemplateRenderer &sheetTemplateRenderer, const Event &metaEvent);
			//// Meta Event Handler
			void sheetTemplatePosition(const fm::String &cmd);
			SheetTemplateRenderer *currentSheetTemplateRenderer_ = nullptr;
		};
	}
}

#endif