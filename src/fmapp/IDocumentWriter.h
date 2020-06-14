#ifndef FMAPP_IDOCUMENTWRITER_H
#define FMAPP_IDOCUMENTWRITER_H

#include <forward.hpp>

namespace fmapp {
    class IDocumentWriter {
    public:
        virtual void write(sheet::DocumentPtr document) = 0;
    };
    typedef std::shared_ptr<IDocumentWriter> IDocumentWriterPtr; 
}

#endif