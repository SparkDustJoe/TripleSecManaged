
#pragma once

#include "Keccak-org.h"
#include "Keccak.h"

namespace MFCTest
{
    class KeccakTest : public TestBaseCrypto
    {
        private:

            Keccak::Keccak* m_hash;

        protected:

            virtual CString GetTestVectorsDir()
            {
                return "Keccak";
            }

            virtual CString GetTestName()
            {
                return "Keccak";
            }

            virtual int GetMaxBufferSize()
            {
                return 200;
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
                m_hash = new Keccak::Keccak(a_hashSizeBits / 8);
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            KeccakTest()
            {
                m_hash = nullptr;
            }

            ~KeccakTest()
            {
                delete m_hash;
            }
    };

    class KeccakOrgTest : public TestBaseCrypto
    {
        private:

        KeccakOrg::hashState m_hashState;

        protected:

        virtual CString GetTestVectorsDir()
        {
            return "Keccak";
        }

        virtual CString GetTestName()
        {
            return "Keccak-Org";
        }

        virtual int GetMaxBufferSize()
        {
            return 200;
        }

        virtual void TransformBytes(byte* a_data, int a_index, int a_length)
        {
            KeccakOrg::Update(&m_hashState, a_data + a_index, a_length * 8);
        }

        virtual byte* ComputeBytes(byte* a_data, int a_length)
        {
            byte* out = new byte[m_hashState.hashbitlen / 8];
            KeccakOrg::Hash(m_hashState.hashbitlen, a_data, a_length * 8, out);
            return out;
        }

        virtual byte* TransformFinal()
        {
            byte* out = new byte[m_hashState.hashbitlen / 8];
            KeccakOrg::Final(&m_hashState, out);
            return out;
        }

        virtual void CreateHash(int a_hashSizeBits)
        {
            KeccakOrg::Init(&m_hashState, a_hashSizeBits);
        }

        virtual void InitializeHash()
        {
            KeccakOrg::Init(&m_hashState, m_hashState.hashbitlen);
        }
    };
}
