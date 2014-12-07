using System;
using System.Configuration;

namespace HashLibQualityTest.Configurations
{
    public class LastOpenFiles : ConfigurationSection
    {
        private static String SECTION_NAME = "LastOpenFiles";
        private static LastOpenFiles s_lastOpenFiles;

        static LastOpenFiles()
        {
            try
            {
                s_lastOpenFiles = Config.Instance.GetSection(SECTION_NAME) as LastOpenFiles;
            }
            catch
            {
                s_lastOpenFiles = null;
            }

            if (s_lastOpenFiles == null)
            {
                s_lastOpenFiles = new LastOpenFiles();
                s_lastOpenFiles.SectionInformation.AllowExeDefinition = ConfigurationAllowExeDefinition.MachineToLocalUser;
                Config.Instance.Sections.Remove(SECTION_NAME);
                Config.Instance.Sections.Add(SECTION_NAME, s_lastOpenFiles);
            }

            s_lastOpenFiles.Check();
        }

        public static LastOpenFiles Instance
        {
            get
            {
                return s_lastOpenFiles;
            }
        }

        [ConfigurationProperty("HashFile", DefaultValue = "")]
        public string HashFile
        {
            get
            {
                return (string)base["HashFile"];
            }
            set
            {
                base["HashFile"] = value;
                try
                {
                    if (TomanuExtensions.Utils.FileUtils.IsFilePathValid(value))
                        HashFileDir = new System.IO.FileInfo(value).DirectoryName;
                }
                catch (Exception)
                {
                    HashFileDir = "";
                }

                if (HashFile != CheckFileName(HashFile))
                    HashFile = CheckFileName(HashFile);

                Config.Instance.Save();
            }
        }

        [ConfigurationProperty("HashText", DefaultValue = "")]
        public string HashText
        {
            get
            {
                return (string)base["HashText"];
            }
            set
            {
                base["HashText"] = value;
            }
        }

        [ConfigurationProperty("HashFileDir", DefaultValue = "")]
        public string HashFileDir
        {
            get
            {
                return (string)base["HashFileDir"];
            }
            set
            {
                base["HashFileDir"] = value;

                if (HashFileDir != CheckDirectory(HashFileDir))
                    HashFileDir = CheckDirectory(HashFileDir);
            }
        }

        [ConfigurationProperty("HashFileCrypto", DefaultValue = "")]
        public string HashFileCrypto
        {
            get
            {
                return (string)base["HashFileCrypto"];
            }
            set
            {
                base["HashFileCrypto"] = value;
                try
                {
                    if (TomanuExtensions.Utils.FileUtils.IsFilePathValid(value))
                        HashFileDirCrypto = new System.IO.FileInfo(value).DirectoryName;
                }
                catch (Exception)
                {
                    HashFileDirCrypto = "";
                }

                if (HashFileCrypto != CheckFileName(HashFileCrypto))
                    HashFileCrypto = CheckFileName(HashFileCrypto);

                Config.Instance.Save();
            }
        }

        [ConfigurationProperty("HashTextCrypto", DefaultValue = "")]
        public string HashTextCrypto
        {
            get
            {
                return (string)base["HashTextCrypto"];
            }
            set
            {
                base["HashTextCrypto"] = value;
            }
        }

        [ConfigurationProperty("HashFileDirCrypto", DefaultValue = "")]
        public string HashFileDirCrypto
        {
            get
            {
                return (string)base["HashFileDirCrypto"];
            }
            set
            {
                base["HashFileDirCrypto"] = value;

                if (HashFileDirCrypto != CheckDirectory(HashFileDirCrypto))
                    HashFileDirCrypto = CheckDirectory(HashFileDirCrypto);
            }
        }

        [ConfigurationProperty("HashFileKey", DefaultValue = "")]
        public string HashFileKey
        {
            get
            {
                return (string)base["HashFileKey"];
            }
            set
            {
                base["HashFileKey"] = value;
                try
                {
                    if (TomanuExtensions.Utils.FileUtils.IsFilePathValid(value))
                        HashFileDirKey = new System.IO.FileInfo(value).DirectoryName;
                }
                catch (Exception)
                {
                    HashFileDirKey = "";
                }

                if (HashFileKey != CheckFileName(HashFileKey))
                    HashFileKey = CheckFileName(HashFileKey);

                Config.Instance.Save();
            }
        }

        [ConfigurationProperty("HashTextKey", DefaultValue = "")]
        public string HashTextKey
        {
            get
            {
                return (string)base["HashTextKey"];
            }
            set
            {
                base["HashTextKey"] = value;
            }
        }

        [ConfigurationProperty("HashFileDirKey", DefaultValue = "")]
        public string HashFileDirKey
        {
            get
            {
                return (string)base["HashFileDirKey"];
            }
            set
            {
                base["HashFileDirKey"] = value;

                if (HashFileDirKey != CheckDirectory(HashFileDirKey))
                    HashFileDirKey = CheckDirectory(HashFileDirKey);
            }
        }

        [ConfigurationProperty("HashKey", DefaultValue = "")]
        public string HashKey
        {
            get
            {
                return (string)base["HashKey"];
            }
            set
            {
                base["HashKey"] = value;
            }
        }

        private string CheckFileName(string a_file_name)
        {
            try
            {
                if (!TomanuExtensions.Utils.FileUtils.IsFilePathValid(a_file_name) ||
                    !new System.IO.FileInfo(a_file_name).Exists)
                {
                    return string.Empty;
                }

                return a_file_name;
            }
            catch (Exception)
            {
                return String.Empty;
            }
        }

        private string CheckDirectory(string a_file_name)
        {
            try
            {
                if (TomanuExtensions.Utils.FileUtils.IsFilePathValid(a_file_name))
                    return new System.IO.DirectoryInfo(a_file_name).FindExistingDirectory();
                else
                    return AppDomain.CurrentDomain.BaseDirectory + "Examples";
            }
            catch (Exception)
            {
                return string.Empty;
            }
        }

        public void Check()
        {
            HashFile = CheckFileName(HashFile);
            HashFileCrypto = CheckFileName(HashFileCrypto);
            HashFileKey = CheckFileName(HashFileKey);

            HashFileDir = CheckDirectory(HashFileDir);
            HashFileDirCrypto = CheckDirectory(HashFileDirCrypto);
            HashFileDirKey = CheckDirectory(HashFileDirKey);
        }
    }
}