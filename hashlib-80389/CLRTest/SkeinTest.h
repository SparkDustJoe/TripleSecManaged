
#pragma once 

#include "Skein.h"

namespace CLRTest
{
    ref class SkeinTest : TestBaseCrypto
    {
        private:

            Skein::SkeinBase^ m_hash;

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
                    m_hash = gcnew Skein::Skein256(a_hashSizeBits / 8);
                else
                    m_hash = gcnew Skein::Skein512(a_hashSizeBits / 8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Skein");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Skein-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 64;
            }

        public: 

            static void DoTest()
            {
                SkeinTest^ test = gcnew SkeinTest();
                test->Test();
            }
    };

    ref class SkeinCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateSkein(HashLib::HashSize(a_hashSizeBits / 8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Skein");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Skein-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 64;
            }

        public: 

            static void DoTest()
            {
                SkeinCSharpTest^ test = gcnew SkeinCSharpTest();
                test->Test();
            }
    };
}
