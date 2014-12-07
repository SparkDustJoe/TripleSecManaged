using System;
using System.Linq;
using System.Collections.Generic;
using HashLibQualityTest.Configurations;
using System.Windows.Forms;
using HashLibQualityTest.DataSourceRows;
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace HashLibQualityTest
{
    public partial class HashLibQualityTestForm : Form
    {
        public List<AvalancheTestDataSourceRow> m_avalancheDataSource = 
            new List<AvalancheTestDataSourceRow>();

        public List<AvalancheTestDataSourceRow> m_avalancheCryptoDataSource =
            new List<AvalancheTestDataSourceRow>();

        public List<SpeedTestDataSourceRow> m_speedTestDataSource =
            new List<SpeedTestDataSourceRow>();

        public List<SpeedTestDataSourceRow> m_speedTestCryptoDataSource =
            new List<SpeedTestDataSourceRow>();

        public List<CalculatorDataSourceRow> m_calculatorDataSource =
            new List<CalculatorDataSourceRow>();

        public List<CalculatorDataSourceRow> m_calculatorCryptoDataSource =
            new List<CalculatorDataSourceRow>();

        public List<CalculatorDataSourceRow> m_calculatorKeyDataSource =
            new List<CalculatorDataSourceRow>();

        public HashLibQualityTestForm()
        {
            #if !DEBUG
            Process.GetCurrentProcess().ProcessorAffinity = (IntPtr)1;
            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.RealTime;
            Thread.CurrentThread.Priority = ThreadPriority.Highest;
            #endif

            InitializeComponent();

            FormState.Instance.Init(this);
        }

        private void HashLibQualityTestForm_Load(object sender, EventArgs e)
        {
            string mode = (IntPtr.Size == 8) ? " (64bit)" : " (32bit)";
            Text = Text + mode;

            speedTestDataGridView.AutoGenerateColumns = false;
            speedTestCryptoDataGridView.AutoGenerateColumns = false;
            avalancheDataGridView.AutoGenerateColumns = false;
            avalancheCryptoDataGridView.AutoGenerateColumns = false;
            calculatorDataGridView.AutoGenerateColumns = false;
            calculatorKeyDataGridView.AutoGenerateColumns = false;
            calculatorCryptoDataGridView.AutoGenerateColumns = false;

            m_avalancheDataSource = (from f in HashesList.AvalancheList
                                     select new AvalancheTestDataSourceRow(f)).ToList();

            avalancheDataGridView.DataSource = m_avalancheDataSource;

            m_avalancheCryptoDataSource = (from f in HashesList.AvalancheListCrypto
                                           select new AvalancheTestDataSourceRow(f)).ToList();

            avalancheCryptoDataGridView.DataSource = m_avalancheCryptoDataSource;

            m_speedTestDataSource = (from f in HashesList.SpeedList
                                     select new SpeedTestDataSourceRow(f)).ToList();

            foreach (SpeedTestDataSourceRow row in m_speedTestDataSource)
            {
                if (SpeedTests.Instance.List[row.HashFunction.Name] != null)
                    SpeedTests.Instance.List[row.HashFunction.Name].CopyTo(row);
            }

            speedTestDataGridView.DataSource = m_speedTestDataSource;

            m_speedTestCryptoDataSource = (from f in HashesList.SpeedListCrypto
                                           select new SpeedTestDataSourceRow(f)).ToList();

            foreach (SpeedTestDataSourceRow row in m_speedTestCryptoDataSource)
            {
                if (SpeedTests.Instance.List[row.HashFunction.Name] != null)
                    SpeedTests.Instance.List[row.HashFunction.Name].CopyTo(row);
            }

            speedTestCryptoDataGridView.DataSource = m_speedTestCryptoDataSource;

            m_calculatorDataSource = (from f in HashesList.CalculatorList
                                      select new CalculatorDataSourceRow(f)).ToList();

            calculatorDataGridView.DataSource = m_calculatorDataSource;

            m_calculatorCryptoDataSource = (from f in HashesList.CalculatorListCrypto
                                            select new CalculatorDataSourceRow(f)).ToList();

            calculatorCryptoDataGridView.DataSource = m_calculatorCryptoDataSource;

            m_calculatorKeyDataSource = (from f in HashesList.CalculatorListKey
                                         select new CalculatorDataSourceRow(f)).ToList();

            calculatorKeyDataGridView.DataSource = m_calculatorKeyDataSource;

            hashFileTextBox.Text = LastOpenFiles.Instance.HashFile;
            hashCryptoFileTextBox.Text = LastOpenFiles.Instance.HashFileCrypto;
            hashKeyFileTextBox.Text = LastOpenFiles.Instance.HashFileCrypto;

            hashTextBox.Text = LastOpenFiles.Instance.HashText;
            hashCryptoTextBox.Text = LastOpenFiles.Instance.HashTextCrypto;
            hashKeyTextBox.Text = LastOpenFiles.Instance.HashTextKey;

            keyTextBox.Text = LastOpenFiles.Instance.HashKey;

            tabControl.SelectedIndex = FormState.Instance.LastTabIndex;
        }

        private void speedTestbutton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;

            try
            {
                foreach (SpeedTestDataSourceRow row in m_speedTestDataSource)
                {
                    if (row.Calculate)
                    {
                        new SpeedTest().Test(row);

                        SpeedTests.Instance.List.Add(row);
                        Config.Instance.Save();

                        speedTestDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.None;
                        speedTestDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                        speedTestDataGridView.Invalidate();
                    }

                    Application.DoEvents();
                }
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
            }
        }

        private void avalancheTestButton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (AvalancheTestDataSourceRow row in m_avalancheDataSource)
                {
                    if (row.Calculate)
                    {
                        new AvalancheTest().Test(row);
                        row.Save();

                        avalancheDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.None;
                        avalancheDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                        avalancheDataGridView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.None;
                        avalancheDataGridView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
                        avalancheDataGridView.Invalidate();

                        Application.DoEvents();
                    }
                }
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
            }
        }

        private void speedTestDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_speedTestDataSource, e, speedTestDataGridView);
        }

        private void avalancheDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_avalancheDataSource, e, avalancheDataGridView);
        }

        private void calculatorDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_calculatorDataSource, e, calculatorDataGridView);
        }

        private void DataGridViewColumnHeaderMouseClick<T>(
            List<T> a_dataSource, DataGridViewCellMouseEventArgs e, DataGridView a_dataGridView) where T : TestDataSourceRow
        {
            a_dataGridView.EndEdit();

            if (e.ColumnIndex == 0)
            {
                if (a_dataSource.All(r => r.Calculate))
                    a_dataSource.ForEach(r => r.Calculate = false);
                else 
                    a_dataSource.ForEach(r => r.Calculate = true);
            }
            else
            {
                if (a_dataGridView.Columns[e.ColumnIndex].SortMode != DataGridViewColumnSortMode.NotSortable)
                {
                    a_dataGridView.Columns[e.ColumnIndex].HeaderCell.SortGlyphDirection =
                        a_dataGridView.Columns[e.ColumnIndex].HeaderCell.SortGlyphDirection ==
                            SortOrder.Ascending ? SortOrder.Descending : SortOrder.Ascending;

                    List<object> tosort = new List<object>();
                    for (int row = 0; row < a_dataGridView.Rows.Count; row++)
                        tosort.Add(a_dataGridView.Rows[row].Cells[e.ColumnIndex].Value);

                    Sort(a_dataSource, tosort, a_dataGridView.Columns[e.ColumnIndex].HeaderCell.SortGlyphDirection);
                }
            }

            a_dataGridView.Invalidate();
        }

        private void Sort<T>(List<T> a_list, List<object> a_sort, SortOrder a_sortOrder)
        {
            if (a_sortOrder == SortOrder.Ascending)
            {
                List<T> l =  a_list.Select((obj, index) => new { obj = a_list[index], sort_obj = a_sort[index] }).
                    OrderBy(gr => gr.sort_obj).Select(gr => gr.obj).ToList();
                a_list.Clear();
                a_list.AddRange(l);
            }
            else
            {
                List<T> l = a_list.Select((obj, index) => new { obj = a_list[index], sort_obj = a_sort[index] }).
                    OrderByDescending(gr => gr.sort_obj).Select(gr => gr.obj).ToList();
                a_list.Clear();
                a_list.AddRange(l);
            }
        }

        private void hashCalculateButton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (CalculatorDataSourceRow row in m_calculatorDataSource)
                    if (row.Calculate)
                        row.CalculateHashFromText(hashTextBox.Text);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                calculatorDataGridView.Invalidate();
            }
        }

        private void hashCalculateFileButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (!new FileInfo(hashFileTextBox.Text).Exists)
                {
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
                if (new DirectoryInfo(hashFileTextBox.Text).Exists)
                {
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
            }
            catch
            {
                System.Media.SystemSounds.Exclamation.Play();
                return;
            }

            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (CalculatorDataSourceRow row in m_calculatorDataSource)
                    if (row.Calculate)
                        row.CalculateHashFromFile(hashFileTextBox.Text);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
                calculatorDataGridView.Invalidate();
            }
        }

        private void hashFileChooseButton_Click(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.Check();

            openFileDialog.InitialDirectory = LastOpenFiles.Instance.HashFileDir;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = openFileDialog.FileName;

                if (fileName.Equals(""))
                    return;


                LastOpenFiles.Instance.HashFile = fileName;
                openFileDialog.FileName = new FileInfo(fileName).Name;

                hashFileTextBox.Text = fileName;
            }
        }

        private void hashTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashText = hashTextBox.Text;
        }

        private void hashFileTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashFile = hashFileTextBox.Text;
        }

        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            FormState.Instance.LastTabIndex = tabControl.SelectedIndex;
        }

        private void speedTestCryptoDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_speedTestCryptoDataSource, e, speedTestCryptoDataGridView);
        }

        private void speedTestCryptoButton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (SpeedTestDataSourceRow row in m_speedTestCryptoDataSource)
                {
                    if (row.Calculate)
                    {
                        new SpeedTest().Test(row);

                        SpeedTests.Instance.List.Add(row);
                        Config.Instance.Save();

                        speedTestCryptoDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.None;
                        speedTestCryptoDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                        speedTestCryptoDataGridView.Invalidate();
                    }

                    Application.DoEvents();
                }
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
            }
        }

        private void avalancheCryptoDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_avalancheCryptoDataSource, e, avalancheCryptoDataGridView);
        }

        private void avalancheTestCryptoButton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (AvalancheTestDataSourceRow row in m_avalancheCryptoDataSource)
                {
                    if (row.Calculate)
                    {
                        new AvalancheTest().Test(row);
                        row.Save();

                        avalancheCryptoDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.None;
                        avalancheCryptoDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                        avalancheCryptoDataGridView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.None;
                        avalancheCryptoDataGridView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
                        avalancheCryptoDataGridView.Invalidate();
                    }
                }
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
            }
        }

        private void hashCryptoFileTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashFileCrypto = hashCryptoFileTextBox.Text;
        }

        private void hashCryptoTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashTextCrypto = hashCryptoTextBox.Text;
        }

        private void hashKeyTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashTextKey = hashKeyTextBox.Text;
        }

        private void hashCryptoFileChooseButton_Click(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.Check();

            openFileDialog.InitialDirectory = LastOpenFiles.Instance.HashFileDirCrypto;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = openFileDialog.FileName;

                if (fileName.Equals(""))
                    return;

                LastOpenFiles.Instance.HashFileCrypto = fileName;
                openFileDialog.FileName = new FileInfo(fileName).Name;

                hashCryptoFileTextBox.Text = fileName;
            }
        }

        private void hashKeyFileChooseButton_Click(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.Check();

            openFileDialog.InitialDirectory = LastOpenFiles.Instance.HashFileDirKey;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = openFileDialog.FileName;

                if (fileName.Equals(""))
                    return;

                LastOpenFiles.Instance.HashFileKey = fileName;
                openFileDialog.FileName = new FileInfo(fileName).Name;

                hashKeyFileTextBox.Text = fileName;
            }
        }

        private void hashCryptoCalculateFileButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (!new FileInfo(hashCryptoFileTextBox.Text).Exists)
                {
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
                if (new DirectoryInfo(hashCryptoFileTextBox.Text).Exists)
                {
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
            }
            catch
            {
                System.Media.SystemSounds.Exclamation.Play();
                return;
            }

            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (CalculatorDataSourceRow row in m_calculatorCryptoDataSource)
                    if (row.Calculate)
                        row.CalculateHashFromFile(hashCryptoFileTextBox.Text);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
                calculatorCryptoDataGridView.Invalidate();
            }
        }

        private void hashKeyCalculateFileButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (!new FileInfo(hashKeyFileTextBox.Text).Exists)
                {
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
                if (new DirectoryInfo(hashKeyFileTextBox.Text).Exists)
                {
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
            }
            catch
            {
                System.Media.SystemSounds.Exclamation.Play();
                return;
            }

            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (CalculatorDataSourceRow row in m_calculatorKeyDataSource)
                    if (row.Calculate)
                        row.CalculateKeyFromFile(hashKeyFileTextBox.Text, keyTextBox.Text);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                System.Media.SystemSounds.Beep.Play();
                calculatorKeyDataGridView.Invalidate();
            }
        }

        private void hashCryptoCalculateButton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (CalculatorDataSourceRow row in m_calculatorCryptoDataSource)
                    if (row.Calculate)
                        row.CalculateHashFromText(hashCryptoTextBox.Text);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                calculatorCryptoDataGridView.Invalidate();
            }
        }

        private void hashKeyCalculateButton_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                foreach (CalculatorDataSourceRow row in m_calculatorKeyDataSource)
                    if (row.Calculate)
                        row.CalculateKeyFromText(hashKeyTextBox.Text, keyTextBox.Text);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
                calculatorKeyDataGridView.Invalidate();
            }
        }

        private void hashKeyFileTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashFileKey = hashKeyFileTextBox.Text;
        }

        private void keyTextBox_TextChanged(object sender, EventArgs e)
        {
            LastOpenFiles.Instance.HashKey = keyTextBox.Text;
        }

        private void calculatorCryptoDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_calculatorCryptoDataSource, e, calculatorCryptoDataGridView);
        }

        private void calculatorKeyDataGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumnHeaderMouseClick(m_calculatorKeyDataSource, e, calculatorKeyDataGridView);
        }

        private void hashTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                hashCalculateButton_Click(sender, e);
        }

        private void hashCryptoTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                hashCryptoCalculateButton_Click(sender, e);
        }

        private void hashKeyTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                hashKeyCalculateButton_Click(sender, e);
        }

        private void keyTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                hashKeyCalculateButton_Click(sender, e);
        }
    }
}