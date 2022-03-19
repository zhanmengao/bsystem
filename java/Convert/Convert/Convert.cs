using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


public class Convert
{
    public static string ByteToString(byte[] byteArr, uint size)
    {
        return System.Text.Encoding.GetEncoding("gb2312").GetString(byteArr, 0, (int)size);
    }
    public static byte[] ToByte(float num)
    {
        return BitConverter.GetBytes(num);
    }
    public static byte[] ToByte(string str)
    {
        return System.Text.Encoding.GetEncoding("gb2312").GetBytes(str); ;
    }
    public static byte[] ToByte(int value)
    {
        byte[] src = new byte[4];
        src[3] = (byte)((value >> 24) & 0xFF);
        src[2] = (byte)((value >> 16) & 0xFF);
        src[1] = (byte)((value >> 8) & 0xFF);
        src[0] = (byte)(value & 0xFF);
        return src;
    }
    public static int ToInt(byte[] src, int offset)
    {
        int value;
        value = (int)((src[offset] & 0xFF)
                | ((src[offset + 1] & 0xFF) << 8)
                | ((src[offset + 2] & 0xFF) << 16)
                | ((src[offset + 3] & 0xFF) << 24));
        return value;
    }
}

