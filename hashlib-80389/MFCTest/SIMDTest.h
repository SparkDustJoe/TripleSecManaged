
#pragma once

#include "SIMD-org.h"
#include "SIMD.h"

namespace MFCTest
{
    class SIMDTest : public TestBaseCrypto
    {
        private:

            SIMD::SIMDBase* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "SIMD";
            }

            virtual CString GetTestName()
            {
                return "SIMD";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                m_hash->TransformBytes(a_data, a_index, a_length); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                return m_hash->ComputeBytes(a_data, a_length);
            }

            virtual byte* TransformFinal()
            {
                return m_hash->TransformFinal();
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                delete m_hash;

                if (a_hashSizeBits <= 256)
                    m_hash = new SIMD::SIMD256Base(a_hashSizeBits / 8);
                else
                    m_hash = new SIMD::SIMD512Base(a_hashSizeBits / 8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            SIMDTest()
            {
                m_hash = nullptr;
            }

            ~SIMDTest()
            {
                delete m_hash;
            }
    };

    class SIMDOrgTest : public TestBaseCrypto
    {
        private:

            SIMDOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "SIMD";
            }

            virtual CString GetTestName()
            {
                return "SIMD-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                SIMDOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                SIMDOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                SIMDOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                SIMDOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                SIMDOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
