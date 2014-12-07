using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;

namespace HashLibQualityTest
{
    public class BitmapData
    {
        private byte[] m_values;
        private int m_delta;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        protected int m_width;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        protected int m_height;

        public int Width
        {
            [DebuggerStepThrough]
            get
            {
                return m_width;
            }
        }

        public int Height
        {
            [DebuggerStepThrough]
            get
            {
                return m_height;
            }
        }

        public BitmapData(int a_width, int a_height)
        {
            using (Bitmap bmp = new Bitmap(a_width, a_height))
            {
                Create(bmp);
            }
        }

        public BitmapData(Bitmap a_bmp)
        {
            Create(a_bmp);
        }

        private void Create(Bitmap a_bmp)
        {
            m_width = a_bmp.Size.Width;
            m_height = a_bmp.Size.Height;

            System.Drawing.Imaging.BitmapData bd = a_bmp.LockBits(new Rectangle(0, 0, m_width, m_height), 
                ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);

            try
            {
                IntPtr ptr = bd.Scan0;
                int bytes = bd.Stride * a_bmp.Height;
                m_values = new byte[bytes];
                System.Runtime.InteropServices.Marshal.Copy(ptr, m_values, 0, bytes);
                m_delta = (bd.Stride - m_width * 3);
            }
            finally
            {
                a_bmp.UnlockBits(bd);
            }
        }

        public Bitmap Bitmap
        {
            get
            {
                Bitmap bmp = new Bitmap(Width, Height);

                System.Drawing.Imaging.BitmapData bd = bmp.LockBits(new Rectangle(0, 0, Width, Height),
                    ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);

                try
                {
                    IntPtr ptr = bd.Scan0;
                    int bytes = bd.Stride * bmp.Height;
                    System.Runtime.InteropServices.Marshal.Copy(m_values, 0, ptr, bytes);
                }
                finally
                {
                    bmp.UnlockBits(bd);
                }

                return bmp;
            }
        }

        public Color GetPixel(int a_x, int a_y)
        {
            int index = a_y * ((m_width * 3) + m_delta) + (a_x * 3);
            return Color.FromArgb(m_values[index + 2], m_values[index + 1], m_values[index]);
        }

        public void SetPixel(int a_x, int a_y, Color a_color)
        {
            int index = a_y * ((m_width * 3) + m_delta) + (a_x * 3);
            m_values[index++] = a_color.B;
            m_values[index++] = a_color.G;
            m_values[index] = a_color.R;
            
            
        }
    }
}
