
#pragma once

#include "BlueMidnightWish.h"
#include "BlueMidnightWish-org.h"

namespace MFCTest
{
    class BlueMidnightWishTest : public TestBaseCrypto
    {
        private:

            BMW::BMWBase* m_bmw;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "BlueMidnightWish";
            }

            virtual CString GetTestName()
            {
                return "BlueMidnightWish";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                m_bmw->TransformBytes(a_data, a_index, a_length); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                return m_bmw->ComputeBytes(a_data, a_length);
            }

            virtual byte* TransformFinal()
            {
                return m_bmw->TransformFinal();
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                delete m_bmw;
                if (a_hashSizeBits <= 256)
                    m_bmw = new BMW::BMW256Base(a_hashSizeBits/8);
                else
                    m_bmw = new BMW::BMW512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash()
            {
                m_bmw->Initialize();
            }

        public:

            BlueMidnightWishTest()
            {
                m_bmw = nullptr;
            }

            ~BlueMidnightWishTest()
            {
                delete m_bmw;
            }
    };

    class BlueMidnightWishOrgTest : public TestBaseCrypto
    {
        private:

            BMWORG::hashState m_hashState;

        protected:
    
            virtual CString GetTestVectorsDir()
            {
                return "BlueMidnightWish";
            }

            virtual CString GetTestName()
            {
                return "BlueMidnightWish-Org";
            }

            virtual int GetMaxBufferSize()
            {
                return 128;
            }

            virtual void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                BMWORG::Update(&m_hashState, a_data + a_index, a_length*8); 
            }

            virtual byte* ComputeBytes(byte* a_data, int a_length)
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                BMWORG::Hash(m_hashState.hashbitlen, a_data, a_length*8, out);
                return out;
            }

            virtual byte* TransformFinal()
            {
                byte* out = new byte[m_hashState.hashbitlen/8];
                BMWORG::Final(&m_hashState, out);
                return out;
            }

            virtual void CreateHash(int a_hashSizeBits)
            {
                BMWORG::Init(&m_hashState, a_hashSizeBits);
            }

            virtual void InitializeHash()
            {
                BMWORG::Init(&m_hashState, m_hashState.hashbitlen);
            }
    };
}
