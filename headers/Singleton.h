#pragma once

template <typename T>

class Singleton 
{
public:
    static T& getInstance() 
    {
        static DerivedInstance instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    class DerivedInstance : public T 
    {
    };
};