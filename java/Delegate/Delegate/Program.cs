using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Delegate
{
  public delegate void NotReturn();
  class Program
  {
    static void Main(string[] args)
    {
      GamaManager manager = new GamaManager();
      //manager.OneMin += CallBack;
      UI ui = new UI();
      manager.OneMin += ui.OnClick;
      manager.Start();
    }

    private static void CallBack()
    {
      Console.WriteLine("回调");
    }
  }
}
