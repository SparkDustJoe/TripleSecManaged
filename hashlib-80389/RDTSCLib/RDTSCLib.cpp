
#include "RDTSCLib.h"
#include <intrin.h>

using namespace System;

namespace RDTSCLib 
{
    #pragma unmanaged

    ulong UnmanagedRDTSC::rdtsc()
    {
        return __rdtsc();
    }

    #pragma managed

    ulong RDTSC::rdtsc()
    {
        return UnmanagedRDTSC::rdtsc();
    }
}
