#include <iostream>
#include <coroutine>


template<typename T>
struct generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T value_;
        std::exception_ptr exception_;

        generator get_return_object() {
            return generator(handle_type::from_promise(*this));
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { exception_ = std::current_exception(); }

        template<std::convertible_to<T> From>
        std::suspend_always yield_value(From&& from) {
            value_ = std::forward<From>(from);
            return {};
        }
        void return_void() {}
    };

    handle_type h_;

    generator(handle_type h) : h_(h) {}
    ~generator() { h_.destroy(); }

    explicit operator bool() {
        fill();
        return !h_.done();
    }
    T operator()() {
        fill();
        full_ = false;
        return std::move(h_.promise().value_);
    }

private:
    bool full_ = false;

    void fill() {
        if (!full_) {
            h_();
            if (h_.promise().exception_)
                std::rethrow_exception(h_.promise().exception_);
            full_ = true;
        }
    }
};



generator<int> counter()
{
    std::cout << "=== Generator start" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "Yielding: " << i << std::endl;
        co_yield i;
    }
    std::cout << "=== Generator end" << std::endl;
}

int main(int argc, char** argv)
{
    auto gen = counter();
    while (gen)
        std::cout << "Receiving: " << gen() << std::endl;

    return 0;
}
