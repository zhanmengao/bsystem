using System;
using System.Threading;

public partial class ThreadProgram
{
    public static void tMainCreateThread()
    {
        try
        {
            // 计数到 10
            for (int i = 0; i <= 1000; i++)
            {
                Console.Write('y');
                Thread.Sleep(10);
            }
        }
        catch (ThreadAbortException e)
        {
            Console.WriteLine();
            Console.WriteLine("Thread Abort Exception");
        }
    }
    public static void MainCreateThread()
    {
        ThreadStart ts = new ThreadStart(tMainCreateThread);
        Thread t = new Thread(ts);
        t.Start();
        for (int i = 0; i < 500; i++)
        {
            Console.Write('x');
        }
        t.Abort();
    }
}

