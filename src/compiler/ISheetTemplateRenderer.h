#pragma once

#include <memory>

namespace documentModel
{
    struct Track;
}
namespace compiler
{
    class ISheetTemplateRenderer
    {
    public:
        virtual void render(Track *sheetTrack) = 0;
    };
    typedef std::shared_ptr<ISheetTemplateRenderer> ISheetTemplateRendererPtr;
}
