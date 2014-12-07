
#pragma once

using namespace System;

namespace RDTSCLib 
{
    typedef unsigned __int64 ulong;

    #pragma unmanaged

    class UnmanagedRDTSC
    {
        public: static ulong rdtsc();
    };

    #pragma managed

    public ref class RDTSC
    {
        public: static ulong rdtsc();
    };
}
