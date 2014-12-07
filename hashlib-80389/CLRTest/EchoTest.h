

#pragma once 

#include "Echo.h"

namespace CLRTest
{
    ref class EchoTest : TestBaseCrypto
    {
        private:

            Echo::EchoBase^ m_hash;

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
                    m_hash = gcnew Echo::Echo256Base(a_hashSizeBits/8);
                else
                    m_hash = gcnew Echo::Echo512Base(a_hashSizeBits/8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Echo");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Echo-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 256;
            }

        public: 

            static void DoTest()
            {
                EchoTest^ test = gcnew EchoTest();
                test->Test();
            }
    };

    ref class EchoCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateEcho((HashLib::HashSize)(a_hashSizeBits/8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Echo");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Echo-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 256;
            }

        public: 

            static void DoTest()
            {
                EchoCSharpTest^ test = gcnew EchoCSharpTest();
                test->Test();
            }
    };
}
