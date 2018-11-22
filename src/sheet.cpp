#include <fm/werckmeister.hpp>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "compiler/MidiContext.h"
#include "sheet.h"

namespace sheet {

    fm::midi::MidiPtr processFile(const std::string &file)
    {
        auto &wm = fm::getWerckmeister();
        sheet::compiler::DocumentParser docparser;
        auto doc = docparser.parse(fm::to_wstring(file));
        auto context = std::dynamic_pointer_cast<sheet::compiler::MidiContext>( wm.createContext() );
        auto midi = wm.createMidi();
        context->midi(midi);
        context->styleDefServer(doc.get());
        auto compiler = wm.createCompiler();
        compiler->context(context);
        compiler->compile(doc);
        return midi;
    }

}