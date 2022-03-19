using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace software_to_go_GUI
{
    class IniFileTool
    {
        public string inipath;
        private string defaultSection = "default";

        //声明API函数
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString
            (string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString
            (string section, string key, string def, StringBuilder retVal, int size, string filePath);

        /// <summary> 
        /// 构造方法 
        public IniFileTool(string INIPath) { inipath = INIPath; }

        /// <summary> 
        /// 写入INI文件 
        /// </summary> 
        /// <param name="Section">项目名称(如 [TypeName] )</param> 
        /// <param name="Key">键</param> 
        /// <param name="Value">值</param> 
        public void IniWriteValue(string Section, string Key, string Value)
        {
            WritePrivateProfileString(Section, Key, Value, this.inipath);
        }
        public void IniWriteValue(string Key, string Value)
        {
            WritePrivateProfileString(defaultSection, Key, Value, this.inipath);
        }
        /// <summary> 
        /// 读出INI文件 
        /// </summary> 
        /// <param name="Section">项目名称(如 [TypeName] )</param> 
        /// <param name="Key">键</param> 
        public string IniReadValue(string Section, string Key)
        {
            StringBuilder temp = new StringBuilder(500);
            int i = GetPrivateProfileString(Section, Key, "", temp, 500, this.inipath);
            return temp.ToString();
        }
        public string IniReadValue(string Key)
        {
            StringBuilder temp = new StringBuilder(500);
            int i = GetPrivateProfileString(defaultSection, Key, "", temp, 500, this.inipath);
            return temp.ToString();
        }

        public bool Exists()
        {
            return File.Exists(inipath);
        }
    }
}
