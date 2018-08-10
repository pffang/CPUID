#include "CPUID.hpp"

#include <cstdint>
#include <cstring>

#if defined(_MSC_VER)
#    include <intrin.h>
/* %ecx */
#    define bit_SSE3 (1 << 0)
#    define bit_PCLMUL (1 << 1)
#    define bit_LZCNT (1 << 5)
#    define bit_SSSE3 (1 << 9)
#    define bit_FMA (1 << 12)
#    define bit_CMPXCHG16B (1 << 13)
#    define bit_SSE4_1 (1 << 19)
#    define bit_SSE4_2 (1 << 20)
#    define bit_MOVBE (1 << 22)
#    define bit_POPCNT (1 << 23)
#    define bit_AES (1 << 25)
#    define bit_XSAVE (1 << 26)
#    define bit_OSXSAVE (1 << 27)
#    define bit_AVX (1 << 28)
#    define bit_F16C (1 << 29)
#    define bit_RDRND (1 << 30)

/* %edx */
#    define bit_CMPXCHG8B (1 << 8)
#    define bit_CMOV (1 << 15)
#    define bit_MMX (1 << 23)
#    define bit_FXSAVE (1 << 24)
#    define bit_SSE (1 << 25)
#    define bit_SSE2 (1 << 26)

/* Extended Features (%eax == 0x80000001) */
/* %ecx */
#    define bit_LAHF_LM (1 << 0)
#    define bit_ABM (1 << 5)
#    define bit_SSE4a (1 << 6)
#    define bit_PRFCHW (1 << 8)
#    define bit_XOP (1 << 11)
#    define bit_LWP (1 << 15)
#    define bit_FMA4 (1 << 16)
#    define bit_TBM (1 << 21)
#    define bit_MWAITX (1 << 29)

/* %edx */
#    define bit_MMXEXT (1 << 22)
#    define bit_LM (1 << 29)
#    define bit_3DNOWP (1 << 30)
#    define bit_3DNOW (1u << 31)

/* %ebx  */
#    define bit_CLZERO (1 << 0)
#    define bit_WBNOINVD (1 << 9)

/* Extended Features (%eax == 7) */
/* %ebx */
#    define bit_FSGSBASE (1 << 0)
#    define bit_SGX (1 << 2)
#    define bit_BMI (1 << 3)
#    define bit_HLE (1 << 4)
#    define bit_AVX2 (1 << 5)
#    define bit_BMI2 (1 << 8)
#    define bit_RTM (1 << 11)
#    define bit_MPX (1 << 14)
#    define bit_AVX512F (1 << 16)
#    define bit_AVX512DQ (1 << 17)
#    define bit_RDSEED (1 << 18)
#    define bit_ADX (1 << 19)
#    define bit_AVX512IFMA (1 << 21)
#    define bit_CLFLUSHOPT (1 << 23)
#    define bit_CLWB (1 << 24)
#    define bit_AVX512PF (1 << 26)
#    define bit_AVX512ER (1 << 27)
#    define bit_AVX512CD (1 << 28)
#    define bit_SHA (1 << 29)
#    define bit_AVX512BW (1 << 30)
#    define bit_AVX512VL (1u << 31)

/* %ecx */
#    define bit_PREFETCHWT1 (1 << 0)
#    define bit_AVX512VBMI (1 << 1)
#    define bit_PKU (1 << 3)
#    define bit_OSPKE (1 << 4)
#    define bit_AVX512VBMI2 (1 << 6)
#    define bit_SHSTK (1 << 7)
#    define bit_GFNI (1 << 8)
#    define bit_VAES (1 << 9)
#    define bit_AVX512VNNI (1 << 11)
#    define bit_VPCLMULQDQ (1 << 10)
#    define bit_AVX512BITALG (1 << 12)
#    define bit_AVX512VPOPCNTDQ (1 << 14)
#    define bit_RDPID (1 << 22)
#    define bit_MOVDIRI (1 << 27)
#    define bit_MOVDIR64B (1 << 28)

/* %edx */
#    define bit_AVX5124VNNIW (1 << 2)
#    define bit_AVX5124FMAPS (1 << 3)
#    define bit_IBT (1 << 20)
#    define bit_PCONFIG (1 << 18)
/* XFEATURE_ENABLED_MASK register bits (%eax == 13, %ecx == 0) */
#    define bit_BNDREGS (1 << 3)
#    define bit_BNDCSR (1 << 4)

/* Extended State Enumeration Sub-leaf (%eax == 13, %ecx == 1) */
#    define bit_XSAVEOPT (1 << 0)
#    define bit_XSAVEC (1 << 1)
#    define bit_XSAVES (1 << 3)

#elif defined(__GNUC__)
#    include <cpuid.h>
#    include <x86intrin.h>
#endif

#if !defined(_XCR_XFEATURE_ENABLED_MASK)
#    define _XCR_XFEATURE_ENABLED_MASK 0
#endif

static inline uint64_t __i_xgetbv(uint32_t index)
{
#if _MSC_VER >= 1600 || (__GNUC__ >= 8 && __GNUC_MINOR__ >= 2 && defined(_XSAVEINTRIN_H_INCLUDED) && defined(__XSAVE__))  // -mxsave
// gcc 8 built in _xgetbv() doesn't return a correct result, please see the gcc bug: https://gcc.gnu.org/bugzilla/show_bug.cgi?format=multiple&id=85684
#    pragma message("use compiler builtin _xgetbv")
    return _xgetbv(index);
#else
    uint32_t eax, edx;
#    if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)) || __clang_major__ >= 5
#        pragma message("use inline assembly xgetbv")
    __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
#    else
#        pragma message("use inline assembly bytecode")
    __asm__ __volatile__(".byte 0x0f, 0x01, 0xd0" : "=a"(eax), "=d"(edx) : "c"(index));
#    endif
    return ((uint64_t)edx << 32) | eax;
#endif
}

static inline void __i_cpuid(uint32_t id, uint32_t subid, uint32_t *info)
{
#if defined(_MSC_VER)
    __cpuidex((int *)info, id, subid);
#elif defined(__GNUC__)
    __cpuid_count(id, subid, info[0], info[1], info[2], info[3]);
#endif
}

CPUID::CPUID_Internal::CPUID_Internal() : f_1_ECX_(0), f_1_EDX_(0), f_7_EBX_(0), f_7_ECX_(0), f_81_ECX_(0), f_81_EDX_(0)
{
    // get vendor string
    uint32_t info[4];
    __i_cpuid(0, 0, info);
    if (0 == info[0])
        return;
    uint32_t nIds = info[0];
    char name[49];
    memcpy(name + 0, info + 1, 4);
    memcpy(name + 4, info + 3, 4);
    memcpy(name + 8, info + 2, 4);
    name[12] = '\0';
    vendor   = name;

    if (nIds >= 1)
    {
        __i_cpuid(1, 0, info);
        f_1_ECX_ = info[2];
        f_1_EDX_ = info[3];
    }

    if (nIds >= 7)
    {
        __i_cpuid(7, 0, info);
        f_7_EBX_ = info[1];
        f_7_ECX_ = info[2];
    }

    __i_cpuid(0x80000000, 0, info);
    if (0 == info[0])
        return;

    uint32_t nExIds_ = info[0];
    // get CPU brand string
    if (nExIds_ >= 0x80000004)
    {
        for (uint32_t i = 0x80000002, j = 0; i <= 0x80000004; i++, j++)
        {
            __i_cpuid(i, 0, info);
            memcpy(name + j * 16, info, 16);
        }
        name[48] = '\0';
        brand    = name;
    }
}

const CPUID::CPUID_Internal CPUID::id;

const std::string &CPUID::Vendor()
{
    return id.vendor;
}

const std::string &CPUID::Brand()
{
    return id.brand;
}

bool CPUID::SSE()
{
    return id.f_1_EDX_ & bit_SSE;
}

bool CPUID::SSE2()
{
    return id.f_1_EDX_ & bit_SSE2;
}

bool CPUID::SSE3()
{
    return id.f_1_ECX_ & bit_SSE3;
}

bool CPUID::AVX()
{
    return id.f_1_ECX_ & bit_AVX;
}

bool CPUID::OS_AVX()
{
    bool OS_AVX_Supported = false;
    if (OSXSAVE() && AVX())
    {
        uint64_t xcrFeatureMask = __i_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
        OS_AVX_Supported        = xcrFeatureMask & 0x6;
    }
    return OS_AVX_Supported;
}

bool CPUID::AVX2()
{
    return id.f_7_EBX_ & bit_AVX2;
}

bool CPUID::OS_AVX2()
{
    bool OS_AVX2_Supported = false;
    if (OSXSAVE() && AVX2())
    {
        uint64_t xcrFeatureMask = __i_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
        OS_AVX2_Supported       = xcrFeatureMask & 0x6;
    }
    return OS_AVX2_Supported;
}

bool CPUID::OSXSAVE()
{
    return id.f_1_ECX_ & bit_OSXSAVE;
}

bool CPUID::SHA()
{
    return id.f_7_EBX_ & bit_SHA;
}
bool CPUID::AES()
{
    return id.f_1_ECX_ & bit_AES;
}
