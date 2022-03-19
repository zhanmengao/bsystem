using System;
using System.Drawing;
using System.Drawing.Printing;
using System.Collections.Generic;
using System.Management;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Threading;

namespace PrinterCs
{
    class Program
    {
        private static bool PrinterExists()
        {
            PrintDocument prtdoc = new PrintDocument();
            string strDefaultPrinter = prtdoc.PrinterSettings.PrinterName;      //获取默认的打印机名    

            PrinterSettings.StringCollection snames = PrinterSettings.InstalledPrinters;

            foreach (string s in snames)
            {
                PrinterSettings printer = new PrinterSettings();
                printer.PrinterName = s;
                Console.WriteLine(printer.PrinterName);
            }
            return false;
        }
        private static Dictionary<string, List<KeyValuePair<string, object>>> Printer()
        {
            Dictionary<string, List<KeyValuePair<string, object>>> lst = new Dictionary<string, List<KeyValuePair<string, object>>>();
            ManagementClass mc = new ManagementClass("Win32_Printer");
            ManagementObjectCollection moc = mc.GetInstances();
            foreach (var mo in moc)
            {
                List<KeyValuePair<string, object>> ls = new List<KeyValuePair<string, object>>();
                foreach (var pd in mo.Properties)
                {
                    ls.Add(new KeyValuePair<string, object>(pd.Name, pd.Value));
                }
                lst.Add(mo.ToString(), ls);
            }
            return lst;
        }
        static void Main3(string[] args)
        {

        }
    }
}