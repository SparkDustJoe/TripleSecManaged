﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using BC = Org.BouncyCastle;
using SSC = System.Security.Cryptography;

namespace TripleSecManaged
{
    public partial class V3
    {
        public const string TRIPLESEC_MAGICPLUSVERSION3_HEXBYTES = "1c94d7de00000003";
        public static readonly byte[] TRIPLESEC_MAGICPLUSVERSION3_BYTES = { 0x1c, 0x94, 0xd7, 0xde, 0, 0, 0, 3 };
        public const short TRIPLESEC_MAGICPLUSVERSION3_BYTELENGTH = 8;
        private const int SALT_START = 8;
        private const int SALT_LEN = 16;
        private const int HMACSHA512_START = SALT_START + SALT_LEN;
        private const int HMACSHA512_LEN = 64;
        private const int HMACKECCAK512_START = HMACSHA512_START + HMACSHA512_LEN;
        private const int HMACKECCAK512_LEN = 64;
        private const int AES_IV_START = HMACKECCAK512_START + HMACKECCAK512_LEN;
        private const int AES_IV_LEN = 16;
        private const int CIPHERTEXT_START = AES_IV_START + AES_IV_LEN;
        // ['blockcontents' 'LENGTH in bytes']
        // [magicbytes 4][version 4][salt 16][hmacsha512 64][hmackeccak512 64][aesiv 16][ciphertext (variable length)]
        // embedded in the ciphertext are an additional 16 bytes for the twofish IV, and 24 for the xsalsa20 IV

        internal static void ProcessArray(BC.Crypto.IBlockCipher Cipher, byte[] Data, byte[] Output)
        {
            if (Data == null || Output == null || Cipher == null)
                throw new InvalidOperationException("Null parameters not allowed.");
            int blockSize = Cipher.GetBlockSize();
            byte[] XOR_ME = new byte[Cipher.GetBlockSize()];
            for (int i = 0; i < Data.Length; i += blockSize)
            {
                int count = Math.Min(blockSize, Data.Length - i);
                Buffer.BlockCopy(Data, i, XOR_ME, 0, count);
                Cipher.ProcessBlock(XOR_ME, 0, XOR_ME, 0);
                Buffer.BlockCopy(XOR_ME, 0, Output, i, count);
            }
        }

        internal static void ProcessArray(BC.Crypto.IBlockCipher Cipher, byte[] Data, int DataOffset, byte[] Output, int OutputOffset, int Length)
        {
            if (Data == null || Output == null || Cipher == null)
                throw new InvalidOperationException("Null parameters not allowed.");
            if (Length + DataOffset > Data.Length)
                throw new ArgumentOutOfRangeException("Length + DataOffset > Data.Length");
            if (DataOffset >= Data.Length)
                throw new ArgumentOutOfRangeException("DataOffset > Data.Length");
            if (Length + OutputOffset > Output.Length)
                throw new ArgumentOutOfRangeException("Length + OutputOffset > Output.Length");
            if (OutputOffset >= Output.Length)
                throw new ArgumentOutOfRangeException("OutputOffset > Output.Length");
            int blockSize = Cipher.GetBlockSize();
            byte[] XOR_ME = new byte[Cipher.GetBlockSize()];
            for (int i = 0; i < Length && i < Data.Length; i += blockSize)
            {
                int count = Math.Min(blockSize, Length - i);
                Buffer.BlockCopy(Data, i + DataOffset, XOR_ME, 0, count);
                Cipher.ProcessBlock(XOR_ME, 0, XOR_ME, 0);
                Buffer.BlockCopy(XOR_ME, 0, Output, i + OutputOffset, count);
            }
        }

        // HASHLIB REMOVED ------------!

        /// <summary>
        /// Encrypt an array of bytes using the TripleSec protocol with the supplied passphrase.
        /// </summary>
        /// <param name="Source"></param>
        /// <param name="Passphrase"></param>
        /// <returns></returns>
        public static byte[] Encrypt(byte[] Source, byte[] Passphrase)
        {
            if (Source == null || Source.Length == 0)
                throw new ArgumentNullException("Source");
            if (Passphrase == null || Passphrase.Length == 0)
                throw new ArgumentNullException("Passphrase");
#if DEBUG
#pragma warning disable 618 // we know in DEBUG mode this will throw an Obsolete warning, this is a legitimate use of this function, so supress it
#endif
            RNGV3 rng = new RNGV3(); // get a new salt and IV's
            KeysV3 keys = new KeysV3(Passphrase, rng); // build the internal keys based on the Passphrase and generated salt
            return Encrypt(Source, keys, rng);
#if DEBUG
#pragma warning restore 618
#endif
        }

#if DEBUG
        /// <summary>
        /// Encrypt an array of bytes using the TripleSec protocol with the supplied passphrase.  USED FOR DEBUGGING PURPOSES ONLY!!!
        /// </summary>
        /// <param name="Source">Source array of bytes to process.</param>
        /// <param name="Keys">The KeysV3 object initialized with the proper keys.</param>
        /// <param name="IVs">The RNGV3 object initialized with the proper initialization arrays.</param>
        /// <returns>The encrypted array.</returns>
        [Obsolete("USED FOR DEBUGGING PURPOSES ONLY! Clear DEBUG flag during build (or use Release build configuration) to remove this public function definiation from final library.")]
        public static byte[] Encrypt(byte[] Source, KeysV3 Keys, RNGV3 IVs)
#else
        /// <summary>
        /// Encrypt an array of bytes using the TripleSec protocol with the supplied passphrase.  Internal function, not intended for use by an end coder.
        /// </summary>
        /// <param name="Source">Source array of bytes to process.</param>
        /// <param name="Keys">The KeysV3 object initialized with the proper keys.</param>
        /// <param name="IVs">The RNGV3 object initialized with the proper initialization arrays.</param>
        /// <returns>The encrypted array.</returns>
        internal static byte[] Encrypt(byte[] Source, KeysV3 Keys, RNGV3 IVs)
#endif
        {
            if (Source == null || Source.Length == 0)
                throw new ArgumentNullException("Source");
            if (Keys == null || !Keys.IsInitialized || IVs == null || !IVs.IsInitialized)
                throw new InvalidOperationException("Keys and IVs objects must not be null (and both must be initialized)");
            
            // the added IV's and the signatures and the header always add up to the same length added to the original message length
            //  if we're going to run out of memory, it will be here, or shortly thereafter
            byte[] OUT_BUFFER = new byte[Source.Length + 208];
            //standard header stuff
            // All the header pieces are now copied by the built-in SHA512 method TransformBlock

            //XSalsa20 is up first
            byte[] interim = Chaos.NaCl.XSalsa20.Process(Source, Keys.XSalsa20_KEY, IVs.XSalsa20_IV);
            byte[] interim2 = new byte[interim.Length + IVs.XSalsa20_IV.Length]; // IV included here as it is part of the input to the next process
            Buffer.BlockCopy(IVs.XSalsa20_IV, 0, interim2, 0, IVs.XSalsa20_IV.Length);
            Buffer.BlockCopy(interim, 0, interim2, IVs.XSalsa20_IV.Length, interim.Length);
            
            //Twofish is up next
            interim.Wipe();  // DON'T LEAK!
            interim = new byte[interim2.Length + IVs.Twofish_IV.Length]; // IV included here as it is part of the input to the next process
            Buffer.BlockCopy(IVs.Twofish_IV, 0, interim, 0, IVs.Twofish_IV.Length);
            BC.Crypto.Modes.SicBlockCipher tf = new BC.Crypto.Modes.SicBlockCipher(new BC.Crypto.Engines.TwofishEngine());
            tf.Init(true, new BC.Crypto.Parameters.ParametersWithIV(new BC.Crypto.Parameters.KeyParameter(Keys.Twofish_KEY), IVs.Twofish_IV));
            ProcessArray(tf, interim2, interim2); // process array in-place
            Buffer.BlockCopy(interim2, 0, interim, IVs.Twofish_IV.Length, interim2.Length);
            tf.Reset(); tf = null;
            
            //AES is last
            interim2.Wipe(); // DON'T LEAK!
            BC.Crypto.Modes.SicBlockCipher aes = new BC.Crypto.Modes.SicBlockCipher(new BC.Crypto.Engines.AesEngine());
            aes.Init(true, new BC.Crypto.Parameters.ParametersWithIV(new BC.Crypto.Parameters.KeyParameter(Keys.AES_KEY), IVs.AES_IV));
            ProcessArray(aes, interim, interim); // process array in-place
            //the HMACSHA512 process will copy the data to the output as it hashes
            aes.Reset();
            aes = null;

            HMACSHA512 sha512 = new HMACSHA512(Keys.HMACSHA512_KEY);
            sha512.TransformBlock(TRIPLESEC_MAGICPLUSVERSION3_BYTES, 0, TRIPLESEC_MAGICPLUSVERSION3_BYTELENGTH, OUT_BUFFER, 0);
            sha512.TransformBlock(Keys.Salt, 0, Keys.Salt.Length, OUT_BUFFER, SALT_START);
            sha512.TransformBlock(IVs.AES_IV, 0, IVs.AES_IV.Length, OUT_BUFFER, AES_IV_START);
            sha512.TransformBlock(interim, 0, interim.Length, OUT_BUFFER, CIPHERTEXT_START);
            sha512.TransformFinalBlock(interim, 0, 0); // transform final block doesn't copy the output
            Buffer.BlockCopy(sha512.Hash, 0, OUT_BUFFER, HMACSHA512_START, sha512.Hash.Length);


            // Proposed SHA3 uses a delimiter/padding byte of 0x01 intead of 0x06 or 0x1f (SHAKE128/256), that's the only difference
            // This is IMPORTANT for V3, as it used a pre-release version of the proposed SHA3 standard.  NIST changed SHA3 well after
            //  TripleSec was released and had been used in the wild.
            SHA3Managed.HMAC_Proposed_SHA3_512 sha3_512 = new SHA3Managed.HMAC_Proposed_SHA3_512(Keys.HMACKeccak512_KEY);
            sha3_512.HashCore(TRIPLESEC_MAGICPLUSVERSION3_BYTES, 0, TRIPLESEC_MAGICPLUSVERSION3_BYTELENGTH);
            sha3_512.HashCore(Keys.Salt, 0, Keys.Salt.Length);
            sha3_512.HashCore(IVs.AES_IV, 0, IVs.AES_IV.Length);
            sha3_512.HashCore(interim, 0, interim.Length);
            sha3_512.HashFinal(interim, 0, 0); // transform final block doesn't copy the output
            Buffer.BlockCopy(sha3_512.Hash, 0, OUT_BUFFER, HMACKECCAK512_START, sha3_512.Hash.Length);
//#if DEBUG
//            System.Diagnostics.Debug.Print("ENCRYPT SIGNATURES:");
//            System.Diagnostics.Debug.Print(BitConverter.ToString(sha512.Hash).Replace("-", ""));
//            System.Diagnostics.Debug.Print(BitConverter.ToString(sha3_512.Hash).Replace("-", ""));
//            System.Diagnostics.Debug.Print(BitConverter.ToString(OUT_BUFFER).Replace("-",""));
//#endif
            // HASHLIB REMOVED -----------!

            interim.Wipe(); // this isn't really leaking since it's encrypted AND copied to the output buffer, but clean house all the same.
            
            return OUT_BUFFER;
        }

        public static bool TryDecrypt(byte[] Source, byte[] Passphrase, out byte[] Result)
        {
            //is this the correct version and is this a TripleSec payload to begin with
            byte[] buffer = new byte[4 + 4];
            Buffer.BlockCopy(Source, 0, buffer, 0, buffer.Length);
            if (buffer.ToHexString() != TRIPLESEC_MAGICPLUSVERSION3_HEXBYTES)
                throw new ArgumentException("Not a TripleSec v3 encrypted payload!");

            byte[] theSalt = new byte[SALT_LEN];
            Buffer.BlockCopy(Source, SALT_START, theSalt, 0, SALT_LEN);
            // need the keys to verify the signatures
#if DEBUG
#pragma warning disable 618 // we know in DEBUG mode this will throw an Obsolete warning; this is a legitimate use of this function, so supress it
#endif
            KeysV3 keys = new KeysV3(Passphrase, theSalt);
#if DEBUG
#pragma warning restore 618
#endif
            // VERIFY SIGNATURES HERE
            // get existing signatures
            byte[] SENT_HMACSHA512 = new byte[HMACSHA512_LEN];
            Buffer.BlockCopy(Source, HMACSHA512_START, SENT_HMACSHA512, 0, HMACSHA512_LEN);
            byte[] SENT_HMACKECCAK512 = new byte[HMACKECCAK512_LEN];
            Buffer.BlockCopy(Source, HMACKECCAK512_START, SENT_HMACKECCAK512, 0, HMACKECCAK512_LEN);
            string SENT_SIGNATURE = Utilities.BytesToHexString(SENT_HMACSHA512) + Utilities.BytesToHexString(SENT_HMACKECCAK512);

            //calculate hash on sent data before decryption
            int sourceLength = Source.Length - CIPHERTEXT_START; // the AES_IV isn't counted here

            byte[] AES_IV = new byte[AES_IV_LEN];
            Buffer.BlockCopy(Source, AES_IV_START, AES_IV, 0, AES_IV_LEN);
            //buffer = new byte[TRIPLESEC_MAGICPLUSVERSION3_BYTELENGTH + theSalt.Length + AES_IV.Length + sourceLength];

            HMACSHA512 sha512 = new HMACSHA512(keys.HMACSHA512_KEY);
            SHA3Managed.HMAC_Proposed_SHA3_512 sha3_512 = new SHA3Managed.HMAC_Proposed_SHA3_512(keys.HMACKeccak512_KEY);
            sha512.TransformBlock(TRIPLESEC_MAGICPLUSVERSION3_BYTES, 0, TRIPLESEC_MAGICPLUSVERSION3_BYTELENGTH, TRIPLESEC_MAGICPLUSVERSION3_BYTES, 0);
            sha3_512.HashCore(TRIPLESEC_MAGICPLUSVERSION3_BYTES, 0, TRIPLESEC_MAGICPLUSVERSION3_BYTELENGTH);
            sha512.TransformBlock(theSalt, 0, theSalt.Length, theSalt, 0);
            sha3_512.HashCore(theSalt, 0, theSalt.Length);
            sha512.TransformBlock(AES_IV, 0, AES_IV.Length, AES_IV, 0);
            sha3_512.HashCore(AES_IV, 0, AES_IV.Length);
            sha512.TransformBlock(Source, CIPHERTEXT_START, sourceLength, Source, CIPHERTEXT_START);
            sha3_512.HashCore(Source, CIPHERTEXT_START, sourceLength);
            sha512.TransformFinalBlock(Source, 0, 0);
            sha3_512.HashFinal(Source, 0, 0);
//#if DEBUG
//            System.Diagnostics.Debug.Print("DECRYPT SIGNATURES:");
//            System.Diagnostics.Debug.Print(BitConverter.ToString(sha512.Hash).Replace("-", ""));
//            System.Diagnostics.Debug.Print(BitConverter.ToString(sha3_512.Hash).Replace("-", ""));
//#endif
            // HASHLIB REMOVED ----!!

            string CALCULATED_SIGNATURE = Utilities.BytesToHexString(sha512.Hash) + Utilities.BytesToHexString(sha3_512.Hash);

            if (CALCULATED_SIGNATURE != SENT_SIGNATURE)
            {
                Result = null;
                return false;
            }
            //==================================================================================

            byte[] OUTPUT = new byte[sourceLength];
            // AES is first (outermost)
            BC.Crypto.Modes.SicBlockCipher mySIC_AES = new BC.Crypto.Modes.SicBlockCipher(new BC.Crypto.Engines.AesEngine());
            mySIC_AES.Init(true,
                new BC.Crypto.Parameters.ParametersWithIV(
                    new BC.Crypto.Parameters.KeyParameter(keys.AES_KEY),
                    AES_IV));
            ProcessArray(mySIC_AES, Source, CIPHERTEXT_START, OUTPUT, 0, sourceLength);

            // prepare for next
            byte[] IV_TWOFISH = new byte[16];
            Buffer.BlockCopy(OUTPUT, 0, IV_TWOFISH, 0, IV_TWOFISH.Length);
            byte[] INPUT = new byte[OUTPUT.Length - IV_TWOFISH.Length];
            Buffer.BlockCopy(OUTPUT, IV_TWOFISH.Length, INPUT, 0, INPUT.Length);
            OUTPUT.Wipe(); // DON'T LEAK!
            OUTPUT = new byte[INPUT.Length];

            //Twofish is next
            BC.Crypto.Modes.SicBlockCipher mySIC_TF = new BC.Crypto.Modes.SicBlockCipher(new BC.Crypto.Engines.TwofishEngine());
            mySIC_TF.Init(true,
                new BC.Crypto.Parameters.ParametersWithIV(
                    new BC.Crypto.Parameters.KeyParameter(keys.Twofish_KEY),
                    IV_TWOFISH));
            ProcessArray(mySIC_TF, INPUT, OUTPUT);
            INPUT.Wipe(); // DON'T LEAK!
            
            // prepare for next
            byte[] IV_XSALSA20 = new byte[24];
            Buffer.BlockCopy(OUTPUT, 0, IV_XSALSA20, 0, IV_XSALSA20.Length);
            INPUT = new byte[OUTPUT.Length - IV_XSALSA20.Length];
            Buffer.BlockCopy(OUTPUT, IV_XSALSA20.Length, INPUT, 0, INPUT.Length);
            OUTPUT.Wipe(); // DON'T LEAK!

            //XSalsa20 is last
            OUTPUT = Chaos.NaCl.XSalsa20.Process(INPUT, keys.XSalsa20_KEY, IV_XSALSA20);
            INPUT.Wipe(); // DON'T LEAK!
            Result = OUTPUT;
            return true;
        }

    }
}
