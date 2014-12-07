using System;
using System.Configuration;

namespace HashLibQualityTest.Configurations
{
    public class Config
    {
        private static Configuration s_config;

        public static Configuration Instance
        {
            get
            {
                if (s_config == null)
                {
                    s_config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.PerUserRoaming);
                }

                return s_config;
            }
        }

        public void Save()
        {
            s_config.Save(ConfigurationSaveMode.Modified, true);
        }
    }
}
