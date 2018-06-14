#include <cstdint>
#include <string>

class CPUID
{
    class CPUID_Internal
    {
    public:
        CPUID_Internal();

        std::string vendor;
        std::string brand;
        uint32_t f_1_ECX_;
        uint32_t f_1_EDX_;
        uint32_t f_7_EBX_;
        uint32_t f_7_ECX_;
        uint32_t f_81_ECX_;
        uint32_t f_81_EDX_;
    };

public:
    CPUID()  = default;
    ~CPUID() = default;

    static const std::string &Vendor();
    static const std::string &Brand();

    static bool SSE();
    static bool SSE2();
    static bool SSE3();
    static bool AVX();
    static bool OS_AVX();
    static bool AVX2();
    static bool OS_AVX2();
    static bool OSXSAVE();
    static bool SHA();
    static bool AES();

private:
    static const CPUID_Internal id;
};
