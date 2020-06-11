#ifndef ICOMPILER_SHEET_TEMPLATERENDERER_H
#define ICOMPILER_SHEET_TEMPLATERENDERER_H


namespace sheet {
    struct Track;
    namespace compiler {
        class ISheetTemplateRenderer {
        public:
            virtual void render(Track * sheetTrack) = 0;
        };
    }
}

#endif