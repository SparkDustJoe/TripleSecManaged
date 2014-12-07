
#pragma once 

#include "stdafx.h"

namespace MFCTest
{
    class TestBaseNonCryptoWithKey : public TestBaseBlock
    {
        protected: byte* GetKey(CString& a_str)
        {
            wchar_t* buf = a_str.GetBuffer();
            byte* r = HexToBuffer(&buf[4], a_str.GetLength() - 4);
            a_str.ReleaseBuffer();
            return r;
        }

        protected: virtual CStringArray* LoadTestVectors(const wchar_t* a_filePath) override
        {
            CStringArray* list = TestBase::LoadTestVectors(a_filePath);

            ASSERT(list->GetCount() % 4 == 0);

            return list;
        }

        protected: void TestFile(const wchar_t* a_filePath)
        {
            wprintf(L"Testing file '%s' \n", CFile(a_filePath, CFile::modeRead | CFile::shareDenyNone).GetFileName());

            CStringArray* list = LoadTestVectors(a_filePath);

            for (int i=0; i<list->GetCount()/4; i++)
            {
                byte* key = GetKey(list->ElementAt(i * 4));
                int len = GetLength(list->ElementAt(i * 4 + 1));

                byte* msg = GetMsg(list->ElementAt(i * 4 + 2));
                byte* res = GetDigest(list->ElementAt(i * 4 + 3));
                InitializeHash();
                SetKey(key);
                byte* out = ComputeBytes(msg, len);

                if (memcmp(out, res, GetHashSize()) != 0)
                {
                    wprintf(L"ERROR: Test 1 failed for length=%d \n", len);
                    delete out;
                    delete res;
                    break;
                }

                delete out;

                // only for longest test vector.
                if (i + 1 == list->GetCount() / 4)
                {
                    // do it ten times for sure.
                    for (int j=0; j<10; j++)
                    {
                        InitializeHash();
                        SetKey(key);

                        int pos = 0;
                        int L = len;

                        while (L > 0)
                        {
                            int buf_len = rand() % (GetMaxBufferSize() * 3 / 2);
                            if (buf_len > L)
                                buf_len = L;

                            TransformBytes(msg, pos, buf_len);

                            pos += buf_len;
                            L -= buf_len;
                        }

                        out = TransformFinal();

                        byte* res2 = ComputeBytes(msg, len);

                        if (memcmp(out, res, GetHashSize()) != 0)
                        {
                            wprintf(L"ERROR: Test 2 failed #1 - random pieces (len: %d)\n", len);
                            delete out;
                            delete res2;
                            break;
                        }
                        else if (memcmp(out, res2, GetHashSize()) != 0)
                        {
                            wprintf(L"ERROR: Test 2 failed #2 - reinitialization error probably (len: %d)\n", len);
                            delete out;
                            delete res2;
                            break;
                        }
                        else if (memcmp(res, res2, GetHashSize()) != 0)
                        {
                            wprintf(L"ERROR: Test 2 failed #3 - reinitialization error probably (len: %d)\n", len);
                            delete out;
                            delete res2;
                            break;
                        }
                        else
                        {
                            delete out;
                            delete res2;
                        }
                    }
                }

                delete res;
                delete msg;
                delete key;
            }

            delete list;
        }

        protected: void GenerateShorts(const wchar_t* a_filePath)
        {
            CFile vectors(a_filePath, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate);
            wprintf(L"Generating into file '%s'\n", vectors.GetFileName());

            byte* keys;
            int keys_count;
            GenerateKeys(keys, keys_count);

            for (int length = 0; length <= GetMaxBufferSize() * 3 + 1; length++)
            {
                for (int key = 0; key < keys_count; key++)
                {
                    InitializeHash();
                    SetKey(&keys[key *  GetKeyLength()]);
                    byte* data = GetRandomBytes(length);
                    byte* bytes = ComputeBytes(data, length);
                    CString hash = BufferToHex(bytes, GetHashSize());
                    CString data_str = BufferToHex(data, length);
                    CString key_str = BufferToHex(&keys[key *  GetKeyLength()], GetKeyLength());

                    CStringA str = StringFormat(L"Key=%s\nLen = %d\nMsg = %s\nMD = %s\n\n", key_str, length, data_str, hash);
                    vectors.Write(str.GetBuffer(), str.GetLength());
                    str.ReleaseBuffer();

                    delete data;
                    delete bytes;
                }
            }

            delete keys;

            vectors.Flush();
            vectors.Close();

        }

        public: void Test()
        {
            wprintf(L"%s - Starting \n\n", GetTestName());

            CString dir = GetTestsDir();

            #if GENERATE_SHORT_HASHES
            GenerateShorts(dir + StringFormat(L"\\TestVectors\\%s.txt", GetTestVectorsDir()));
            #endif

            #if TEST_SHORT_HASHES
            TestFile(dir + StringFormat(L"\\TestVectors\\%s.txt", GetTestVectorsDir()));
            #endif

            wprintf(L"\n");
        }

        protected: virtual void SetKey(byte* a_key) = 0;
        protected: virtual void GenerateKeys(byte*& a_keys, int& a_count) = 0;
        protected: virtual int GetKeyLength() = 0;
        protected: virtual int GetHashSize() = 0;
        public: virtual void InitializeHash() = 0;
    };
}