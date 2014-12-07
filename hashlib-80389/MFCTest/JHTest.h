
#pragma once

#include "JH.h"
#include "JH-org.h"

namespace MFCTest
{
    class JHTest : public TestBaseCrypto
    {
        private:

            JH::JH* m_hash;

        protected:

            virtual CString GetTestVectorsDir()
            {
                return "JH";
            }

            virtual CString GetTestName()
            {
                return "JH";
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
                m_hash = new  JH::JH(a_hashSizeBits / 8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            JHTest()
            {
                m_hash = nullptr;
            }

            ~JHTest()
            {
                delete m_hash;
            }
    };

    class JHOrgTest : public TestBaseCrypto
    {
        private:

            JHOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "JH";
            }

            virtual CString GetTestName()
            {
                return "JH-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                JHOrg::Update(&m_hashState, a_data + a_index, a_length * 8);
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                JHOrg::Hash(m_hashState.hashbitlen, a_data, a_length * 8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                JHOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                JHOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                JHOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
