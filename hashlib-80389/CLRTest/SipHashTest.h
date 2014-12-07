
#pragma once

#include "SipHash.h"

namespace CLRTest
{
    ref class SipHashTest : TestBaseNonCryptoWithKey
    {
        private:

            SipHash::SipHash^ m_hash = gcnew SipHash::SipHash();

        protected:

            virtual int GetHashSize() override
            {
                return m_hash->HashSize;
            }

            virtual void SetKey(array<byte>^ a_key) override
            {
                m_hash->SetKey(a_key);
            }

            virtual String^ GetTestVectorsDir() override
            {
                return "SipHash";
            }

            virtual String^ GetTestName() override
            {
                return "SipHash-CLR";
            }

            virtual int GetMaxBufferSize() override 
            {
                return 8;
            }

        public:

            virtual void TransformBytes(array<byte>^ a_data, int a_index, int a_length) override
            {
                return m_hash->TransformBytes(a_data, a_index, a_length);
            }

            virtual array<byte>^ ComputeBytes(array<byte>^ a_data) override
            {
                return m_hash->ComputeBytes(a_data);
            }

            virtual array<byte>^ TransformFinal() override
            {
                return m_hash->TransformFinal();
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            static void DoTest()
            {
                SipHashTest^ test = gcnew SipHashTest();
                test->Test();
            }
    };

    ref class SipHashCSharpTest : TestBaseNonCryptoWithKey
    {
        private:

            HashLib::IHashWithKey^ m_hash = HashLib::HashFactory::Hash64::CreateSipHash();

        protected:

            virtual int GetHashSize() override
            {
                return m_hash->HashSize;
            }

            virtual void SetKey(array<byte>^ a_key) override
            {
                m_hash->Key = a_key;
            }

            virtual String^ GetTestVectorsDir() override
            {
                return "SipHash";
            }

            virtual String^ GetTestName() override
            {
                return "SipHash-CSharp";
            }

            virtual int GetMaxBufferSize() override 
            {
                return 8;
            }

        public:

            virtual void TransformBytes(array<byte>^ a_data, int a_index, int a_length) override
            {
                return m_hash->TransformBytes(a_data, a_index, a_length);
            }

            virtual array<byte>^ ComputeBytes(array<byte>^ a_data) override
            {
                return m_hash->ComputeBytes(a_data)->GetBytes();
            }

            virtual array<byte>^ TransformFinal() override
            {
                return m_hash->TransformFinal()->GetBytes();
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            static void DoTest()
            {
                SipHashCSharpTest^ test = gcnew SipHashCSharpTest();
                test->Test();
            }
    };
}

