using System;
using HashLibQualityTest.DataSourceRows;
using System.Drawing;
using HashLib;

namespace HashLibQualityTest
{
    public class AvalancheTest
    {
        private Random m_random = new Random();

        public void Test(AvalancheTestDataSourceRow a_row)
        {
            if (!(a_row.HashFunction is HashLib.ICrypto))
            {
                a_row.BmpByte = TestBytes(a_row, 1);
                a_row.BmpUShort = TestBytes(a_row, 2);
                a_row.BmpUInt = TestBytes(a_row, 4);
                a_row.BmpULong = TestBytes(a_row, 8);
                a_row.BmpDouble = TestBytes(a_row, 8);
            }

            a_row.BmpString = TestBytes(a_row, 32);
            a_row.Bmp128 = TestBytes(a_row, 16);
            a_row.Bmp256 = TestBytes(a_row, 32);
        }

        private Bitmap ScaleBitmap(Bitmap a_bmp)
        {
            const int SCALE = 3;

            using (a_bmp)
            {
                Bitmap bmp2 = new Bitmap(a_bmp.Width * SCALE, a_bmp.Height * SCALE);

                using (Graphics g2 = Graphics.FromImage(bmp2))
                {
                    g2.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                    g2.DrawImage(a_bmp, new Rectangle(0, 0, a_bmp.Width * SCALE, a_bmp.Height * SCALE));
                }

                return bmp2;
            }
        }

        public Bitmap TestBytes(AvalancheTestDataSourceRow a_row, int a_value_length)
        {
            double[,] output = CalculateAvalancheArrayBytes(a_row, a_value_length);
            Bitmap bmp = PrepareBitapFromAvalancheArray(output);
            return ScaleBitmap(bmp);
        }

        private double[,] CalculateAvalancheArrayBytes(AvalancheTestDataSourceRow a_row, int a_value_length)
        {
            double[,] output = new double[a_value_length * 8, a_row.HashFunction.HashSize * 8];

            byte[] value = new byte[a_value_length];
            byte[] value_alter = new byte[a_value_length];

            const int TRIES = 10000;
            const double STEP = 1.0 / TRIES;

            IHash hb = a_row.HashFunction;

            for (int tries = 0; tries < TRIES; tries++)
            {
                m_random.NextBytes(value);
                Array.Copy(value, value_alter, value.Length);

                byte[] hash = hb.ComputeBytes(value).GetBytes();

                int input_bit_index_in_array = 0;

                for (int input_byte_index_in_array = 0; input_byte_index_in_array < value.Length; input_byte_index_in_array++)
                {
                    uint input_bit_value_in_byte = 1;

                    for (int input_bit_index_in_byte = 0; input_bit_index_in_byte < 8; input_bit_index_in_byte++)
                    {
                        value_alter[input_byte_index_in_array] = (byte)(value[input_byte_index_in_array] ^ input_bit_value_in_byte);

                        byte[] hash_alter = hb.ComputeBytes(value_alter).GetBytes();

                        int output_bit_index_in_array = 0;

                        for (int output_byte_index_in_array = 0; output_byte_index_in_array < hash.Length; output_byte_index_in_array++)
                        {
                            uint output_bit_value_in_byte = 1;

                            for (int output_bit_index_in_byte = 0; output_bit_index_in_byte < 8; output_bit_index_in_byte++)
                            {
                                if ((hash[output_byte_index_in_array] & output_bit_value_in_byte) !=
                                    (hash_alter[output_byte_index_in_array] & output_bit_value_in_byte))
                                {
                                    output[input_bit_index_in_array, output_bit_index_in_array] += STEP;
                                }

                                output_bit_value_in_byte <<= 1;
                                output_bit_index_in_array++;
                            }
                        }

                        input_bit_index_in_array++;
                        input_bit_value_in_byte <<= 1;
                    }

                    value_alter[input_byte_index_in_array] = value[input_byte_index_in_array];
                }
            }

            return output;
        }

        private Bitmap PrepareBitapFromAvalancheArray(double[,] a_output)
        {
            BitmapData bd = new BitmapData(a_output.GetLength(0), a_output.GetLength(1));

            for (int i = 0; i < bd.Width; i++)
            {
                for (int j = 0; j < bd.Height; j++)
                {
                    double delta = Math.Abs(a_output[i, j] - 0.5);

                    int r = 0;
                    int g = 0;
                    int b = 0;

                    if (delta < 0.05)
                        g = (int)Math.Round(delta / 0.05 * 255);
                    else if (delta < 0.33)
                    {
                        g = 255 - (int)Math.Round((delta - 0.05) / (0.33 - 0.05) * 255);
                        r = (int)Math.Round((delta - 0.05) / (0.33 - 0.05) * 255);
                    } 
                    else
                    {
                        g = (int)Math.Round((delta - 0.33) / (0.5 - 0.33) * 255);
                        r = 255;
                    }

                    r = Math.Min(r, 255);
                    g = Math.Min(g, 255);
                    b = Math.Min(b, 255);

                    r = Math.Max(r, 0);
                    g = Math.Max(g, 0);
                    b = Math.Max(b, 0);

                    bd.SetPixel(i, j, Color.FromArgb(r, g, b));
                }
            }

            return bd.Bitmap;
        }
    }
}
