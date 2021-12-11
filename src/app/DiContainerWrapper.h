#ifndef FMAPP_DICONTAINER_WRAPPER_HPP
#define FMAPP_DICONTAINER_WRAPPER_HPP


#include <vector>

namespace app {
    /**
     * Boost.DI has several examples for "Bind multiple bindings", but none of them 
     * shows how to create multiple bindingy dynamically. I've tried several approaches, to get this to work.
     * So at the end, as workarounf, I inject a DiContainerWrapper and build the content dynamically:
     * 	di::bind<app::DiContainerWrapper<app::IPlayerLoopVisitorPtr>>().to([&](const auto &injector) {
	 *		app::DiContainerWrapper<app::IPlayerLoopVisitorPtr> wrapper;
	 *		wrapper.container.push_back( injector.template create< std::shared_ptr<app::PlayerTimePrinter>>() );
	 *		return wrapper;
	 *	})
     *  
     **/
    template <typename T>
    struct DiContainerWrapper {
        typedef std::vector<T> Container;
        Container container;
    };
}
#endif