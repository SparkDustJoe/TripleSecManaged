
#pragma once

#include "Shabal-org.h"
#include "Shabal.h"

namespace MFCTest
{
    class ShabalTest : public TestBaseCrypto
    {
        private:

            Shabal::Shabal* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Shabal";
            }

            virtual CString GetTestName()
            {
                return "Shabal";
            }

            virtual int GetMaxBufferSize()
            {
                return 192; 
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
                m_hash = new Shabal::Shabal(a_hashSizeBits/8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            ShabalTest()
            {
                m_hash = nullptr;
            }

            ~ShabalTest()
            {
                delete m_hash;
            }
    };

    class ShabalOrgTest : public TestBaseCrypto
    {
        private:

            ShabalOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Shabal";
            }

            virtual CString GetTestName()
            {
                return "Shabal-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 192;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                ShabalOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                ShabalOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                ShabalOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                ShabalOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                ShabalOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
