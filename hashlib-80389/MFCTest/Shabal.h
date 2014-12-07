
#pragma once

#include "stdafx.h"

namespace Shabal
{
    class Shabal
    {
        private: byte* m_buffer;
        private: int m_buffer_pos;
        private: uint* m_A;
        private: uint* m_B;
        private: uint* m_C;
        private: ulong m_processedBytes;

        public: int HashSize;
        public: int BlockSize;

        public: Shabal(int a_hashSize)
        {
            m_buffer = new byte[64];
            m_A = new uint[12];
            m_B = new uint[16];
            m_C = new uint[16];

            BlockSize = 64;
            HashSize = a_hashSize;

            Initialize();
        }

        public: ~Shabal()
        {
            delete m_buffer;
            delete m_A;
            delete m_B;
            delete m_C;
        }

        protected: void TransformBlock(byte* a_data, int a_index);
        protected: void Finish();
        protected: byte* GetResult();

        public: void Initialize();
        public: void TransformBytes(byte* a_data, int a_index, int a_length);
        public: byte* TransformFinal();
        public: byte* ComputeBytes(byte* a_data, int a_lengh);
    };
}