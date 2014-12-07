
#pragma once

namespace Trans
{
    ref class HashBuffer
    {
        private: 
            
            array<byte>^ m_data;
            int m_pos;

        public:

            HashBuffer(int a_length)
            {
                Debug::Assert(a_length > 0);

                m_data = gcnew array<byte>(a_length);

                Initialize();
            }
            
            void Initialize()
            {
                m_pos = 0;
            }

            property int Length
            {
                int get()
                {
                    return m_data->Length;
                }
            }

            array<byte>^ GetBytes()
            {
                Debug::Assert(IsFull);

                m_pos = 0;
                return m_data;
            }

            array<byte>^ GetBytesZeroPadded()
            {
                Array::Clear(m_data, m_pos, m_data->Length - m_pos);
                m_pos = 0;
                return m_data;
            }

            bool Feed(array<byte>^ a_data, int& a_startIndex, int& a_length, ulong& a_processedBytes)
            {
                Debug::Assert(a_data != nullptr);
                Debug::Assert(a_startIndex >= 0);
                Debug::Assert(a_length >= 0);
                Debug::Assert(a_startIndex + a_length <= a_data->Length);

                if (m_pos == m_data->Length)
                    return true;

                if (a_data->Length == 0)
                    return false;

                if (a_length == 0)
                    return false;

                int length = m_data->Length - m_pos;
                if (length > a_length)
                    length = a_length;

                Array::Copy(a_data, a_startIndex, m_data, m_pos, length);

                m_pos += length;
                a_startIndex += length;
                a_length -= length;
                a_processedBytes += (ulong)length;

                return IsFull;
            }

            bool Feed(array<byte>^ a_data, int& a_startIndex, int& a_length)
            {
                Debug::Assert(a_data != nullptr);
                Debug::Assert(a_startIndex >= 0);
                Debug::Assert(a_length >= 0);
                Debug::Assert(a_startIndex + a_length <= a_data->Length);

                if (m_pos == m_data->Length)
                    return true;

                if (a_data->Length == 0)
                    return false;

                if (a_length == 0)
                    return false;

                int length = m_data->Length - m_pos;
                if (length > a_length)
                    length = a_length;

                Array::Copy(a_data, a_startIndex, m_data, m_pos, length);

                m_pos += length;
                a_startIndex += length;
                a_length -= length;

                return IsFull;
            }

            bool Feed(array<byte>^ a_data, int a_length)
            {
                Debug::Assert(a_data != nullptr);
                Debug::Assert(a_length >= 0);
                Debug::Assert(a_length <= a_data->Length);

                if (m_pos == m_data->Length)
                    return true;

                if (a_data->Length == 0)
                    return false;

                if (a_length == 0)
                    return false;

                int length = m_data->Length - m_pos;
                if (length > a_length)
                    length = a_length;

                Array::Copy(a_data, 0, m_data, m_pos, length);

                m_pos += length;

                return IsFull;
            }

            property bool IsEmpty
            {
                bool get()
                {
                    return m_pos == 0;
                }
            }

            property int Pos
            {
                int get()
                {
                    return m_pos;
                }
            }

            property bool IsFull
            {
                bool get()
                {
                    return (m_pos == m_data->Length);
                }
            }
    };
}
