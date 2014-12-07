

#pragma once 

#include "CubeHash.h"

namespace CLRTest
{
    ref class CubeHashTest : TestBaseCrypto
    {
        private:

            CubeHash::CubeHash^ m_hash;

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
                m_hash = gcnew CubeHash::CubeHash(a_hashSizeBits/8);
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("CubeHash");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("CubeHash-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public: 

            static void DoTest()
            {
                CubeHashTest^ test = gcnew CubeHashTest();
                test->Test();
            }
    };

    ref class CubeHashCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateCubeHash(HashLib::HashSize(a_hashSizeBits/8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("CubeHash");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("CubeHash-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 128;
            }

        public:

            static void DoTest()
            {
                CubeHashCSharpTest^ test = gcnew CubeHashCSharpTest();
                test->Test();
            }

    };
}
