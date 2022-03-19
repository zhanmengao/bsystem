using ExitGames.Client.Photon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
 
  class Program
  {
    static void Main(string[] args)
    {
      ChatListener listener=new ChatListener();
      PhotonPeer peer = new PhotonPeer(listener,ConnectionProtocol.Tcp);
      peer.Connect("192.168.2.102:4530", "ChatServer");//添加请求到服务器
      Console.WriteLine("正在连接...");
      while(!listener.m_IsConnect)
      {
        peer.Service();//立即发包
      }
      Dictionary<byte,object> dic=new Dictionary<byte,object>();
      dic.Add(1, "aaa123");
      peer.OpCustom(1, dic, true);
      while(true)
      {
        peer.Service();
      }
    }
  }
}
