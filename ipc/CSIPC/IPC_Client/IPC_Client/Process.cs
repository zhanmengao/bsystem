using System;

[Serializable]
public class MsgData
{
    public string rData;
    public int index;
}
class Program
{
    static void Main(string[] args)
    {
        FifoCS<MsgData> mFifo = new FifoCS<MsgData>("ServerRead", "ServerWrite");
        object nRead = "";
        int index = 0;
        while (true)
        {
            string str = Console.ReadLine();
            MsgData msg = new MsgData();
            msg.index = index;
            msg.rData = str;
            mFifo.Write(msg);
            if ((msg = mFifo.Read()) != null)
            {
                Console.WriteLine("Recv:    " + msg.rData + "   Index:" + msg.index);
            }
            index++;
        }
    }
}

