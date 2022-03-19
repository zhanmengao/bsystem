using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Ipc;
using System.Text;
using System.Threading.Tasks;

namespace IPC_Client
{
    class Other
    {
        static void MainOther(string[] args)
        {
            ConnectServer();
            Send();
        }
        static RemoteObject objRemoteObject;
        private static void Send()
        {
            while (true)
            {
                string message = Console.ReadLine();
                try
                {
                    objRemoteObject.SendMessage(message);
                }
                catch (System.Runtime.Remoting.RemotingException)
                {
                    Console.WriteLine("can not connect message server");
                }
            }
        }
        private static void ConnectServer()
        {
            IpcClientChannel channel = new IpcClientChannel();
            ChannelServices.RegisterChannel(channel, false);
            objRemoteObject = (RemoteObject)Activator.GetObject(typeof(RemoteObject), "ipc://ServerChannel/RemoteObject");
        }
    }
}
