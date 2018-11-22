#ifndef AMIDI_PROVIDER_H
#define AMIDI_PROVIDER_H



namespace fmapp {
	class AMidiProvider {
	public:
		virtual ~AMidiProvider() = default;
		virtual void reset() {}
	};
}

#endif