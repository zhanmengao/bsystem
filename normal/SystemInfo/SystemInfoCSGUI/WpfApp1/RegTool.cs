using Microsoft.Win32;

namespace WpfApp1
{
    class RegTool
    {
        private const string PATH_OF_APPLICATIONS = @"SOFTWARE\Microsoft\Windows NT\CurrentVersion\Terminal Server\TSAppAllowList\Applications\";
        public static RemoteAppInfo GetRemoteAppInfo(string key)
        {
            RegistryKey keyOfApplication = Registry.LocalMachine.OpenSubKey(PATH_OF_APPLICATIONS + key, false);
            return new RemoteAppInfo
            {
                Name = keyOfApplication.GetValue("name").ToString(),
                CommandLineSetting = keyOfApplication.GetValue("commandLineSetting").ToString(),

                Path = keyOfApplication.GetValue("path").ToString(),
                RequiredCommandLine = keyOfApplication.GetValue("requiredCommandLine").ToString()
            };
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
        public static void RemoveRemoteApp(RemoteAppInfo remoteApp) {
            RegistryKey applicationKeys = Registry.LocalMachine.OpenSubKey(PATH_OF_APPLICATIONS, true);
            //RegistryKey remoteAppKey = applicationKeys.CreateSubKey(remoteApp.Name + "_tool");
            applicationKeys.DeleteSubKey(remoteApp.Name + "_tool");
        }
    }
}
