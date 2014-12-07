

#pragma once 

#include "Luffa.h"

namespace CLRTest
{
    ref class LuffaTest : TestBaseCrypto
    {
        private:

            Luffa::LuffaBase^ m_hash;

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
                    case 256: m_hash = gcnew Luffa::Luffa256Base(a_hashSizeBits/8); break;
                    case 384: m_hash = gcnew Luffa::Luffa384(a_hashSizeBits/8); break;
                    case 512: m_hash = gcnew Luffa::Luffa512(a_hashSizeBits/8); break;
                }
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Luffa");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Luffa-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 256;
            }

        public: 

            static void DoTest()
            {
                LuffaTest^ test = gcnew LuffaTest();
                test->Test();
            }
    };

    ref class LuffaCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateLuffa(HashLib::HashSize(a_hashSizeBits / 8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Luffa");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Luffa-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 256;
            }

        public: 

            static void DoTest()
            {
                LuffaCSharpTest^ test = gcnew LuffaCSharpTest();
                test->Test();
            }
    };
}
