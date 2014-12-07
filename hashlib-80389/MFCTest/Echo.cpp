
#include "stdafx.h"
#include "Echo.h"

namespace Echo
{
    void Echo256Base::TransformBlock(byte* a_data, int a_index)
    {
        memcpy(&m_state[8], &a_data[a_index], BlockSize);

        uint WA, WB, WC, WD;

        ulong cv[8];

        cv[0] = m_state[0]; 
        cv[1] = m_state[1];
        cv[2] = m_state[2];
        cv[3] = m_state[3]; 
        cv[4] = m_state[4]; 
        cv[5] = m_state[5]; 
        cv[6] = m_state[6]; 
        cv[7] = m_state[7]; 

        for (int r=1; r<4; r++)
        {
            cv[0] ^= m_state[8*r+0]; 
            cv[1] ^= m_state[8*r+1];
            cv[2] ^= m_state[8*r+2];
            cv[3] ^= m_state[8*r+3]; 
            cv[4] ^= m_state[8*r+4]; 
            cv[5] ^= m_state[8*r+5]; 
            cv[6] ^= m_state[8*r+6]; 
            cv[7] ^= m_state[8*r+7]; 
        }
       
        ulong WL0 = m_state[0];
        ulong WH0 = m_state[1];
        ulong WL1 = m_state[2];
        ulong WH1 = m_state[3];
        ulong WL2 = m_state[4];
        ulong WH2 = m_state[5];
        ulong WL3 = m_state[6];
        ulong WH3 = m_state[7];
        ulong WL4 = m_state[8];
        ulong WH4 = m_state[9];
        ulong WL5 = m_state[10];
        ulong WH5 = m_state[11];
        ulong WL6 = m_state[12];
        ulong WH6 = m_state[13];
        ulong WL7 = m_state[14];
        ulong WH7 = m_state[15];
        ulong WL8 = m_state[16];
        ulong WH8 = m_state[17];
        ulong WL9 = m_state[18];
        ulong WH9 = m_state[19];
        ulong WL10 = m_state[20];
        ulong WH10 = m_state[21];
        ulong WL11 = m_state[22];
        ulong WH11 = m_state[23];
        ulong WL12 = m_state[24];
        ulong WH12 = m_state[25];
        ulong WL13 = m_state[26];
        ulong WH13 = m_state[27];
        ulong WL14 = m_state[28];
        ulong WH14 = m_state[29];
        ulong WL15 = m_state[30];
        ulong WH15 = m_state[31];
        
        int r = 8;
        
        ulong cnt = 0;
        if (!m_last_block)
            cnt = m_processedBytes * 8;

        do 
        {
            WA = s_T0[(byte)(WL0)] ^ s_T1[(byte)(WL0 >> 40)] ^ s_T2[(byte)(WH0 >> 16)] ^ s_T3[(byte)(WH0 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL0 >> 32)] ^ s_T1[(byte)(WH0 >> 8)] ^ s_T2[(byte)(WH0 >> 48)] ^ s_T3[(byte)(WL0 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH0)] ^ s_T1[(byte)(WH0 >> 40)] ^ s_T2[(byte)(WL0 >> 16)] ^ s_T3[(byte)(WL0 >> 56)];
            WD = s_T0[(byte)(WH0 >> 32)] ^ s_T1[(byte)(WL0 >> 8)] ^ s_T2[(byte)(WL0 >> 48)] ^ s_T3[(byte)(WH0 >> 24)]; 
            cnt++;
            WL0 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL0 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH0 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH0 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL1)] ^ s_T1[(byte)(WL1 >> 40)] ^ s_T2[(byte)(WH1 >> 16)] ^ s_T3[(byte)(WH1 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL1 >> 32)] ^ s_T1[(byte)(WH1 >> 8)] ^ s_T2[(byte)(WH1 >> 48)] ^ s_T3[(byte)(WL1 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH1)] ^ s_T1[(byte)(WH1 >> 40)] ^ s_T2[(byte)(WL1 >> 16)] ^ s_T3[(byte)(WL1 >> 56)];
            WD = s_T0[(byte)(WH1 >> 32)] ^ s_T1[(byte)(WL1 >> 8)] ^ s_T2[(byte)(WL1 >> 48)] ^ s_T3[(byte)(WH1 >> 24)]; 
            cnt++;
            WL1 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL1 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH1 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH1 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL2)] ^ s_T1[(byte)(WL2 >> 40)] ^ s_T2[(byte)(WH2 >> 16)] ^ s_T3[(byte)(WH2 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL2 >> 32)] ^ s_T1[(byte)(WH2 >> 8)] ^ s_T2[(byte)(WH2 >> 48)] ^ s_T3[(byte)(WL2 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH2)] ^ s_T1[(byte)(WH2 >> 40)] ^ s_T2[(byte)(WL2 >> 16)] ^ s_T3[(byte)(WL2 >> 56)];
            WD = s_T0[(byte)(WH2 >> 32)] ^ s_T1[(byte)(WL2 >> 8)] ^ s_T2[(byte)(WL2 >> 48)] ^ s_T3[(byte)(WH2 >> 24)]; 
            cnt++;
            WL2 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL2 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH2 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH2 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL3)] ^ s_T1[(byte)(WL3 >> 40)] ^ s_T2[(byte)(WH3 >> 16)] ^ s_T3[(byte)(WH3 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL3 >> 32)] ^ s_T1[(byte)(WH3 >> 8)] ^ s_T2[(byte)(WH3 >> 48)] ^ s_T3[(byte)(WL3 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH3)] ^ s_T1[(byte)(WH3 >> 40)] ^ s_T2[(byte)(WL3 >> 16)] ^ s_T3[(byte)(WL3 >> 56)];
            WD = s_T0[(byte)(WH3 >> 32)] ^ s_T1[(byte)(WL3 >> 8)] ^ s_T2[(byte)(WL3 >> 48)] ^ s_T3[(byte)(WH3 >> 24)]; 
            cnt++;
            WL3 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL3 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH3 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH3 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL4)] ^ s_T1[(byte)(WL4 >> 40)] ^ s_T2[(byte)(WH4 >> 16)] ^ s_T3[(byte)(WH4 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL4 >> 32)] ^ s_T1[(byte)(WH4 >> 8)] ^ s_T2[(byte)(WH4 >> 48)] ^ s_T3[(byte)(WL4 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH4)] ^ s_T1[(byte)(WH4 >> 40)] ^ s_T2[(byte)(WL4 >> 16)] ^ s_T3[(byte)(WL4 >> 56)];
            WD = s_T0[(byte)(WH4 >> 32)] ^ s_T1[(byte)(WL4 >> 8)] ^ s_T2[(byte)(WL4 >> 48)] ^ s_T3[(byte)(WH4 >> 24)]; 
            cnt++;
            WL4 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL4 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH4 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH4 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL5)] ^ s_T1[(byte)(WL5 >> 40)] ^ s_T2[(byte)(WH5 >> 16)] ^ s_T3[(byte)(WH5 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL5 >> 32)] ^ s_T1[(byte)(WH5 >> 8)] ^ s_T2[(byte)(WH5 >> 48)] ^ s_T3[(byte)(WL5 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH5)] ^ s_T1[(byte)(WH5 >> 40)] ^ s_T2[(byte)(WL5 >> 16)] ^ s_T3[(byte)(WL5 >> 56)];
            WD = s_T0[(byte)(WH5 >> 32)] ^ s_T1[(byte)(WL5 >> 8)] ^ s_T2[(byte)(WL5 >> 48)] ^ s_T3[(byte)(WH5 >> 24)]; 
            cnt++;
            WL5 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL5 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH5 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH5 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL6)] ^ s_T1[(byte)(WL6 >> 40)] ^ s_T2[(byte)(WH6 >> 16)] ^ s_T3[(byte)(WH6 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL6 >> 32)] ^ s_T1[(byte)(WH6 >> 8)] ^ s_T2[(byte)(WH6 >> 48)] ^ s_T3[(byte)(WL6 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH6)] ^ s_T1[(byte)(WH6 >> 40)] ^ s_T2[(byte)(WL6 >> 16)] ^ s_T3[(byte)(WL6 >> 56)];
            WD = s_T0[(byte)(WH6 >> 32)] ^ s_T1[(byte)(WL6 >> 8)] ^ s_T2[(byte)(WL6 >> 48)] ^ s_T3[(byte)(WH6 >> 24)]; 
            cnt++;
            WL6 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL6 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH6 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH6 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL7)] ^ s_T1[(byte)(WL7 >> 40)] ^ s_T2[(byte)(WH7 >> 16)] ^ s_T3[(byte)(WH7 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL7 >> 32)] ^ s_T1[(byte)(WH7 >> 8)] ^ s_T2[(byte)(WH7 >> 48)] ^ s_T3[(byte)(WL7 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH7)] ^ s_T1[(byte)(WH7 >> 40)] ^ s_T2[(byte)(WL7 >> 16)] ^ s_T3[(byte)(WL7 >> 56)];
            WD = s_T0[(byte)(WH7 >> 32)] ^ s_T1[(byte)(WL7 >> 8)] ^ s_T2[(byte)(WL7 >> 48)] ^ s_T3[(byte)(WH7 >> 24)]; 
            cnt++;
            WL7 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL7 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH7 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH7 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL8)] ^ s_T1[(byte)(WL8 >> 40)] ^ s_T2[(byte)(WH8 >> 16)] ^ s_T3[(byte)(WH8 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL8 >> 32)] ^ s_T1[(byte)(WH8 >> 8)] ^ s_T2[(byte)(WH8 >> 48)] ^ s_T3[(byte)(WL8 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH8)] ^ s_T1[(byte)(WH8 >> 40)] ^ s_T2[(byte)(WL8 >> 16)] ^ s_T3[(byte)(WL8 >> 56)];
            WD = s_T0[(byte)(WH8 >> 32)] ^ s_T1[(byte)(WL8 >> 8)] ^ s_T2[(byte)(WL8 >> 48)] ^ s_T3[(byte)(WH8 >> 24)]; 
            cnt++;
            WL8 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL8 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH8 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH8 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL9)] ^ s_T1[(byte)(WL9 >> 40)] ^ s_T2[(byte)(WH9 >> 16)] ^ s_T3[(byte)(WH9 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL9 >> 32)] ^ s_T1[(byte)(WH9 >> 8)] ^ s_T2[(byte)(WH9 >> 48)] ^ s_T3[(byte)(WL9 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH9)] ^ s_T1[(byte)(WH9 >> 40)] ^ s_T2[(byte)(WL9 >> 16)] ^ s_T3[(byte)(WL9 >> 56)];
            WD = s_T0[(byte)(WH9 >> 32)] ^ s_T1[(byte)(WL9 >> 8)] ^ s_T2[(byte)(WL9 >> 48)] ^ s_T3[(byte)(WH9 >> 24)]; 
            cnt++;
            WL9 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL9 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH9 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH9 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL10)] ^ s_T1[(byte)(WL10 >> 40)] ^ s_T2[(byte)(WH10 >> 16)] ^ s_T3[(byte)(WH10 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL10 >> 32)] ^ s_T1[(byte)(WH10 >> 8)] ^ s_T2[(byte)(WH10 >> 48)] ^ s_T3[(byte)(WL10 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH10)] ^ s_T1[(byte)(WH10 >> 40)] ^ s_T2[(byte)(WL10 >> 16)] ^ s_T3[(byte)(WL10 >> 56)];
            WD = s_T0[(byte)(WH10 >> 32)] ^ s_T1[(byte)(WL10 >> 8)] ^ s_T2[(byte)(WL10 >> 48)] ^ s_T3[(byte)(WH10 >> 24)]; 
            cnt++;
            WL10 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL10 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH10 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH10 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL11)] ^ s_T1[(byte)(WL11 >> 40)] ^ s_T2[(byte)(WH11 >> 16)] ^ s_T3[(byte)(WH11 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL11 >> 32)] ^ s_T1[(byte)(WH11 >> 8)] ^ s_T2[(byte)(WH11 >> 48)] ^ s_T3[(byte)(WL11 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH11)] ^ s_T1[(byte)(WH11 >> 40)] ^ s_T2[(byte)(WL11 >> 16)] ^ s_T3[(byte)(WL11 >> 56)];
            WD = s_T0[(byte)(WH11 >> 32)] ^ s_T1[(byte)(WL11 >> 8)] ^ s_T2[(byte)(WL11 >> 48)] ^ s_T3[(byte)(WH11 >> 24)]; 
            cnt++;
            WL11 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL11 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH11 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH11 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL12)] ^ s_T1[(byte)(WL12 >> 40)] ^ s_T2[(byte)(WH12 >> 16)] ^ s_T3[(byte)(WH12 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL12 >> 32)] ^ s_T1[(byte)(WH12 >> 8)] ^ s_T2[(byte)(WH12 >> 48)] ^ s_T3[(byte)(WL12 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH12)] ^ s_T1[(byte)(WH12 >> 40)] ^ s_T2[(byte)(WL12 >> 16)] ^ s_T3[(byte)(WL12 >> 56)];
            WD = s_T0[(byte)(WH12 >> 32)] ^ s_T1[(byte)(WL12 >> 8)] ^ s_T2[(byte)(WL12 >> 48)] ^ s_T3[(byte)(WH12 >> 24)]; 
            cnt++;
            WL12 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL12 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH12 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH12 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL13)] ^ s_T1[(byte)(WL13 >> 40)] ^ s_T2[(byte)(WH13 >> 16)] ^ s_T3[(byte)(WH13 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL13 >> 32)] ^ s_T1[(byte)(WH13 >> 8)] ^ s_T2[(byte)(WH13 >> 48)] ^ s_T3[(byte)(WL13 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH13)] ^ s_T1[(byte)(WH13 >> 40)] ^ s_T2[(byte)(WL13 >> 16)] ^ s_T3[(byte)(WL13 >> 56)];
            WD = s_T0[(byte)(WH13 >> 32)] ^ s_T1[(byte)(WL13 >> 8)] ^ s_T2[(byte)(WL13 >> 48)] ^ s_T3[(byte)(WH13 >> 24)]; 
            cnt++;
            WL13 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL13 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH13 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH13 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL14)] ^ s_T1[(byte)(WL14 >> 40)] ^ s_T2[(byte)(WH14 >> 16)] ^ s_T3[(byte)(WH14 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL14 >> 32)] ^ s_T1[(byte)(WH14 >> 8)] ^ s_T2[(byte)(WH14 >> 48)] ^ s_T3[(byte)(WL14 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH14)] ^ s_T1[(byte)(WH14 >> 40)] ^ s_T2[(byte)(WL14 >> 16)] ^ s_T3[(byte)(WL14 >> 56)];
            WD = s_T0[(byte)(WH14 >> 32)] ^ s_T1[(byte)(WL14 >> 8)] ^ s_T2[(byte)(WL14 >> 48)] ^ s_T3[(byte)(WH14 >> 24)]; 
            cnt++;
            WL14 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL14 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH14 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH14 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL15)] ^ s_T1[(byte)(WL15 >> 40)] ^ s_T2[(byte)(WH15 >> 16)] ^ s_T3[(byte)(WH15 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL15 >> 32)] ^ s_T1[(byte)(WH15 >> 8)] ^ s_T2[(byte)(WH15 >> 48)] ^ s_T3[(byte)(WL15 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH15)] ^ s_T1[(byte)(WH15 >> 40)] ^ s_T2[(byte)(WL15 >> 16)] ^ s_T3[(byte)(WL15 >> 56)];
            WD = s_T0[(byte)(WH15 >> 32)] ^ s_T1[(byte)(WL15 >> 8)] ^ s_T2[(byte)(WL15 >> 48)] ^ s_T3[(byte)(WH15 >> 24)]; 
            cnt++;
            WL15 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL15 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH15 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH15 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            ulong WT0 = WL2;
            WL2 = WL10;
            WL10 = WT0;
            ulong WT1 = WH2;
            WH2 = WH10;
            WH10 = WT1;

            WT0 = WL1;
            WL1 = WL5;
            WL5 = WT0;
            WT1 = WH1;
            WH1 = WH5;
            WH5 = WT1;

            WT0 = WL3;
            WL3 = WL15;
            WL15 = WT0;
            WT1 = WH3;
            WH3 = WH15;
            WH15 = WT1;

            WT0 = ((WL0 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL0 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL1 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL1 << 1) & 0xFEFEFEFEFEFEFEFE);
            ulong WT2 = ((WL2 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL2 << 1) & 0xFEFEFEFEFEFEFEFE);
            ulong WT3 = ((WL3 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL3 << 1) & 0xFEFEFEFEFEFEFEFE);
            ulong WT4 = WL0 ^ WL1 ^ WL2 ^ WL3; 
            WL0 ^= WT0 ^ WT1 ^ WT4;
            WL1 ^= WT1 ^ WT2 ^ WT4;
            WL2 ^= WT2 ^ WT3 ^ WT4;
            WL3 ^= WT0 ^ WT4 ^ WT3;  

            WT0 = ((WH0 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH0 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH1 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH1 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH2 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH2 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH3 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH3 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH0 ^ WH1 ^ WH2 ^ WH3; 
            WH0 ^= WT0 ^ WT1 ^ WT4;
            WH1 ^= WT1 ^ WT2 ^ WT4;
            WH2 ^= WT2 ^ WT3 ^ WT4;
            WH3 ^= WT0 ^ WT4 ^ WT3;  

            WT0 = WL13;
            WL13 = WL9;
            WL9 = WT0;
            WT1 = WH13;
            WH13 = WH9;
            WH9 = WT1;

            WT0 = WL11;
            WL11 = WL7;
            WL7 = WT0;
            WT1 = WH11;
            WH11 = WH7;
            WH7 = WT1;

            WT0 = ((WL8 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL8 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL9 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL9 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WL10 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL10 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WL11 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL11 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WL8 ^ WL9 ^ WL10 ^ WL11; 
            WL8 ^= WT0 ^ WT1 ^ WT4;
            WL9 ^= WT1 ^ WT2 ^ WT4;
            WL10 ^= WT2 ^ WT3 ^ WT4;
            WL11 ^= WT0 ^ WT4 ^ WT3; 

            WT0 = ((WH8 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH8 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH9 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH9 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH10 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH10 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH11 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH11 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH8 ^ WH9 ^ WH10 ^ WH11; 
            WH8 ^= WT0 ^ WT1 ^ WT4;
            WH9 ^= WT1 ^ WT2 ^ WT4;
            WH10 ^= WT2 ^ WT3 ^ WT4;
            WH11 ^= WT0 ^ WT4 ^ WT3;  

            WT0 = WL6;
            WL6 = WL14;
            WL14 = WT0;
            WT1 = WH6;
            WH6 = WH14;
            WH14 = WT1;

            WT0 = WL13;
            WL13 = WL5;
            WL5 = WT0;
            WT1 = WH13;
            WH13 = WH5;
            WH5 = WT1;

            WT0 = WL15;
            WL15 = WL7;
            WL7 = WT0;
            WT1 = WH15;
            WH15 = WH7;
            WH7 = WT1;

            WT0 = ((WL4 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL4 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL5 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL5 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WL6 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL6 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WL7 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL7 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WL4 ^ WL5 ^ WL6 ^ WL7; 
            WL4 ^= WT0 ^ WT1 ^ WT4;
            WL5 ^= WT1 ^ WT2 ^ WT4;
            WL6 ^= WT2 ^ WT3 ^ WT4;
            WL7 ^= WT0 ^ WT4 ^ WT3;

            WT0 = ((WH4 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH4 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH5 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH5 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH6 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH6 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH7 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH7 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH4 ^ WH5 ^ WH6 ^ WH7; 
            WH4 ^= WT0 ^ WT1 ^ WT4;
            WH5 ^= WT1 ^ WT2 ^ WT4;
            WH6 ^= WT2 ^ WT3 ^ WT4;
            WH7 ^= WT0 ^ WT4 ^ WT3; 

            WT0 = ((WL12 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL12 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL13 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL13 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WL14 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL14 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WL15 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL15 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WL12 ^ WL13 ^ WL14 ^ WL15; 
            WL12 ^= WT0 ^ WT1 ^ WT4;
            WL13 ^= WT1 ^ WT2 ^ WT4;
            WL14 ^= WT2 ^ WT3 ^ WT4;
            WL15 ^= WT0 ^ WT4 ^ WT3;

            WT0 = ((WH12 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH12 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH13 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH13 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH14 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH14 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH15 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH15 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH12 ^ WH13 ^ WH14 ^ WH15; 
            WH12 ^= WT0 ^ WT1 ^ WT4;
            WH13 ^= WT1 ^ WT2 ^ WT4;
            WH14 ^= WT2 ^ WT3 ^ WT4;
            WH15 ^= WT0 ^ WT4 ^ WT3;

            r--;
        } 
        while (r);

        WL8 ^= WL12 ^ WL4  ^ WL0; 
        WH8 ^= WH12 ^ WH4  ^ WH0; 
        WL9 ^= WL13 ^ WL5  ^ WL1; 
        WH9 ^= WH13 ^ WH5  ^ WH1; 
        WL10 ^= WL14 ^ WL6  ^ WL2; 
        WH10 ^= WH14 ^ WH6  ^ WH2; 
        WL11 ^= WL15 ^ WL7  ^ WL3; 
        WH11 ^= WH15 ^ WH7  ^ WH3; 
        m_state[0] = cv[0] ^ WL8; 
        m_state[1] = cv[1] ^ WH8; 
        m_state[2] = cv[2] ^ WL9; 
        m_state[3] = cv[3] ^ WH9; 
        m_state[4] = cv[4] ^ WL10; 
        m_state[5] = cv[5] ^ WH10; 
        m_state[6] = cv[6] ^ WL11; 
        m_state[7] = cv[7] ^ WH11; 
    }

    void Echo512Base::TransformBlock(byte* a_data, int a_index)
    {
        memcpy(&m_state[16], &a_data[a_index], BlockSize);

        uint WA, WB, WC, WD;

        ulong cv[16];

        cv[0] = m_state[0] ^ m_state[16]; 
        cv[1] = m_state[1] ^ m_state[17];
        cv[2] = m_state[2] ^ m_state[18];
        cv[3] = m_state[3] ^ m_state[19];  
        cv[4] = m_state[4] ^ m_state[20];  
        cv[5] = m_state[5] ^ m_state[21];  
        cv[6] = m_state[6] ^ m_state[22];  
        cv[7] = m_state[7] ^ m_state[23];  
        cv[8] = m_state[8] ^ m_state[24];  
        cv[9] = m_state[9] ^ m_state[25]; 
        cv[10] = m_state[10] ^ m_state[26]; 
        cv[11] = m_state[11] ^ m_state[27];  
        cv[12] = m_state[12] ^ m_state[28];  
        cv[13] = m_state[13] ^ m_state[29];  
        cv[14] = m_state[14] ^ m_state[30];  
        cv[15] = m_state[15] ^ m_state[31];  

        ulong WL0 = m_state[0];
        ulong WH0 = m_state[1];
        ulong WL1 = m_state[2];
        ulong WH1 = m_state[3];
        ulong WL2 = m_state[4];
        ulong WH2 = m_state[5];
        ulong WL3 = m_state[6];
        ulong WH3 = m_state[7];
        ulong WL4 = m_state[8];
        ulong WH4 = m_state[9];
        ulong WL5 = m_state[10];
        ulong WH5 = m_state[11];
        ulong WL6 = m_state[12];
        ulong WH6 = m_state[13];
        ulong WL7 = m_state[14];
        ulong WH7 = m_state[15];
        ulong WL8 = m_state[16];
        ulong WH8 = m_state[17];
        ulong WL9 = m_state[18];
        ulong WH9 = m_state[19];
        ulong WL10 = m_state[20];
        ulong WH10 = m_state[21];
        ulong WL11 = m_state[22];
        ulong WH11 = m_state[23];
        ulong WL12 = m_state[24];
        ulong WH12 = m_state[25];
        ulong WL13 = m_state[26];
        ulong WH13 = m_state[27];
        ulong WL14 = m_state[28];
        ulong WH14 = m_state[29];
        ulong WL15 = m_state[30];
        ulong WH15 = m_state[31];
        
        int r = 10;
        
        ulong cnt = 0;
        if (!m_last_block)
            cnt = m_processedBytes * 8;

        do 
        {
            WA = s_T0[(byte)(WL0)] ^ s_T1[(byte)(WL0 >> 40)] ^ s_T2[(byte)(WH0 >> 16)] ^ s_T3[(byte)(WH0 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL0 >> 32)] ^ s_T1[(byte)(WH0 >> 8)] ^ s_T2[(byte)(WH0 >> 48)] ^ s_T3[(byte)(WL0 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH0)] ^ s_T1[(byte)(WH0 >> 40)] ^ s_T2[(byte)(WL0 >> 16)] ^ s_T3[(byte)(WL0 >> 56)];
            WD = s_T0[(byte)(WH0 >> 32)] ^ s_T1[(byte)(WL0 >> 8)] ^ s_T2[(byte)(WL0 >> 48)] ^ s_T3[(byte)(WH0 >> 24)]; 
            cnt++;
            WL0 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL0 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH0 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH0 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL1)] ^ s_T1[(byte)(WL1 >> 40)] ^ s_T2[(byte)(WH1 >> 16)] ^ s_T3[(byte)(WH1 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL1 >> 32)] ^ s_T1[(byte)(WH1 >> 8)] ^ s_T2[(byte)(WH1 >> 48)] ^ s_T3[(byte)(WL1 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH1)] ^ s_T1[(byte)(WH1 >> 40)] ^ s_T2[(byte)(WL1 >> 16)] ^ s_T3[(byte)(WL1 >> 56)];
            WD = s_T0[(byte)(WH1 >> 32)] ^ s_T1[(byte)(WL1 >> 8)] ^ s_T2[(byte)(WL1 >> 48)] ^ s_T3[(byte)(WH1 >> 24)]; 
            cnt++;
            WL1 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL1 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH1 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH1 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL2)] ^ s_T1[(byte)(WL2 >> 40)] ^ s_T2[(byte)(WH2 >> 16)] ^ s_T3[(byte)(WH2 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL2 >> 32)] ^ s_T1[(byte)(WH2 >> 8)] ^ s_T2[(byte)(WH2 >> 48)] ^ s_T3[(byte)(WL2 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH2)] ^ s_T1[(byte)(WH2 >> 40)] ^ s_T2[(byte)(WL2 >> 16)] ^ s_T3[(byte)(WL2 >> 56)];
            WD = s_T0[(byte)(WH2 >> 32)] ^ s_T1[(byte)(WL2 >> 8)] ^ s_T2[(byte)(WL2 >> 48)] ^ s_T3[(byte)(WH2 >> 24)]; 
            cnt++;
            WL2 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL2 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH2 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH2 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL3)] ^ s_T1[(byte)(WL3 >> 40)] ^ s_T2[(byte)(WH3 >> 16)] ^ s_T3[(byte)(WH3 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL3 >> 32)] ^ s_T1[(byte)(WH3 >> 8)] ^ s_T2[(byte)(WH3 >> 48)] ^ s_T3[(byte)(WL3 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH3)] ^ s_T1[(byte)(WH3 >> 40)] ^ s_T2[(byte)(WL3 >> 16)] ^ s_T3[(byte)(WL3 >> 56)];
            WD = s_T0[(byte)(WH3 >> 32)] ^ s_T1[(byte)(WL3 >> 8)] ^ s_T2[(byte)(WL3 >> 48)] ^ s_T3[(byte)(WH3 >> 24)]; 
            cnt++;
            WL3 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL3 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH3 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH3 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL4)] ^ s_T1[(byte)(WL4 >> 40)] ^ s_T2[(byte)(WH4 >> 16)] ^ s_T3[(byte)(WH4 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL4 >> 32)] ^ s_T1[(byte)(WH4 >> 8)] ^ s_T2[(byte)(WH4 >> 48)] ^ s_T3[(byte)(WL4 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH4)] ^ s_T1[(byte)(WH4 >> 40)] ^ s_T2[(byte)(WL4 >> 16)] ^ s_T3[(byte)(WL4 >> 56)];
            WD = s_T0[(byte)(WH4 >> 32)] ^ s_T1[(byte)(WL4 >> 8)] ^ s_T2[(byte)(WL4 >> 48)] ^ s_T3[(byte)(WH4 >> 24)]; 
            cnt++;
            WL4 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL4 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH4 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH4 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL5)] ^ s_T1[(byte)(WL5 >> 40)] ^ s_T2[(byte)(WH5 >> 16)] ^ s_T3[(byte)(WH5 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL5 >> 32)] ^ s_T1[(byte)(WH5 >> 8)] ^ s_T2[(byte)(WH5 >> 48)] ^ s_T3[(byte)(WL5 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH5)] ^ s_T1[(byte)(WH5 >> 40)] ^ s_T2[(byte)(WL5 >> 16)] ^ s_T3[(byte)(WL5 >> 56)];
            WD = s_T0[(byte)(WH5 >> 32)] ^ s_T1[(byte)(WL5 >> 8)] ^ s_T2[(byte)(WL5 >> 48)] ^ s_T3[(byte)(WH5 >> 24)]; 
            cnt++;
            WL5 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL5 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH5 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH5 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL6)] ^ s_T1[(byte)(WL6 >> 40)] ^ s_T2[(byte)(WH6 >> 16)] ^ s_T3[(byte)(WH6 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL6 >> 32)] ^ s_T1[(byte)(WH6 >> 8)] ^ s_T2[(byte)(WH6 >> 48)] ^ s_T3[(byte)(WL6 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH6)] ^ s_T1[(byte)(WH6 >> 40)] ^ s_T2[(byte)(WL6 >> 16)] ^ s_T3[(byte)(WL6 >> 56)];
            WD = s_T0[(byte)(WH6 >> 32)] ^ s_T1[(byte)(WL6 >> 8)] ^ s_T2[(byte)(WL6 >> 48)] ^ s_T3[(byte)(WH6 >> 24)]; 
            cnt++;
            WL6 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL6 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH6 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH6 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL7)] ^ s_T1[(byte)(WL7 >> 40)] ^ s_T2[(byte)(WH7 >> 16)] ^ s_T3[(byte)(WH7 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL7 >> 32)] ^ s_T1[(byte)(WH7 >> 8)] ^ s_T2[(byte)(WH7 >> 48)] ^ s_T3[(byte)(WL7 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH7)] ^ s_T1[(byte)(WH7 >> 40)] ^ s_T2[(byte)(WL7 >> 16)] ^ s_T3[(byte)(WL7 >> 56)];
            WD = s_T0[(byte)(WH7 >> 32)] ^ s_T1[(byte)(WL7 >> 8)] ^ s_T2[(byte)(WL7 >> 48)] ^ s_T3[(byte)(WH7 >> 24)]; 
            cnt++;
            WL7 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL7 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH7 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH7 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL8)] ^ s_T1[(byte)(WL8 >> 40)] ^ s_T2[(byte)(WH8 >> 16)] ^ s_T3[(byte)(WH8 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL8 >> 32)] ^ s_T1[(byte)(WH8 >> 8)] ^ s_T2[(byte)(WH8 >> 48)] ^ s_T3[(byte)(WL8 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH8)] ^ s_T1[(byte)(WH8 >> 40)] ^ s_T2[(byte)(WL8 >> 16)] ^ s_T3[(byte)(WL8 >> 56)];
            WD = s_T0[(byte)(WH8 >> 32)] ^ s_T1[(byte)(WL8 >> 8)] ^ s_T2[(byte)(WL8 >> 48)] ^ s_T3[(byte)(WH8 >> 24)]; 
            cnt++;
            WL8 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL8 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH8 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH8 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL9)] ^ s_T1[(byte)(WL9 >> 40)] ^ s_T2[(byte)(WH9 >> 16)] ^ s_T3[(byte)(WH9 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL9 >> 32)] ^ s_T1[(byte)(WH9 >> 8)] ^ s_T2[(byte)(WH9 >> 48)] ^ s_T3[(byte)(WL9 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH9)] ^ s_T1[(byte)(WH9 >> 40)] ^ s_T2[(byte)(WL9 >> 16)] ^ s_T3[(byte)(WL9 >> 56)];
            WD = s_T0[(byte)(WH9 >> 32)] ^ s_T1[(byte)(WL9 >> 8)] ^ s_T2[(byte)(WL9 >> 48)] ^ s_T3[(byte)(WH9 >> 24)]; 
            cnt++;
            WL9 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL9 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH9 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH9 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL10)] ^ s_T1[(byte)(WL10 >> 40)] ^ s_T2[(byte)(WH10 >> 16)] ^ s_T3[(byte)(WH10 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL10 >> 32)] ^ s_T1[(byte)(WH10 >> 8)] ^ s_T2[(byte)(WH10 >> 48)] ^ s_T3[(byte)(WL10 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH10)] ^ s_T1[(byte)(WH10 >> 40)] ^ s_T2[(byte)(WL10 >> 16)] ^ s_T3[(byte)(WL10 >> 56)];
            WD = s_T0[(byte)(WH10 >> 32)] ^ s_T1[(byte)(WL10 >> 8)] ^ s_T2[(byte)(WL10 >> 48)] ^ s_T3[(byte)(WH10 >> 24)]; 
            cnt++;
            WL10 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL10 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH10 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH10 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL11)] ^ s_T1[(byte)(WL11 >> 40)] ^ s_T2[(byte)(WH11 >> 16)] ^ s_T3[(byte)(WH11 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL11 >> 32)] ^ s_T1[(byte)(WH11 >> 8)] ^ s_T2[(byte)(WH11 >> 48)] ^ s_T3[(byte)(WL11 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH11)] ^ s_T1[(byte)(WH11 >> 40)] ^ s_T2[(byte)(WL11 >> 16)] ^ s_T3[(byte)(WL11 >> 56)];
            WD = s_T0[(byte)(WH11 >> 32)] ^ s_T1[(byte)(WL11 >> 8)] ^ s_T2[(byte)(WL11 >> 48)] ^ s_T3[(byte)(WH11 >> 24)]; 
            cnt++;
            WL11 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL11 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH11 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH11 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL12)] ^ s_T1[(byte)(WL12 >> 40)] ^ s_T2[(byte)(WH12 >> 16)] ^ s_T3[(byte)(WH12 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL12 >> 32)] ^ s_T1[(byte)(WH12 >> 8)] ^ s_T2[(byte)(WH12 >> 48)] ^ s_T3[(byte)(WL12 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH12)] ^ s_T1[(byte)(WH12 >> 40)] ^ s_T2[(byte)(WL12 >> 16)] ^ s_T3[(byte)(WL12 >> 56)];
            WD = s_T0[(byte)(WH12 >> 32)] ^ s_T1[(byte)(WL12 >> 8)] ^ s_T2[(byte)(WL12 >> 48)] ^ s_T3[(byte)(WH12 >> 24)]; 
            cnt++;
            WL12 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL12 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH12 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH12 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL13)] ^ s_T1[(byte)(WL13 >> 40)] ^ s_T2[(byte)(WH13 >> 16)] ^ s_T3[(byte)(WH13 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL13 >> 32)] ^ s_T1[(byte)(WH13 >> 8)] ^ s_T2[(byte)(WH13 >> 48)] ^ s_T3[(byte)(WL13 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH13)] ^ s_T1[(byte)(WH13 >> 40)] ^ s_T2[(byte)(WL13 >> 16)] ^ s_T3[(byte)(WL13 >> 56)];
            WD = s_T0[(byte)(WH13 >> 32)] ^ s_T1[(byte)(WL13 >> 8)] ^ s_T2[(byte)(WL13 >> 48)] ^ s_T3[(byte)(WH13 >> 24)]; 
            cnt++;
            WL13 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL13 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH13 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH13 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL14)] ^ s_T1[(byte)(WL14 >> 40)] ^ s_T2[(byte)(WH14 >> 16)] ^ s_T3[(byte)(WH14 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL14 >> 32)] ^ s_T1[(byte)(WH14 >> 8)] ^ s_T2[(byte)(WH14 >> 48)] ^ s_T3[(byte)(WL14 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH14)] ^ s_T1[(byte)(WH14 >> 40)] ^ s_T2[(byte)(WL14 >> 16)] ^ s_T3[(byte)(WL14 >> 56)];
            WD = s_T0[(byte)(WH14 >> 32)] ^ s_T1[(byte)(WL14 >> 8)] ^ s_T2[(byte)(WL14 >> 48)] ^ s_T3[(byte)(WH14 >> 24)]; 
            cnt++;
            WL14 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL14 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH14 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH14 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            WA = s_T0[(byte)(WL15)] ^ s_T1[(byte)(WL15 >> 40)] ^ s_T2[(byte)(WH15 >> 16)] ^ s_T3[(byte)(WH15 >> 56)] ^ ((uint)cnt);
            WB = s_T0[(byte)(WL15 >> 32)] ^ s_T1[(byte)(WH15 >> 8)] ^ s_T2[(byte)(WH15 >> 48)] ^ s_T3[(byte)(WL15 >> 24)] ^ ((ulong)cnt >> 32);
            WC = s_T0[(byte)(WH15)] ^ s_T1[(byte)(WH15 >> 40)] ^ s_T2[(byte)(WL15 >> 16)] ^ s_T3[(byte)(WL15 >> 56)];
            WD = s_T0[(byte)(WH15 >> 32)] ^ s_T1[(byte)(WL15 >> 8)] ^ s_T2[(byte)(WL15 >> 48)] ^ s_T3[(byte)(WH15 >> 24)]; 
            cnt++;
            WL15 = (ulong)(s_T0[(byte)(WA)] ^ s_T1[(byte)(WB >> 8)] ^ s_T2[(byte)(WC >> 16)] ^ s_T3[(byte)(WD >> 24)]);
            WL15 ^= (ulong)(s_T0[(byte)(WB)] ^ s_T1[(byte)(WC >> 8)] ^ s_T2[(byte)(WD >> 16)] ^ s_T3[(byte)(WA >> 24)]) << 32;
            WH15 = (ulong)(s_T0[(byte)(WC)] ^ s_T1[(byte)(WD >> 8)] ^ s_T2[(byte)(WA >> 16)] ^ s_T3[(byte)(WB >> 24)]);
            WH15 ^= (ulong)(s_T0[(byte)(WD)] ^ s_T1[(byte)(WA >> 8)] ^ s_T2[(byte)(WB >> 16)] ^ s_T3[(byte)(WC >> 24)]) << 32;

            ulong WT0 = WL2;
            WL2 = WL10;
            WL10 = WT0;
            ulong WT1 = WH2;
            WH2 = WH10;
            WH10 = WT1;

            WT0 = WL1;
            WL1 = WL5;
            WL5 = WT0;
            WT1 = WH1;
            WH1 = WH5;
            WH5 = WT1;

            WT0 = WL3;
            WL3 = WL15;
            WL15 = WT0;
            WT1 = WH3;
            WH3 = WH15;
            WH15 = WT1;

            WT0 = ((WL0 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL0 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL1 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL1 << 1) & 0xFEFEFEFEFEFEFEFE);
            ulong WT2 = ((WL2 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL2 << 1) & 0xFEFEFEFEFEFEFEFE);
            ulong WT3 = ((WL3 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL3 << 1) & 0xFEFEFEFEFEFEFEFE);
            ulong WT4 = WL0 ^ WL1 ^ WL2 ^ WL3; 
            WL0 ^= WT0 ^ WT1 ^ WT4;
            WL1 ^= WT1 ^ WT2 ^ WT4;
            WL2 ^= WT2 ^ WT3 ^ WT4;
            WL3 ^= WT0 ^ WT4 ^ WT3;  

            WT0 = ((WH0 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH0 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH1 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH1 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH2 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH2 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH3 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH3 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH0 ^ WH1 ^ WH2 ^ WH3; 
            WH0 ^= WT0 ^ WT1 ^ WT4;
            WH1 ^= WT1 ^ WT2 ^ WT4;
            WH2 ^= WT2 ^ WT3 ^ WT4;
            WH3 ^= WT0 ^ WT4 ^ WT3;  

            WT0 = WL13;
            WL13 = WL9;
            WL9 = WT0;
            WT1 = WH13;
            WH13 = WH9;
            WH9 = WT1;

            WT0 = WL11;
            WL11 = WL7;
            WL7 = WT0;
            WT1 = WH11;
            WH11 = WH7;
            WH7 = WT1;

            WT0 = ((WL8 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL8 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL9 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL9 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WL10 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL10 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WL11 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL11 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WL8 ^ WL9 ^ WL10 ^ WL11; 
            WL8 ^= WT0 ^ WT1 ^ WT4;
            WL9 ^= WT1 ^ WT2 ^ WT4;
            WL10 ^= WT2 ^ WT3 ^ WT4;
            WL11 ^= WT0 ^ WT4 ^ WT3; 

            WT0 = ((WH8 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH8 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH9 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH9 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH10 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH10 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH11 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH11 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH8 ^ WH9 ^ WH10 ^ WH11; 
            WH8 ^= WT0 ^ WT1 ^ WT4;
            WH9 ^= WT1 ^ WT2 ^ WT4;
            WH10 ^= WT2 ^ WT3 ^ WT4;
            WH11 ^= WT0 ^ WT4 ^ WT3;  

            WT0 = WL6;
            WL6 = WL14;
            WL14 = WT0;
            WT1 = WH6;
            WH6 = WH14;
            WH14 = WT1;

            WT0 = WL13;
            WL13 = WL5;
            WL5 = WT0;
            WT1 = WH13;
            WH13 = WH5;
            WH5 = WT1;

            WT0 = WL15;
            WL15 = WL7;
            WL7 = WT0;
            WT1 = WH15;
            WH15 = WH7;
            WH7 = WT1;

            WT0 = ((WL4 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL4 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL5 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL5 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WL6 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL6 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WL7 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL7 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WL4 ^ WL5 ^ WL6 ^ WL7; 
            WL4 ^= WT0 ^ WT1 ^ WT4;
            WL5 ^= WT1 ^ WT2 ^ WT4;
            WL6 ^= WT2 ^ WT3 ^ WT4;
            WL7 ^= WT0 ^ WT4 ^ WT3;

            WT0 = ((WH4 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH4 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH5 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH5 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH6 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH6 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH7 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH7 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH4 ^ WH5 ^ WH6 ^ WH7; 
            WH4 ^= WT0 ^ WT1 ^ WT4;
            WH5 ^= WT1 ^ WT2 ^ WT4;
            WH6 ^= WT2 ^ WT3 ^ WT4;
            WH7 ^= WT0 ^ WT4 ^ WT3; 

            WT0 = ((WL12 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WL12 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WL13 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WL13 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WL14 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WL14 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WL15 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WL15 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WL12 ^ WL13 ^ WL14 ^ WL15; 
            WL12 ^= WT0 ^ WT1 ^ WT4;
            WL13 ^= WT1 ^ WT2 ^ WT4;
            WL14 ^= WT2 ^ WT3 ^ WT4;
            WL15 ^= WT0 ^ WT4 ^ WT3;

            WT0 = ((WH12 >> 7) & 0x0101010101010101) * 27;
            WT0 ^= ((WH12 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT1 = ((WH13 >> 7) & 0x0101010101010101) * 27;
            WT1 ^= ((WH13 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT2 = ((WH14 >> 7) & 0x0101010101010101) * 27;
            WT2 ^= ((WH14 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT3 = ((WH15 >> 7) & 0x0101010101010101) * 27;
            WT3 ^= ((WH15 << 1) & 0xFEFEFEFEFEFEFEFE);
            WT4 = WH12 ^ WH13 ^ WH14 ^ WH15; 
            WH12 ^= WT0 ^ WT1 ^ WT4;
            WH13 ^= WT1 ^ WT2 ^ WT4;
            WH14 ^= WT2 ^ WT3 ^ WT4;
            WH15 ^= WT0 ^ WT4 ^ WT3;

            r--;
        } 
        while (r);

        m_state[0] = cv[0] ^ WL0 ^ WL8; 
        m_state[1] = cv[1] ^ WH0 ^ WH8; 
        m_state[2] = cv[2] ^ WL1 ^ WL9; 
        m_state[3] = cv[3] ^ WH1 ^ WH9; 
        m_state[4] = cv[4] ^ WL2 ^ WL10; 
        m_state[5] = cv[5] ^ WH2 ^ WH10; 
        m_state[6] = cv[6] ^ WL3 ^ WL11; 
        m_state[7] = cv[7] ^ WH3 ^ WH11; 
        m_state[8] = cv[8] ^ WL4 ^ WL12; 
        m_state[9] = cv[9] ^ WH4 ^ WH12; 
        m_state[10] = cv[10] ^ WL5 ^ WL13; 
        m_state[11] = cv[11] ^ WH5 ^ WH13; 
        m_state[12] = cv[12] ^ WL6 ^ WL14; 
        m_state[13] = cv[13] ^ WH6 ^ WH14; 
        m_state[14] = cv[14] ^ WL7 ^ WL15; 
        m_state[15] = cv[15] ^ WH7 ^ WH15; 
    }

    void EchoBase::TransformBytes(byte* a_data, int a_index, int a_length)
    {
        if (m_buffer_pos > 0)
        {
            int left = BlockSize - m_buffer_pos;
            if (left > a_length)
                left = a_length;

            memcpy(&m_buffer[m_buffer_pos], &a_data[a_index], left);
            a_length -= left;
            a_index += left;
            m_buffer_pos += left;
            m_processedBytes += left;
               
            if (m_buffer_pos == BlockSize)
            {
                TransformBlock(m_buffer, 0);
                m_buffer_pos = 0;
            }
        }

        while (a_length >= BlockSize)
        {
            m_processedBytes += BlockSize;
            TransformBlock(a_data, a_index);
            a_index += BlockSize;
            a_length -= BlockSize;
        }

        if (a_length > 0)
        {
            memcpy(&m_buffer[m_buffer_pos], &a_data[a_index], a_length);
            m_buffer_pos += a_length;
            m_processedBytes += a_length;
        }
    }

    void EchoBase::Finish()
    {
        byte pad[192];
        memset(pad, 0, 192);

        pad[0] = 0x80;

        ulong bits = m_processedBytes * 8;

        if ((BlockSize - m_buffer_pos) <= 18)
        {
            int pad_len = BlockSize - m_buffer_pos;
            m_processedBytes -= pad_len;
            TransformBytes(pad, 0, BlockSize - m_buffer_pos);
            m_processedBytes += pad_len;
            memset(pad, 0, 192);
        } 

        int index = BlockSize - m_buffer_pos - 18;

        pad[index++] = (byte)(HashSize * 8);
        pad[index++] = (byte)((HashSize * 8) >> 8);

        pad[index++] = (byte)(bits >> 0);
        pad[index++] = (byte)(bits >> 8);
        pad[index++] = (byte)(bits >> 16);
        pad[index++] = (byte)(bits >> 24);
        pad[index++] = (byte)(bits >> 32);
        pad[index++] = (byte)(bits >> 40);
        pad[index++] = (byte)(bits >> 48);
        pad[index] = (byte)(bits >> 56);

        m_last_block = (m_buffer_pos == 0);

        int pad_len = BlockSize - m_buffer_pos;
        m_processedBytes -= pad_len;
        TransformBytes(pad, 0, BlockSize - m_buffer_pos);
        m_processedBytes += pad_len;
    }
}