using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace CSIPC2CPP
{
    class Program
    {

        private static IntPtr ConnectPipe(string strPipeName)
        {
            IntPtr hPipe;
            while (true)
            {
                if (PipeNative.WaitNamedPipe((strPipeName), 0) == false)
                {
                    Console.WriteLine("等待命名管道实例失败：{0} error:{0}", strPipeName, PipeNative.GetLastError());
                    continue;
                }

                hPipe = PipeNative.CreateFile(
                     strPipeName,  //管道名称
                     FileDesiredAccess.GENERIC_READ | FileDesiredAccess.GENERIC_WRITE,  //访问模式，读模式或写模式
                     FileShareMode.Zero,  //0表示不共享，共享模式
                     IntPtr.Zero,  //一个只读字段，代表已初始化为零的指针或句柄。指向安全属性的指针
                     FileCreationDisposition.OPEN_EXISTING, //如何创建。文件必须已经存在。由设备提出要求
                     0,  //文件属性
                     0);  //用于复制文件句柄，不使用模板
                if (hPipe.ToInt32() != PipeNative.INVALID_HANDLE_VALUE) break;//PipeNative.INVALID_HANDLE_VALUE = -1.管道创建失败

                if (PipeNative.GetLastError() != PipeNative.ERROR_PIPE_BUSY   //PipeNative.ERROR_PIPE_BUSY = 231
                    || PipeNative.WaitNamedPipe(strPipeName, 5 * 1000))       //在超时时间前管道的一个实例有效则返回非0，在超时时间内没有一个有效的实例，则返回0 
                {
                    Console.WriteLine("无法连接管道：{0} ERROR:{1}", strPipeName, PipeNative.GetLastError());
                    return hPipe;
                }
            }
            Console.WriteLine("管道{0}连接成功。", strPipeName);
            return hPipe;
        }
        static void Main(string[] args)
        {
            var hPipe = ConnectPipe("\\\\.\\Pipe\\myPipe2");
            byte[] lpBuffer = new byte[1024];
            uint WriteNum = 0;
            IntPtr iPtr = new IntPtr();
            while (true)
            {
                //读控制台
                string cin = Console.ReadLine();
                lpBuffer = System.Text.Encoding.Default.GetBytes(cin);
                if (PipeNative.WriteFile(hPipe, lpBuffer, (uint)lpBuffer.Length, out WriteNum, iPtr))
                {
                    if (PipeNative.WriteFile(hPipe, lpBuffer, (uint)lpBuffer.Length, out WriteNum, iPtr) == false)
                    {
                        Console.WriteLine("数据写入管道失败,关闭管道：{0} ERROR:{1}", hPipe, PipeNative.GetLastError());
                        PipeNative.CloseHandle(hPipe);
                        break;
                    }
                }
            }
        }
    }
}
