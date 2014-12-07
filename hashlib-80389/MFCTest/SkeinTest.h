
#pragma once

#include "Skein.h"
#include "Skein-org.h"

namespace MFCTest
{
    class SkeinTest : public TestBaseCrypto
    {
        private:

            Skein::Skein* m_hash;

        protected:

            virtual CString GetTestVectorsDir()
            {
                return "Skein";
            }

            virtual CString GetTestName()
            {
                return "Skein";
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
                    m_hash = new Skein::Skein256(a_hashSizeBits / 8);
                else
                    m_hash = new Skein::Skein512(a_hashSizeBits / 8);

                m_hash->Initialize();
            }

            virtual void InitializeHash()
            {
                m_hash->Initialize();
            }

        public:

            SkeinTest()
            {
                m_hash = nullptr;
            }

            ~SkeinTest()
            {
                delete m_hash;
            }
    };

    class SkeinOrgTest : public TestBaseCrypto
    {
        private:

            SkeinOrg::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "Skein";
            }

            virtual CString GetTestName()
            {
                return "Skein-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 64;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                SkeinOrg::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.u.h.hashBitLen/8];
                SkeinOrg::Hash((int)m_hashState.u.h.hashBitLen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.u.h.hashBitLen/8];
                SkeinOrg::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                SkeinOrg::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                SkeinOrg::Init(&m_hashState, (int)m_hashState.u.h.hashBitLen);
            }
    };
}
