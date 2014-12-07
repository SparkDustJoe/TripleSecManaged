

#pragma once 

#include "Shabal.h"

namespace CLRTest
{
    ref class ShabalTest : TestBaseCrypto
    {
        private:

            Shabal::Shabal^ m_hash;

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
                m_hash = gcnew Shabal::Shabal(a_hashSizeBits / 8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Shabal");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Shabal-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 192;
            }

        public: 

            static void DoTest()
            {
                ShabalTest^ test = gcnew ShabalTest();
                test->Test();
            }
    };

    ref class ShabalCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateShabal(HashLib::HashSize(a_hashSizeBits / 8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Shabal");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Shabal-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 192;
            }

        public: 

            static void DoTest()
            {
                ShabalCSharpTest^ test = gcnew ShabalCSharpTest();
                test->Test();
            }
    };
}
