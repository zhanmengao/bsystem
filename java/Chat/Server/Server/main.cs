using Photon.SocketServer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Server
{
  //继承自ApplicationBase类 是Server的入口程序
  class main:ApplicationBase
  {
    //一个客户端连接时会调用
    protected override PeerBase CreatePeer(InitRequest initRequest)
    {
      ChatPeer peer = new ChatPeer(initRequest.Protocol, initRequest.PhotonPeer);
      return peer;
    }

    //Server启动时被调用
    protected override void Setup()
    {

    }

    //服务器程序被终止
    protected override void TearDown()
    {

    }
  }
}
