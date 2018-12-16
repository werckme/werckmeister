#include <fm/werckmeister.hpp>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "compiler/MidiContext.h"
#include "sheet.h"
#include <iostream>

namespace sheet {

    fm::midi::MidiPtr processFile(const std::string &file)
    {
        auto doc = createDocument(file);
        return processFile(doc);
    }

    fm::midi::MidiPtr processFile(sheet::DocumentPtr doc) 
    {
        auto &wm = fm::getWerckmeister();
        auto context = std::dynamic_pointer_cast<sheet::compiler::MidiContext>( wm.createContext() );
        auto midi = wm.createMidi();
        context->midi(midi);
        context->styleDefServer(doc.get());
        auto compiler = wm.createCompiler();
        compiler->context(context);
        compiler->compile(doc);
		for (const auto &warning : context->warnings) {
			std::cout << warning << std::endl;
		}
        return midi;
    }

	sheet::DocumentPtr createDocument(const std::string &file)
    {
        sheet::compiler::DocumentParser docparser;
        auto doc = docparser.parse(fm::to_wstring(file));
        return doc;
    }

    sheet::DocumentPtr createDocumentByString(const fm::String &sheetText, const std::vector<fm::String> &usings)
    {
        sheet::compiler::DocumentParser docparser;
        auto doc = docparser.parseString(sheetText, usings);
        return doc;
    }
}