using ClassLibrary1;
using IWshRuntimeLibrary;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace software_to_go_GUI
{
    class FileTool
    {

        public static List<string> ScanAll(string path)
        {
            List<string> fileList = new List<string>();
            if (Directory.Exists(path))
            {
                foreach (string file in Directory.GetFiles(path))
                {
                    if (".lnk".Equals(Path.GetExtension(file)) && path != "" && !@"c:\windows".Equals(path.Substring(10).ToLower()))
                    {
                        fileList.Add(file);
                    }
                }
                string[] dirs = Directory.GetDirectories(path);
                foreach (string d in dirs)
                {
                    fileList = fileList.Union(ScanAll(d)).ToList();
                }
            }
            return fileList;
        }

        public static List<string> Scan(string path)
        {
            List<string> fileList = new List<string>();
            if (Directory.Exists(path) == true)
            {
                foreach (string file in Directory.GetFiles(path))
                {
                    fileList.Add(file);
                }
            }
            return fileList;
        }


        public static List<string> Scan(string path, string extension)
        {
            List<string> fileList = new List<string>();
            if (Directory.Exists(path) == true)
            {
                foreach (string file in Directory.GetFiles(path))
                {
                    if (extension.Equals(Path.GetExtension(file)))
                    {
                        fileList.Add(file);
                    }
                }
            }
            return fileList;
        }



        ///引用COM组件Windows Script Host Object Model
        public static RemoteAppInfo GetShortcutInfo(string link_path)
        {
            WshShell shell = new WshShell();
            IWshShortcut shortcut = (IWshShortcut)shell.CreateShortcut(link_path);
            string setting = shortcut.Arguments.Equals("") ? "0" : "2";
            RemoteAppInfo info = null;
            try
            {
                info = new RemoteAppInfo
                {
                    Name = Path.GetFileName(shortcut.FullName).Split('.')[0],
                    Path = shortcut.TargetPath,
                    CommandLineSetting = setting,
                    RequiredCommandLine = shortcut.Arguments,
                    Description = shortcut.Description
                };
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
            return info;
        }
    }
}
