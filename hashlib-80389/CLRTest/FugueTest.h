

#pragma once 

#include "Fugue.h"

namespace CLRTest
{
    ref class FugueTest : TestBaseCrypto
    {
        private:

            Fugue::FugueBase^ m_hash;

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
                    case 224: m_hash = gcnew Fugue::Fugue224(); break;
                    case 256: m_hash = gcnew Fugue::Fugue256(); break;
                    case 384: m_hash = gcnew Fugue::Fugue384(); break;
                    case 512: m_hash = gcnew Fugue::Fugue512(); break;
                }
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Fugue");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Fugue-CLR");
            }

            virtual int GetMaxBufferSize() override
            {
                return 144;
            }

        public: 

            static void DoTest()
            {
                FugueTest^ test = gcnew FugueTest();
                test->Test();
            }
    };

    ref class FugueCSharpTest : TestBaseCrypto
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
                m_hash = HashLib::HashFactory::Crypto::SHA3::CreateFugue((HashLib::HashSize)(a_hashSizeBits/8));
            }

            virtual void InitializeHash() override
            {
                m_hash->Initialize();
            }

            virtual String^ GetTestVectorsDir() override
            {
                return String::Format("Fugue");
            }

            virtual String^ GetTestName() override
            {
                return String::Format("Fugue-CSharp");
            }

            virtual int GetMaxBufferSize() override
            {
                return 144;
            }

        public: 

            static void DoTest()
            {
                FugueCSharpTest^ test = gcnew FugueCSharpTest();
                test->Test();
            }
    };
}
