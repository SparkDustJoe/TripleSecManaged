TripleSecManaged
================

a C# port of the TripleSec encryption scheme created by Chris Coyne, Maxwell Krohn, and Filippo Valsorda

Implements v3 (and soon, v4)

Initial Upload on Dec 7 2014

Uses MS Visual Studio 2012 or later

Uses a modified version of Chaos.NaCl from GitHub (XSalsa20 class added without Poly1305 MAC scheme)
  https://github.com/CodesInChaos/Chaos.NaCl
  
Uses an unmodified version of Hashlib from CodePlex (the NuGet package for the same code has errors; original source from
  CodePlex works as of build 80389; for HMACKeccak512 and HMACSHA512)  https://hashlib.codeplex.com/ 
  
Uses an unmodified NuGet package of CryptSharp.SCryptSubset DLL (for SCrypt)

Uses the Legion of the BouncyCastle C# library DLL 1.7 (for AES-CTR and Twofish-CTR mode encryption schemes;
  LoBC calls that mode SIC)  http://www.bouncycastle.org/csharp/
  
All other code written by Dustin J. Sparks using reference material from the official TripleSec GitHub repository
  as guidance, both for the algorithm and the underlying methodology  https://github.com/keybase/triplesec
