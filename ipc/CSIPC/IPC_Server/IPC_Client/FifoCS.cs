using System;
using System.Collections.Generic;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Ipc;
using System.Runtime.Remoting;

//MarshalByRefObject 允许在支持远程处理的应用程序中跨应用程序域边界访问对象。

public class RemoteObject : MarshalByRefObject
{
    public static Queue<object> qMessage { get; set; } //使用消息队列储存消息
    public void SendMessage(object message)
    {
        if (qMessage == null)
        {
            qMessage = new Queue<object>();
        }
        qMessage.Enqueue(message);
    }
}

class FifoCS<T> where T : class
{
    private RemoteObject mFifo = null;
    public FifoCS(string readFifoName, string writeFifoName)
    {
        InitClient(writeFifoName);
        InitServer(readFifoName);
    }
    private void InitClient(string fifoName)
    {
        IpcClientChannel channel = new IpcClientChannel();
        ChannelServices.RegisterChannel(channel, false);
        string cFifo = "ipc://" + fifoName + "/RemoteObject";
        mFifo = (RemoteObject)Activator.GetObject(typeof(RemoteObject), cFifo);
    }
    private void InitServer(string fifoName)
    {
        IpcServerChannel channel = new IpcServerChannel(fifoName);
        ChannelServices.RegisterChannel(channel, false);
        RemotingConfiguration.RegisterWellKnownServiceType(typeof(RemoteObject), "RemoteObject",
            WellKnownObjectMode.SingleCall);
        Console.WriteLine("message server running...");
    }
    public T Read()
    {
        Queue<object> qMessage = RemoteObject.qMessage;
        if (qMessage != null)
        {
            if (qMessage.Count > 0)
            {
                T obj = qMessage.Dequeue() as T;              
                return obj;
            }
        }
        return null;
    }
    public void Write(T rData)
    {
        mFifo.SendMessage(rData);
    }
}

