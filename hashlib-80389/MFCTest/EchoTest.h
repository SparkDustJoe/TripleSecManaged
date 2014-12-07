
#pragma once

#include "Echo-org.h"
#include "Echo.h"

namespace MFCTest
{
    class EchoTest : public TestBaseCrypto
    {
        private:

            Echo::EchoBase* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Echo";
            }

            virtual CString GetTestName()
            {
                return "Echo";
            }

            virtual int GetMaxBufferSize()
            {
                return 256;
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
                    m_hash = new Echo::Echo256Base(a_hashSizeBits/8);
                else
                    m_hash = new Echo::Echo512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            EchoTest()
            {
                m_hash = nullptr;
            }

            ~EchoTest()
            {
                delete m_hash;
            }
    };

    class EchoOrgTest : public TestBaseCrypto
    {
        private:

            EchoOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Echo";
            }

            virtual CString GetTestName()
            {
                return "Echo-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 256;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                EchoOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                EchoOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                EchoOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                EchoOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                EchoOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
