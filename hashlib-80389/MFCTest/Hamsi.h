
#pragma once

#include "stdafx.h"

namespace Hamsi
{
    class HamsiBase
    {
        protected:

            int m_buffer_pos;
            byte* m_buffer; 
            uint* m_state;
            ulong m_processedBytes;
            uint* m_block;
            int m_rounds;
            uint* m_alpha;
            byte* m_pad;

            void Finish();
            byte* GetResult();

            virtual void TransformBlock(byte* a_data, int a_index) = 0;

        public:

            HamsiBase(int a_hashSize)
            {
                HashSize = a_hashSize;

                BlockSize = (HashSize <= 32) ? 4 : 8;

                m_buffer = new byte[BlockSize];
                m_state = new uint[BlockSize*2];
                m_block = new uint[BlockSize*5];
                m_pad = new byte[8];

                Initialize();
            }

            ~HamsiBase()
            {
                delete m_buffer;
                delete m_state;
                delete m_block;
                delete m_pad;
            }

            int HashSize;
            int BlockSize;

            void Initialize();
            void TransformBytes(byte* a_data, int a_index, int a_length);
            byte* TransformFinal();
            byte* ComputeBytes(byte* a_data, int a_length);
    };

    class Hamsi256Base : public HamsiBase
    {
        protected:

            virtual void TransformBlock(byte* a_data, int a_index);

        public:

            Hamsi256Base(int a_hashSize)
                : HamsiBase(a_hashSize)
            {
            }
    };

    class Hamsi512Base : public HamsiBase
    {
        protected:

            virtual void TransformBlock(byte* a_data, int a_index);

        public:

            Hamsi512Base(int a_hashSize)
                : HamsiBase(a_hashSize)
            {
            }
    };
}