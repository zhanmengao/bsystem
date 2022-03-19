using Photon.SocketServer;
using PhotonHostRuntimeInterfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Server
{
  //与客户端进行通信
  class ChatPeer : PeerBase
  {
    public ChatPeer(IRpcProtocol protocol, IPhotonPeer unmanagedPeer):base(protocol,unmanagedPeer)
    {
      
    }
    protected override void OnDisconnect(PhotonHostRuntimeInterfaces.DisconnectReason reasonCode, string reasonDetail)
    {

    }
    //相应客户端发起的请求
    protected override void OnOperationRequest(OperationRequest operationRequest, SendParameters sendParameters)
    {
      Dictionary<byte, object> dict = new Dictionary<byte, object>();
      foreach(KeyValuePair<byte,object> item in operationRequest.Parameters)
      {
        //MSG_Login msg = (MSG_Login)item.Value;
        dict.Add(item.Key,item.Value);
      }
      //dict.Add(1, operationRequest.Parameters.Values.);
      OperationResponse res = new OperationResponse(1, dict);
      //给客户端响应
      SendOperationResponse(res, sendParameters);
    }
  }
}
