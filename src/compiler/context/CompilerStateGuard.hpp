#pragma once

#include "IContext.h"

namespace compiler
{
    struct CompilerStateGuard
    {
        IContextPtr ctx;
        CompilerStateGuard(IContextPtr ctx, IContext::CompilerState state) : ctx(ctx)
        {
            ctx->beginState(state);
        }
        ~CompilerStateGuard()
        {
            ctx->endState();
        }
    };
}