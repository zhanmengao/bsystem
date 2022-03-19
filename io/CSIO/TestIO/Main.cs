using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Configuration;
using System.Text;

namespace TestIO
{
    class MainClass
    {
        private const string FileRoot = "C:\\MyWorkSpace\\InputStream\\DrawCard\\DrawCard\\";
        public const string TablePath = FileRoot + "p4.txt";
        public const string RememberPath = FileRoot + "Remember.txt";
        public static void InitPeopleList()
        {
            string[] contents = File.ReadAllLines(TablePath, Encoding.Default);          //读表1
            string names = File.ReadAllText(RememberPath, Encoding.UTF8);               //读表2
            Console.WriteLine(names);
            List<string> nameList = new List<string>(names.Split(' '));
            for (int i = 0; i < contents.Length; i++)           //遍历每一行
            {
                string[] tuple = contents[i].Split('	');//按空格分割该元组
                if (nameList.Contains(tuple[1]))
                {
                    Console.WriteLine(tuple[1] + "重复");
                }
            }
        }

        private static List<string> FormatDescContent(string content)
        {
            int startIndex = 0;
            List<string> ret = new List<string>();
            char[] cArr = content.ToCharArray();
            for (int i = 0; i < cArr.Length - 1; i++)
            {
                if (cArr[i] == '#')
                {
                    if (cArr[i + 1] == 'r')
                    {
                        string str = new string(cArr, startIndex, i - startIndex);
                        ret.Add(str);
                        i += 2;                 //越过#r
                        startIndex = i;
                    }
                }
            }
            ret.Add(new string(cArr, startIndex, cArr.Length  - startIndex));
            return ret;
        }
        public static void Main(string[] args)
        {
            string aa = "我是#r参数1;";
            List<string> ret=FormatDescContent(aa);
            foreach(string temp in ret)
            {
                Console.WriteLine(temp);
            }
            aa = "我是参数1;";
            ret = FormatDescContent(aa);
            foreach (string temp in ret)
            {
                Console.WriteLine(temp);
            }
        }
    }
}
