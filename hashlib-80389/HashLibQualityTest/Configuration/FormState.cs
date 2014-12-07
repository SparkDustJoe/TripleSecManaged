using System;
using System.Configuration;
using System.Windows.Forms;

namespace HashLibQualityTest.Configurations
{
    public class FormState : ConfigurationSection
    {
        private static String SECTION_NAME = "FormState";
        private static FormState s_formState;

        static FormState()
        {
            try
            {
                s_formState = Config.Instance.GetSection(SECTION_NAME) as FormState;
            }
            catch
            {
                s_formState = null;
            }
   
            if (s_formState == null)
            {
                s_formState = new FormState();
                s_formState.SectionInformation.AllowExeDefinition = ConfigurationAllowExeDefinition.MachineToLocalUser;
                Config.Instance.Sections.Remove(SECTION_NAME);
                Config.Instance.Sections.Add(SECTION_NAME, s_formState);
            }  
        }

        public static FormState Instance 
        {
            get
            {
                return s_formState;
            }
        }

        public void Init(Form a_form)
        {
            a_form.Load += OnFormLoad;
            a_form.FormClosing += OnFormClosing;
        }

        private void OnFormLoad(object sender, EventArgs e)
        {
            Form form = sender as Form;
            form.Left = Left;
            form.Top = Top;
            form.Width = Width;
            form.Height = Height;
            form.WindowState = WindowState;
        }

        private void OnFormClosing(object sender, FormClosingEventArgs e)
        {
            Form form = sender as Form;
            WindowState = form.WindowState;

            if (form.WindowState == FormWindowState.Normal)
            {
                Left = form.Left;
                Top = form.Top;
                Width = form.Width;
                Height = form.Height;
            }

            Config.Instance.Save();
        }

        [ConfigurationProperty("WindowState", DefaultValue = FormWindowState.Maximized)]
        public FormWindowState WindowState
        {
            get
            {
                return (FormWindowState)base["WindowState"];
            }
            set
            {
                base["WindowState"] = value;
            }
        }

        [ConfigurationProperty("Left", DefaultValue = 100)]
        public int Left
        {
            get
            {
                return (int)base["Left"];
            }
            set
            {
                base["Left"] = value;
            }
        }

        [ConfigurationProperty("Top", DefaultValue = 100)]
        public int Top
        {
            get
            {
                return (int)base["Top"];
            }
            set
            {
                base["Top"] = value;
            }
        }

        [ConfigurationProperty("Width", DefaultValue = 700)]
        public int Width
        {
            get
            {
                return (int)base["Width"];
            }
            set
            {
                base["Width"] = value;
            }
        }

        [ConfigurationProperty("Height", DefaultValue = 500)]
        public int Height
        {
            get
            {
                return (int)base["Height"];
            }
            set
            {
                base["Height"] = value;
            }
        }

        [ConfigurationProperty("LastTabIndex", DefaultValue = 0)]
        public int LastTabIndex
        {
            get
            {
                return (int)base["LastTabIndex"];
            }
            set
            {
                base["LastTabIndex"] = value;
            }
        }
    }
}
