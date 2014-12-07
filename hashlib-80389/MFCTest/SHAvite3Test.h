
#pragma once

#include "SHAvite3.h"
#include "SHAvite3-org.h"

namespace MFCTest
{
    class SHAvite3Test : public TestBaseCrypto
    {
        private:

            SHAvite3::SHAvite3Base* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "SHAvite3";
            }

            virtual CString GetTestName()
            {
                return "SHAvite3";
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
                    m_hash = new SHAvite3::SHAvite3_256Base(a_hashSizeBits/8);
                else
                    m_hash = new SHAvite3::SHAvite3_512Base(a_hashSizeBits/8);
                m_hash->Initialize();
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            SHAvite3Test()
            {
                m_hash = nullptr;
            }

            ~SHAvite3Test()
            {
                delete m_hash;
            }
    };

    class SHAvite3OrgTest : public TestBaseCrypto
    {
        private:

            SHAvite3Org::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "SHAvite3";
            }

            virtual CString GetTestName()
            {
                return "SHAvite3-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                SHAvite3Org::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.DigestSize/8];
                SHAvite3Org::Hash(m_hashState.DigestSize, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.DigestSize/8];
                SHAvite3Org::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                SHAvite3Org::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                SHAvite3Org::Init(&m_hashState, m_hashState.DigestSize);
            }
    };
}
