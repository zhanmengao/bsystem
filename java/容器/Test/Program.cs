using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    class Program
    {
        //C# - HashSet == C++ : set 
        public static void HashSetText()
        {
            HashSet<ulong> mSet = new HashSet<ulong>();
            mSet.Add(1);
        }
        static void Main(string[] args)
        {
            HashSetText();
        }
    }
    class Utils
    {
        public static void printf(string content)
        {
            Console.WriteLine(content);
        }
    }
}
