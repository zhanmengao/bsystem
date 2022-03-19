using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace ClassLibrary1
{
    public class RegistryTool
    {
        private const string PATH_OF_APPLICATIONS = @"SOFTWARE\Microsoft\Windows NT\CurrentVersion\Terminal Server\TSAppAllowList\Applications\";
        public static RemoteAppInfo GetRemoteAppInfo(string key)
        {
            RegistryKey keyOfApplication = Registry.LocalMachine.OpenSubKey(PATH_OF_APPLICATIONS + key, false);
            RemoteAppInfo info = null;
            try
            {
                info = new RemoteAppInfo
                {
                    Name = GetValueString(keyOfApplication, "name"),
                    Path = GetValueString(keyOfApplication, "path"),
                    CommandLineSetting = GetValueString(keyOfApplication, "commandLineSetting"),
                    RequiredCommandLine = GetValueString(keyOfApplication, "requiredCommandLine")
                };
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
            return info;
        }
        private static string GetValueString(RegistryKey key, string name)
        {
            object value = key.GetValue(name);
            return value != null ? value.ToString() : "注册表项异常";
        }
        public static string[] GetApplicationKeys()
        {
            RegistryKey applicationKeys = Registry.LocalMachine.OpenSubKey(PATH_OF_APPLICATIONS, false);
            return applicationKeys?.GetSubKeyNames();
        }
        public static void WriteRemoteApp(RemoteAppInfo remoteApp)
        {
            RegistryKey applicationKeys = Registry.LocalMachine.OpenSubKey(PATH_OF_APPLICATIONS, true);
            RegistryKey remoteAppKey = applicationKeys.CreateSubKey(remoteApp.Name + "_tool");
            remoteAppKey.SetValue("Name", remoteApp.Name, RegistryValueKind.String);
            remoteAppKey.SetValue("Path", remoteApp.Path, RegistryValueKind.String);
            remoteAppKey.SetValue("CommandLineSetting", remoteApp.CommandLineSetting, RegistryValueKind.DWord);
            remoteAppKey.SetValue("RequiredCommandLine", remoteApp.RequiredCommandLine, RegistryValueKind.String);
            remoteAppKey.SetValue("_description", remoteApp.Description, RegistryValueKind.String);
        }
        public static void RemoveRemoteApp(RemoteAppInfo remoteApp)
        {
            RegistryKey applicationKeys = Registry.LocalMachine.OpenSubKey(PATH_OF_APPLICATIONS, true);
            //RegistryKey remoteAppKey = applicationKeys.CreateSubKey(remoteApp.Name + "_tool");
            applicationKeys.DeleteSubKey(remoteApp.Name + "_tool");
        }
        public static string GetIP()
        {
            RegistryKey keyOfNetworkCards = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards\", false);
            string[] networkCards = keyOfNetworkCards.GetSubKeyNames();
            List<string> ipList = new List<string>();
            foreach (string card in networkCards)
            {
                string serviceName = keyOfNetworkCards.OpenSubKey(card, false).GetValue("ServiceName").ToString();
                RegistryKey Interfaces = Registry.LocalMachine.OpenSubKey(@"SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\Interfaces\" + serviceName, false);
                string[] list = (string[])Interfaces.GetValue("IPAddress");
                if (list != null && list.Length > 0)
                {
                    foreach (string s in list)
                    {
                        ipList.Add(s);
                    }
                }
            }
            Debug.WriteLine("ipList.Count{0}", ipList.Count);
            return ipList[0];
        }
    }
}
