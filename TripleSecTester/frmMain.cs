using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SSC = System.Security.Cryptography;

namespace TripleSecTester
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            InitializeComponent();
        }

        private void btnGO_Click(object sender, EventArgs e)
        {
            this.Enabled = false;

            SSC.RNGCryptoServiceProvider rng = new SSC.RNGCryptoServiceProvider();
            byte[] source_length = new byte[2];
            rng.GetNonZeroBytes(source_length);
            byte[] SOURCE = new byte[source_length[0] + (source_length[1] * 256)];
            rng.GetBytes(SOURCE);

            string hashIN = Convert.ToBase64String(new SSC.SHA1Managed().ComputeHash(SOURCE));

            // V3 V3 V3 V3

            // use these two lines for specific passphrases
            //string PASSPHRASE = "TripleSecManaged";
            //byte[] PASSPHRASE_BYTES = new UTF8Encoding().GetBytes(PASSPHRASE);

            // use these two lines for random passphrases
            byte[] PASSPHRASE_BYTES = new byte[source_length[0]];
            rng.GetBytes(PASSPHRASE_BYTES);                        

            byte[] stuffIN = TripleSecManaged.V3.Encrypt(SOURCE, PASSPHRASE_BYTES);
            byte[] stuffOUT = null;
            bool didItWork = TripleSecManaged.V3.TryDecrypt(stuffIN, PASSPHRASE_BYTES, out stuffOUT);

            string hashOUT = didItWork ?  // don't try to do anything with stuffOUT if the function failed
                Convert.ToBase64String(new SSC.SHA1Managed().ComputeHash(stuffOUT)) : "[null]";
            txtOutput.Clear();
            if (hashIN != hashOUT || didItWork == false)
                txtOutput.AppendText("V3 FAILURE!!!\r\n" + hashIN + "\r\n" + hashOUT);
            else
                txtOutput.AppendText("V3 SUCCESS!!!\r\n" + hashOUT);
            txtOutput.AppendText("\r\nV3 Source Length: " + SOURCE.Length);
            txtOutput.AppendText("\r\nV3 Passphrase Length: " + PASSPHRASE_BYTES.Length);
            txtOutput.AppendText("\r\n\"V3 Did It Work?\": " + didItWork);
            //txtOutput.AppendText("\r\n\r\n" + TripleSecManaged.Utilities.BytesToHexString(stuffIN));

            // V4 V4 V4 V4

            // use these two lines for specific passphrases
            //string PASSPHRASE = "TripleSecManaged";
            //byte[] PASSPHRASE_BYTES = new UTF8Encoding().GetBytes(PASSPHRASE);

            // use these two lines for random passphrases
            PASSPHRASE_BYTES = new byte[source_length[0]];
            rng.GetBytes(PASSPHRASE_BYTES);

            stuffIN = TripleSecManaged.V4.Encrypt(SOURCE, PASSPHRASE_BYTES);
            stuffOUT = null;
            didItWork = TripleSecManaged.V4.TryDecrypt(stuffIN, PASSPHRASE_BYTES, out stuffOUT);

            hashOUT = didItWork ?  // don't try to do anything with stuffOUT if the function failed
                Convert.ToBase64String(new SSC.SHA1Managed().ComputeHash(stuffOUT)) : "[null]";
            if (hashIN != hashOUT || didItWork == false)
                txtOutput.AppendText("\r\n\r\nV4 FAILURE!!!\r\n" + hashIN + "\r\n" + hashOUT);
            else
                txtOutput.AppendText("\r\n\r\nV4 SUCCESS!!!\r\n" + hashOUT);
            txtOutput.AppendText("\r\nV4 Source Length: " + SOURCE.Length);
            txtOutput.AppendText("\r\nV4 Passphrase Length: " + PASSPHRASE_BYTES.Length);
            txtOutput.AppendText("\r\n\"V4 Did It Work?\": " + didItWork);

            this.Enabled = true;
        }

        private void btnKATS_Click(object sender, EventArgs e)
        {
            //if compilation errors crop up that KeysV3 or RNGV3 does not exist, it's because the DLL was compiled without the DEBUG flag and those
            // functions and classes were internalized

            TestParameters tests = new TestParameters();
            this.Enabled = false;

            txtOutput.Clear();
            foreach (TestParameters.ParamsV3 test in tests.TestsV3)
            {
//#if DEBUG
//                System.Diagnostics.Debug.Print("==========================================================================================");
//                System.Diagnostics.Debug.Print("==TEST " + test.ID + "====================================================================");
//#endif
                txtOutput.AppendText("PROCEEDING WITH V3 TEST #" + test.ID + "\r\n");
                Application.DoEvents();
#pragma warning disable 618 // we know in DEBUG mode this will throw an Obsolete warning, this is a legitimate use of this function, so supress it
                TripleSecManaged.RNGV3 r = new TripleSecManaged.RNGV3(test.RNGBytes);
                TripleSecManaged.KeysV3 k = new TripleSecManaged.KeysV3(test.Key, r.Salt);
#pragma warning restore 618 
                if (!TestParameters.ArrayContentsAreEqual(test.HMACSHA512Key, k.HMACSHA512_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! HMACSHA512 Key\r\n");
                else
                    txtOutput.AppendText("Pass HMACSHA512 Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.HMACKeccak512Key, k.HMACKeccak512_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! HMACKeccak512 Key\r\n");
                else
                    txtOutput.AppendText("Pass HMACKeccak512 Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.AESKey, k.AES_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! AES Key\r\n");
                else
                    txtOutput.AppendText("Pass AES Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.TwofishKey, k.Twofish_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! Twofish Key\r\n");
                else
                    txtOutput.AppendText("Pass Twofish Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.XSalsa20Key, k.XSalsa20_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! XSalsa20 Key\r\n");
                else
                    txtOutput.AppendText("Pass XSalsa20 Key\r\n");
                Application.DoEvents();


#pragma warning disable 618 // we know in DEBUG mode this will throw an Obsolete warning, this is a legitimate use of this function, so supress it
                byte[] testedstuff = TripleSecManaged.V3.Encrypt(test.PlainText, k, r);
#pragma warning restore 618 
                if (!TestParameters.ArrayContentsAreEqual(test.CipherText, testedstuff))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! Encrypt: Ciphertexts don't match\r\n");
                else
                    txtOutput.AppendText("Pass Encrypt\r\n");
                Application.DoEvents();

                //use the test cipher text here to run the tests for Decrypt separately from Encrypt
                bool success = TripleSecManaged.V3.TryDecrypt(test.CipherText, test.Key, out testedstuff);
                bool match = success ? // if we succeeded on the last step, test them, else assume false match
                    TestParameters.ArrayContentsAreEqual(test.PlainText, testedstuff) : false;
                if (success && !match)
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! TryDecrypt = TRUE, Plaintexts don't match\r\n");
                else if (success && match)
                    txtOutput.AppendText("Pass TryDecrypt\r\n");
                else if (!success) // no need to test match on outright failure
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! TryDecrypt = FALSE\r\n");
            }

            foreach (TestParameters.ParamsV4 test in tests.TestsV4)
            {
//#if DEBUG
//                System.Diagnostics.Debug.Print("==========================================================================================");
//                System.Diagnostics.Debug.Print("==TEST " + test.ID + "====================================================================");
//#endif
                txtOutput.AppendText("PROCEEDING WITH V4 TEST #" + test.ID + "\r\n");
                Application.DoEvents();
#pragma warning disable 618 // we know in DEBUG mode this will throw an Obsolete warning, this is a legitimate use of this function, so supress it
                TripleSecManaged.RNGV4 r = new TripleSecManaged.RNGV4(test.RNGBytes);
                TripleSecManaged.KeysV4 k = new TripleSecManaged.KeysV4(test.Key, r.Salt);
#pragma warning restore 618 
                if (!TestParameters.ArrayContentsAreEqual(test.HMACSHA512Key, k.HMACSHA512_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! HMACSHA512 Key\r\n");
                else
                    txtOutput.AppendText("Pass HMACSHA512 Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.HMACSHA3_512Key, k.HMACSHA3_512_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! HMACKeccak512 Key\r\n");
                else
                    txtOutput.AppendText("Pass HMACKeccak512 Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.AESKey, k.AES_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! AES Key\r\n");
                else
                    txtOutput.AppendText("Pass AES Key\r\n");
                if (!TestParameters.ArrayContentsAreEqual(test.XSalsa20Key, k.XSalsa20_KEY))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! XSalsa20 Key\r\n");
                else
                    txtOutput.AppendText("Pass XSalsa20 Key\r\n");
                Application.DoEvents();


#pragma warning disable 618 // we know in DEBUG mode this will throw an Obsolete warning, this is a legitimate use of this function, so supress it
                byte[] testedstuff = TripleSecManaged.V4.Encrypt(test.PlainText, k, r);
#pragma warning restore 618 
                if (!TestParameters.ArrayContentsAreEqual(test.CipherText, testedstuff))
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! Encrypt: Ciphertexts don't match\r\n");
                else
                    txtOutput.AppendText("Pass Encrypt\r\n");
                Application.DoEvents();

                //use the test cipher text here to run the tests for Decrypt separately from Encrypt
                bool success = TripleSecManaged.V4.TryDecrypt(test.CipherText, test.Key, out testedstuff);
                bool match = success ? // if we succeeded on the last step, test them, else assume false match
                    TestParameters.ArrayContentsAreEqual(test.PlainText, testedstuff) : false;
                if (success && !match)
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! TryDecrypt = TRUE, Plaintexts don't match\r\n");
                else if (success && match)
                    txtOutput.AppendText("Pass TryDecrypt\r\n");
                else if (!success) // no need to test match on outright failure
                    txtOutput.AppendText("*TEST " + test.ID + " FAILED! TryDecrypt = FALSE\r\n");
            }

            txtOutput.AppendText("===COMPLETE===\r\n");
            txtOutput.AppendText("Any failures? " + txtOutput.Text.Contains("FAILED!").ToString());
            this.Enabled = true;
        }

    }
}
