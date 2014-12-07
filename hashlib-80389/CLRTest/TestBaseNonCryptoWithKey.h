
#pragma once 

namespace CLRTest
{
    ref class TestBaseNonCryptoWithKey abstract : TestBase
    {
        protected:

            array<byte>^ GetKey(String^ a_str)
            {
                return Converters::ConvertHexStringToBytes(a_str->Substring(4)->ToUpper());
            }

            virtual List<String^>^ LoadTestVectors(String^ a_filePath) override
            {
                auto list = TestBase::LoadTestVectors(a_filePath);

                Debug::Assert(list->Count % 4 == 0);

                return list;
            }

            void TestFile(String^ a_filePath)
            {
                System::Console::WriteLine("Testing file '{0}'", Path::GetFileName(a_filePath));

                List<String^>^ list = LoadTestVectors(a_filePath);

                for (int i=0; i<list->Count/4; i++)
                {
                    array<byte>^ key = GetKey(list[i * 4]);
                    int len = GetLength(list[i * 4 + 1]);
                    array<byte>^ msg = GetMsg(list[i * 4 + 2]);
                    array<byte>^ res = GetDigest(list[i * 4 + 3]);
                    SetKey(key);
                    array<byte>^ out = ComputeBytes(msg);

                    if (Trans::memcmp(out, res) != 0)
                    {
                        System::Console::WriteLine("ERROR: Test 1 failed for length={0} \n", len);
                        break;
                    }

                    // only for longest test vector.
                    if (i + 1 == list->Count / 4)
                    {
                        Random^ random = gcnew Random();

                        // do it ten times for sure.
                        for (int j=0; j<10; j++)
                        {
                            SetKey(key);

                            int pos = 0;
                            int L = len;

                            while (L > 0)
                            {
                                int buf_len = random->Next(GetMaxBufferSize() * 3 / 2);
                                if (buf_len > L)
                                    buf_len = L;

                                TransformBytes(msg, pos, buf_len);

                                pos += buf_len;
                                L -= buf_len;
                            }

                            out = TransformFinal();

                            array<byte>^ res2 = ComputeBytes(msg);

                            if (Trans::memcmp(out, res) != 0)
                            {
                                System::Console::WriteLine("ERROR: Test 2 failed #1 - random pieces (len: {0})\n", len);
                                break;
                            }
                            else if (Trans::memcmp(out, res2) != 0)
                            {
                                System::Console::WriteLine("ERROR: Test 2 failed #2 - reinitialization error probably (len: {0})\n", len);
                                break;
                            }
                            else if (Trans::memcmp(res, res2) != 0)
                            {
                                System::Console::WriteLine("ERROR: Test 2 failed #3 - reinitialization error probably (len: {0})\n", len);
                                break;
                            }
                        }
                    }
                }
            }

            virtual void SetKey(array<byte>^ a_key) abstract;
            virtual int GetHashSize() abstract;

        public:

            virtual void Test() override
            {
                System::Console::WriteLine("{0} - Starting \n\n", GetTestName());

                String^ dir = GetTestsDir();

                #if TEST_SHORT_HASHES
                TestFile(dir + String::Format("\\MFCTest\\TestVectors\\{0}.txt", GetTestVectorsDir()));
                #endif

                System::Console::WriteLine("\n");
            }
    };
}