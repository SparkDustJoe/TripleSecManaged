using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using HashLib;

namespace HashLibQualityTest.DataSourceRows
{
    public class AvalancheTestDataSourceRow : TestDataSourceRow
    {
        public AvalancheTestDataSourceRow(IHash a_hashFunction)
            : base(a_hashFunction)
        {
            BmpByte = LoadBitmap("Byte");
            BmpUShort = LoadBitmap("UShort");
            BmpUInt = LoadBitmap("UInt");
            BmpULong = LoadBitmap("ULong");
            BmpDouble = LoadBitmap("Double");
            Bmp128 = LoadBitmap("128");
            Bmp256 = LoadBitmap("256");
            BmpString = LoadBitmap("String");
        }

        public Bitmap BmpByte { get; set; }
        public Bitmap BmpUShort { get; set; }
        public Bitmap BmpUInt { get; set; }
        public Bitmap BmpULong { get; set; }
        public Bitmap BmpDouble { get; set; }
        public Bitmap Bmp128 { get; set; }
        public Bitmap Bmp256 { get; set; }
        public Bitmap BmpString { get; set; }

        private Bitmap LoadBitmap(string a_name)
        {
            a_name = GetAvalancheBmpFileName(a_name);

            if (new FileInfo(a_name).Exists)
            {
                using (FileStream fs = new FileStream(a_name, FileMode.Open))
                    return new Bitmap(fs);
            }
            else
                return new Bitmap(8, 1);
        }

        private string GetAvalancheBmpFileName(string a_type)
        {         
            DirectoryInfo di = new DirectoryInfo(Path.GetDirectoryName(Application.ExecutablePath)).Parent.Parent;
            if (di.Name == "bin")
                di = di.Parent;

            string dir = di.CreateSubdirectory("Images").FullName;

            new DirectoryInfo(dir).Create();

            return new FileInfo(dir + Path.DirectorySeparatorChar + HashFunction.Name + 
                "_" + a_type + ".png").FullName;
        }

        public void Save()
        {
            if (!(HashFunction is HashLib.ICrypto))
            {
                BmpByte.Save(GetAvalancheBmpFileName("Byte"));
                BmpUShort.Save(GetAvalancheBmpFileName("UShort"));
                BmpUInt.Save(GetAvalancheBmpFileName("UInt"));
                BmpULong.Save(GetAvalancheBmpFileName("ULong"));
                BmpDouble.Save(GetAvalancheBmpFileName("Double"));
            }

            Bmp128.Save(GetAvalancheBmpFileName("128"));
            Bmp256.Save(GetAvalancheBmpFileName("256"));
            BmpString.Save(GetAvalancheBmpFileName("String"));
        }
    }
}
