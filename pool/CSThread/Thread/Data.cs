using System;
using System.Threading;


public partial class ThreadProgram
{
    bool bInit;                 //该变量共享
    static bool isInit;      //该变量也共享
    public void tMainData()
    {
        if(!bInit)
        {
            bInit = true;
            Console.WriteLine("Init");
        }
    }
    public static void tStatMainData()
    {
        //缺乏同步机制，可能打印一次，也可能打印两次
        if (!isInit)
        {
            Console.WriteLine("Static Init");               
            isInit = true;
        }
    }
    static readonly object _locker = new object();
    static bool isLockInit;
    public static void tStatLockMainData()
    {
        lock(_locker)
        {
            if (!isLockInit)
            {
                Console.WriteLine("Lock Static Init");
                isLockInit = true;
            }
        }
    }
    public static void MainData()
    {
        ThreadProgram  tp = new ThreadProgram();
        Thread t = new Thread(tp.tMainData);
        t.Start();
        tp.tMainData();

        Thread t2 = new Thread(tStatMainData);
        t2.Start();
        tStatMainData();

        Thread t3 = new Thread(tStatMainData);
        t3.Start();
        tStatLockMainData();
    }
}