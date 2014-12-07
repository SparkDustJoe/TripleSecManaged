
#pragma once

#include "Blake.h"
#include "Blake-org.h"

namespace MFCTest
{
    class BlakeTest : public TestBaseCrypto
    {
        private:

            Blake::BlakeBase* m_blake;

        protected:

            virtual CString GetTestVectorsDir()
            {
                return "Blake";
            }

            virtual CString GetTestName()
            {
                return "Blake";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                m_blake->TransformBytes(a_data, a_index, a_length);
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                return m_blake->ComputeBytes(a_data, 0, a_length);
            }

            virtual byte* TransformFinal()
            {
                return m_blake->TransformFinal();
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                delete m_blake;

                if (a_hashSizeBits <= 256)
                    m_blake = new Blake::Blake256Base(a_hashSizeBits/8);
                else
                    m_blake = new Blake::Blake512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash()
            {
                m_blake->Initialize();
            }

        public:

            BlakeTest()
            {
                m_blake = nullptr;
            }

            ~BlakeTest()
            {
                delete m_blake;
            }
    };

    class BlakeOrgTest : public TestBaseCrypto
    {
        private:

            BlakeOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Blake";
            }

            virtual CString GetTestName()
            {
                return "Blake-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                BlakeOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                BlakeOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                BlakeOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                BlakeOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                BlakeOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
