#pragma once

namespace mscorlib::System
{
    template <typename T>
    class Array
    {
    private:
        void* klass;
        void* monitor;
        void* bounds;
        int   max_length;
        void* vector[1];

    public:
        int length() { return max_length; }
        T* data() { return (T*)vector; }

        T Get(int i)
        {
            if (i >= max_length)
                return nullptr;
            return ((T*)vector)[i];
        }
    };
}

using namespace mscorlib;
