
#pragma once

#include "CubeHash.h"
#include "CubeHash-org.h"

namespace MFCTest
{
    class CubeHashTest : public TestBaseCrypto
    {
        private:

            CubeHash::CubeHash* m_hash;

        protected:

            virtual CString GetTestVectorsDir()
            {
                CString str;
                str.Format(L"CubeHash");
                return str;
            }

            virtual CString GetTestName()
            {
                CString str;
                str.Format(L"CubeHash");
                return str;
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                m_hash->TransformBytes(a_data + a_index, a_length); 
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
                m_hash = new CubeHash::CubeHash(a_hashSizeBits/8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            CubeHashTest()
            {
                m_hash = nullptr;
            }

            ~CubeHashTest()
            {
                delete m_hash;
            }
    };

    class CubeHashOrgTest : public TestBaseCrypto
    {
        private:

            CubeHashOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "CubeHash";
            }

            virtual CString GetTestName()
            {
                return "CubeHash-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                CubeHashOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                CubeHashOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                CubeHashOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                CubeHashOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                CubeHashOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
