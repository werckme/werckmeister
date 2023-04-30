#pragma once

#include "ICompiler.h"
#include "ISheetTemplateRenderer.h"
#include "ASheetEventRenderer.h"
#include <compiler/context/IContext.h>
#include "ICompilerVisitor.h"
#include "IPreprocessor.h"
#include <com/ILogger.h>
#include "IPhraseRenderer.h"

namespace documentModel
{
	struct Event;
}
namespace compiler
{
	class Compiler : public ICompiler
	{
	private:
		IContextPtr context_;
		documentModel::DocumentWPtr document_;
		ASheetEventRendererPtr sheetEventRenderer_;
		ISheetTemplateRendererPtr sheetTemplateRenderer_;
		ICompilerVisitorPtr compilerVisitorPtr_;
		com::ILoggerPtr logger_;
		IPreprocessorPtr preprocessor_;
		IPhraseRendererPtr phraseRenderer_;

	public:
		Compiler(
			IContextPtr context,
			ASheetEventRendererPtr sheetEventRenderer,
			ISheetTemplateRendererPtr sheetTemplateRenderer,
			ICompilerVisitorPtr compilerVisitorPtr,
			com::ILoggerPtr logger,
			IPreprocessorPtr preprocessor,
			IPhraseRendererPtr phraseRenderer) : 
				context_(context), 
				sheetEventRenderer_(sheetEventRenderer), 
				sheetTemplateRenderer_(sheetTemplateRenderer), 
				compilerVisitorPtr_(compilerVisitorPtr), 
				logger_(logger), 
				preprocessor_(preprocessor),
				phraseRenderer_(phraseRenderer)
		{
		}
		Compiler(const Compiler &) = delete;
		Compiler &operator=(const Compiler &) = delete;
		IContextPtr context() const { return context_; }
		virtual void compile(documentModel::DocumentPtr document) override;
		virtual ~Compiler() = default;
		ASheetEventRendererPtr sheetEventRenderer();

	protected:
		void renderTracks();
		void renderAccompTrack();
	};
}
