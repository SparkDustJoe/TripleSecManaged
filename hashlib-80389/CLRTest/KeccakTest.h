
#pragma once 

#include "Keccak.h"

namespace CLRTest
{
    ref class KeccakTest : TestBaseCrypto
    {
        private:

            Keccak::Keccak^ m_hash;

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
                m_hash = gcnew Keccak::Keccak(a_hashSizeBits / 8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Keccak");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Keccak-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 200;
            }

        public: 

            static void DoTest()
            {
                KeccakTest^ test = gcnew KeccakTest();
                test->Test();
            }
    };

    ref class KeccakCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateKeccak(HashLib::HashSize(a_hashSizeBits / 8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Keccak");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Keccak-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 200;
            }

        public: 

            static void DoTest()
            {
                KeccakCSharpTest^ test = gcnew KeccakCSharpTest();
                test->Test();
            }
    };
}
