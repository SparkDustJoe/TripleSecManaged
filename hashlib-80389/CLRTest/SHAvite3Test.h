

#pragma once 

#include "SHAvite3.h"

namespace CLRTest
{
    ref class SHAvite3Test : TestBaseCrypto
    {
        private:

            SHAvite3::SHAvite3Base^ m_hash;

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
                    m_hash = gcnew SHAvite3::SHAvite3_256Base(a_hashSizeBits/8);
                else
                    m_hash = gcnew SHAvite3::SHAvite3_512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("SHAvite3");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("SHAvite3-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                SHAvite3Test^ test = gcnew SHAvite3Test();
                test->Test();
            }
    };

    ref class SHAvite3CSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateSHAvite3(HashLib::HashSize(a_hashSizeBits / 8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("SHAvite3");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("SHAvite3-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                SHAvite3CSharpTest^ test = gcnew SHAvite3CSharpTest();
                test->Test();
            }
    };
}
