
#pragma once

#include "Hamsi-org.h"
#include "Hamsi.h"

namespace MFCTest
{

    class HamsiTest : public TestBaseCrypto
    {
        private:

            Hamsi::HamsiBase* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Hamsi";
            }

            virtual CString GetTestName()
            {
                return "Hamsi";
            }

            virtual int GetMaxBufferSize()
            {
                return 64;
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
                    m_hash = new Hamsi::Hamsi256Base(a_hashSizeBits/8);
                else
                    m_hash = new Hamsi::Hamsi512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            HamsiTest()
            {
                m_hash = nullptr;
            }

            ~HamsiTest()
            {
                delete m_hash;
            }
    };

    class HamsiOrgTest : public TestBaseCrypto
    {
        private:

            HamsiOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Hamsi";
            }

            virtual CString GetTestName()
            {
                return "Hamsi-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 64;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                HamsiOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                HamsiOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                HamsiOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                HamsiOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                HamsiOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
