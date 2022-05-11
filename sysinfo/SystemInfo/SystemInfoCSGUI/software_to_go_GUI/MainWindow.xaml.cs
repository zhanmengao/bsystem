using dotnet_etcd;
using Etcdserverpb;
using Microsoft.VisualBasic.Devices;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Management;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using ClassLibrary1;
using Newtonsoft.Json;
using System.Buffers.Text;
using System.Text;

namespace software_to_go_GUI
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly string iniPath = Environment.CurrentDirectory + "/config.ini";
        private const string START_MENU = @"C:\ProgramData\Microsoft\Windows\Start Menu\Programs\";
        //ObservableCollection<RemoteAppInfo> localAppList = new ObservableCollection<RemoteAppInfo>();
        // ObservableCollection<RemoteAppInfo> remoteAppList = new ObservableCollection<RemoteAppInfo>();
        //IList selectedLocalAppList = new List<RemoteAppInfo>();
        // IList selectedRemoteAppList = new List<RemoteAppInfo>();
        UIModel uIModel = new UIModel();

        Config config = new Config();
        IniFileTool iniFile;
        Etcd etcd;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Thread thread = new Thread(() => ReadOrInitConfig());
            thread.Start();


        }
        private void ReadOrInitConfig()
        {
            Dispatcher.Invoke(new Action(() =>
            {
                host_type_block.DataContext = config;
                host_ip_block.DataContext = config;
                //work_type
                work_type_block.DataContext = config;
                rd1.DataContext = config;
                rd2.DataContext = config;
                etcd_addr_textbox.DataContext = config;
                etcd_port_textbox.DataContext = config;

                iniFile = new IniFileTool(iniPath);
                if (iniFile.Exists())
                {
                    config.Cpu_serial_number = iniFile.IniReadValue(Config.CPU);
                    config.Host_type = iniFile.IniReadValue(Config.HOST_TYPE);
                    config.Work_type = iniFile.IniReadValue(Config.WORK_TYPE);
                    config.Host_ip = iniFile.IniReadValue(Config.HOST_IP);
                    config.Etcd_addr = iniFile.IniReadValue(Config.ETCD_ADDR);
                    config.Etcd_port = iniFile.IniReadValue(Config.ETCD_PORT);
                }
                else
                {
                    config.Cpu_serial_number = GetCPUSerialNumber(); ;
                    config.Host_type = new ComputerInfo().OSFullName;
                    config.Work_type = "0";
                    config.Host_ip = RegistryTool.GetIP();
                    config.Etcd_addr = "127.0.0.1";
                    config.Etcd_port = "2379";
                    WriteConfigIniFile(config);
                }

                /*
                string hosttype = Convert.ToBase64String(Encoding.UTF8.GetBytes(config.Host_type));

                string publicSoftList = "/prefix/host/" + hosttype + "/templet";
                Debug.WriteLine(publicSoftList);*/
                //init some list
                InitListView(uIModel);

            }));
        }
        private void WriteConfigIniFile(Config config)
        {
            iniFile.IniWriteValue(Config.CPU, config.Cpu_serial_number);
            iniFile.IniWriteValue(Config.HOST_IP, config.Host_ip);
            iniFile.IniWriteValue(Config.HOST_TYPE, config.Host_type);
            iniFile.IniWriteValue(Config.WORK_TYPE, config.Work_type);
            iniFile.IniWriteValue(Config.ETCD_ADDR, config.Etcd_addr);
            iniFile.IniWriteValue(Config.ETCD_PORT, config.Etcd_port);
        }
        private void InitListView(UIModel model)
        {
            listview_local.ItemsSource = model.localSoftwareList;
            UpdateLocalSoftwareInfoList(model);

            listview_remote.ItemsSource = model.remoteAppList;
            UpdateRemoteAppInfoList(model);
        }


        private void UpdateLocalSoftwareInfoList(UIModel model)
        {
            List<string> shortcutFilePathList = FileTool.ScanAll(START_MENU);
            foreach (string path in shortcutFilePathList)
            {
                RemoteAppInfo info = FileTool.GetShortcutInfo(path);
                if (info != null)
                {
                    info.ImageStr = IconTool.GetImageBase64String(info.Path);
                    model.localSoftwareList.Add(info);
                }
            }
            tab_local.Header = "本地程序(" + model.localSoftwareList.Count + ")";

        }

        private void UpdateRemoteAppInfoList(UIModel model)
        {
            model.remoteAppList.Clear();
            string[] appKeys = RegistryTool.GetApplicationKeys();
            if (appKeys != null && appKeys.Length > 0)
            {
                foreach (string appKey in appKeys)
                {
                    RemoteAppInfo info = RegistryTool.GetRemoteAppInfo(appKey);
                    if (info != null)
                    {
                        model.remoteAppList.Add(info);
                    }
                }
                tab_remote.Header = "已发布(" + appKeys.Length + ")";
            }
            else
            {
                tab_remote.Header = "已发布(空)";
            }
        }

        private void PublishApp(IList list)
        {
            foreach (RemoteAppInfo info in list)
            {
                Debug.WriteLine(info);
                RegistryTool.WriteRemoteApp(info);
            }

            /*
            string hosttype = Convert.ToBase64String(Encoding.UTF8.GetBytes(config.Host_type));
            string publicSoftListKey = "/prefix/host/" + hosttype + "/templet";
            Debug.WriteLine(publicSoftListKey);
            */
            // string payload = JsonConvert.SerializeObject(remoteAppList);
            //Debug.WriteLine(payload);
            //  EtcdClient client = etcd.GetClient();
            //  client.Put(publicSoftListKey, payload);
            //链接etcd，写该类主机公共应用数据

        }
        private void RemoveApp(IList list)
        {
            foreach (RemoteAppInfo info in list)
            {
                RegistryTool.RemoveRemoteApp(info);
            }
        }
        public static string GetCPUSerialNumber()
        {
            try
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher("Select * From Win32_Processor");
                string sCPUSerialNumber = "";
                foreach (ManagementObject mo in searcher.Get())
                {
                    sCPUSerialNumber = mo["ProcessorId"].ToString().Trim();
                    break;
                }
                return sCPUSerialNumber;
            }
            catch
            {
                return "";
            }
        }
        private void Open_btn_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog ofd = new Microsoft.Win32.OpenFileDialog
            {
                DefaultExt = ".exe",
                Filter = "程序文件|*.exe"
            };
            if (ofd.ShowDialog() == true)
            {
                var shortName = ofd.SafeFileName;
                var path = ofd.FileName;
                //Debug.WriteLine(ofd.SafeFileName);
                uIModel.localSoftwareList.Add(new RemoteAppInfo()
                {
                    Name = shortName,
                    Path = path,
                    ImageStr = IconTool.GetImageBase64String(path),
                });
            }
        }

        private void Publish_btn_Click(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine(uIModel.selectedLocalSoftwareList.Count);
            tab_remote.IsSelected = true;
            PublishApp(uIModel.selectedLocalSoftwareList);
            UpdateRemoteAppInfoList(uIModel);
        }

        private void Remove_btn_Click(object sender, RoutedEventArgs e)
        {
            RemoveApp(uIModel.selectedRemoteAppList);
            UpdateRemoteAppInfoList(uIModel);
        }

        private void Listview_local_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selected = listview_local.SelectedItems;
            publish_btn.Content = "已选" + selected.Count + "款，发布";
            uIModel.selectedLocalSoftwareList = selected;
        }

        private void Listview_remote_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selected = listview_remote.SelectedItems;
            remove_btn.Content = "已选" + selected.Count + "款，移除";
            uIModel.selectedRemoteAppList = selected;
        }

        private void TestETCD_btn_Click(object sender, RoutedEventArgs e)
        {
            string ip = config.Etcd_addr;
            string port = config.Etcd_port;
            if (VailIP(ip) && VailPort(port))
            {
                string url = "http://" + ip + ":" + port;
                try
                {
                    etcd = new Etcd(url);
                    EtcdClient client = etcd.GetClient();
                    var x = client.Status(new StatusRequest());
                    MessageBox.Show(x.ToString(), "连接" + url);
                }
                catch (Exception exception)
                {
                    Debug.WriteLine(exception);
                    MessageBox.Show("连接失败");
                }
            }
            else
            {
                MessageBox.Show("检查地址");
            }
        }
        private bool VailIP(string ip)
        {
            Regex regexIp = new Regex(@"^((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})(\.((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})){3}$");
            return regexIp.IsMatch(ip);                //? ip : "127.0.0.1";
        }
        private bool VailPort(string port)
        {
            Regex regexPort = new Regex(@"^([0-9]|[1-9]\d{1,3}|[1-5]\d{4}|6[0-4]\d{4}|65[0-4]\d{2}|655[0-2]\d|6553[0-5])$");
            return regexPort.IsMatch(port);// ? port : "65535";
        }

        private void Save_config_btn_Click(object sender, RoutedEventArgs e)
        {
            config.Host_ip = "pantyhose";
            config.Etcd_port = "pantyhose";
        }

        private void Save_ETCD_btn_Click(object sender, RoutedEventArgs e)
        {
            if (!VailIP(config.Etcd_addr) && VailPort(config.Etcd_port))
            {
                MessageBox.Show("请输入正确的或端口");
            }
            else
            {
                iniFile.IniWriteValue(Config.ETCD_ADDR, config.Etcd_addr);
                iniFile.IniWriteValue(Config.ETCD_PORT, config.Etcd_port);
            }
        }


    }
}
