/*
 * Implementation of the Shabal hash function (header file). This header
 * is used for both the "reference" and the "optimized" implementations.
 *
 * (c) 2008 SAPHIR project. This software is provided 'as-is', without
 * any express or implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to no restriction.
 *
 * Technical remarks and questions can be addressed to:
 * <thomas.pornin@cryptolog.com>
 */

#pragma once

namespace ShabalOrg
{
typedef unsigned int shabal_word32;

#define SHABAL_BLOCK_SIZE   16
#ifndef SHABAL_PARAM_R
#define SHABAL_PARAM_R      12
#endif

/*
 * Input messages and hash outputs are sequences of bits, stored
 * in arrays of C bytes.
 */
typedef unsigned char BitSequence;

typedef unsigned long long DataLength;

/*
 * Status codes.
 */
typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	BAD_HASHBITLEN = 2
} HashReturn;

/*
 * State structure for Shabal. A structure instance holds the running
 * state for a hash computation. The function implementation is
 * thread-safe and reentrant, as long as the different has computations
 * use distinct instances of this structure. State structures are
 * meant to be allocated by the caller; allocation can be performed
 * in any suitable memory area, including local variables, global
 * variables, and the heap. The structure contains no pointer value
 * and thus can be moved, duplicated (with memcpy()) or serialized
 * at will. In particular, making a copy of a structure instance takes
 * a "snapshot" of the hash computation at that point, which can be
 * resumed later on with alternate subsequence input data.
 *
 * The "hashbitlen" field contains the intended output length, in bits.
 * It is initialized in the Init() function. The other fields are private
 * and should not be accessed externally.
 */
typedef struct {
	BitSequence buffer[SHABAL_BLOCK_SIZE * 4];
	size_t buffer_ptr;
	unsigned last_byte_significant_bits;
	int hashbitlen;
	shabal_word32 A[SHABAL_PARAM_R];
	shabal_word32 B[SHABAL_BLOCK_SIZE];
	shabal_word32 C[SHABAL_BLOCK_SIZE];
	shabal_word32 Whigh, Wlow;
} hashState;

/*
 * Initialize a state structure for a new hash computation. The intended
 * output length (in bits) is provided as the "hashbitlen" parameter;
 * supported output bit lengths are 192, 224, 256, 384 and 512. The
 * "state" pointer should reference a state structure allocated with
 * the proper alignment.
 *
 * Returned value is SUCCESS (0) on success, or a non-zero error code
 * otherwise (namely BAD_HASHBITLEN if the specified "hashbitlen" is
 * not one of the supported output lengths). On failure, the structure
 * contents are indeterminate.
 */
HashReturn Init(hashState *state, int hashbitlen);

/*
 * Continue a running hash computation. The state structure is provided.
 * The additional input data is a sequence of bits; the "data"
 * parameter points to the start of that sequence, and the "databitlen"
 * contains the sequence length, expressed in bits.
 *
 * The bits within a byte are ordered from most significant to least
 * significant. The input bit sequence MUST begin with the most
 * significant bit of the first byte pointed to by "data". The bit
 * sequence length MUST be a multiple of 8 if this call is not the
 * last Update() call performed for this hash computation. In other
 * words, the input message chunks MUST consist of entire and aligned
 * bytes, except for the very last input byte, which may be "partial".
 *
 * Returned value is SUCCESS (0) on success, or a non-zero error code
 * otherwise.
 */
HashReturn Update(hashState *state,
	const BitSequence *data, DataLength databitlen);

/*
 * Terminate a running hash computation. The state structure is provided.
 * The hash output is written out in the buffer pointed to by "hashval";
 * the hash output length was specified when the structure was initialized.
 * The same bit ordering conventions than for input data are used in the
 * hash data output; since all supported lengths are multiple of 8, the
 * hash output necessarily consists of an integral number of bytes.
 *
 * After this call, the state structure contents are indeterminate. If
 * the structure is to be used for a new hash computation, then it
 * shall be initialized again with Init().
 *
 * Returned value is SUCCESS (0) on success, or a non-zero error code
 * otherwise.
 */
HashReturn Final(hashState *state, BitSequence *hashval);

/*
 * Perform a complete hash computation. This combines calls to Init(),
 * Update() and Final(), with a state structure which this function
 * allocates and releases itself. This function can thus be called
 * independantly of any other runnimg hash computation. The parameters
 * are:
 * - hashbitlen    the output length, in bits (192, 224, 256, 384 or 512)
 * - data          pointer to the input message start
 * - databitlen    input message length, in bits
 * - hashval       pointer to the buffer which receives the hash output
 *
 * Returned value is SUCCESS (0) on success, or a non-zero error code
 * otherwise.
 */
HashReturn Hash(int hashbitlen, const BitSequence *data,
	DataLength databitlen, BitSequence *hashval);

}