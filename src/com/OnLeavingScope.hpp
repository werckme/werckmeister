#include <functional>

namespace com
{
    class OnLeavingScope 
    {
    public:
        typedef std::function<void()> Handler;
        OnLeavingScope(Handler handler) : _handler(handler) {}
        ~OnLeavingScope() { _handler(); }
    private:
        Handler _handler;
    };
}