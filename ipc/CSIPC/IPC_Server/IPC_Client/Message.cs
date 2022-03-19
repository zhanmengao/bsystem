using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class test : MarshalByRefObject
{
    private int iCount = 0;
    public int count()
    {
        iCount++;
        return iCount;
    }

    public int Add(int x)
    {
        iCount += x;
        return iCount;
    }
}
