using System;
using System.IO;

namespace HashLibQualityTest
{
    public static class FileUtils
    {
        public static bool IsFilePathValid(string a_path)
        {
            if (a_path.Trim() == string.Empty)
            {
                return false;
            }

            string pathname;
            string filename;
            try
            {
                pathname = Path.GetPathRoot(a_path);
                filename = Path.GetFileName(a_path);
            }
            catch (ArgumentException)
            {
                // GetPathRoot() and GetFileName() above will throw exceptions
                // if pathname/filename could not be parsed.

                return false;
            }

            // Make sure the filename part was actually specified
            if (filename.Trim() == string.Empty)
            {
                return false;
            }

            // Not sure if additional checking below is needed, but no harm done
            if (pathname.IndexOfAny(Path.GetInvalidPathChars()) >= 0)
            {
                return false;
            }

            if (filename.IndexOfAny(Path.GetInvalidFileNameChars()) >= 0)
            {
                return false;
            }

            return true;
        }

        public static string FindExistingDirectory(this DirectoryInfo a_di)
        {
            string str = a_di.FullName;

            for (;;)
            {
                DirectoryInfo di = new DirectoryInfo(str);

                if (di.Exists)
                    return str;

                if (di.Parent == null)
                    return "";

                str = di.Parent.FullName;

            }
        }

    }
}
