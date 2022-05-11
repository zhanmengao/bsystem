using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
namespace WpfApp1
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private const string START_MENU = @"C:\ProgramData\Microsoft\Windows\Start Menu\Programs\";
        ObservableCollection<RemoteAppInfo> localAppList = new ObservableCollection<RemoteAppInfo>();
        ObservableCollection<RemoteAppInfo> readyAppList = new ObservableCollection<RemoteAppInfo>();
        IList selectedLocalAppList = new List<RemoteAppInfo>();
        IList selectedRemoteAppList = new List<RemoteAppInfo>();
        public MainWindow()
        {
            InitializeComponent();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitListView();
            LoadLocalSoftwareShortcutFile();
            LoadRemoteAppList();
           
        }
     

        private void InitListView()
        {
            listview_local.ItemsSource = localAppList;
            listview_ready.ItemsSource = readyAppList;
        }

        private void LoadLocalSoftwareShortcutFile()
        {
            List<string> shortcutFilePathList = FileTool.ScanAll(START_MENU);
            foreach (string path in shortcutFilePathList)
            {
                RemoteAppInfo info = FileTool.GetShortcutInfo(path);
                if (info != null && info.Path != ""
                    && !@"c:\windows".Equals(info.Path.Substring(0, 10).ToLower())
                    )
                {
                    info.ImageStr = IconTool.GetImageBase64String(info.Path);
                    localAppList.Add(info);
                }
            }
            tab_local.Header = "本地程序(" + localAppList.Count + ")";
        }
        private void LoadRemoteAppList()
        {
            readyAppList.Clear();
            string[] appKeys = RegTool.GetApplicationKeys();
            int index = 0;
            if (appKeys != null && appKeys.Length > 0)
            {
                tab_ready.Header = "已发布(" + appKeys.Length + ")";
                foreach (string appKey in appKeys)
                {
                    RemoteAppInfo info = RegTool.GetRemoteAppInfo(appKey);
                    info.Index = index++;
                    readyAppList.Add(info);
                }
            }
            else { tab_ready.Header = "已发布(空)"; }
        }

        private void Write(IList list)
        {
            foreach (RemoteAppInfo info in list)
            {
                Debug.WriteLine(info);
                RegTool.WriteRemoteApp(info);
            }
            LoadRemoteAppList();
        }

        private void Listview_local_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selected = listview_local.SelectedItems;
            publish_btn.Content = "已选" + selected.Count + "款，发布";
            selectedLocalAppList = selected;
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
                //此处做你想做的事 ...=ofd.FileName; 
                var shortName = ofd.SafeFileName;
                var path = ofd.FileName;
                Debug.WriteLine(ofd.SafeFileName);
                localAppList.Add(new RemoteAppInfo()
                {
                    Name = shortName,
                    Path = path,
                    ImageStr = IconTool.GetImageBase64String(path),
                });
            }
        }

        private void Publish_btn_Click(object sender, RoutedEventArgs e)
        {
            tab_ready.IsSelected = true;
            Debug.WriteLine("已选择{0}", selectedLocalAppList.Count);
            Write(selectedLocalAppList);
        }

        private void Remove_btn_Click(object sender, RoutedEventArgs e)
        {
            foreach (RemoteAppInfo info in selectedRemoteAppList)
            {
                RegTool.RemoveRemoteApp(info);
            }
            LoadRemoteAppList();
        }


        private void Listview_ready_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selected = listview_ready.SelectedItems;
            remove_btn.Content = "已选" + selected.Count + "款，移除";
            selectedRemoteAppList = selected;
        }
    }
}
