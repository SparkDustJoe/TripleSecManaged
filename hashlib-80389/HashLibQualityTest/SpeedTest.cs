using System;
using HashLibQualityTest.DataSourceRows;
using HashLib;
using System.Diagnostics;
using System.Linq;
using RDTSCLib;
using TomanuExtensions.Utils;
using System.Threading;
using TomanuExtensions;

namespace HashLibQualityTest
{
    public class SpeedTest
    {
        const int MEASURE_TIME = 1000;
        const int MIN_REPEATS = 3;
        const int BYTES_ARRAY_LENGTH_CYCLES_PER_SECOND = 100000;
        const int BYTES_ARRAY_LENGTH = 12000000;
        const int SINGLE_ELEMENT_RATIO = 5;

        private static void HighPriority()
        {
            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.High;
            Thread.CurrentThread.Priority = ThreadPriority.AboveNormal;
        }

        private static void NormalPriority()
        {
            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.High;
            Thread.CurrentThread.Priority = ThreadPriority.AboveNormal;
        }

        private string Measure(int a_bytes, Action a_action)
        {
            Stopwatch sw = new Stopwatch();

            int repeats = 0;

            a_action();

            for (; ; )
            {
                HighPriority();

                try
                {
                    sw.Start();
                    a_action();
                    sw.Stop();
                }
                finally
                {
                    NormalPriority();
                }

                repeats++;

                if (sw.ElapsedMilliseconds > MEASURE_TIME)
                    if (repeats > MIN_REPEATS)
                        break;
            }

            double mbs = a_bytes / 1024.0 / 1024.0 * repeats;
            double seconds = sw.ElapsedMilliseconds / 1000.0;

            return String.Format((mbs / seconds).ToString("F2"));
        }

        public void Test(SpeedTestDataSourceRow a_row)
        {
            if (a_row.HashFunction is ICrypto)
                MeasureBytesPerCycle(a_row);

            Measure(a_row); 
        }

        private void MeasureBytesPerCycle(SpeedTestDataSourceRow a_row)
        {
            Stopwatch sw = new Stopwatch();
            IHash hash = a_row.HashFunction;
            MersenneTwister r = new MersenneTwister();

            hash.Initialize();

            var msg = r.NextBytes(BYTES_ARRAY_LENGTH_CYCLES_PER_SECOND);

            RDTSC.rdtsc();
            hash.ComputeBytes(msg);

            int repeats = 0;
            ulong min_delta = UInt64.MaxValue;

            for (; ; )
            {
                HighPriority();

                try
                {
                    sw.Start();
                    ulong before = RDTSC.rdtsc();
                    hash.ComputeBytes(msg);
                    ulong after = RDTSC.rdtsc();
                    sw.Stop();
                    ulong delta = after - before;
                    min_delta = Math.Min(min_delta, delta);
                }
                finally
                {
                    NormalPriority();
                }

                repeats++;

                if (sw.ElapsedMilliseconds > MEASURE_TIME)
                    if (repeats > MIN_REPEATS)
                        break;
            }

            a_row.BytesPerCycle = ((double)min_delta / BYTES_ARRAY_LENGTH_CYCLES_PER_SECOND).ToString("F2");
        }

        private void Measure(SpeedTestDataSourceRow a_row)
        {
            MersenneTwister r = new MersenneTwister();

            {
                var ar = r.NextBytes(BYTES_ARRAY_LENGTH);

                a_row.BytesSpeed = Measure(ar.Length, () =>
                {
                    a_row.HashFunction.ComputeBytes(ar);
                });
            }

            IFastHash32 hash = a_row.HashFunction as IFastHash32;

            if (hash == null)
                return;

            {
                var ar = r.NextBytes(BYTES_ARRAY_LENGTH / SINGLE_ELEMENT_RATIO);

                a_row.ByteSpeed = Measure(ar.Length, () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeByteFast(ar[i]);
                });
            }

            {
                var ar = r.NextChars(BYTES_ARRAY_LENGTH / sizeof(char));

                a_row.CharsSpeed = Measure(ar.Length * sizeof(char), () =>
                {
                    hash.ComputeCharsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.CharSpeed = Measure(ar.Length * sizeof(char), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeCharFast(ar[i]);
                });
            }

            {
                var ar = r.NextShorts(BYTES_ARRAY_LENGTH / sizeof(short));

                a_row.ShortsSpeed = Measure(ar.Length * sizeof(short), () =>
                {
                    hash.ComputeShortsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.ShortSpeed = Measure(ar.Length * sizeof(short), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeShortFast(ar[i]);
                });
            }

            {
                var ar = r.NextUShorts(BYTES_ARRAY_LENGTH / sizeof(ushort));

                a_row.UShortsSpeed = Measure(ar.Length * sizeof(ushort), () =>
                {
                    hash.ComputeUShortsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.UShortSpeed = Measure(ar.Length * sizeof(ushort), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeUShortFast(ar[i]);
                });
            }

            {
                var ar = r.NextInts(BYTES_ARRAY_LENGTH / sizeof(int));

                a_row.IntsSpeed = Measure(ar.Length * sizeof(int), () =>
                {
                    hash.ComputeIntsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.IntSpeed = Measure(ar.Length * sizeof(int), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeIntFast(ar[i]);
                });
            }

            {
                var ar = r.NextUInts(BYTES_ARRAY_LENGTH / sizeof(uint));

                a_row.UIntsSpeed = Measure(ar.Length * sizeof(uint), () =>
                {
                    hash.ComputeUIntsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.UIntSpeed = Measure(ar.Length * sizeof(uint), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeUIntFast(ar[i]);
                });
            }

            {
                var ar = r.NextLongs(BYTES_ARRAY_LENGTH / sizeof(long));

                a_row.LongsSpeed = Measure(ar.Length * sizeof(long), () =>
                {
                    hash.ComputeLongsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.LongSpeed = Measure(ar.Length * sizeof(long), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeLongFast(ar[i]);
                });
            }

            {
                var ar = r.NextULongs(BYTES_ARRAY_LENGTH / sizeof(ulong));

                a_row.ULongsSpeed = Measure(ar.Length * sizeof(ulong), () =>
                {
                    hash.ComputeULongsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.ULongSpeed = Measure(ar.Length * sizeof(ulong), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeULongFast(ar[i]);
                });
            }

            {
                var ar = r.NextFloatsFullSafe(BYTES_ARRAY_LENGTH / sizeof(float));

                a_row.FloatsSpeed = Measure(ar.Length * sizeof(float), () =>
                {
                    hash.ComputeFloatsFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.FloatSpeed = Measure(ar.Length * sizeof(float), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeFloatFast(ar[i]);
                });
            }

            {
                var ar = r.NextDoublesFullSafe(BYTES_ARRAY_LENGTH / sizeof(double));

                a_row.DoublesSpeed = Measure(ar.Length * sizeof(double), () =>
                {
                    hash.ComputeDoublesFast(ar);
                });

                ar = ArrayExtensions.SubArray(ar, 0, ar.Length / SINGLE_ELEMENT_RATIO);

                a_row.DoubleSpeed = Measure(ar.Length * sizeof(double), () =>
                {
                    for (int i = 0; i < ar.Length; i++)
                        hash.ComputeDoubleFast(ar[i]);
                });
            }

            {
                var ar = r.NextString(BYTES_ARRAY_LENGTH / sizeof(char));

                a_row.StringSpeed = Measure(ar.Length * sizeof(char), () =>
                {
                    hash.ComputeStringFast(ar);
                });
            }
        }
    }
}