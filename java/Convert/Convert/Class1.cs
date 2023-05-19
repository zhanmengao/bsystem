using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;


namespace FNFramework
{
    public class AESPacketCrypto
    {
        private static readonly ICryptoTransform DefaultEncryptor;
        private static readonly ICryptoTransform DefaultDecryptor;


        static AESPacketCrypto()
        {
            DefaultEncryptor = _CreateEncryptor(null);
            DefaultDecryptor = _CreateDecryptor(null);
        }

        private const string DefaultCryptoKey = "XSZWoJKcnIhz9v9i";

        private static ICryptoTransform _CreateEncryptor(string key)
        {
            try
            {
                string cryptoKey = key ?? DefaultCryptoKey;
                byte[] VIKey = new byte[16];
                Array.Copy(Encoding.ASCII.GetBytes(cryptoKey), 0, VIKey, 0, 16);
                var symmetricKey = new RijndaelManaged { Mode = CipherMode.CBC, Padding = PaddingMode.PKCS7 };
                return symmetricKey.CreateEncryptor(Encoding.ASCII.GetBytes(cryptoKey), VIKey);
            }
            catch (Exception e)
            {
                return null;
            }
        }

        private static ICryptoTransform _CreateDecryptor(string key)
        {
            try
            {
                string cryptoKey = key ?? DefaultCryptoKey;
                byte[] VIKey = new byte[16];
                Array.Copy(Encoding.ASCII.GetBytes(cryptoKey), 0, VIKey, 0, 16);
                var symmetricKey = new RijndaelManaged { Mode = CipherMode.CBC, Padding = PaddingMode.PKCS7 };
                return symmetricKey.CreateDecryptor(Encoding.ASCII.GetBytes(cryptoKey), VIKey);
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public AESPacketCrypto() { }


        private static readonly byte[] _encBuffer = new byte[1024 * 8];
        private static readonly byte[] _decBuffer = new byte[1024 * 8];

        public static byte[] Encrypt(byte[] buffer, string key)
        {
            int outputLength = Encrypt(buffer, 0, buffer.Length, _encBuffer, 0, key);
            byte[] result = new byte[outputLength];
            Array.Copy(_encBuffer, result, outputLength);
            return result;
        }

        public static byte[] Decrypt(byte[] buffer, string key)
        {
            int outputLength = Decrypt(buffer, 0, buffer.Length, _decBuffer, 0, key);
            byte[] result = new byte[outputLength];
            Array.Copy(_decBuffer, result, outputLength);
            return result;
        }

        public static byte[] Md5Encode16(string inputData)
        {
            var md5 = new MD5CryptoServiceProvider();
            return md5.ComputeHash(Encoding.ASCII.GetBytes(inputData));
        }

        public static int Encrypt(byte[] inBuffer, int inStartIndex, int inCount, byte[] outBuffer, int outStartIndex, string cryptoKey)
        {
            ICryptoTransform encryptor;
            if (!string.IsNullOrEmpty(cryptoKey) || !DefaultEncryptor.CanReuseTransform)
            {
                encryptor = _CreateEncryptor(cryptoKey);
            }
            else
            {
                encryptor = DefaultEncryptor;
            }

            using (var memoryStream = new MemoryStream(outBuffer))
            {
                using (var cryptoStream = new CryptoStream(memoryStream, encryptor, CryptoStreamMode.Write))
                {
                    memoryStream.Position = outStartIndex;
                    cryptoStream.Write(inBuffer, inStartIndex, inCount);
                    cryptoStream.FlushFinalBlock();

                    int encryptedLength = (int)memoryStream.Position - outStartIndex;

                    cryptoStream.Close();
                    memoryStream.Close();

                    return encryptedLength;
                }
            }
        }

        public static int Decrypt(byte[] inBuffer, int inStartIndex, int inCount, byte[] outBuffer, int outStartIndex, string cryptoKey)
        {
            ICryptoTransform decryptor;
            if (!string.IsNullOrEmpty(cryptoKey) || !DefaultEncryptor.CanReuseTransform)
            {
                decryptor = _CreateDecryptor(cryptoKey);
            }
            else
            {
                decryptor = DefaultDecryptor;
            }

            using (var inMemoryStream = new MemoryStream(inBuffer, inStartIndex, inCount))
            {
                inMemoryStream.Position = 0;
                using (var cryptoStream = new CryptoStream(inMemoryStream, decryptor, CryptoStreamMode.Read))
                {
                    int count;
                    int startIndex = outStartIndex;
                    while ((count = cryptoStream.Read(outBuffer, startIndex, 1024)) != 0)
                    {
                        startIndex += count;
                    }
                    cryptoStream.Flush();
                    cryptoStream.Close();
                    inMemoryStream.Close();
                    return startIndex - outStartIndex;
                }
            }
        }
        static void Main(string[] args)
        {
            byte[] rcv64 = System.Convert.FromBase64String("Qk2AAAAAAAAAAAAAAAAAAL7jB3KTUKcCNnYSQuJxotpT6qqd9sIVKqXNHOD1CuzvJmW0NVX3oL/ZPDH0Rub7DTLyyo4g4sphB5SyBiNo6+P6qTqCiHNpBPE0w3+d4icUfMZj9Qq+O3lHtOno/kgQx0Mdh2sH6+l6u6ZmHV2FbtvO6ZBDXd32AzKBbJcu5uRv");
            var key = "QF6uG0HgVLJYrKcf";
            var body = Decrypt(rcv64, key);
        }
    }


}