
#pragma once

#include "Luffa-org.h"
#include "Luffa.h"

namespace MFCTest
{
    class LuffaTest : public TestBaseCrypto
    {
        private:

            Luffa::LuffaBase* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Luffa";
            }

            virtual CString GetTestName()
            {
                return "Luffa";
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
                switch (a_hashSizeBits)
                {
                    case 224:
                    case 256: m_hash = new Luffa::Luffa256Base(a_hashSizeBits/8); break;
                    case 384: m_hash = new Luffa::Luffa384(a_hashSizeBits/8); break;
                    case 512: m_hash = new Luffa::Luffa512(a_hashSizeBits/8); break;
                }
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            LuffaTest()
            {
                m_hash = nullptr;
            }

            ~LuffaTest()
            {
                delete m_hash;
            }
    };

    class LuffaOrgTest : public TestBaseCrypto
    {
        private:

            LuffaOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Luffa";
            }

            virtual CString GetTestName()
            {
                return "Luffa-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 256;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                LuffaOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                LuffaOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                LuffaOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                LuffaOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                LuffaOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
