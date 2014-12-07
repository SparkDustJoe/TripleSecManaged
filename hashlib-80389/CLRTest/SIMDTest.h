

#pragma once 

#include "SIMD.h"

namespace CLRTest
{
    ref class SIMDTest : TestBaseCrypto
    {
        private:

            SIMD::SIMDBase^ m_hash;

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
                    m_hash = gcnew SIMD::SIMD256Base(a_hashSizeBits/8);
                else
                    m_hash = gcnew SIMD::SIMD512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("SIMD");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("SIMD-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                SIMDTest^ test = gcnew SIMDTest();
                test->Test();
            }
    };

    ref class SIMDCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateSIMD(HashLib::HashSize(a_hashSizeBits / 8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("SIMD");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("SIMD-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                SIMDCSharpTest^ test = gcnew SIMDCSharpTest();
                test->Test();
            }
    };
}
