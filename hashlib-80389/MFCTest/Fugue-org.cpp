
#include "stdafx.h"
#include "Fugue-org.h"

namespace FugueOrg
{
HashReturn
Init (hashState *state, int hashbitlen)
{
    if (Init_Fugue(state, hashbitlen)) return SUCCESS;
    return FAIL;
}

HashReturn
Update (hashState *state, const BitSequence *data, DataLength databitlen)
{
    if (!state || !state->Cfg)
        return FAIL;
    if (!databitlen)
        return SUCCESS;
    if (state->TotalBits&7)
        return FAIL;
    if (state->TotalBits&31)
    {
        int need = 32-(state->TotalBits&31);
        if (need>databitlen)
        {
            memcpy ((uint8*)state->Partial+((state->TotalBits&31)/8), data, (databitlen+7)/8);
            state->TotalBits += databitlen;
            return SUCCESS;
        }
        else
        {
            memcpy ((uint8*)state->Partial+((state->TotalBits&31)/8), data, need/8);
            Next_Fugue (state, state->Partial, 1);
            state->TotalBits += need;
            databitlen -= need;
            data += need/8;
        }
    }
    if (databitlen>31)
    {
        Next_Fugue (state, (uint32*)data, databitlen/32);
        state->TotalBits += (databitlen/32)*32;
        data += (databitlen/32)*4;
        databitlen &= 31;
    }
    if (databitlen)
    {
        memcpy ((uint8*)state->Partial, data, (databitlen+7)/8);
        state->TotalBits += databitlen;
    }
    return SUCCESS;
}

HashReturn
Final (hashState *state, BitSequence *hashval)
{
    if (!state || !state->Cfg)
        return FAIL;
    if (state->TotalBits&31)
    {
        int need = 32-(state->TotalBits&31);
        memset ((uint8*)state->Partial+(((state->TotalBits&31)+7)/8), 0, need/8);
        Next_Fugue (state, state->Partial, 1);
    }
    state->TotalBits = HO2BE_8 (state->TotalBits);
    Next_Fugue (state, (uint32*)&state->TotalBits, 2);
    Done_Fugue (state, (uint32*)hashval, NULL);
    return SUCCESS;
}

HashReturn
Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
    hashState HS;

    if (Init (&HS, hashbitlen) == SUCCESS)
        if (Update (&HS, data, databitlen) == SUCCESS)
            return Final (&HS, hashval);
    return FAIL;
}

//////////////////

static struct {
    int       hashbitlen;
    hashCfg   Cfg;
    uint32    IV[16];
    }         hashSizes[] = {{224,{7 ,30,2,5,13},{0}}
                            ,{256,{8 ,30,2,5,13},{0}}
                            ,{384,{12,36,3,6,13},{0}}
                            ,{512,{16,36,4,8,13},{0}}
                            ,{0}
                            };

int make_fugue = 0;

void
Make_Fugue ()
{
    uint_32t input[16];
    hashState hs;
    int i;

    if (make_fugue) return;
    make_fugue = 1;

    /* generate IV for each hashbitlen in hashSizes table */
    for (i=0; hashSizes[i].hashbitlen; i++)
    {
        int n = hashSizes[i].Cfg.n;

        /* clear initial hash state, this also effectively swets the IV to zero's */
        memset (&hs, 0, sizeof (hashState));

        /* set NIST hashbitlen field in hashState based on hashwordlen field of hashSizes table */
        hs.hashbitlen = n*32;

        /* save pointer to Fugue configuration for hashbitlen in Cfg field in hashState */
        hs.Cfg = &hashSizes[i].Cfg;

        /* set 1 word message to 32-bit big-endian of hashbitlen number */
        input[0] = HO2BE_4 (hashSizes[i].hashbitlen);

        /* process 1 word message into hashState */
        switch (hs.hashbitlen)
        {
        case 224:
        case 256: fugue_update_256 (&hs, (char*) input, 1); break;
        case 384: fugue_update_384 (&hs, (char*) input, 1); break;
        case 512: fugue_update_512 (&hs, (char*) input, 1); break;
        }

        /* process final rounds and place message digest in hashSizes tables IV field */
        switch (hs.hashbitlen)
        {
        case 224:
        case 256: fugue_final_256 (&hs, (char*) hashSizes[i].IV); break;
        case 384: fugue_final_384 (&hs, (char*) hashSizes[i].IV); break;
        case 512: fugue_final_512 (&hs, (char*) hashSizes[i].IV); break;
        }

    }
}

uint32
Init_Fugue (hashState *hs, int hashbitlen)
{
    int i;

    Make_Fugue();
    if (!hs) return 0;
    memset (hs, 0, sizeof (hashState));
    for (i=0; hashSizes[i].hashbitlen; i++)
        if (hashSizes[i].hashbitlen==hashbitlen)
        {
            int n = hashSizes[i].Cfg.n;
            int s = hashSizes[i].Cfg.s;
            hs->hashbitlen = n*32;
            hs->Cfg = &hashSizes[i].Cfg;
            memcpy (&hs->State[s-n].d, hashSizes[i].IV, n*4);
            return 1;
        }
    return 0;
}

uint32
Load_Fugue (hashState* hs, int hashbitlen, const uint32* iv, int hashwordlen)
{
    int i;

    Make_Fugue ();
    if (!hs) return 0;
    memset (hs, 0, sizeof (hashState));
    for (i=0; hashSizes[i].hashbitlen; i++)
        if (hashSizes[i].hashbitlen==hashbitlen && hashSizes[i].Cfg.n==hashwordlen)
        {
            int n = hashSizes[i].Cfg.n;
            int s = hashSizes[i].Cfg.s;
            hs->hashbitlen = n*32;
            hs->Cfg = &hashSizes[i].Cfg;
            memcpy (&hs->State[s-n].d, iv, n*4);
            return 1;
        }
    return 0;
}

uint32
Next_Fugue (hashState* hs, const uint32* msg, ulong len)
{
    if (hs && hs->Cfg)
    {
        switch (hs->hashbitlen)
        {
        case 224:
        case 256: fugue_update_256 (hs, (char*) msg, len); break;
        case 384: fugue_update_384 (hs, (char*) msg, len); break;
        case 512: fugue_update_512 (hs, (char*) msg, len); break;
        }
        return 1;
    }
    return 0;
}

uint32
Done_Fugue (hashState* hs, uint32* md, int* hashwordlen)
{
    if (hs && hs->Cfg)
    {
        int n = hs->Cfg->n;
        switch (hs->hashbitlen)
        {
        case 224:
        case 256: fugue_final_256 (hs, (char*) md); break;
        case 384: fugue_final_384 (hs, (char*) md); break;
        case 512: fugue_final_512 (hs, (char*) md); break;
        }
        if (hashwordlen) *hashwordlen = n;
        return 1;
    }
    return 0;
}

//////////////////////////// 256

// set the IMPL_TYPE to _ASM for ASM_INLINE
// eet it to _OPT for C OPTIMIZED
//same rules hold for EXTRA_ROUND_IMPL_TYPE, though they cant be set to _ASM...yet !
// ASS_INLINE will be used in some macro names as a suiffix _ASM
// and   C OPTIMIZED as _OPT

/* Uncomment the desired default configuration */


#define IMPL_TYPE  _OPT
#define EXTRA_ROUND_IMPL_TYPE  _OPT

#define FUGUE_SIZE 30
#define FUGUE_FAR 24


#define column(i) col[i]
#define row(i) row[i]


#define vector2words(rc,i) \
bytes2word(bval(rc[0],i),  bval(rc[1],i),   bval(rc[2],i),   bval(rc[3],i))

#define off(p,n) ((n+p+2*FUGUE_SIZE) % FUGUE_SIZE)

#define tix(p,j) \
  state[off(p,10)] ^= tmp_zero;  state[off(p,0)] = ((uint_32t *)in)[j];  \
  state[off(p,8)]  ^= state[off(p,0)];  state[off(p,1)] ^= state[off(p,FUGUE_FAR)]


#define htix(p,j) \
  tmp_in = *(uint_64t *) &((uint_32t *)in)[j];	\
  state[off(p,1)] ^= state[off(p,FUGUE_FAR)];	\
  state[off(p,10)] ^= tmp_zero;  \
  state[off(p,8)]  ^= state[off(p,0)] = (uint_32t) tmp_in;		\
  state[off(p,-3)] ^= state[off(p,1)];  state[off(p,-2)] ^= state[off(p,2)];   \
state[off(p,-1)] ^= state[off(p,3)];\
  state[off(p,12)] ^= state[off(p,1)];  state[off(p,13)] ^= state[off(p,2)];   \
state[off(p,14)] ^= state[off(p,3)];\
  tmp_in = (uint_32t) (tmp_in >>32); 


#define gtix(p,j) \
  state[off(p,8)] ^= state[off(p,0)] = (uint_32t) (tmp_in);  \
  state[off(p,10)] ^= tmp_zero;  \
  state[off(p,1)]  ^= state[off(p,FUGUE_FAR)]


#define column_mix(p)\
  state[off(p,0)] ^= state[off(p,4)];   state[off(p,1)] ^= state[off(p,5)];   state[off(p,2)] ^= state[off(p,6)];\
  state[off(p,15)] ^= state[off(p,4)];  state[off(p,16)] ^= state[off(p,5)];  state[off(p,17)] ^= state[off(p,6)]



  //  *(uint_64t*) &(state[off(p,0)]) ^= *(uint_64t *) &(state[off(p,4)]);  state[off(p,2)] ^= state[off(p,6)]; \
  //  *(uint_64t*) &(state[off(p,15)]) ^= *(uint_64t*) &(state[off(p,4)]);  state[off(p,17)] ^= state[off(p,6)]


#define print_rc_OPT
/*printf("In type 2\n");
printf("col: %x %x %x %x \n", column(0),column(1),column(2),column(3));\
printf("row: %x %x %x %x \n", row(0), row(1), row(2), row(3));*/


#define put_back_to_b_OPT(i)  print_rc_OPT;


#define table_index_OPT(p,i,k) tmp = aes_style_table[k][bval(state[off(p,i)],k)]
#define xor_col_OPT(p,k,i) column(i) ^=  aes_style_table[k][bval(state[off(p,i)],k)]
#define set_col_OPT(p,k,i) column(i)  =  aes_style_table[k][bval(state[off(p,i)],k)]
#define xor_row_xor_col_OPT(p,k,i) row(k) ^= tmp; column(i) ^= tmp;
#define xor_row_set_col_OPT(p,k,i) row(k) ^= column(i) = aes_style_table[k][bval(state[off(p,i)],k)]
#define set_row_xor_col_OPT(p,k,i) column(i) ^= row(k) = aes_style_table[k][bval(state[off(p,i)],k)]
#define set_row_set_col_OPT(p,k,i) row(k) = column(i)  = aes_style_table[k][bval(state[off(p,i)],k)]


#define xor_short_form_i_eq_k(p,i,k,type)       \
  /*table_index##type(p,i,k);*/ xor_col##type(p,k,i)

#define xor_short_form_i_eq_k_col_first(p,i,k,type)     \
  /* table_index##type(p,i,k);*/  set_col##type(p,k,i)

#define xor_short_form_i_neq_k(p,i,k,type)  \
  table_index##type(p,i,k);   xor_row_xor_col##type(p,k,i)

#define xor_short_form_i_neq_k_col_first(p,i,k,type)  \
  /*  table_index##type(p,i,k);*/ xor_row_set_col##type(p,k,i)

#define xor_short_form_i_neq_k_row_first(p,i,k,type)  \
  /* table_index##type(p,i,k);*/  set_row_xor_col##type(p,k,i)

#define xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)  \
  /* table_index##type(p,i,k);*/   set_row_set_col##type(p,k,i)

#define xor_rc_3_0(p,i,k,type)  xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)
#define xor_rc_3_1(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_3_2(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_3_3(p,i,k,type)  xor_short_form_i_eq_k(p,i,k,type)

//Note col 3 is being done first

#define xor_rc_0_0(p,i,k,type)  xor_short_form_i_eq_k_col_first(p,i,k,type)
#define xor_rc_0_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_0_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_0_3(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)

#define xor_rc_1_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_1_1(p,i,k,type)  xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_1_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_1_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_2_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_2_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_2_2(p,i,k,type)  xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_2_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_outer(p,i,type)\
   xor_rc_##i##_0(p,i,0,type);\
   xor_rc_##i##_1(p,i,1,type); xor_rc_##i##_2(p,i,2,type); xor_rc_##i##_3(p,i,3,type)

#define xor_row_and_col(p,type) \
     xor_rc_outer(p,3,type); \
     xor_rc_outer(p,0,type); \
     xor_rc_outer(p,1,type); \
     xor_rc_outer(p,2,type); 

#define rotate_regs_OPT \
row(0)=shift_right64(row(0),8);\
row(1)=shift_right64(row(1),8);\
row(2)=shift_right64(row(2),8);\
row(3)=shift_left64 (row(3),24);

#define write_back_to_state_listed_OPT_0(p,i) \
  col0 = ((uint_32t) column(0)) ^ ((uint_64t)column(2) <<32);	\
  col1 = ((uint_32t) column(1)) ^ ((uint_64t)column(3) <<32); \
  col2 = ((uint_32t) column(2)) ^ ((uint_64t)column(0) <<32);	\
  col3 = ((uint_32t) column(3)) ^ ((uint_64t)column(1) <<32);	\
  col0col2  = ((row(0) ^ col0) & 0xff000000ffULL) ^	\
              ((row(1) ^ col1) & 0xff000000ff00ULL) ^	\
              ((row(2) ^ col2) & 0xff000000ff0000ULL) ^		\
              ((row(3) ^ col3) & 0xff000000ff000000ULL);	\
  rotate_regs_OPT 


#define write_back_to_state_listed_OPT_1(p,i) \
  col1col3  = ((row(0) ^ col1) & 0xff000000ffULL) ^    \
              ((row(1) ^ col2) & 0xff000000ff00ULL) ^	\
              ((row(2) ^ col3) & 0xff000000ff0000ULL) ^		\
              ((row(3) ^ col0) & 0xff000000ff000000ULL);


#define write_back_to_state_listed_OPT_2(p,i) \
  tmp_zero = (uint_32t) col0col2; \
  state[off(p,2)] = (uint_32t) (col0col2 >>32);
  

#define write_back_to_state_listed_OPT_3(p,i) \
  state[off(p,1)] = (uint_32t) col1col3; \
  state[off(p,3)] = (uint_32t) (col1col3 >>32);

#define write_back_to_state_indexed_OPT(p,i) \
    write_back_to_state_listed_OPT_##i(p,i)


/**************END************/

#define write_back_to_state(p,type) \
write_back_to_state_indexed##type(p,0);write_back_to_state_indexed##type(p,1);\
write_back_to_state_indexed##type(p,2);write_back_to_state_indexed##type(p,3);

#define super_mix(ptr,type)\
 xor_row_and_col(ptr,type); \
 write_back_to_state(ptr,type);

#define adjust_tmp_zero(ptr,type)  state[off(ptr,0)] = tmp_zero;


#define sub_round(ptr,type) \
             column_mix(ptr);   super_mix(ptr,type)

#define xsub_round(ptr,type) \
  column_mix(ptr);   super_mix(ptr,type); adjust_tmp_zero(ptr,type);

#define hsub_round(ptr,type) \
               super_mix(ptr,type); adjust_tmp_zero(ptr,type);

#define super_mix_convert(p,type) super_mix(p,type); adjust_tmp_zero(p,type);

#define ror(state,n) ptr = ((ptr-n+FUGUE_SIZE) % FUGUE_SIZE)

#define cross_mix(p,cross) \
       state[off(p,4)] ^=state[off(p,0)]; \
       state[off(p,cross)] ^=state[off(p,0)]

#define final_sub_round(p,cross,n) \
        cross_mix(p,cross);\
       ror(state,n); super_mix_convert(p,EXTRA_ROUND_IMPL_TYPE)

#define final_round2(ptr,num,c)\
    for (j=0; j<c; j++)      \
         { final_sub_round(ptr, 15,15); \
           final_sub_round(ptr, 16,14); \
           empty_pretty_print(state, ptr, 30, "after fr2 subround 2"); \
           } \
           cross_mix(ptr,15);

#define final_round1(ptr,num,c) \
    for (j=0; j<c; j++)  \
      {xsub_round(((-3)*(2*((num+j)%5)+1)),EXTRA_ROUND_IMPL_TYPE);  \
        empty_update(ptr);empty_pretty_print(state, ptr, 30, "after fr1 subround 1"); \
       xsub_round(((-3)*(2*((num+j)%5)+2)),EXTRA_ROUND_IMPL_TYPE);  \
        empty_update(ptr);empty_pretty_print(state, ptr, 30, "after fr1 subround 2"); \
}



#define round(j,num,type) \
         tix(((-3)*2*num),j); \
           xsub_round(((-3)*(2*num+1)),type); \
              empty_update(ptr); empty_pretty_print(state, ptr, 30, "after subround 1");\
             sub_round(((-3)*(2*num+2)),type); \
              empty_update(ptr); empty_pretty_print(state, ptr, 30, "after subround 2");



#define hround(j,num,type) \
         htix(((-3)*2*num),j); \
           hsub_round(((-3)*(2*num+1)),type); \
              empty_update(ptr); empty_pretty_print(state, ptr, 30, "after h  subround 1");\
             sub_round(((-3)*(2*num+2)),type); \
              empty_update(ptr); empty_pretty_print(state, ptr, 30, "after h subround 2");


#define ground(j,num,type) \
         gtix(((-3)*2*num),j); \
	 xsub_round(((-3)*(2*num+1)),type); \
         empty_update(ptr); empty_pretty_print(state, ptr, 30, "after g subround 1");\
           sub_round(((-3)*(2*num+2)),type); \
             empty_update(ptr); empty_pretty_print(state, ptr, 30, "afterg  subround 2");



#define empty_update(ptr) 
//ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;


#define update(ptr) \
ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;



#define empty_pretty_print(s,a,b,c)

void pretty_print(uint_32t * state, int ptr, int n, char * st)
{
    int i,j;
    uint_32t tmp;

    printf("%s\n", st);
    for (i=0; i<n; i++)
    {
        tmp = state[off(ptr,i)];
        for (j=0; j<4; j++)
        {
            printf("%02x", (uint_8t)tmp);
            tmp = tmp>>8;
        }
        printf(" ");
    }
    printf("\n\n");
}





int fugue_update_256 (hashState *hs, const char *in, uint_64t len)
{
    uint_32t state[30];

    uint_64t row[4];
    uint_64t col[4];
    uint_64t col0,col1,col2,col3,col0col2,col1col3;

    uint_64t tmp, tmp_in;
    uint_32t tmp_zero;
    short len_mod;
    uint_64t len_div;
    uint_64t j,j1,j2;
    short ptr = hs->Base;

    j1 = (5-hs->Base/6)%5;  // this is the left over sliding window pointer (divided by 2*3)

    for (j=0; j < 30; j++) state[j] = hs->State[j].d;

    j=0;
    
    tmp_zero = state[off(ptr,0)];
    if (j1 && len)
    {
	if  (j1 <=1)         { round(j,1,IMPL_TYPE); j++; len--; ptr -=6; }
	if ((j1 <=2) && len) { round(j,2,IMPL_TYPE); j++; len--; ptr -=6; }
        if ((j1 <=3) && len) { round(j,3,IMPL_TYPE); j++; len--; ptr -=6; }
        if ((j1 <=4) && len) { round(j,4,IMPL_TYPE); j++; len--; ptr -=6; }
    }

    len_mod = (len)%10;
    len_div = (len)/10;

    for (j2=0; j2<len_div;j2++)
    {
        hround(j,0,IMPL_TYPE); j++;
        ground(j,1,IMPL_TYPE); j++;
        hround(j,2,IMPL_TYPE); j++;
        ground(j,3,IMPL_TYPE); j++;
        hround(j,4,IMPL_TYPE); j++;
        ground(j,0,IMPL_TYPE); j++;
        hround(j,1,IMPL_TYPE); j++;
        ground(j,2,IMPL_TYPE); j++;
        hround(j,3,IMPL_TYPE); j++;
        ground(j,4,IMPL_TYPE); j++;
    }

    if (len_mod>=5)
      {
        hround(j,0,IMPL_TYPE); j++;
	ground(j,1,IMPL_TYPE); j++;
        hround(j,2,IMPL_TYPE); j++;
        ground(j,3,IMPL_TYPE); j++;
        round(j,4,IMPL_TYPE); j++; 
	len_mod = len_mod -5;
      }

    if (len_mod >=1)
    {
        round(j,0,IMPL_TYPE); j++;
        if (len_mod >=2)
        {
            round(j,1,IMPL_TYPE); j++;
            if (len_mod >=3)
            {
                round(j,2,IMPL_TYPE); j++;
                if (len_mod >=4)
                {
                     round(j,3,IMPL_TYPE); j++;
                }
            }
        }
    }
    state[off(ptr-6*len_mod,0)] = tmp_zero;

    empty_pretty_print(state, ptr, 30, "at end");

    for (j=0; j < 30; j++) hs->State[j].d =  state[j];
    hs->Base = (ptr-6*len_mod + 2*FUGUE_SIZE) % FUGUE_SIZE;

    return 0;
}

int fugue_final_256 (hashState *hs, char *out)
{
  // uint_32t state[30];

    uint_32t *state = (uint_32t *)hs->State;
    uint_64t row[4];
    uint_64t col[4];
    uint_64t tmp;
    uint_32t tmp_zero;
    uint_64t col0,col1,col2,col3,col0col2,col1col3;
    uint_64t j,j1 = (5-hs->Base/6)%5;
    short ptr = hs->Base;

    //    for (j=0; j < 30; j++) state[j] = hs->State[j].d;

    final_round1(ptr,j1,5);
    final_round2(ptr,j1,13);

   empty_pretty_print(state, ptr, 30, "at final  end");

    for (j=0; j<4; j++)
    {
        ((uint_32t *)out)[j]   = state[off(ptr,1+j)];
        ((uint_32t *)out)[4+j] = state[off(ptr,15+j)];
    }

    return 0;
}
////////////////////////////// 384

// set the IMPL_TYPE to _ASM for ASM_INLINE
// eet it to _OPT for C OPTIMIZED
//same rules hold for EXTRA_ROUND_IMPL_TYPE, though they cant be set to _ASM...yet !
// ASS_INLINE will be used in some macro names as a suiffix _ASM
// and   C OPTIMIZED as _OPT

/* Uncomment the desired default configuration */
#if !defined(CONFIG_ASM_INLINE) && !defined(CONFIG_C_OPTIMIZED)
//#define CONFIG_ASM_INLINE
#define CONFIG_C_OPTIMIZED
#endif

#ifdef CONFIG_ASM_INLINE
#define IMPL_TYPE  _ASM
#endif

#ifdef CONFIG_C_OPTIMIZED
#define IMPL_TYPE  _OPT
#endif

#define EXTRA_ROUND_IMPL_TYPE  _OPT

#define kkk 3
#define FUGUE_SIZE 36
#define FUGUE_FAR1 27
#define FUGUE_FAR2 30


#define column(i) col[i]
#define row(i) row[i]


#define vector2words(rc,i) \
bytes2word(bval(rc[0],i),  bval(rc[1],i),   bval(rc[2],i),   bval(rc[3],i))

#define off(p,n) ((n+p+FUGUE_SIZE) % FUGUE_SIZE)

#define tix(p,j) \
  state[off(p,16)] ^= state[off(p,0)];  state[off(p,0)] = ((uint_32t *)in)[j];  \
  state[off(p,8)]  ^= state[off(p,0)];  state[off(p,1)] ^= state[off(p,FUGUE_FAR1)];\
  state[off(p,4)] ^= state[off(p,FUGUE_FAR2)]


#define column_mix(p)\
  state[off(p,0)] ^= state[off(p,4)];  state[off(p,1)] ^= state[off(p,5)];  state[off(p,2)] ^= state[off(p,6)];\
  state[off(p,18)] ^= state[off(p,4)];  state[off(p,19)] ^= state[off(p,5)];  state[off(p,20)] ^= state[off(p,6)]

#define print_rc_ASM
/*printf("In type 0\n");
 printf("col: %x %x %x %x \n", b0,b1,b2,b3);*/

#define print_rc_OPT
/*printf("In type 2\n");
printf("col: %x %x %x %x \n", column(0),column(1),column(2),column(3));\
printf("row: %x %x %x %x \n", row(0), row(1), row(2), row(3));*/

#define shift_edi0
#define shift_edi1 "shrl $8,%%edi\n\t"
#define shift_edi2 "shrl $16,%%edi\n\t"
#define shift_edi3 "shrl $24,%%edi\n\t"

#define asm_bval4(y,c,t)\
        asm ("movl %1, %0\n\t"\
             shift_edi##c \
             "andl $255, %%edi"\
             :"=D" (t)       \
             :"m" (y)   \
             :"memory"       \
             );

#define asm_set_template(c,t) \
        asm ("movl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );
#define asm_xor_template(c,t) \
        asm ("xorl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );

#define asm_xor_xor_template(reg,c,t) \
        asm ("xorl %%" reg ", %%esi\n\t"\
             "xorl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_set_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "movl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_xor_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_xor_set_template(reg,c,t) \
        asm ("movl %0, %%esi \n\t"\
             "xorl %%esi, %%" reg "\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );
#define put_back_to_b_ASM(i) \
asm (        "movl %%esi,%0\n\t"\
             :"=m" (b##i)\
             :\
             :"memory", "%esi"\
             );

#define put_back_to_b_OPT(i)  print_rc_OPT;

/***if define (ASS_INLINE) *****/
#define table_index_ASM(p,i,k)  asm_bval4(state[off(p,i)],k,tmp)
#define asm_template0(xs_type,k,i,tmp)  xs_type("eax",k,tmp);
#define asm_template1(xs_type,k,i,tmp)  xs_type("ebx",k,tmp);
#define asm_template2(xs_type,k,i,tmp)  xs_type("ecx",k,tmp);
#define asm_template3(xs_type,k,i,tmp)  xs_type("edx",k,tmp);

#define xor_col_ASM(k,i) asm_xor_template(k,tmp);
#define set_col_ASM(k,i) asm_set_template(k,tmp);

#define xor_row_xor_col_ASM(k,i) asm_template##k(asm_xor_xor_template, k,i,tmp);
#define xor_row_set_col_ASM(k,i) asm_template##k(asm_xor_set_template, k,i,tmp);
#define set_row_xor_col_ASM(k,i) asm_template##k(asm_set_xor_template, k,i,tmp);
#define set_row_set_col_ASM(k,i) asm_template##k(asm_set_set_template, k,i,tmp);
/********ELSE*********/
#define table_index_OPT(p,i,k) tmp = aes_style_table[k][bval(state[off(p,i)],k)]
#define xor_col_OPT(k,i) column(i) ^= tmp;
#define set_col_OPT(k,i) column(i) = tmp;
#define xor_row_xor_col_OPT(k,i) row(k) ^= tmp; column(i) ^= tmp;
#define xor_row_set_col_OPT(k,i) row(k) ^= tmp; column(i) = tmp;
#define set_row_xor_col_OPT(k,i) row(k) = tmp;  column(i) ^= tmp;
#define set_row_set_col_OPT(k,i) row(k) = tmp;  column(i) = tmp;
/*******ENDIF *******/


#define xor_short_form_i_eq_k(p,i,k,type)       \
table_index##type(p,i,k); xor_col##type(k,i)

#define xor_short_form_i_eq_k_col_first(p,i,k,type)     \
table_index##type(p,i,k);  set_col##type(k,i)

#define xor_short_form_i_neq_k(p,i,k,type)  \
table_index##type(p,i,k);   xor_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_col_first(p,i,k,type)  \
table_index##type(p,i,k); xor_row_set_col##type(k,i)

#define xor_short_form_i_neq_k_row_first(p,i,k,type)  \
table_index##type(p,i,k);  set_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)  \
table_index##type(p,i,k);   set_row_set_col##type(k,i)

#define xor_rc_0_0(p,i,k,type) xor_short_form_i_eq_k_col_first(p,i,k,type)
#define xor_rc_0_1(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_0_2(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_0_3(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)

#define xor_rc_1_0(p,i,k,type)  xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)
#define xor_rc_1_1(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_1_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_1_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_2_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_2_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_2_2(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_2_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_3_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_3_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_3_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_3_3(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)

#define xor_rc_outer(p,i,type)\
   xor_rc_##i##_0(p,i,0,type);\
   xor_rc_##i##_1(p,i,1,type); xor_rc_##i##_2(p,i,2,type); xor_rc_##i##_3(p,i,3,type)

#define xor_row_and_col(p,type) \
     xor_rc_outer(p,0,type); put_back_to_b##type(0)\
     xor_rc_outer(p,1,type); put_back_to_b##type(1)\
     xor_rc_outer(p,2,type); put_back_to_b##type(2) \
     xor_rc_outer(p,3,type); put_back_to_b##type(3)




/******if defined (ASS_INLINE) *******/

#define init_row_and_col_ASM

#define load_eax(mask) "movl %%eax,%%esi\n\t"\
                 "xorl %1,%%esi\n\t"\
                 "andl $" mask ",%%esi\n\t"\
                 "movl %%esi, %%edi\n\t"
#define xor_reg(reg,c,mask) \
             "movl %%" reg ",%%esi\n\t"\
             "xorl %" c ",%%esi\n\t"\
             "andl $" mask ",%%esi\n\t"\
             "xorl %%esi, %%edi\n\t"

#define rotate_regs\
 asm("ror $8,%%eax\n\t"\
     "ror $8,%%ebx\n\t"\
     "ror $8,%%ecx\n\t"\
     "ror $8,%%edx\n\t"\
     :\
     :\
     :"%eax","%ebx","%ecx","%edx")

/* can improve this write_back_to_state listed even when p, i are variables
   and not constants. Write now, if these are variables, there is no index register
   to put them into. So, if they are variables. They can be computed, before hand
   into a temporary variable, and then loaded into %esi, and then
   the final movl edi, %0 can be movl edi, %0(%esi).
*/

#define write_back_to_state_listed(p,i,a,b,c,ddd) \
 asm(load_eax("255") \
     xor_reg("ebx","2","65280") \
     xor_reg("ecx","3","16711680")  \
     xor_reg("edx","4","4278190080")\
     "movl %%edi,%0"\
     :"=m" (state[off(p,i)])\
     :"m" (a), "m" (b), "m" (c), "m" (ddd)\
     :"%esi","%edi","%eax","%ebx","%ecx","%edx");

#define write_back_to_state_listed0(p,i) \
 write_back_to_state_listed(p,i, b0,b1,b2,b3); rotate_regs

#define write_back_to_state_listed1(p,i) \
 write_back_to_state_listed(p,i, b1,b2,b3,b0); rotate_regs

#define write_back_to_state_listed2(p,i) \
 write_back_to_state_listed(p,i, b2,b3,b0,b1); rotate_regs

#define write_back_to_state_listed3(p,i) \
 write_back_to_state_listed(p,i, b3,b0,b1,b2)

#define write_back_to_state_indexed_ASM(p,i) \
    write_back_to_state_listed##i(p,i)

/*************ELSE**********/

#define rotate_regs_OPT \
row(0)=brot(row(0),24);\
row(1)=brot(row(1),24);\
row(2)=brot(row(2),24);\
row(3)=brot(row(3),24);

#define write_back_to_state_listed_OPT(p,i,a,b,c,d) \
  state[off(p,i)]= ((row(0)^column(a)) & 0xff) ^   \
                   ((row(1)^column(b)) & 0xff00) ^ \
                   ((row(2)^column(c)) & 0xff0000) ^ \
                   ((row(3)^column(d)) & 0xff000000)


#define write_back_to_state_listed_OPT_0(p,i) \
 write_back_to_state_listed_OPT(p,i, 0,1,2,3); rotate_regs_OPT

#define write_back_to_state_listed_OPT_1(p,i) \
 write_back_to_state_listed_OPT(p,i, 1,2,3,0); rotate_regs_OPT

#define write_back_to_state_listed_OPT_2(p,i) \
 write_back_to_state_listed_OPT(p,i, 2,3,0,1); rotate_regs_OPT

#define write_back_to_state_listed_OPT_3(p,i) \
 write_back_to_state_listed_OPT(p,i, 3,0,1,2)

#define write_back_to_state_indexed_OPT(p,i) \
    write_back_to_state_listed_OPT_##i(p,i)

#define init_row_and_col_OPT    column(0)=column(1)=column(2)=column(3)=0; row(0)= row(1)= row(2)=row(3) =0;

#define xor_col_to_rows_ASM print_rc_ASM;
#define xor_col_to_rows_OPT print_rc_OPT;

/**************END************/

#define write_back_to_state(p,type) \
write_back_to_state_indexed##type(p,0);write_back_to_state_indexed##type(p,1);\
write_back_to_state_indexed##type(p,2);write_back_to_state_indexed##type(p,3);

#define super_mix(ptr,type)\
 init_row_and_col##type;  \
 xor_row_and_col(ptr,type); \
 xor_col_to_rows##type; \
 write_back_to_state(ptr,type);


#define sub_round(ptr,type) \
             column_mix(ptr);   super_mix(ptr,type)

#define super_mix_convert(p,type) super_mix(p,type)

#define ror(state,n) ptr = ((ptr-n+FUGUE_SIZE) % FUGUE_SIZE)

#define cross_mix(p,c1,c2) \
       state[off(p,4)] ^=state[off(p,0)]; \
       state[off(p,c1)] ^=state[off(p,0)];\
       state[off(p,c2)] ^=state[off(p,0)];


#define final_sub_round(p,c1,c2,n) \
        cross_mix(p,c1,c2);\
        ror(state,n); \
           super_mix_convert(p,EXTRA_ROUND_IMPL_TYPE)

#define final_round2(ptr,num,c)\
    for (j=0; j<c; j++)      \
         { final_sub_round(ptr, 12,24,12); \
           final_sub_round(ptr, 13,24,12); \
           final_sub_round(ptr, 13,25,11);} \
           cross_mix(ptr,12,24);

#define final_round1(ptr,num,c) \
    for (j=0; j<c; j++)  \
      {sub_round(((-3)*(kkk*((num+j)%4)+1)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(kkk*((num+j)%4)+2)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(kkk*((num+j)%4)+3)),EXTRA_ROUND_IMPL_TYPE); \
      }

#define empty_pretty_print(s,a,b,c)

#define round(j,num,type) \
         tix(((-3)*kkk*num),j); \
          empty_pretty_print(state, ptr, 36, "after tix");\
           sub_round(((-3)*(kkk*num+1)),type); \
            empty_update(ptr); empty_pretty_print(state, ptr, 36, "after subround 1");\
             sub_round(((-3)*(kkk*num+2)),type);\
              empty_update(ptr);empty_pretty_print(state, ptr, 36, "after subround 2");\
                sub_round(((-3)*(kkk*num+3)),type);\
                 empty_update(ptr);empty_pretty_print(state, ptr, 36, "after subround 2");\



void pretty_print_384 (uint_32t * state, int ptr, int n, char * st)
{
    int i,j;
    uint_32t tmp;

    printf("%s\n", st);
    for (i=0; i<n; i++)
    {
        tmp = state[off(ptr,i)];
        for (j=0; j<4; j++)
        {
            printf("%02x", (uint_8t)tmp);
            tmp = tmp>>8;
        }
        printf(" ");
    }
    printf("\n\n");
}

void print_byte_stream_384 ( char * str, uint_32t * output, int len)
{
    int i,j;

    printf("%s\n",str);
    for (i=0; i<len; i++)
    {
        for (j=0; j<4; j++)
        printf("%02x", ((char*) output)[4*i+j] & 0xff);
        printf(" ");
    }
    printf("\n\n");
}

#define empty_update(ptr)

#define update(ptr) \
ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;

int fugue_update_384 (hashState *hs, const char *in, uint_64t len)
{
    uint_32t state[FUGUE_SIZE];
#ifdef CONFIG_ASM_INLINE
    uint_32t b0,b1,b2,b3;
#else
    uint_32t row[4];
    uint_32t col[4];
#endif
    uint_32t tmp;
    short len_mod;
    uint_64t len_div;
    uint_64t j,j1,j2;
    short ptr = hs->Base;

    j1 = (4-hs->Base/9)%4;  // this is the left over sliding window pointer (divided by 4*3)

    for (j=0; j < FUGUE_SIZE; j++) state[j] = hs->State[j].d;

    j=0;
    if (j1 && len)
    {
	if  (j1 <=1)         { round(j,1,IMPL_TYPE); j++; len--; ptr -=9; }
	if ((j1 <=2) && len) { round(j,2,IMPL_TYPE); j++; len--; ptr -=9; }
	if ((j1 <=3) && len) { round(j,3,IMPL_TYPE); j++; len--; ptr -=9; }
    }

    len_mod = (len)%4;
    len_div = (len)/4;

    for (j2=0; j2<len_div;j2++)
    {
        round(j,0,IMPL_TYPE); j++;
        round(j,1,IMPL_TYPE); j++;
        round(j,2,IMPL_TYPE); j++;
        round(j,3,IMPL_TYPE); j++;
    }

    if (len_mod >=1)
    {
        round(j,0,IMPL_TYPE); j++;
        if (len_mod >=2)
        {
            round(j,1,IMPL_TYPE); j++;
	    if (len_mod >=3)
	    {
		round(j,2,IMPL_TYPE); j++;
	    }
        }
    }


    for (j=0; j < FUGUE_SIZE; j++) hs->State[j].d = state[j];
    hs->Base = (ptr-9*len_mod + 2*FUGUE_SIZE) % FUGUE_SIZE;

    return 0;
}

int fugue_final_384 (hashState *hs, char *out)
{
    uint_32t *state = (uint_32t *)hs->State;
    uint_32t row[4];
    uint_32t col[4];
    uint_32t tmp;
    uint_64t j,j1 = (4-hs->Base/9)%4;
    short ptr = hs->Base;

    final_round1(ptr,j1,6);
    ptr = ptr - ((6*3*3)%FUGUE_SIZE);
    final_round2(ptr,j1,13);

    for (j=0; j<4; j++)
    {
        ((uint_32t *)out)[j]   = state[off(ptr,1+j)];
        ((uint_32t *)out)[4+j] = state[off(ptr,12+j)];
        ((uint_32t *)out)[8+j] = state[off(ptr,24+j)];
    }

    return 0;
}

/////////////////////// 512

// set the IMPL_TYPE to _ASM for ASM_INLINE
// eet it to _OPT for C OPTIMIZED
//same rules hold for EXTRA_ROUND_IMPL_TYPE, though they cant be set to _ASM...yet !
// ASS_INLINE will be used in some macro names as a suiffix _ASM
// and   C OPTIMIZED as _OPT

/* Uncomment the desired default configuration */
#if !defined(CONFIG_ASM_INLINE) && !defined(CONFIG_C_OPTIMIZED)
//#define CONFIG_ASM_INLINE
#define CONFIG_C_OPTIMIZED
#endif

#ifdef CONFIG_ASM_INLINE
#define IMPL_TYPE  _ASM
#endif

#ifdef CONFIG_C_OPTIMIZED
#define IMPL_TYPE  _OPT
#endif

#define EXTRA_ROUND_IMPL_TYPE  _OPT

#define kkk 4
#define FUGUE_SIZE 36
#define FUGUE_FAR1 24
#define FUGUE_FAR2 27
#define FUGUE_FAR3 30


#define column(i) col[i]
#define row(i) row[i]


#define vector2words(rc,i) \
bytes2word(bval(rc[0],i),  bval(rc[1],i),   bval(rc[2],i),   bval(rc[3],i))

#define off(p,n) ((n+p+FUGUE_SIZE) % FUGUE_SIZE)

#define tix(p,j) \
  state[off(p,22)] ^= state[off(p,0)];  state[off(p,0)] = ((uint_32t *)in)[j];  \
  state[off(p,8)]  ^= state[off(p,0)];  state[off(p,1)] ^= state[off(p,FUGUE_FAR1)];\
  state[off(p,4)] ^= state[off(p,FUGUE_FAR2)];\
  state[off(p,7)] ^= state[off(p,FUGUE_FAR3)]

#define column_mix(p)\
  state[off(p,0)] ^= state[off(p,4)];  state[off(p,1)] ^= state[off(p,5)];  state[off(p,2)] ^= state[off(p,6)];\
  state[off(p,18)] ^= state[off(p,4)];  state[off(p,19)] ^= state[off(p,5)];  state[off(p,20)] ^= state[off(p,6)]

#define print_rc_ASM
/*printf("In type 0\n");
 printf("col: %x %x %x %x \n", b0,b1,b2,b3);*/

#define print_rc_OPT
/*printf("In type 2\n");
printf("col: %x %x %x %x \n", column(0),column(1),column(2),column(3));\
printf("row: %x %x %x %x \n", row(0), row(1), row(2), row(3));*/

#define shift_edi0
#define shift_edi1 "shrl $8,%%edi\n\t"
#define shift_edi2 "shrl $16,%%edi\n\t"
#define shift_edi3 "shrl $24,%%edi\n\t"

#define asm_bval4(y,c,t)\
        asm ("movl %1, %0\n\t"\
             shift_edi##c \
             "andl $255, %%edi"\
             :"=D" (t)       \
             :"m" (y)   \
             :"memory"       \
             );

#define asm_set_template(c,t) \
        asm ("movl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );
#define asm_xor_template(c,t) \
        asm ("xorl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );

#define asm_xor_xor_template(reg,c,t) \
        asm ("xorl %%" reg ", %%esi\n\t"\
             "xorl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_set_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "movl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_xor_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_xor_set_template(reg,c,t) \
        asm ("movl %0, %%esi \n\t"\
             "xorl %%esi, %%" reg "\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );
#define put_back_to_b_ASM(i) \
asm (        "movl %%esi,%0\n\t"\
             :"=m" (b##i)\
             :\
             :"memory", "%esi"\
             );

#define put_back_to_b_OPT(i)  print_rc_OPT;

/***if define (ASS_INLINE) *****/
#define table_index_ASM(p,i,k)  asm_bval4(state[off(p,i)],k,tmp)
#define asm_template0(xs_type,k,i,tmp)  xs_type("eax",k,tmp);
#define asm_template1(xs_type,k,i,tmp)  xs_type("ebx",k,tmp);
#define asm_template2(xs_type,k,i,tmp)  xs_type("ecx",k,tmp);
#define asm_template3(xs_type,k,i,tmp)  xs_type("edx",k,tmp);

#define xor_col_ASM(k,i) asm_xor_template(k,tmp);
#define set_col_ASM(k,i) asm_set_template(k,tmp);

#define xor_row_xor_col_ASM(k,i) asm_template##k(asm_xor_xor_template, k,i,tmp);
#define xor_row_set_col_ASM(k,i) asm_template##k(asm_xor_set_template, k,i,tmp);
#define set_row_xor_col_ASM(k,i) asm_template##k(asm_set_xor_template, k,i,tmp);
#define set_row_set_col_ASM(k,i) asm_template##k(asm_set_set_template, k,i,tmp);
/********ELSE*********/
#define table_index_OPT(p,i,k) tmp = aes_style_table[k][bval(state[off(p,i)],k)]
#define xor_col_OPT(k,i) column(i) ^= tmp;
#define set_col_OPT(k,i) column(i) = tmp;
#define xor_row_xor_col_OPT(k,i) row(k) ^= tmp; column(i) ^= tmp;
#define xor_row_set_col_OPT(k,i) row(k) ^= tmp; column(i) = tmp;
#define set_row_xor_col_OPT(k,i) row(k) = tmp;  column(i) ^= tmp;
#define set_row_set_col_OPT(k,i) row(k) = tmp;  column(i) = tmp;
/*******ENDIF *******/


#define xor_short_form_i_eq_k(p,i,k,type)       \
table_index##type(p,i,k); xor_col##type(k,i)

#define xor_short_form_i_eq_k_col_first(p,i,k,type)     \
table_index##type(p,i,k);  set_col##type(k,i)

#define xor_short_form_i_neq_k(p,i,k,type)  \
table_index##type(p,i,k);   xor_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_col_first(p,i,k,type)  \
table_index##type(p,i,k); xor_row_set_col##type(k,i)

#define xor_short_form_i_neq_k_row_first(p,i,k,type)  \
table_index##type(p,i,k);  set_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)  \
table_index##type(p,i,k);   set_row_set_col##type(k,i)

#define xor_rc_0_0(p,i,k,type) xor_short_form_i_eq_k_col_first(p,i,k,type)
#define xor_rc_0_1(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_0_2(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_0_3(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)

#define xor_rc_1_0(p,i,k,type)  xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)
#define xor_rc_1_1(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_1_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_1_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_2_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_2_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_2_2(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_2_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_3_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_3_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_3_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_3_3(p,i,k,type) xor_short_form_i_eq_k(p,i,k,type)

#define xor_rc_outer(p,i,type)\
   xor_rc_##i##_0(p,i,0,type);\
   xor_rc_##i##_1(p,i,1,type); xor_rc_##i##_2(p,i,2,type); xor_rc_##i##_3(p,i,3,type)

#define xor_row_and_col(p,type) \
     xor_rc_outer(p,0,type); put_back_to_b##type(0)\
     xor_rc_outer(p,1,type); put_back_to_b##type(1)\
     xor_rc_outer(p,2,type); put_back_to_b##type(2) \
     xor_rc_outer(p,3,type); put_back_to_b##type(3)




/******if defined (ASS_INLINE) *******/

#define init_row_and_col_ASM

#define load_eax(mask) "movl %%eax,%%esi\n\t"\
                 "xorl %1,%%esi\n\t"\
                 "andl $" mask ",%%esi\n\t"\
                 "movl %%esi, %%edi\n\t"
#define xor_reg(reg,c,mask) \
             "movl %%" reg ",%%esi\n\t"\
             "xorl %" c ",%%esi\n\t"\
             "andl $" mask ",%%esi\n\t"\
             "xorl %%esi, %%edi\n\t"

#define rotate_regs\
 asm("ror $8,%%eax\n\t"\
     "ror $8,%%ebx\n\t"\
     "ror $8,%%ecx\n\t"\
     "ror $8,%%edx\n\t"\
     :\
     :\
     :"%eax","%ebx","%ecx","%edx")

/* can improve this write_back_to_state listed even when p, i are variables
   and not constants. Write now, if these are variables, there is no index register
   to put them into. So, if they are variables. They can be computed, before hand
   into a temporary variable, and then loaded into %esi, and then
   the final movl edi, %0 can be movl edi, %0(%esi).
*/

#define write_back_to_state_listed(p,i,a,b,c,ddd) \
 asm(load_eax("255") \
     xor_reg("ebx","2","65280") \
     xor_reg("ecx","3","16711680")  \
     xor_reg("edx","4","4278190080")\
     "movl %%edi,%0"\
     :"=m" (state[off(p,i)])\
     :"m" (a), "m" (b), "m" (c), "m" (ddd)\
     :"%esi","%edi","%eax","%ebx","%ecx","%edx");

#define write_back_to_state_listed0(p,i) \
 write_back_to_state_listed(p,i, b0,b1,b2,b3); rotate_regs

#define write_back_to_state_listed1(p,i) \
 write_back_to_state_listed(p,i, b1,b2,b3,b0); rotate_regs

#define write_back_to_state_listed2(p,i) \
 write_back_to_state_listed(p,i, b2,b3,b0,b1); rotate_regs

#define write_back_to_state_listed3(p,i) \
 write_back_to_state_listed(p,i, b3,b0,b1,b2)

#define write_back_to_state_indexed_ASM(p,i) \
    write_back_to_state_listed##i(p,i)

/*************ELSE**********/

#define rotate_regs_OPT \
row(0)=brot(row(0),24);\
row(1)=brot(row(1),24);\
row(2)=brot(row(2),24);\
row(3)=brot(row(3),24);

#define write_back_to_state_listed_OPT(p,i,a,b,c,d) \
  state[off(p,i)]= ((row(0)^column(a)) & 0xff) ^   \
                   ((row(1)^column(b)) & 0xff00) ^ \
                   ((row(2)^column(c)) & 0xff0000) ^ \
                   ((row(3)^column(d)) & 0xff000000)


#define write_back_to_state_listed_OPT_0(p,i) \
 write_back_to_state_listed_OPT(p,i, 0,1,2,3); rotate_regs_OPT

#define write_back_to_state_listed_OPT_1(p,i) \
 write_back_to_state_listed_OPT(p,i, 1,2,3,0); rotate_regs_OPT

#define write_back_to_state_listed_OPT_2(p,i) \
 write_back_to_state_listed_OPT(p,i, 2,3,0,1); rotate_regs_OPT

#define write_back_to_state_listed_OPT_3(p,i) \
 write_back_to_state_listed_OPT(p,i, 3,0,1,2)

#define write_back_to_state_indexed_OPT(p,i) \
    write_back_to_state_listed_OPT_##i(p,i)

#define init_row_and_col_OPT    column(0)=column(1)=column(2)=column(3)=0; row(0)= row(1)= row(2)=row(3) =0;

#define xor_col_to_rows_ASM print_rc_ASM;
#define xor_col_to_rows_OPT print_rc_OPT;

/**************END************/

#define write_back_to_state(p,type) \
write_back_to_state_indexed##type(p,0);write_back_to_state_indexed##type(p,1);\
write_back_to_state_indexed##type(p,2);write_back_to_state_indexed##type(p,3);

#define super_mix(ptr,type)\
 init_row_and_col##type;  \
 xor_row_and_col(ptr,type); \
 xor_col_to_rows##type; \
 write_back_to_state(ptr,type);


#define sub_round(ptr,type) \
             column_mix(ptr);   super_mix(ptr,type)

#define super_mix_convert(p,type) super_mix(p,type)

#define ror(state,n) ptr = ((ptr-n+FUGUE_SIZE) % FUGUE_SIZE)

#define cross_mix(p,c1,c2,c3) \
       state[off(p,4)] ^=state[off(p,0)]; \
       state[off(p,c1)] ^=state[off(p,0)];\
       state[off(p,c2)] ^=state[off(p,0)];\
       state[off(p,c3)] ^=state[off(p,0)];

#define final_sub_round(p,c1,c2,c3,n) \
        cross_mix(p,c1,c2,c3);\
        ror(state,n); \
           super_mix_convert(p,EXTRA_ROUND_IMPL_TYPE)

#define final_round2(ptr,num,c)\
    for (j=0; j<c; j++)      \
         { final_sub_round(ptr, 9,18,27,9); \
           final_sub_round(ptr, 10,18,27,9); \
           final_sub_round(ptr, 10,19,27,9); \
           final_sub_round(ptr, 10,19,28,8);} \
           cross_mix(ptr,9,18,27);

#define final_round1(ptr,num,c) \
    for (j=0; j<c; j++)  \
      {sub_round(((-3)*(kkk*((num+j)%3)+1)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(kkk*((num+j)%3)+2)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(kkk*((num+j)%3)+3)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(kkk*((num+j)%3)+4)),EXTRA_ROUND_IMPL_TYPE); \
      }

#define empty_pretty_print(s,a,b,c)

#define round(j,num,type) \
         tix(((-3)*kkk*num),j); \
          empty_pretty_print(state, ptr, 36, "after tix");\
           sub_round(((-3)*(kkk*num+1)),type); \
            empty_update(ptr); empty_pretty_print(state, ptr, 36, "after subround 1");\
             sub_round(((-3)*(kkk*num+2)),type);\
              empty_update(ptr);empty_pretty_print(state, ptr, 36, "after subround 2");\
                sub_round(((-3)*(kkk*num+3)),type);\
                 empty_update(ptr);empty_pretty_print(state, ptr, 36, "after subround 2");\
                  sub_round(((-3)*(kkk*num+4)),type);\
                   empty_update(ptr);empty_pretty_print(state, ptr, 36, "after subround 2");


void pretty_print_512 (uint_32t * state, int ptr, int n, char * st)
{
    int i,j;
    uint_32t tmp;

    printf("%s\n", st);
    for (i=0; i<n; i++)
    {
        tmp = state[off(ptr,i)];
        for (j=0; j<4; j++)
        {
            printf("%02x", (uint_8t)tmp);
            tmp = tmp>>8;
        }
        printf(" ");
    }
    printf("\n\n");
}

void print_byte_stream_512 ( char * str, uint_32t * output, int len)
{
    int i,j;

    printf("%s\n",str);
    for (i=0; i<len; i++)
    {
        for (j=0; j<4; j++)
        printf("%02x", ((char*) output)[4*i+j] & 0xff);
        printf(" ");
    }
    printf("\n\n");
}

#define empty_update(ptr)

#define update(ptr) \
ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;

int fugue_update_512 (hashState *hs, const char *in, uint_64t len)
{
    uint_32t state[FUGUE_SIZE];
#ifdef CONFIG_ASM_INLINE
    uint_32t b0,b1,b2,b3;
#else
    uint_32t row[4];
    uint_32t col[4];
#endif
    uint_32t tmp;
    short len_mod;
    uint_64t len_div;
    uint_64t j,j1,j2;
    short ptr = hs->Base;

    j1 = (3-hs->Base/12)%3;  // this is the left over sliding window pointer (divided by 4*3)

    for (j=0; j < FUGUE_SIZE; j++) state[j] = hs->State[j].d;




    j=0;
    if (j1 && len)
    {
	if  (j1 <=1)         { round(j,1,IMPL_TYPE); j++; len--; ptr -=12; }
	if ((j1 <=2) && len) { round(j,2,IMPL_TYPE); j++; len--; ptr -=12; }
    }

    len_mod = (len)%3;
    len_div = (len)/3;

    for (j2=0; j2<len_div;j2++)
    {
        round(j,0,IMPL_TYPE); j++;
        round(j,1,IMPL_TYPE); j++;
        round(j,2,IMPL_TYPE); j++;
    }

    if (len_mod >=1)
    {
        round(j,0,IMPL_TYPE); j++;
        if (len_mod >=2)
        {
            round(j,1,IMPL_TYPE); j++;
        }
    }


    for (j=0; j < FUGUE_SIZE; j++) hs->State[j].d = state[j];
    hs->Base = (ptr-12*len_mod + 2*FUGUE_SIZE) % FUGUE_SIZE;

    return 0;
}

int fugue_final_512 (hashState *hs, char *out)
{
    uint_32t *state = (uint_32t *)hs->State;
    uint_32t row[4];
    uint_32t col[4];
    uint_32t tmp;
    uint_64t j,j1 = (3-hs->Base/12)%3;
    short ptr = hs->Base;

    final_round1(ptr,j1,8);
    ptr = ptr - ((8*4*3)%FUGUE_SIZE);
    final_round2(ptr,j1,13);

    for (j=0; j<4; j++)
    {
        ((uint_32t *)out)[j]    = state[off(ptr,1+j)];
        ((uint_32t *)out)[4+j]  = state[off(ptr,9+j)];
        ((uint_32t *)out)[8+j]  = state[off(ptr,18+j)];
        ((uint_32t *)out)[12+j] = state[off(ptr,27+j)];
    }


    return 0;
}
}