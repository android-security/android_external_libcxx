//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads
//  ... test crashes clang

// <atomic>

// template <class Integral>
//     Integral
//     atomic_fetch_sub_explicit(volatile atomic<Integral>* obj, Integral op,
//                               memory_order m);
// template <class Integral>
//     Integral
//     atomic_fetch_sub_explicit(atomic<Integral>* obj, Integral op,
//                               memory_order m);
// 
// template <class T>
//     T*
//     atomic_fetch_sub_explicit(volatile atomic<T*>* obj, ptrdiff_t op,
//                               memory_order m);
// template <class T>
//     T*
//     atomic_fetch_sub_explicit(atomic<T*>* obj, ptrdiff_t op, memory_order m);

#include <atomic>
#include <type_traits>
#include <cassert>

template <class T>
void
test()
{
    {
        typedef std::atomic<T> A;
        A t;
        std::atomic_init(&t, T(3));
        assert(std::atomic_fetch_sub_explicit(&t, T(2),
                                            std::memory_order_seq_cst) == T(3));
        assert(t == T(1));
    }
    {
        typedef std::atomic<T> A;
        volatile A t;
        std::atomic_init(&t, T(3));
        assert(std::atomic_fetch_sub_explicit(&t, T(2),
                                            std::memory_order_seq_cst) == T(3));
        assert(t == T(1));
    }
}

template <class T>
void
testp()
{
    {
        typedef std::atomic<T> A;
        typedef typename std::remove_pointer<T>::type X;
        A t;
        std::atomic_init(&t, T(3*sizeof(X)));
        assert(std::atomic_fetch_sub_explicit(&t, 2,
                                  std::memory_order_seq_cst) == T(3*sizeof(X)));
        assert(t == T(1*sizeof(X)));
    }
    {
        typedef std::atomic<T> A;
        typedef typename std::remove_pointer<T>::type X;
        volatile A t;
        std::atomic_init(&t, T(3*sizeof(X)));
        assert(std::atomic_fetch_sub_explicit(&t, 2,
                                  std::memory_order_seq_cst) == T(3*sizeof(X)));
        assert(t == T(1*sizeof(X)));
    }
}

struct A
{
    int i;

    explicit A(int d = 0) noexcept {i=d;}
    A(const A& a) : i(a.i) {}
    A(const volatile A& a) : i(a.i) {}

    void operator=(const volatile A& a) volatile {i = a.i;}

    friend bool operator==(const A& x, const A& y)
        {return x.i == y.i;}
};

int main()
{
    test<char>();
    test<signed char>();
    test<unsigned char>();
    test<short>();
    test<unsigned short>();
    test<int>();
    test<unsigned int>();
    test<long>();
    test<unsigned long>();
    test<long long>();
    test<unsigned long long>();
    test<wchar_t>();
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    test<char16_t>();
    test<char32_t>();
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
    testp<int*>();
    testp<const int*>();
}
