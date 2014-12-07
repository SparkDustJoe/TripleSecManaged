
#pragma once

#include "stdafx.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4005)

namespace Fugue
{
    extern const uint IV224[7];
    extern const uint IV256[8];
    extern const uint IV384[12];
    extern const uint IV512[16];
    extern const ulong aes_style_table0[256];
    extern const ulong aes_style_table1[256];
    extern const ulong aes_style_table2[256];
    extern const ulong aes_style_table3[256];

    class FugueBase
    {
        protected:

            int m_base;
            uint m_state[36];
            byte m_buffer[4];
            ulong m_processedBytes;
            
            virtual byte* GetResult() = 0;
            virtual void TransformBytes_(byte* a_data, int a_index, int a_length) = 0;

            void Finish();

        public:

            FugueBase(int a_hashSize, int a_blockSize)
            {
                HashSize = a_hashSize;
                BlockSize = a_blockSize;

                Initialize();
            }

            int HashSize;
            int BlockSize;

            virtual void Initialize();
            void TransformBytes(byte* a_data, int a_index, int a_length);
            byte* TransformFinal();
            byte* ComputeBytes(byte* a_data, int a_length);
    };

    class Fugue256Base : public FugueBase
    {
        protected:

            Fugue256Base(int a_hashSize, int a_blockSize)
                : FugueBase(a_hashSize, a_blockSize)
            {
            }

            virtual byte* GetResult();
            virtual void TransformBytes_(byte* a_data, int a_index, int a_length);  
    };

    class Fugue224 : public Fugue256Base
    {
        public:

            Fugue224() 
                : Fugue256Base(28, 30)
            {
            }

            virtual void Initialize()
            {
                FugueBase::Initialize();

                memcpy (&m_state[BlockSize-HashSize/4], IV224, HashSize);
            }
    };

    class Fugue256 : public Fugue256Base
    {
        public:

            Fugue256() 
                : Fugue256Base(32, 30)
            {
            }

            virtual void Initialize()
            {
                FugueBase::Initialize();

                memcpy (&m_state[BlockSize-HashSize/4], IV256, HashSize);
            }
    };

    class Fugue384 : public FugueBase
    {
        protected:

            virtual byte* GetResult();
            virtual void TransformBytes_(byte* a_data, int a_index, int a_length);

        public:

            Fugue384() 
                : FugueBase(48, 36)
            {
            }

            virtual void Initialize()
            {
                FugueBase::Initialize();

                memcpy (&m_state[BlockSize-HashSize/4], IV384, HashSize);
            }
    };

    class Fugue512 : public FugueBase
    {
        protected:

            virtual byte* GetResult();
            virtual void TransformBytes_(byte* a_data, int a_index, int a_length);

        public:

            Fugue512() 
                : FugueBase(64, 36)
            {
            }

            virtual void Initialize()
            {
                FugueBase::Initialize();

                memcpy (&m_state[BlockSize-HashSize/4], IV512, HashSize);
            }
    };
}