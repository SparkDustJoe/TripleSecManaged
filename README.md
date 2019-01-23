TripleSecManaged
================

a C# port of the TripleSec encryption scheme created by Chris Coyne, Maxwell Krohn, and Filippo Valsorda

Implements v3 and v4 (verified as of Jan 2019)

Initial Upload on Dec 7 2014, last updated Jan 2019

Uses MS Visual Studio 2017 or later (there aren't many .Net tricks in use, so this could easily be ported back to an older version of .Net, plans are in the future to cross-compile in .Net Core and .Net Standard for completeness; Note: in doing so, the SHA3Managed and modified Chaos-NaCl projects would also have to be back-ported).

Uses a modified version of Chaos.NaCl from GitHub (XSalsa20 class added without Poly1305 MAC scheme)
  https://github.com/CodesInChaos/Chaos.NaCl

Uses the Legion of the BouncyCastle C# library DLL 1.7 (for AES-CTR and Twofish-CTR (V3 only) mode encryption schemes;
  LoBC calls that mode SIC)  http://www.bouncycastle.org/csharp/
  
All other code written by Dustin J. Sparks using reference material from the official TripleSec GitHub repository
  as guidance, both for the algorithm and the underlying methodology  https://github.com/keybase/triplesec
