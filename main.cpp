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

    auto do_other_stuff() {
        static_cast<Type*>(this)->do_other_stuff;
    }
};

class Derived : public Base<Derived> {
private:
    uint64_t n;

public:
    auto do_stuff() {
        return n;
    }

    auto do_other_stuff() {
        return n += 1;
    }
};

template <typename Type>
void RunCRTP(Base<Type>* object)
{
    const unsigned int N = 40000;
    for(auto i = 0; i < N; i++) {
        for(auto j = 0; j < i; j++) {
            object->do_other_stuff();
        }
    }
}


class VBase {
public:
    virtual uint64_t v_do_stuff() = 0;
    virtual uint64_t v_do_other_stuff() = 0;
};

class VDerived : public VBase {
private:
    uint64_t n;
public:
    virtual uint64_t v_do_stuff() override {
        return n;
    }

    virtual uint64_t v_do_other_stuff() override {
        return n += 1;
    }
};

void RunNormal(VBase  * object)
{
    
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

