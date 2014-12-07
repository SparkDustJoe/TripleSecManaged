using System;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace HashLibQualityTest
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Process.GetCurrentProcess().ProcessorAffinity = (IntPtr)1;

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new HashLibQualityTestForm());
        }
    }
}
