
#pragma once

namespace CLRTest
{
    ref class TestBase abstract
    {
        protected:

            String^ GetTestsDir()
            {
                DirectoryInfo^ dir_info =
                    gcnew DirectoryInfo(
                    Path::GetDirectoryName(System::Reflection::Assembly::GetExecutingAssembly()->CodeBase->Remove(0, 8)));
                String^ dir = dir_info->Parent->Parent->Parent->FullName;

                return dir;
            }

            array<byte>^ GetRandomBytes(int a_length)
            {
                array<byte>^ r = gcnew array<byte>(a_length);

                Random^ random = gcnew Random();
                random->NextBytes(r);

                return r;
            }

            int GetLength(String^ a_str)
            {
                return Int32::Parse(a_str->Substring(6));
            }

            int GetRepeat(String^ a_str)
            {
                return Int32::Parse(a_str->Substring(9));
            }

            array<byte>^ GetText(String^ a_str)
            {
                return System::Text::Encoding::ASCII->GetBytes(a_str->Substring(7));
            }

            array<byte>^ GetMsg(String^ a_str)
            {
                return Converters::ConvertHexStringToBytes(a_str->Substring(6)->ToUpper());
            }

            array<byte>^ GetDigest(String^ a_str)
            {
                return Converters::ConvertHexStringToBytes(a_str->Substring(5)->ToUpper());
            }

            virtual List<String^>^ LoadTestVectors(String^ a_filePath)
            {
                List<String^>^ list = gcnew List<String^>();

                for each (String^ str in File::ReadAllLines(a_filePath))
                    list->Add(str);

                for (int i=list->Count-1; i>=0; i--)
                {
                    if (String::IsNullOrWhiteSpace(list[i]))
                        list->RemoveAt(i);
                }

                return list;
            }

            virtual void TransformBytes(array<byte>^ a_data, int a_index, int a_length) = 0;
            virtual array<byte>^ ComputeBytes(array<byte>^ a_data) = 0;
            virtual array<byte>^ TransformFinal() = 0;
            virtual void InitializeHash() = 0;
            virtual String^ GetTestVectorsDir() = 0;
            virtual String^ GetTestName() = 0;
            virtual int GetMaxBufferSize() = 0;

        public:

            static void Init()
            {
                #if ONE_CORE && defined(NDEBUG)
                Process::GetCurrentProcess()->ProcessorAffinity = (IntPtr)1;
                Process::GetCurrentProcess()->PriorityClass = ProcessPriorityClass::AboveNormal;
                Thread::CurrentThread->Priority = ThreadPriority::AboveNormal;
                #endif
            }

            virtual void Test() abstract;
    };
}

