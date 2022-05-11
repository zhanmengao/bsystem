using System;
using System.Threading;

public partial class ThreadProgram
{
    public static void tMainParameter(object rData)
    {
        Console.WriteLine(rData);
    }
    public static void tMainTempParameter(object rData)
    {
        Console.Write(rData + "    ");
    }
    public static void MainParameter()
    {
        //Lambda C#3.0之后支持
        Thread t = new Thread(() => tMainParameter("From Main"));
        t.Start();

        //旧方法，将参数从Start传
        Thread t2 = new Thread(tMainParameter);
        t2.Start("From Main 2");
        Thread.Sleep(1000);
        for(int i = 0;i<100;i++)
        {
            //这样可能会有问题，因为按Object形式传递参数，会拿到变量的引用
            //所有线程拿到的是同一个i
            Thread tempT = new Thread(() => tMainTempParameter(i));
            tempT.Start();
        }
        Thread.Sleep(1000);
        Console.WriteLine();
        for (int i = 0; i < 100; i++)
        {
            //正确做法，传递一个缓存
            int tmp = i;
            Thread tempT = new Thread(() => tMainTempParameter(tmp));
            tempT.Start();
        }
    }
}