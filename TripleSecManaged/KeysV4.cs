using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SSC = System.Security.Cryptography;

namespace TripleSecManaged
{
    /// <summary>
    /// Key generator for the Version 4 TripleSec Encryption process.
    /// </summary>
#if DEBUG
    [Obsolete("USED FOR DEBUGGING PURPOSES ONLY! Clear DEBUG flag during build (or use Release build configuration) to internalize this class in final library.")]
    public sealed class KeysV4
#else
    internal sealed class KeysV4
#endif
    {
        const int SCRYPT_CPU_COST = 32768; // 2^15
        const int SCRYPT_PARALLEL_THREADS = 1;
        const int SCRYPT_BLOCKS = 8;
        const int SCRYPT_OUTPUT_BYTES = 192;
        private byte[] _hmacsha512key = null;
        private byte[] _hmacsha3_512key = null;
        private byte[] _aeskey = null;
        private byte[] _xsalsa20key = null;
        private byte[] _salt = null;
        private bool _ready = false;
        private static object LOCKTHIS = new object();

        public bool IsInitialized
        {
            get
            {
                lock (LOCKTHIS) // prevent race conditions where there are blank arrays created but not filled with appropriate data
                {
                    return (_ready == true);
                }
            }
        }

        /// <summary>
        /// The Salt as a series of bytes either passed into the construtor or Initialize or as was randomly generated if one wasn't provided.
        /// </summary>
        public byte[] Salt { get { return IsInitialized ? (byte[])_salt.Clone() : null; } }
        /// <summary>
        /// The 48 byte HMAC SHA-512 key as a series of bytes.
        /// </summary>
        public byte[] HMACSHA512_KEY { get { return IsInitialized ? (byte[])_hmacsha512key.Clone() : null; } }
        /// <summary>
        /// The 48 byte HMAC Keccak512 key as a series of bytes.
        /// </summary>
        public byte[] HMACSHA3_512_KEY { get { return IsInitialized ? (byte[])_hmacsha3_512key.Clone() : null; } }
        /// <summary>
        /// The 32 byte AES encryption key as a series of bytes.
        /// </summary>
        public byte[] AES_KEY { get { return IsInitialized ? (byte[])_aeskey.Clone() : null; } }
        /// <summary>
        /// The 32 byte XSalsa20 encryption key as a series of bytes.
        /// </summary>
        public byte[] XSalsa20_KEY { get { return IsInitialized ? (byte[])_xsalsa20key.Clone() : null; } }
        
        // No default contructor without parameters, force initialization!!!

        /// <summary>
        /// Creates a new KeysV3 object with the Passphrase converted into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="Passphrase">The Passphrase string.</param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public KeysV4(string Passphrase, bool WipePassphrase = false)
        {
            Initialize(Passphrase, null, WipePassphrase);
        }

        /// <summary>
        /// Creates a new KeysV3 object with the Passphrase converted into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="PassphraseBytes">The Passphrase as a series of bytes.</param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public KeysV4(byte[] PassphraseBytes, bool WipePassphrase = false)
        {
            Initialize(PassphraseBytes, (byte[])null, WipePassphrase); // casting needed to prevent ambiguity error
        }

        /// <summary>
        /// Creates a new KeysV3 object with the Passphrase converted into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="Passphrase">The Passphrase string.</param>
        /// <param name="RNG">A previously generated RNGV3 object (needed for the Salt).</param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public KeysV4(string Passphrase, RNGV4 RNG, bool WipePassphrase = false)
        {
            if (RNG == null)
                RNG = new RNGV4();
            Initialize(Passphrase, RNG, WipePassphrase);
        }

        /// <summary>
        /// Creates a new KeysV3 object with the Passphrase converted into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="PassphraseBytes">The Passphrase as a series of bytes.</param>
        /// <param name="RNG">A previously generated RNGV4 object (needed for the Salt).</param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public KeysV4(byte[] PassphraseBytes, RNGV4 RNG, bool WipePassphrase = false)
        {
            if (RNG == null)
                RNG = new RNGV4();
            Initialize(PassphraseBytes, RNG, WipePassphrase);
        }

        /// <summary>
        /// Creates a new KeysV3 object with the Passphrase converted into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="PassphraseBytes">The Passphrase as a series of bytes.</param>
        /// <param name="Salt"></param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public KeysV4(byte[] PassphraseBytes, byte[] Salt = null, bool WipePassphrase = false)
        {
            Initialize(PassphraseBytes, Salt, WipePassphrase);
        }

        /// <summary>
        /// Resets the current KeysV3 object with the Passphrase, converting it into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="Passphrase">The Passphrase string.</param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public void Initialize(string Passphrase, bool WipePassphrase = false)
        {
            Initialize(new UTF8Encoding().GetBytes(Passphrase), (byte[])null, WipePassphrase); // casting needed to prevent ambiguity error
        }

        /// <summary>
        /// Resets the current KeysV3 object with the Passphrase, converting it into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="Passphrase">The Passphrase string.</param>
        /// <param name="RNG"></param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public void Initialize(string Passphrase, RNGV4 RNG, bool WipePassphrase = false)
        {
            if (RNG == null)
                RNG = new RNGV4();
            Initialize(new UTF8Encoding().GetBytes(Passphrase), RNG, WipePassphrase);
        }

        /// <summary>
        /// Resets the current KeysV3 object with the Passphrase, converting it into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="PassphraseBytes">The Passphrase as a series of bytes.</param>
        /// <param name="RNG"></param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public void Initialize(byte[] PassphraseBytes, RNGV4 RNG, bool WipePassphrase = false)
        {
            if (RNG == null)
                RNG = new RNGV4();
            Initialize(PassphraseBytes, RNG.Salt, WipePassphrase);
        }

        /// <summary>
        /// Resets the current KeysV3 object with the Passphrase, converting it into the proper internal TripleSec encryption and HMAC keys.
        /// </summary>
        /// <param name="PassphraseBytes">The Passphrase as a series of bytes.</param>
        /// <param name="Salt">The Salt as a series of bytes or NULL/NOTHING to use a newly created random sequence of bytes (use the Salt property to retrieve the created array).</param>
        /// <param name="WipePassphrase">Wipe the passphrase from memory when no longer needed? WARNING: USE WITH CAUTION!!!</param>
        public void Initialize(byte[] PassphraseBytes, byte[] Salt = null, bool WipePassphrase = false)
        {
            // all constructors and initializers end up here
            if (PassphraseBytes == null || PassphraseBytes.Length == 0)
                throw new ArgumentNullException("PassphraseBytes");
            byte[] _tempSalt = null;
            if (Salt == null)
            {
                _tempSalt = new RNGV4().Salt;
            }
            else if (Salt.Length != 16)
                throw new ArgumentOutOfRangeException("Salt", "Salt must be 16 bytes or NULL/NOTHING.");
            else
                _tempSalt = (byte[])Salt.Clone();
            lock (LOCKTHIS)
            {
                byte[] KEYS = CryptSharp.Utility.SCrypt.ComputeDerivedKey(
                    PassphraseBytes, _tempSalt,
                    SCRYPT_CPU_COST, SCRYPT_BLOCKS, SCRYPT_PARALLEL_THREADS, SCRYPT_PARALLEL_THREADS, SCRYPT_OUTPUT_BYTES);
                // USE THE FOLLOWING WITH EXTREME CAUTION, THE PASSPHRASE WILL BE CLEANED A LONG WAY UP THE HEAP!!!!
                if (WipePassphrase) // WARNING: only do this if asked, as it will wipe ALL the way back up the call stack!!!!!
                    PassphraseBytes.Wipe(); // get it out of memory absolutely as soon as possible!
                this.Clear();
                _salt = (byte[])_tempSalt.Clone();
                _tempSalt.Wipe();
                this._hmacsha512key = new byte[48];
                this._hmacsha3_512key = new byte[48];
                this._aeskey = new byte[32];
                this._xsalsa20key = new byte[32];

                Buffer.BlockCopy(KEYS, 0, this._hmacsha512key, 0, this._hmacsha512key.Length);
                Buffer.BlockCopy(KEYS, 48, this._hmacsha3_512key, 0, this._hmacsha3_512key.Length);
                Buffer.BlockCopy(KEYS, 48 + 48, this._aeskey, 0, this._aeskey.Length);
                Buffer.BlockCopy(KEYS, 48 + 48 + 32, this._xsalsa20key, 0, this._xsalsa20key.Length);
                _ready = true;
//#if DEBUG
//                System.Diagnostics.Debug.Print("V4 KEYS-----------------");
//                System.Diagnostics.Debug.Print("Passphrase Input: " + BitConverter.ToString(PassphraseBytes).Replace("-", "").ToLowerInvariant());
//                System.Diagnostics.Debug.Print("Salt:             " + BitConverter.ToString(this._salt).Replace("-", "").ToLowerInvariant()); 
//                System.Diagnostics.Debug.Print("HMACSHA512 Key:   " + BitConverter.ToString(this._hmacsha512key).Replace("-","").ToLowerInvariant());
//                System.Diagnostics.Debug.Print("HMACSHA3_512 Key: " + BitConverter.ToString(this._hmacsha3_512key).Replace("-", "").ToLowerInvariant());
//                System.Diagnostics.Debug.Print("AES Key:          " + BitConverter.ToString(this._aeskey).Replace("-", "").ToLowerInvariant());
//                System.Diagnostics.Debug.Print("XSALSA20 Key:     " + BitConverter.ToString(this._xsalsa20key).Replace("-", "").ToLowerInvariant());
//#endif
            }
        }

        /// <summary>
        /// Wipes the memory of the current keys.  Use Initialize after this call to be able to use the object again, else all properties will return NULL/NOTHING.
        /// </summary>
        public void Clear()
        {
            lock (LOCKTHIS)
            {
                _ready = false; // prevent race condition where later arrays may be available while the earlier arrays have already been (or are being) wiped.
                this._aeskey.Wipe();
                this._hmacsha512key.Wipe();
                this._hmacsha3_512key.Wipe();
                this._salt.Wipe();
                //this._twofishkey.Wipe();
                this._xsalsa20key.Wipe();
            }
        }

        ~KeysV4()
        {
            //CLEAN UP IN AISLE FIVE!!!!
            this.Clear();
        }

#if DEBUG   
        [Obsolete("USED FOR DEBUGGING PURPOSES ONLY! Clear DEBUG flag during build (or use Release build configuration) to remove this function from final library.")]
        public KeysV4(byte[] salt, byte[] hmacsha512key, byte[] hmacsha3_512key, byte[] aeskey, byte[] xsalsa20key)
        {
            //USED ONLY FOR DEBUGGING AND TROUBLESHOOTING
            if (salt == null || _hmacsha512key == null || _hmacsha3_512key == null ||
                _aeskey == null || _xsalsa20key == null)
                throw new Exception(); // purposely not helpful, this should only be used when you have access to the source and are debugging an application
            if (salt.Length != 16 || 
                _hmacsha512key.Length != 48 || _hmacsha3_512key.Length != 48 ||
                _aeskey.Length != 32 || _xsalsa20key.Length != 32)
                throw new Exception(); // purposely not helpful, this should only be used when you have access to the source and are debugging an application
            lock (LOCKTHIS)
            {
                this._salt = (byte[])salt.Clone();
                this._hmacsha512key = (byte[])hmacsha512key.Clone();
                this._hmacsha3_512key = (byte[])hmacsha3_512key.Clone();
                this._aeskey = (byte[])aeskey.Clone();
                this._xsalsa20key = (byte[])xsalsa20key.Clone();
                _ready = true;
            }
        }
#endif
    }

}
