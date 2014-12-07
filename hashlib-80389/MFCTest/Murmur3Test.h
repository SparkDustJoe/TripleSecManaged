
#pragma once

#include "stdafx.h"
#include "Murmur3-org.h"
#include "Murmur3.h"

namespace MFCTest
{
    class Murmur3_32_OrgTest : public TestBaseNonCryptoNonBlock
    {
        protected: 
        
            virtual int GetHashSize()
            {
                return 4;
            }

            virtual CString GetTestVectorsDir()
            {
                return "Murmur3_32";
            }

            virtual CString GetTestName()
            {
                return "Murmur3_32-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 25; // real: 4
            }

        public:

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[GetHashSize()];
                Murmur3Org::MurmurHash3_x86_32(a_data, a_length, 0xc58f1a7b, out);
                return out;
            }

            virtual void InitializeHash()
            {
            }
    };

    class Murmur3_128_OrgTest : public TestBaseNonCryptoNonBlock
    {
        protected: 
        
            virtual int GetHashSize()
            {
                return 16;
            }

            virtual CString GetTestVectorsDir()
            {
                return "Murmur3_128";
            }

            virtual CString GetTestName()
            {
                return "Murmur3_128-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 16;
            }

        public:

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[GetHashSize()];
                Murmur3Org::MurmurHash3_x64_128(a_data, a_length, 0xc58f1a7b, out);
                return out;
            }

            virtual void InitializeHash()
            {
            }
    };

    class Murmur3_32_Test : public TestBaseNonCryptoNonBlock
    {
        private:

            Murmur3::Murmur3 m_hash;

        protected: 
        
            virtual int GetHashSize()
            {
                return 4;
            }

            virtual CString GetTestVectorsDir()
            {
                return "Murmur3_32";
            }

            virtual CString GetTestName()
            {
                return "Murmur3_32";
            }

            virtual int GetMaxBufferSize()
            {
                return 25; // real: 4
            }

        public:

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                return m_hash.MurmurHash3_x86_32(a_data, a_length);
            }

            virtual void InitializeHash()
            {
            }
    };

    class Murmur3_128_Test : public TestBaseNonCryptoNonBlock
    {
        private:

            Murmur3::Murmur3 m_hash;

        protected: 
        
            virtual int GetHashSize()
            {
                return 16;
            }

            virtual CString GetTestVectorsDir()
            {
                return "Murmur3_128";
            }

            virtual CString GetTestName()
            {
                return "Murmur3_128";
            }

            virtual int GetMaxBufferSize()
            {
                return 16;
            }

        public:

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                return m_hash.MurmurHash3_x64_128(a_data, a_length);
            }

            virtual void InitializeHash()
            {
            }
    };
}