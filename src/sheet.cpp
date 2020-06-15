#include <fm/werckmeister.hpp>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "compiler/Compiler.h"
#include "parser/parser.h"
#include "compiler/context/MidiContext.h"
#include "sheet.h"
#include "compiler/SheetEventRenderer.h"
#include <fmapp/os.hpp>
#include <iostream>
#include <boost/filesystem.hpp>

namespace sheet {

    namespace {
        void addSearchPaths() 
        {
            using boost::filesystem::path;
            using boost::filesystem::system_complete;
            auto &wm = fm::getWerckmeister();
            auto execPath = path(fmapp::os::getExecutablePath());
            wm.addSearchPath(execPath.string());
            wm.addSearchPath(system_complete(execPath / path("../share/werckmeister")).string());
#ifndef WIN32
            wm.addSearchPath((path("/usr/local/share/werckmeister").string()));
#endif
        }
    }

    std::pair<fm::midi::MidiPtr, sheet::DocumentPtr> processFile(const std::string &file, sheet::Warnings &outWarnings, const fm::midi::MidiConfig *midiConfig)
    {
        auto doc = createDocument(file);
        return std::make_pair(processFile(doc, outWarnings, midiConfig), doc);
    }

    fm::midi::MidiPtr processFile(sheet::DocumentPtr doc, sheet::Warnings &outWarnings, const fm::midi::MidiConfig *midiConfig) 
    {
        auto &wm = fm::getWerckmeister();
        auto context = std::dynamic_pointer_cast<sheet::compiler::MidiContext>( wm.createContext() );
        // there is one case where context and event renderer is one object -> TimelineVisitor
        auto eventRenderer = std::dynamic_pointer_cast<sheet::compiler::SheetEventRenderer>( context );
        auto midi = wm.createMidi();
        if (midiConfig) {
            midi->midiConfig = *midiConfig;
        }
        context->midi(midi);
        context->sheetTemplateDefServer(doc.get());
        auto compiler = wm.createCompiler();
        compiler->compile(doc);
        context->warnings.swap(outWarnings);
        return midi;
    }

	sheet::DocumentPtr createDocument(const std::string &file)
    {
        addSearchPaths();
        sheet::compiler::DocumentParser docparser;
        auto doc = docparser.parse(file);
        return doc;
    }

    void onCompilerError(const fm::Exception &ex)
    {
        std::cerr << ex.toString() << std::endl;
    }

    void onCompilerError(const std::exception &ex) {
	    std::cerr << ex.what() << std::endl;
    }

    void onCompilerError() {
        std::cerr << "unkown error" << std::endl;
    }
}