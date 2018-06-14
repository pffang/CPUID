#include "CPUID.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char **argv)
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
    return EXIT_SUCCESS;
}
