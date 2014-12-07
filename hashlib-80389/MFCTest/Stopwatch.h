
#pragma once 

namespace MFCTest
{
    class Stopwatch
    {
        private:
 
            LARGE_INTEGER start;
            LARGE_INTEGER stop;
            LARGE_INTEGER frequency;
            bool started;
            bool stopped;

        public:
 
            Stopwatch(bool autoStart = true)
            {
                start.QuadPart = 0;
                stop.QuadPart = 0;
                started = false;
                stopped = false;
 
                QueryPerformanceFrequency(&frequency);
 
                if (autoStart)
                    Start();
            }
 
            void Start()
            {
                if (started)
                    AfxThrowNotSupportedException();
 
                started = true;
                stopped = false;
 
                QueryPerformanceCounter(&start);
            }
 
            void Stop()
            {
                if (!started)
                    AfxThrowNotSupportedException();

                QueryPerformanceCounter(&stop);
 
                started = false;
                stopped = true;
            }
 
            double GetElapsedMilliseconds()
            {
                if (!started && !stopped)
                    AfxThrowNotSupportedException();

                if (!stopped)
                    QueryPerformanceCounter(&stop);
 
                return (double)(stop.QuadPart - start.QuadPart) * 1000.0 / (double)frequency.QuadPart;
            }

            void Restart()
            {
                Stop();
                Start();
            }
    };
}