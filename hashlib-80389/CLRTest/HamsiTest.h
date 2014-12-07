
#pragma once 

#include "Hamsi.h"

namespace CLRTest
{
    ref class HamsiTest : TestBaseCrypto
    {
        private:

            Hamsi::HamsiBase^ m_hash;

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
                switch (a_hashSizeBits)
                {
                    case 224: 
                    case 256: m_hash = gcnew Hamsi::Hamsi256Base(a_hashSizeBits/8); break;
                    case 384: 
                    case 512: m_hash = gcnew Hamsi::Hamsi512Base(a_hashSizeBits/8); break;
                }
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Hamsi");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Hamsi-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 64;
            }

        public: 

            static void DoTest()
            {
                HamsiTest^ test = gcnew HamsiTest();
                test->Test();
            }
    };

    ref class HamsiCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateHamsi(HashLib::HashSize(a_hashSizeBits/8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Hamsi");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Hamsi-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 64;
            }

        public: 

            static void DoTest()
            {
                HamsiCSharpTest^ test = gcnew HamsiCSharpTest();
                test->Test();
            }
    };
}
