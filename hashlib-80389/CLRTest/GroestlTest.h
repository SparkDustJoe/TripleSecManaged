

#pragma once 

#include "Groestl.h"

namespace CLRTest
{
    ref class GroestlTest : TestBaseCrypto
    {
        private:

            Groestl::GroestlBase^ m_hash;

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
                delete m_hash;

                switch (a_hashSizeBits)
                {
                    case 224: 
                    case 256: m_hash = gcnew Groestl::Groestl256(a_hashSizeBits/8); break;
                    case 384: 
                    case 512: m_hash = gcnew Groestl::Groestl512(a_hashSizeBits/8); break;
                }
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Groestl");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Groestl-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                GroestlTest^ test = gcnew GroestlTest();
                test->Test();
            }
    };

    ref class GroestlCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateGroestl(HashLib::HashSize(a_hashSizeBits/8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Groestl");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Groestl-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                GroestlCSharpTest^ test = gcnew GroestlCSharpTest();
                test->Test();
            }
    };
}
