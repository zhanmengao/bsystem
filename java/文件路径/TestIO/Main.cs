using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
namespace pathtest
{
    class program
    {
        static void Main(string[] args)
        {
            main1();
        }
        public  static void main1()
        {
            string rootPath = "";
            string BaseDirectoryPath = AppDomain.CurrentDomain.BaseDirectory; // F:\project\WPF\AstroATE-PDR\04. 程序\01. 源代码\AstroATE\AstroATE\bin\Debug
            // 向上回退三级，得到需要的目录
            rootPath = BaseDirectoryPath.Substring(0, BaseDirectoryPath.LastIndexOf("\\")); // 第一个\是转义符，所以要写两个
            rootPath = rootPath.Substring(0, rootPath.LastIndexOf(@"\"));   // 或者写成这种格式
            rootPath = rootPath.Substring(0, rootPath.LastIndexOf("\\")); // @"F:\project\WPF\AstroATE-PDR\04. 程序\01. 源代码\AstroATE\AstroATE
            Console.WriteLine(rootPath);
        }

        public static void main2()
        {
            //使用appdomain获取当前应用程序集的执行目录
            string dir = AppDomain.CurrentDomain.BaseDirectory;
            string info = string.Format("appdomain方法获取当前程序集目录：{0}", dir);
            Console.WriteLine(info);

            //使用path获取当前应用程序集的执行的上级目录
            dir = Path.GetFullPath("..");
            info = string.Format("path方法获取当前程序集上级目录：{0}", dir);
            Console.WriteLine(info);

            //使用path获取当前应用程序集的执行目录的上级的上级目录
            dir = Path.GetFullPath(@"....");
            info = string.Format("path方法获取当前程序集目录的级的上级目录：{0}", dir);
            Console.WriteLine(info);

            //使用path获取当前应用程序集的执行目录的上级目录
            dir = Path.GetFullPath(@"......");
            info = string.Format("path方法获取当前程序集目录的上级目录的上级目录：{0}", dir);
            Console.WriteLine(info);

            //在当前程序集目录中添加指定目录
            dir = Path.GetFullPath(@"io");
            info = string.Format("在当前程序集目录中添加指定目录：{0}", dir);
            Console.WriteLine(info);
        }
    }
}