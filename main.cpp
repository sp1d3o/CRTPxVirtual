#include <iostream>
#include <stdint.h>
#include <chrono>

template <typename Type>
class Base {
private:
    Base();
    friend Type;
public:
    auto do_stuff() {
        static_cast<Type*>(this)->do_stuff;
    }

    auto do_other_stuff(uint64_t n) {
        static_cast<Type*>(this)->do_other_stuff;
    }
};

class Derived : public Base<Derived> {
private:
    uint64_t counter;

public:
    auto do_stuff() {
        return counter;
    }

    auto do_other_stuff(uint64_t n) {
        return counter += n;
    }
};

template <typename Type>
void RunCRTP(Base<Type>* object)
{
    const unsigned int N = 40000;
    for(auto i = 0; i < N; i++) {
        for(auto j = 0; j < i; j++) {
            object->do_other_stuff(j);
        }
    }

    std::cout << object->do_stuff() << std::endl;
}


class VBase {
public:
    virtual uint64_t v_do_stuff() = 0;
    virtual uint64_t v_do_other_stuff(uint64_t n) = 0;
};

class VDerived : public VBase {
private:
    uint64_t counter;
public:
    virtual uint64_t v_do_stuff() override {
        return counter;
    }

    virtual uint64_t v_do_other_stuff(uint64_t n) override {
        return counter += n;
    }
};

void RunNormal(VBase  * object)
{
    const unsigned int N = 40000;
    for(auto i = 0; i < N; i++) {
        for(auto j = 0; j < i; j++) {
            object->v_do_other_stuff(j);
        }
    }

    std::cout << object->v_do_stuff() << std::endl;
}

template <typename T, typename S>
void Benchmark(T & object, S function, const std::string name)
{
    auto start = std::chrono::high_resolution_clock::now();

    function(&object);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Name" << ":" << name << std::endl;
    std::cout << "Time" <<":" << duration.count() << std::endl;
}

int main()
{
    VDerived v_object;
    Benchmark(v_object, RunNormal, "Virtual Interface");

    Derived object;
    Benchmark(object, RunCRTP<Derived>, "CRTP interface");

    return 0;

}
