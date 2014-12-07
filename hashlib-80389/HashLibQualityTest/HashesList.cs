using System;
using System.Collections.ObjectModel;
using HashLib;
using System.Linq;

namespace HashLibQualityTest
{
    public static class HashesList
    {
        public readonly static ReadOnlyCollection<IHash> SpeedList;
        public readonly static ReadOnlyCollection<IHash> SpeedListCrypto;
        public readonly static ReadOnlyCollection<IHash> AvalancheList;
        public readonly static ReadOnlyCollection<IHash> AvalancheListCrypto;
        public readonly static ReadOnlyCollection<IHash> CalculatorList;
        public readonly static ReadOnlyCollection<IHash> CalculatorListCrypto;
        public readonly static ReadOnlyCollection<IHash> CalculatorListKey;

        static HashesList()
        {
            SpeedList = (from hf in Hashes.All
                         where Hashes.Hash32.Contains(hf) || HashLib.Hashes.Hash64.Contains(hf) ||
                               HashLib.Hashes.Hash128.Contains(hf) || HashLib.Hashes.Checksums.Contains(hf) 
                         where hf.Name != "CRC32"
                         where hf.Name != "CRC64"
                         select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();

            SpeedListCrypto = (from hf in Hashes.All
                               where Hashes.CryptoAll.Contains(hf)
                               select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();

            AvalancheList = (from hf in Hashes.AllUnique
                             where Hashes.Hash32.Contains(hf) || HashLib.Hashes.Hash64.Contains(hf) ||
                                   HashLib.Hashes.Hash128.Contains(hf)
                             select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();

            AvalancheListCrypto = (from hf in Hashes.AllUnique
                                   where Hashes.CryptoAll.Contains(hf)
                                   select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();

            var cl = (from hf in Hashes.AllUnique
                      where Hashes.Hash32.Contains(hf) || HashLib.Hashes.Hash64.Contains(hf) ||
                            HashLib.Hashes.Hash128.Contains(hf)
                      select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();

            CalculatorList = (from hf in cl
                              where hf.Name != "DotNet"
                              select hf).ToList().AsReadOnly();

            CalculatorListCrypto = (from hf in Hashes.AllUnique
                                    where Hashes.CryptoAll.Contains(hf)
                                    select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();

            CalculatorListKey = (from hf in Hashes.AllUnique
                                 where Hashes.CryptoAll.Contains(hf) ||
                                  Hashes.WithKey.Contains(hf)
                                 select (IHash)Activator.CreateInstance(hf)).ToList().AsReadOnly();
        }
    }
}
