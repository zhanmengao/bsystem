using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Delegate
{
  class GamaManager
  {
    public delegate void NotReturn();
    public NotReturn OneMin;

    public GamaManager()
    {

    }

    public void Start()
    {
      while(true)
      {
        if (OneMin != null)
        {
          foreach (NotReturn item in OneMin.GetInvocationList())
          {
            item();
          }
        }
      }
    }

  }
}
