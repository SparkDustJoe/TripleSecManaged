using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SSC = System.Security.Cryptography;

namespace TripleSecManaged
{
    /// <summary>
    /// Random number generator for the Initialization Vectors and SALT for the Version 4 TripleSec Encryption Process
    /// </summary>
#if DEBUG
    [Obsolete("USED FOR DEBUGGING PURPOSES ONLY! Clear DEBUG flag during build (or use Release build configuration) to internalize this class in final library.")]
    public sealed class RNGV4
#else
    internal sealed class RNGV4
#endif
    {
        private byte[] _salt = null;
        private byte[] _aesIV = null;
        private byte[] _xsalsa20IV = null;
        private bool _ready = false;

        /// <summary>
        /// Has this instance been initialized?
        /// </summary>
        public bool IsInitialized
        {
            get
            {
                // prevent race condition where test happens at moment new blank arrays were created and not filled with random data
                return (_ready);
            }
        }

        /// <summary>
        /// The 16-byte SALT used during Key generation.
        /// </summary>
        public byte[] Salt { get { return IsInitialized ? (byte[])_salt.Clone() : null; } }
        /// <summary>
        /// The Initialization Vector for the AES encryption transformation.
        /// </summary>
        public byte[] AES_IV { get { return IsInitialized ? (byte[])_aesIV.Clone() : null; } }
        /// <summary>
        /// The Initialization Vector for the XSalsa20 encryption transformation.
        /// </summary>
        public byte[] XSalsa20_IV { get { return IsInitialized ? (byte[])_xsalsa20IV.Clone() : null; } }

        /// <summary>
        /// Creates a new and initialized instance of the TripleSec RNG (no parameters).
        /// </summary>
        public RNGV4()
        {
            // sure, the .NET RNG is pretty good, but lets make an attacker's life miserable
            // and also guard against a compromised RNG
            SSC.RNGCryptoServiceProvider rng = new SSC.RNGCryptoServiceProvider();
            byte[] tempKey = new byte[512];
            byte[] tempSalt = new byte[512];
            rng.GetBytes(tempKey);
            rng.GetBytes(tempSalt);
            byte[] interim = new SSC.Rfc2898DeriveBytes(tempKey, tempSalt, 64).GetBytes(1024);
            rng.GetBytes(tempSalt);
            byte[] final = new SSC.Rfc2898DeriveBytes(interim, tempSalt, 64).GetBytes(56);
            tempKey.Wipe();
            tempSalt.Wipe();
            interim.Wipe(); // DON'T LEAK!!
            _salt = new byte[16];
            _aesIV = new byte[16];
            _xsalsa20IV = new byte[24];
            Buffer.BlockCopy(final, 0, _salt, 0, _salt.Length);
            Buffer.BlockCopy(final, 16, _aesIV, 0, _aesIV.Length);
            Buffer.BlockCopy(final, 16 + 16, _xsalsa20IV, 0, _xsalsa20IV.Length);
            _ready = true;
//#if DEBUG
//            System.Diagnostics.Debug.Print("RNGV4:-------------------------------------");
//            System.Diagnostics.Debug.Print("salt:        " + BitConverter.ToString(_salt).Replace("-", "").ToLowerInvariant());
//            System.Diagnostics.Debug.Print("aesIV:       " + BitConverter.ToString(_salt).Replace("-", "").ToLowerInvariant());
//            System.Diagnostics.Debug.Print("xsalsa20IV:  " + BitConverter.ToString(_salt).Replace("-", "").ToLowerInvariant());
//            System.Diagnostics.Debug.Print("final array: " + BitConverter.ToString(final).Replace("-", "").ToLowerInvariant());
//#endif
            final.Wipe(); // DON'T LEAVE COPIES LAYING AROUND!
        }

#if DEBUG
        /// <summary>
        /// Creates a new and initialized instance of the TripleSec RNG with the provided bytes.  DEBUG PURPOSES ONLY!!!!
        /// </summary>
        /// <param name="raw">The raw 56-byte array used to create the Salt and all IV's.  DEBUG PURPOSES ONLY!!!! </param>
        [Obsolete("USED FOR DEBUGGING PURPOSES ONLY! Clear DEBUG flag during build (or use Release build configuration) to remove this function from final library.")]
        public RNGV4(byte[] raw) //USED ONLY FOR DEBUGGING AND TROUBLESHOOTING!!
        {
            if (raw == null || raw.Length != 56)
                throw new Exception(); // purposely not helpful, this should only be used when you have access to the source and are debugging an application

            _salt = new byte[16];
            _aesIV = new byte[16];
            _xsalsa20IV = new byte[24];
            Buffer.BlockCopy(raw, 0, _salt, 0, _salt.Length);
            Buffer.BlockCopy(raw, 16, _aesIV, 0, _aesIV.Length);
            Buffer.BlockCopy(raw, 16 + 16, _xsalsa20IV, 0, _xsalsa20IV.Length);
            _ready = true;

//#if DEBUG
//            System.Diagnostics.Debug.Print("RNGV4 (DEBUG, STUFFED ARRAY):-------------------------------------");
//            System.Diagnostics.Debug.Print("salt:        " + BitConverter.ToString(_salt).Replace("-", "").ToLowerInvariant());
//            System.Diagnostics.Debug.Print("aesIV:       " + BitConverter.ToString(_aesIV).Replace("-", "").ToLowerInvariant());
//            System.Diagnostics.Debug.Print("xsalsa20IV:  " + BitConverter.ToString(_xsalsa20IV).Replace("-", "").ToLowerInvariant());
//#endif
        }
#endif
    }
}
