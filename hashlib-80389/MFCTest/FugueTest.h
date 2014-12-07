
#pragma once

#include "Fugue-org.h"
#include "Fugue.h"

namespace MFCTest
{
    class FugueTest : public TestBaseCrypto
    {
        private:

            Fugue::FugueBase* m_hash;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Fugue";
            }

            virtual CString GetTestName()
            {
                return "Fugue";
            }

            virtual int GetMaxBufferSize()
            {
                return 144;
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
                    case 224: m_hash = new Fugue::Fugue224(); break;
                    case 256: m_hash = new Fugue::Fugue256(); break;
                    case 384: m_hash = new Fugue::Fugue384(); break;
                    case 512: m_hash = new Fugue::Fugue512(); break;
                }

                m_hash->Initialize();
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            FugueTest()
            {
                m_hash = nullptr;
            }

            ~FugueTest()
            {
                delete m_hash;
            }
    };

    class FugueOrgTest : public TestBaseCrypto
    {
        private:

            FugueOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Fugue";
            }

            virtual CString GetTestName()
            {
                return "Fugue-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 144;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                FugueOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                int len = m_hashState.hashbitlen;
                if (m_hashState.hashbitlen == 224)
                    len = 256;

                byte* out = new byte[len/8];
                FugueOrg::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                int len = m_hashState.hashbitlen;
                if (m_hashState.hashbitlen == 224)
                    len = 256;

                byte* out = new byte[len/8];
                FugueOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                FugueOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                FugueOrg::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
