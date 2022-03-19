using System;
using System.Threading;

public partial class ThreadProgram
{
    public static void MainJoin()
    {
        ThreadStart ts = new ThreadStart(tMainCreateThread);
        Thread t = new Thread(ts);
        t.Start();
        t.Join();                   //等待线程结束
        for (int i = 0; i < 500; i++)
        {
            Console.Write('x');
        }
        t.Abort();
    }
}