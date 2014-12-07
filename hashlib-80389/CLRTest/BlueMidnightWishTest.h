

#pragma once 

#include "BlueMidnightWish.h"

namespace CLRTest
{
    ref class BlueMidnightWishTest : TestBaseCrypto
    {
        private:

            BlueMidnightWish::BlueMidnightWishBase^ m_hash;

        protected:

            virtual void TransformBytes(array<byte>^ a_data, int a_index, int a_length) override
            {
                m_hash->TransformBytes(a_data, a_index, a_length);
            }

            virtual array<byte>^ ComputeBytes(array<byte>^ a_data) override
            {
                return m_hash->ComputeBytes(a_data);
            }

            virtual array<byte>^ TransformFinal() override
            {
                return m_hash->TransformFinal();
            }

            virtual void CreateHash(int a_hashSizeBits) override
            {
                if (a_hashSizeBits <= 256)
                    m_hash = gcnew BlueMidnightWish::BlueMidnightWish256Base(a_hashSizeBits/8);
                else
                    m_hash = gcnew BlueMidnightWish::BlueMidnightWish512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return "BlueMidnightWish";
            }

            virtual String^ GetTestName() override
            {
                return "BlueMidnightWish-CLR";
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                BlueMidnightWishTest^ test = gcnew BlueMidnightWishTest();
                test->Test();
            }
    };

    ref class BlueMidnightWishCSharpTest : TestBaseCrypto
    {
        private:

            IHash^ m_hash;

        protected:

            virtual void TransformBytes(array<byte>^ a_data, int a_index, int a_length) override
            {
                m_hash->TransformBytes(a_data, a_index, a_length);
            }

            virtual array<byte>^ ComputeBytes(array<byte>^ a_data) override
            {
                return m_hash->ComputeBytes(a_data)->GetBytes();
            }

            virtual array<byte>^ TransformFinal() override
            {
                return m_hash->TransformFinal()->GetBytes();
            }

            virtual void CreateHash(int a_hashSizeBits) override
            {
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateBlueMidnightWish(HashLib::HashSize(a_hashSizeBits/8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return "BlueMidnightWish";
            }

            virtual String^ GetTestName() override
            {
                return "BlueMidnightWish-CSharp";
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                BlueMidnightWishCSharpTest^ test = gcnew BlueMidnightWishCSharpTest();
                test->Test();
            }
    };
}
