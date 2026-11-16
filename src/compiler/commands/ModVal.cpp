#include "ModVal.h"
#include <compiler/error.hpp>
#include <compiler/context/IContext.h>
#include <compiler/modification/AModification.h>
#include <com/werckmeister.hpp>
#include <algorithm>

namespace compiler
{
    void ModVal::execute(IContextPtr context)
    {
        com::String modName = parameters[argumentNames.ModVal.Mod].value<com::String>();
        auto meta = context->voiceMetaData();
        ///auto  theModification = loadMod(context, modName);
        //meta->modifications.push_back(theModification);
        AModificationPtr theMod = nullptr;
        for(auto modptr : meta->modifications)
        {
            if (modptr->name == modName)
            {
                theMod = modptr;
                break;
            }
        }
        if (theMod == nullptr) 
        {
            auto instrument = context->currentInstrumentDef();
            for(auto modptr : instrument->modifications)
            {
                if (modptr->name == modName)
                {
                    theMod = modptr;
                    break;
                }
            }
        }
        if (theMod == nullptr)
        {
            FM_THROW(Exception, "Mod not found: " + modName);
        }
        for(auto argIt : modArgs)
        {
            update(theMod, argIt.name, argIt.value);
        }
    }

    void ModVal::update(AModificationPtr theMod, const com::String &parameterName, const com::String &stringValue)
    {
        auto &modParams = theMod->getParameters();
        auto modParamIt = modParams.find(parameterName);
        if (modParamIt == modParams.end())
        {
            FM_THROW(Exception, "Mod argument not found: " + parameterName);
        }
        modParamIt->second.strValue(stringValue);
    }

    void ModVal::setArguments(const Arguments &args)
    {
        Base::setArguments(args);
        // remove "use" Argument from collection
        Arguments copy(args.size());
        bool paramUsefound = false;
        auto copiedIt = std::copy_if(args.begin(), args.end(), copy.begin(), [&paramUsefound](const auto& x)
            {
                if (!paramUsefound && x.name == argumentNames.ModVal.Mod)
                {
                    paramUsefound = true;
                }
                return x.name != argumentNames.ModVal.Mod;
            });
        if (paramUsefound)
        {
            copy.resize(std::distance(copy.begin(), copiedIt));
        }
        else
        {
            // not found by name -> skip first arg
            copy = Arguments(copy.begin() + 1, copy.end());
        }
        modArgs.swap(copy);
    }
}