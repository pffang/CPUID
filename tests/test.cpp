#include "CPUID.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

#if __GNUC__ >= 8
#    include <exception>
#    include <x86intrin.h>
#    if defined(_XSAVEINTRIN_H_INCLUDED) && defined(__XSAVE__)  // -mxsave
// gcc 8 built in _xgetbv() doesn't return a correct result, please see the gcc bug: https://gcc.gnu.org/bugzilla/show_bug.cgi?format=multiple&id=85684
#        define TEST_XGETBV
static void test_xgetbv()
{
    uint64_t value_compiler;
    try
    {
        value_compiler = _xgetbv(0);
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception catched : " << e.what() << std::endl;
        std::cerr << "Compiler bug!" << std::endl;
        return;
    }
    std::cout << "xgetbv value compiler built-in: " << value_compiler << std::endl;
    uint32_t eax, edx;
    __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
    uint64_t value_asm = ((uint64_t)edx << 32) | eax;
    std::cout << "xgetbv value asm: " << value_asm << std::endl;
    if (value_asm != value_compiler)
        std::cerr << "Compiler bug!" << std::endl;
}
#    endif
#endif

int main()
{
    CPUID cpuid;
    std::cout << "CPU Vendor: " << cpuid.Vendor() << std::endl;
    std::cout << "CPU Brand: " << cpuid.Brand() << std::endl;
    std::cout << "SSE: " << cpuid.SSE() << std::endl;
    std::cout << "SSE2: " << cpuid.SSE2() << std::endl;
    std::cout << "SSE3: " << cpuid.SSE3() << std::endl;
    std::cout << "AVX: " << cpuid.AVX() << std::endl;
    std::cout << "AVX2: " << cpuid.AVX2() << std::endl;
    std::cout << "OSXSAVE: " << cpuid.OSXSAVE() << std::endl;
    std::cout << "OS_AVX: " << cpuid.OS_AVX() << std::endl;
    std::cout << "OS_AVX2: " << cpuid.OS_AVX2() << std::endl;
    std::cout << "SHA: " << cpuid.SHA() << std::endl;
    std::cout << "AES: " << cpuid.AES() << std::endl;

#if defined(TEST_XGETBV)
    test_xgetbv();
#endif
    return EXIT_SUCCESS;
}
