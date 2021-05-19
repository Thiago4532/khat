#include <utility> // std::forward
#include <functional> // std::invoke

class useless_thread {
public:
    template< class Function, class... Args > 
    explicit useless_thread( Function&& f, Args&&... args ) {
        std::invoke(_decay_copy(std::forward<Function>(f)), 
            _decay_copy(std::forward<Args>(args))...);
    }

    void join() { }
private:
    template <class T>
    static std::decay_t<T> _decay_copy(T&& v) { return std::forward<T>(v); }
};