
#pragma once

namespace Groestl
{
    class GroestlBase
    {
        private:

            void Finish();
            byte* GetResult();

        protected:

            ulong m_state[16];
            ulong m_processed_bytes;
            byte m_buffer[128];
            int m_buffer_pos;
            byte m_pad[136];

            virtual void TransformBlock(byte* a_data, int a_index) abstract;
            virtual void OutputTransformation() abstract;

        public:

            int HashSize;
            int BlockSize;

            GroestlBase(int a_hash_size, int a_block_size)
            {
                HashSize = a_hash_size;
                BlockSize = a_block_size;

                Initialize();
            }

            void Initialize();
            void TransformBytes(byte* a_data, int a_index, int a_length);
            byte* TransformFinal();
            byte* ComputeBytes(byte* a_data, int a_length);
    };

    class Groestl256 : public GroestlBase
    {
        protected:

            virtual void TransformBlock(byte* a_data, int a_index) override;
            virtual void OutputTransformation() override;

        public:

            Groestl256(int a_hash_size) : GroestlBase(a_hash_size, 64)
            {
            }
    };

    class Groestl512 : public GroestlBase
    {
        protected:

            virtual void TransformBlock(byte* a_data, int a_index) override;
            virtual void OutputTransformation() override;

        public:

        Groestl512(int a_hash_size) : GroestlBase(a_hash_size, 128)
        {
        }
    };
}