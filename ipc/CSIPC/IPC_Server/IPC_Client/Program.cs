using System;

[Serializable]
public class MsgData
{
    public string rData;
    public int index;
}
class Program
{
    static void MainCS()
    {
        FifoCS<MsgData> mFifo = new FifoCS<MsgData>("ServerWrite", "ServerRead");
        MsgData nRead;
        while (true)
        {
            if ((nRead = mFifo.Read()) != null)
            {
                Console.WriteLine("Recv:    " + nRead.rData + "   Index:" + nRead.index);
                //回写
                mFifo.Write(nRead);
            }
        }
    }
    const string serverReadPath = ("\\\\.\\Pipe\\myread");
    const string serverWritePath = ("\\\\.\\Pipe\\mywrite");
    static void MainCpp()
    {
        FifoCpp mFifo = new FifoCpp();
        mFifo.InitServer(serverReadPath, serverWritePath);
        while (true)
        {
            string ret = mFifo.Read();
            if (ret != null && ret.Length > 0)
            {
                Console.WriteLine("Recv:    " + ret.TrimEnd());
            }
        }
    }
    static void Main(string[] args)
    {
        MainCS();
    }
    
}

