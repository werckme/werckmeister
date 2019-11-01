#include "midiplayer.h"
#include <fm/config.hpp>


namespace fmapp {
	Midiplayer & getMidiplayer()
	{
		typedef Loki::SingletonHolder<Midiplayer> Holder;
		return Holder::Instance();
	}
}