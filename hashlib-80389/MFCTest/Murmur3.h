
#pragma once

namespace Murmur3
{

    class Murmur3
    {
        private:

            const uint SEED = 0xC58F1A7B;

        public:

            byte* MurmurHash3_x86_32(byte* a_data, int a_length);
            byte* MurmurHash3_x64_128(byte* a_data, int a_length);
    };

}