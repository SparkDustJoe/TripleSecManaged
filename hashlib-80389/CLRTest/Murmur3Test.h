
#pragma once

namespace CLRTest
{
    ref class Murmur3CSharpTest : TestBaseNonCryptoNonBlock
    {
        private:

            HashLib::IHash^ m_hash = HashLib::HashFactory::Hash32::CreateMurmur3();

        protected:

            virtual int GetHashSize() override
            {
                return m_hash->HashSize;
            }

            virtual String^ GetTestVectorsDir() override
            {
                return "Murmur3_32";
            }

            virtual String^ GetTestName() override
            {
                return "Murmur3-CSharp";
            }

            virtual int GetMaxBufferSize() override 
            {
                return 4;
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
                Murmur3CSharpTest^ test = gcnew Murmur3CSharpTest();
                test->Test();
            }
    };

    ref class Murmur3_128_CSharpTest : TestBaseNonCryptoNonBlock
    {
        private:

        HashLib::IHash^ m_hash = HashLib::HashFactory::Hash128::CreateMurmur3_128();

        protected:

        virtual int GetHashSize() override
        {
            return m_hash->HashSize;
        }

        virtual String^ GetTestVectorsDir() override
        {
            return "Murmur3_128";
        }

        virtual String^ GetTestName() override
        {
            return "Murmur3_128-CSharp";
        }

        virtual int GetMaxBufferSize() override
        {
            return 16;
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
            Murmur3_128_CSharpTest^ test = gcnew Murmur3_128_CSharpTest();
            test->Test();
        }
    };
}

