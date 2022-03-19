using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Xml.Linq;

namespace SmsComputerMonitor
{
    /// <summary>
    ///     MainWindow.xaml 的交互逻辑
    /// </summary>
    public sealed partial class MainWindow : INotifyPropertyChanged
    {
        #region Fields and Properties

        private readonly SimpleClock _clock = new SimpleClock(Settings.Default.InterTime);

        private readonly ComputerInfo _computerInfo = new ComputerInfo();

        private readonly Queue<string> _messageQueue = new Queue<string>();

        private readonly NetworkMonitor _monitor = new NetworkMonitor();

        private readonly List<Tuple<string, int>> _processMoniterList = new List<Tuple<string, int>>();

        private int _continuityNoticeFrequency;

        public enum FunctionEnum
        {
            CheckCpu = 0b0000_0000_0000_0001, //1
            CheckMemory = 0b0000_0000_0000_0010, //2
            ChechThreadCount = 0b0000_0000_0000_0100, //3
            CheckDriver = 0b0000_0000_0000_1000, //4

            CheckNetWork = 0b0000_0000_0001_0000 //5
            //Check
            //Check
        }

        #endregion

        #region  Constructors

        public MainWindow()
        {
            InitConfig();
            NotifyIconFunction.GetInstance().SetNotifyIcon(this);
            InitializeComponent();
        }

        #endregion

        #region  Methods

        private bool ChechThreadCountMethod(string ip, DateTime clock)
        {
            var testPoint = (int)FunctionEnum.ChechThreadCount;
            if ((FunctionOpenedList & testPoint) != testPoint)
                return false;
            try
            {
                if (_processMoniterList.Count == 0)
                {
                    MessageCache = "要检测的进程数量为0，请检查！";
                }
                foreach (var tupleProcess in _processMoniterList)
                {
                    var thisProcessMaxThreadCount = ComputerInfo.GetProcessMaxThreadCount(tupleProcess.Item1);
                    if (thisProcessMaxThreadCount < 0)
                    {
                        MessageCache = $"未找到进程名为{tupleProcess.Item1}的进程，请检查";
                        continue;
                    }
                    var addMessage = $"{clock}:{tupleProcess.Item1}进程的最大线程数:[{thisProcessMaxThreadCount}]";
                    MessageCache = addMessage;
                    LogHelper.LogInfo($"ThreadCountCheck | {addMessage}");
                    if (thisProcessMaxThreadCount > tupleProcess.Item2)
                    {
                        LogHelper.LogWarn(
                            $"ThreadCountCheck | {clock}:{tupleProcess.Item1} 线程数超载[{thisProcessMaxThreadCount}>{tupleProcess.Item2}]");
                        if (_continuityNoticeFrequency < Settings.Default.ContinuityNoticeFrequency)
                        {
                            var addWarnMessage =
                                $"{clock}:!{tupleProcess.Item1} 线程数超载[{thisProcessMaxThreadCount}>{tupleProcess.Item2}]!,发送短信至：[{Settings.Default.PhoneList}]";
                            MessageCache = addWarnMessage;
                            SendSmsToPhones(
                                $"{clock}:[{ip}]{tupleProcess.Item1} 线程数超载[{thisProcessMaxThreadCount}>{tupleProcess.Item2}]");
                        }
                        else
                        {
                            MessageCache =
                                $"{clock}:!{tupleProcess.Item1} 线程数超载[{thisProcessMaxThreadCount}>{tupleProcess.Item2}]!,连续报错已经警示{_continuityNoticeFrequency}次 > 连续发送短信警示频率{Settings.Default.ContinuityNoticeFrequency}次！不再发送短信。";
                        }
                        return true;
                    }
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"ChechThreadCountMethod Error:{exception.Message}";
                LogHelper.LogError("ChechThreadCountMethod Error!", exception);
            }
            return false;
        }

        private bool CheckCpuMethod(string ip, DateTime clock)
        {
            var testPoint = (int)FunctionEnum.CheckCpu;
            if ((FunctionOpenedList & testPoint) != testPoint)
                return false;
            try
            {
                double thisCpuload = _computerInfo.CpuLoad;
                var addMessage = $"{clock}:cpu使用率[{thisCpuload:F}%]";
                MessageCache = addMessage;
                LogHelper.LogInfo($"CpuCheck | {addMessage}");
                if (thisCpuload > Settings.Default.CpuLimitedValue)
                {
                    LogHelper.LogWarn(
                        $"CpuCheck | {clock}:CPU超载[{thisCpuload:F}%>{Settings.Default.CpuLimitedValue:F}%]");
                    if (_continuityNoticeFrequency < Settings.Default.ContinuityNoticeFrequency)
                    {
                        var addWarnMessage =
                            $"{clock}:!CPU超载[{thisCpuload:F}%>{Settings.Default.CpuLimitedValue:F}%]!,发送短信至：[{Settings.Default.PhoneList}]";
                        MessageCache = addWarnMessage;
                        SendSmsToPhones($"{clock}:[{ip}]CPU超载[{thisCpuload:F}%>{Settings.Default.CpuLimitedValue:F}%]");
                    }
                    else
                    {
                        MessageCache =
                            $"{clock}:!CPU超载[{thisCpuload:F}%>{Settings.Default.CpuLimitedValue:F}%]!,连续报错已经警示{_continuityNoticeFrequency}次 > 连续发送短信警示频率{Settings.Default.ContinuityNoticeFrequency}次！不再发送短信。";
                    }
                    return true;
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"CheckCpuMethod Error:{exception.Message}";
                LogHelper.LogError("CheckCpuMethod Error!", exception);
            }
            return false;
        }

        private bool CheckDriverMethod(string ip, DateTime clock)
        {
            var testPoint = (int)FunctionEnum.CheckDriver;
            if ((FunctionOpenedList & testPoint) != testPoint)
                return false;
            try
            {
                foreach (var drive in ComputerInfo.GetAllLocalDriveInfo())
                {
                    var freeSpace = ComputerInfo.GetDriveAvailableFreeSpace(drive);
                    var totalSize = ComputerInfo.GetDriveTotalSize(drive);
                    double percentUsed = 1 - freeSpace / totalSize;
                    var addMessage =
                        $"{clock}:[{drive.Name}][可用{freeSpace / GbDiv}/{totalSize / GbDiv}GB(已用{percentUsed:F}%)]";
                    MessageCache = addMessage;
                    LogHelper.LogInfo($"DriverCheck | {addMessage}");
                    if (percentUsed > Settings.Default.DrivePercentUsedLimited)
                    {
                        LogHelper.LogWarn(
                            $"DriverCheck | {clock}:[{drive.Name}]盘过载[可用{freeSpace / MbDiv}/{totalSize / MbDiv}MB(已用{percentUsed:F}%>{Settings.Default.DrivePercentUsedLimited}%)]");
                        if (_continuityNoticeFrequency < Settings.Default.ContinuityNoticeFrequency)
                        {
                            var addWarnMessage =
                                $"{clock}:![{drive.Name}]盘过载[可用{freeSpace / GbDiv}/{totalSize / GbDiv}GB(已用{percentUsed:F}%>{Settings.Default.DrivePercentUsedLimited}%)]!,发送短信至：[{Settings.Default.PhoneList}]";
                            MessageCache = addWarnMessage;
                            SendSmsToPhones(
                                $"{clock}:[{ip}][{drive.Name}]盘过载[可用{freeSpace / GbDiv}/{totalSize / GbDiv}GB(已用{percentUsed:F}%>{Settings.Default.DrivePercentUsedLimited}%)]");
                        }
                        else
                        {
                            MessageCache =
                                $"{clock}:![{drive.Name}]盘过载[可用{freeSpace / GbDiv}/{totalSize / GbDiv}GB(已用{percentUsed:F}%>{Settings.Default.DrivePercentUsedLimited}%)]!,连续报错已经警示{_continuityNoticeFrequency}次 > 连续发送短信警示频率{Settings.Default.ContinuityNoticeFrequency}次！不再发送短信。";
                        }
                        return true;
                    }
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"CheckDriverMethod Error:{exception.Message}";
                LogHelper.LogError("CheckDriverMethod Error!", exception);
            }
            return false;
        }

        private bool CheckMemoryMethod(string ip, DateTime clock)
        {
            var testPoint = (int)FunctionEnum.CheckMemory;
            if ((FunctionOpenedList & testPoint) != testPoint)
                return false;
            try
            {
                double totalMemory = _computerInfo.PhysicalMemory;
                double freeMemory = _computerInfo.MemoryAvailable;
                double percentUsed = _computerInfo.GetSystemMemoryDosageMb()/(totalMemory / MbDiv)*100;
                var addMessage =
                    $"{clock}:内存占用率[可用{freeMemory / GbDiv:F}/{totalMemory / GbDiv:F}GB(已用{percentUsed:F}%)]";
                MessageCache = addMessage;
                LogHelper.LogInfo($"MemoryCheck | {addMessage}");
                if (percentUsed > Settings.Default.MemoryPercentUsedLimited)
                {
                    LogHelper.LogWarn(
                        $"MemoryCheck | {clock}:内存超载[可用{freeMemory / MbDiv:F}/{totalMemory / MbDiv:F}MB(已用{percentUsed:F}%>{Settings.Default.MemoryPercentUsedLimited}%)]");
                    if (_continuityNoticeFrequency < Settings.Default.ContinuityNoticeFrequency)
                    {
                        var addWarnMessage =
                            $"{clock}:!内存超载[可用{freeMemory / GbDiv:F}/{totalMemory / GbDiv:F}GB(已用{percentUsed:F}%>{Settings.Default.MemoryPercentUsedLimited}%)]!,发送短信至：[{Settings.Default.PhoneList}]";
                        MessageCache = addWarnMessage;
                        SendSmsToPhones(
                            $"{clock}:[{ip}]内存超载[可用{freeMemory / GbDiv:F}/{totalMemory / GbDiv:F}GB(已用{percentUsed:F}%>{Settings.Default.MemoryPercentUsedLimited}%)]");
                    }
                    else
                    {
                        MessageCache =
                            $"{clock}:!内存超载[可用{freeMemory / GbDiv:F}/{totalMemory / GbDiv:F}GB(已用{percentUsed:F}%>{Settings.Default.MemoryPercentUsedLimited}%)]!,发送短信至：[{Settings.Default.PhoneList}]!,连续报错已经警示{_continuityNoticeFrequency}次 > 连续发送短信警示频率{Settings.Default.ContinuityNoticeFrequency}次！不再发送短信。";
                    }
                    return true;
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"CheckMemoryMethod Error:{exception.Message}";
                LogHelper.LogError("CheckMemoryMethod Error!", exception);
            }
            return false;
        }

        private bool CheckNetWorkMethod(string ip, DateTime clock)
        {
            var testPoint = (int)FunctionEnum.CheckNetWork;
            if ((FunctionOpenedList & testPoint) != testPoint)
                return false;
            try
            {
                var totalNetSpeed = SelectedAdapter.DownloadSpeedKbps + SelectedAdapter.UploadSpeedKbps;
                var addMessage =
                    $"{clock}:上传{SelectedAdapter.UploadSpeedKbps:F}/下载{SelectedAdapter.DownloadSpeedKbps:F}Kbps【网络总负载[{totalNetSpeed:F}kbps]】";
                MessageCache = addMessage;
                LogHelper.LogInfo($"NetWorkCheck | {addMessage}");
                if (totalNetSpeed > Settings.Default.TotalNetSpeedKbpsLimited)
                {
                    LogHelper.LogWarn(
                        $"NetWorkCheck | {clock}:网络超载[(上传{SelectedAdapter.UploadSpeed}/下载{SelectedAdapter.DownloadSpeed}字节每秒){totalNetSpeed:F}Kbps>{Settings.Default.TotalNetSpeedKbpsLimited:F}Kbps]");
                    if (_continuityNoticeFrequency < Settings.Default.ContinuityNoticeFrequency)
                    {
                        var addWarnMessage =
                            $"{clock}:!网络超载[(上传{SelectedAdapter.UploadSpeed / KbDiv:F}/下载{SelectedAdapter.DownloadSpeed / KbDiv:F}Kbps){totalNetSpeed:F}Kbps>{Settings.Default.TotalNetSpeedKbpsLimited:F}Kbps]!,发送短信至：[{Settings.Default.PhoneList}]";
                        MessageCache = addWarnMessage;
                        SendSmsToPhones(
                            $"{clock}:[{ip}]网络超载[(上传{SelectedAdapter.UploadSpeed / KbDiv:F}/下载{SelectedAdapter.DownloadSpeed / KbDiv:F}Kbps){totalNetSpeed:F}Kbps>{Settings.Default.TotalNetSpeedKbpsLimited:F}Kbps]");
                    }
                    else
                    {
                        MessageCache =
                            $"{clock}:!网络超载[(上传{SelectedAdapter.UploadSpeed / KbDiv:F}/下载{SelectedAdapter.DownloadSpeed / KbDiv:F}Kbps){totalNetSpeed:F}Kbps>{Settings.Default.TotalNetSpeedKbpsLimited:F}Kbps]!,连续报错已经警示{_continuityNoticeFrequency}次 > 连续发送短信警示频率{Settings.Default.ContinuityNoticeFrequency}次！不再发送短信。";
                    }
                    return true;
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"CheckNetWorkMethod Error:{exception.Message}";
                LogHelper.LogError("CheckNetWorkMethod Error!", exception);
            }
            return false;
        }

        private void ClockOnClockChanged(DateTime clock)
        {
            try
            {
                Task.Factory.StartNew(() =>
                {
                    var computerInfo = new ComputerInfo();
                    var thisIp = computerInfo.IpAddressV4S.Count > 0 ? computerInfo.IpAddressV4S[0].ToString() : "";
                    var isCpuBad = CheckCpuMethod(thisIp, clock);
                    var isMemoryBad = CheckMemoryMethod(thisIp, clock);
                    var isThreadCountBad = ChechThreadCountMethod(thisIp, clock);
                    var isDriverBad = CheckDriverMethod(thisIp, clock);
                    var isNetWorkBad = CheckNetWorkMethod(thisIp, clock);
                    if (isCpuBad || isMemoryBad || isThreadCountBad || isDriverBad || isNetWorkBad)
                        _continuityNoticeFrequency++;
                    else
                        _continuityNoticeFrequency = 0;
                });
            }
            catch (Exception exception)
            {
                MessageCache = $"============>定时器内部出错:[{exception.Message}]！<=============";
                LogHelper.LogError(exception.Message);
            }
        }

        private void InitConfig()
        {
            try
            {
                NetWorkInit();
                ProcessMoniterConfigInit();

                _clock.ClockChanged += ClockOnClockChanged;
                MessageCache = $">设定显示缓存信息:[{Settings.Default.CacheMaxCount}]条";
                MessageCache = $">设定连续通告次数:[{Settings.Default.ContinuityNoticeFrequency}]次";
                //cpu
                MessageCache = $">设定定时检查时间间隔:[{Settings.Default.InterTime}]秒";
                MessageCache = $">设定发送手机列表:[{string.Join(",", Settings.Default.PhoneList.Split('|'))}]";
                MessageCache = $">设定Sms调用接口:[{Settings.Default.SmsUrl}]";
                MessageCache = "============>定时检查系统设置读取完毕！<=============";
            }
            catch (Exception exception)
            {
                MessageCache = $"============>定时检查系统配置读取失败:[{exception.Message}]！<=============";
                LogHelper.LogError(exception.Message);
            }
        }

        private void MainWindow_OnClosing(object sender, CancelEventArgs e)
        {
            _monitor.StopMonitoring();
        }

        private void NetWorkInit()
        {
            try
            {
                if (_monitor.Adapters.Length == 0)
                {
                    MessageBox.Show(@"No network adapters found on this computer.");
                }
                else
                {
                    foreach (var adapter in _monitor.Adapters)
                        Adapters.Add(adapter);
                    SelectedAdapter = _monitor.Adapters[0];
                    _monitor.StopMonitoring();
                    // Start a timer to obtain new performance counter sample every second. 
                    _monitor.StartMonitoring(SelectedAdapter);
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"NetWorkInit Error:{exception.Message}";
                LogHelper.LogError("NetWorkInit Error!", exception);
            }
        }

        private void ProcessMoniterConfigInit()
        {
            try
            {
                var processConfigPath = $@"{AppDomain.CurrentDomain.BaseDirectory}ProcessMoniterConfig.xml";
                if (File.Exists(processConfigPath))
                {
                    var xml = XElement.Load(processConfigPath);
                    var itemList = xml.Element("Process")?.Elements("item");
                    var itemElements = itemList as XElement[] ?? itemList?.ToArray();
                    if (itemList != null && itemElements.Any())
                        foreach (var item in itemElements)
                            _processMoniterList.Add(new Tuple<string, int>(item.Element("ProcessName")?.Value,
                                Convert.ToInt32(item.Element("MaxThreadsCount")?.Value)));
                }
            }
            catch (Exception exception)
            {
                MessageCache = $@"ProcessMoniterConfigInit Error:{exception.Message}";
                LogHelper.LogError("ProcessMoniterConfigInit Error!", exception);
            }
        }

        private void SendSmsToPhones(string content)
        {
            foreach (var phone in Settings.Default.PhoneList.Split('|'))
                SmsSender.SendSms(phone, content);
        }

        #endregion

        #region 单位转换进制

        private const int KbDiv = 1024;
        private const int MbDiv = 1024 * 1024;
        private const int GbDiv = 1024 * 1024 * 1024;

        #endregion

        #region RelayCommand OpenCmdCmd

        private RelayCommand _OpenCmdCmd;

        public ICommand OpenCmdCmd => _OpenCmdCmd ?? (_OpenCmdCmd = new RelayCommand(OpenCmd));

        private void OpenCmd(object oParameter)
        {
            if (oParameter is string strParameter)
            {
                var exePath = $@"{AppDomain.CurrentDomain.BaseDirectory}\EmEditor\EmEditor.exe";
                if (!File.Exists(exePath))
                    MessageBox.Show("EmEditor文件不存在！");
                switch (strParameter.ToUpper())
                {
                    case "LOGDIR":
                        Process.Start("ExpLorer", Path.Combine(AppDomain.CurrentDomain.BaseDirectory, @"\logs\"));
                        break;
                    case "INFOLOG":
                        var infologPath =
                            $@"{AppDomain.CurrentDomain.BaseDirectory}\logs\info\{DateTime.Now:yyyy-MM-dd}.log";
                        if (File.Exists(infologPath))
                            Process.Start($"\"{exePath}\"", $"\"{infologPath}\"");
                        else
                            MessageBox.Show("文件不存在！");
                        break;
                    case "ERRORLOG":
                        var errorlogPath =
                            $@"{AppDomain.CurrentDomain.BaseDirectory}\logs\error\{DateTime.Now:yyyy-MM-dd}.log";
                        if (File.Exists(exePath) && File.Exists(errorlogPath))
                            Process.Start($"\"{exePath}\"", $"\"{errorlogPath}\"");
                        else
                            MessageBox.Show("文件不存在！");
                        break;
                    case "WARNLOG":
                        var warnlogPath =
                            $@"{AppDomain.CurrentDomain.BaseDirectory}\logs\warn\{DateTime.Now:yyyy-MM-dd}.log";
                        if (File.Exists(exePath) && File.Exists(warnlogPath))
                            Process.Start($"\"{exePath}\"", $"\"{warnlogPath}\"");
                        else
                            MessageBox.Show("文件不存在！");
                        break;
                    case "CPUPRESS":
                        var cpuPressExePath =
                            $@"{AppDomain.CurrentDomain.BaseDirectory}\CpuPress.exe";
                        if (File.Exists(cpuPressExePath))
                            Process.Start($"\"{cpuPressExePath}\"");
                        else
                            MessageBox.Show("文件不存在！");
                        break;
                    case "SMSTEST":
                        SendSmsToPhones("信息测试！");
                        MessageCache = "测试信息发送完毕！";
                        break;

                        //default: break;
                }
            }
        }

        #endregion

        #region INotifyProperties

        #region OnPropertyChanged(INotifyPropertyChanged)

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

        #region string MessageCache(INotifyPropertyChangedProperty)

        private string _MessageCache;

        public string MessageCache
        {
            get => _MessageCache;
            set
            {
                if (_MessageCache != null && _MessageCache.Equals(value)) return;
                _messageQueue.Enqueue(value);
                if (_messageQueue.Count > Settings.Default.CacheMaxCount)
                    _messageQueue.Dequeue();
                _MessageCache = string.Join("\n", _messageQueue);
                OnPropertyChanged();
            }
        }

        #endregion

        #region int FunctionOpenedList(INotifyPropertyChangedProperty)

        private int _FunctionOpenedList = 0b1111_1111_1111_1111;

        public int FunctionOpenedList
        {
            get => _FunctionOpenedList;
            set
            {
                if (value == 0) return;
                var testPosition = 0b0000_0000_0000_0001 << (Math.Abs(value) - 1);
                if (value > 0)
                {
                    MessageCache = $"#>{Enum.GetName(typeof(FunctionEnum), testPosition)}功能已经打开！";
                    _FunctionOpenedList = _FunctionOpenedList | testPosition;
                }
                else
                {
                    MessageCache = $"##>{Enum.GetName(typeof(FunctionEnum), testPosition)}功能已经关闭！";
                    _FunctionOpenedList = _FunctionOpenedList & ~testPosition;
                }

                OnPropertyChanged();
            }
        }

        #endregion

        #region ObservableCollection<NetworkMonitor.NetworkAdapter> Adapters(INotifyPropertyChangedProperty)

        private ObservableCollection<NetworkMonitor.NetworkAdapter> _Adapters =
            new ObservableCollection<NetworkMonitor.NetworkAdapter>();

        public ObservableCollection<NetworkMonitor.NetworkAdapter> Adapters
        {
            get => _Adapters;
            set
            {
                if (_Adapters != null && _Adapters.Equals(value)) return;
                _Adapters = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region NetworkMonitor.NetworkAdapter SelectedAdapter(INotifyPropertyChangedProperty)

        private NetworkMonitor.NetworkAdapter _SelectedAdapter;

        public NetworkMonitor.NetworkAdapter SelectedAdapter
        {
            get => _SelectedAdapter;
            set
            {
                if (_SelectedAdapter != null && _SelectedAdapter.Equals(value)) return;
                _SelectedAdapter = value;
                _monitor.StopMonitoring();
                // Start a timer to obtain new performance counter sample every second. 
                _monitor.StartMonitoring(value);
                OnPropertyChanged();
            }
        }

        #endregion

        #endregion
    }
}