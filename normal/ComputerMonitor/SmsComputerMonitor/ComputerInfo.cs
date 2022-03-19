using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;

namespace SmsComputerMonitor
{
    public class ComputerInfo
    {
        #region Fields and Properties

        private readonly ManagementScope _LocalManagementScope;

        private readonly PerformanceCounter _pcCpuLoad; //CPU计数器,全局
        private const int GwHwndfirst = 0;
        private const int GwHwndnext = 2;
        private const int GwlStyle = -16;
        private const int WsBorder = 8388608;
        private const int WsVisible = 268435456;

        #region CPU占用率

        /// <summary>
        ///     获取CPU占用率(系统CPU使用率)
        /// </summary>
        public float CpuLoad => _pcCpuLoad.NextValue();

        #endregion

        #region 本地主机名

        public string HostName => Dns.GetHostName();

        #endregion

        #region 本地IPV4列表

        public List<IPAddress> IpAddressV4S
        {
            get
            {
                var ipV4S = new List<IPAddress>();
                foreach (var address in Dns.GetHostAddresses(Dns.GetHostName()))
                    if (address.AddressFamily == AddressFamily.InterNetwork)
                        ipV4S.Add(address);
                return ipV4S;
            }
        }

        #endregion

        #region 本地IPV6列表

        public List<IPAddress> IpAddressV6S
        {
            get
            {
                var ipV6S = new List<IPAddress>();
                foreach (var address in Dns.GetHostAddresses(Dns.GetHostName()))
                    if (address.AddressFamily == AddressFamily.InterNetworkV6)
                        ipV6S.Add(address);
                return ipV6S;
            }
        }

        #endregion

        #region 可用内存

        /// <summary>
        ///     获取可用内存
        /// </summary>
        public long MemoryAvailable
        {
            get
            {
                long availablebytes = 0;
                var managementClassOs = new ManagementClass("Win32_OperatingSystem");
                foreach (var managementBaseObject in managementClassOs.GetInstances())
                    if (managementBaseObject["FreePhysicalMemory"] != null)
                        availablebytes = 1024 * long.Parse(managementBaseObject["FreePhysicalMemory"].ToString());
                return availablebytes;
            }
        }

        #endregion

        #region 物理内存

        /// <summary>
        ///     获取物理内存
        /// </summary>
        public long PhysicalMemory { get; }

        #endregion

        #region CPU个数

        /// <summary>
        ///     获取CPU个数
        /// </summary>
        // ReSharper disable once UnusedAutoPropertyAccessor.Global
        public int ProcessorCount { get; }

        #endregion

        #region 已用内存大小

        public long SystemMemoryUsed => PhysicalMemory - MemoryAvailable;

        #endregion

        #endregion

        #region  Constructors

        /// <summary>
        ///     构造函数，初始化计数器等
        /// </summary>
        public ComputerInfo()
        {
            _LocalManagementScope = new ManagementScope($"\\\\{HostName}\\root\\cimv2");
            //初始化CPU计数器
            _pcCpuLoad = new PerformanceCounter("Processor", "% Processor Time", "_Total") {MachineName = "."};
            _pcCpuLoad.NextValue();

            //CPU个数
            ProcessorCount = Environment.ProcessorCount;

            //获得物理内存
            var managementClass = new ManagementClass("Win32_ComputerSystem");
            var managementObjectCollection = managementClass.GetInstances();
            foreach (var managementBaseObject in managementObjectCollection)
                if (managementBaseObject["TotalPhysicalMemory"] != null)
                    PhysicalMemory = long.Parse(managementBaseObject["TotalPhysicalMemory"].ToString());
        }

        #endregion

        #region  Methods

        #region 结束指定进程

        /// <summary>
        ///     结束指定进程
        /// </summary>
        /// <param name="pid">进程的 Process ID</param>
        public static void EndProcess(int pid)
        {
            try
            {
                var process = Process.GetProcessById(pid);
                process.Kill();
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
        }

        #endregion

        #region 查找所有应用程序标题

        /// <summary>
        ///     查找所有应用程序标题
        /// </summary>
        /// <returns>应用程序标题范型</returns>
        public static List<string> FindAllApps(int handle)
        {
            var apps = new List<string>();

            var hwCurr = GetWindow(handle, GwHwndfirst);

            while (hwCurr > 0)
            {
                var IsTask = WsVisible | WsBorder;
                var lngStyle = GetWindowLongA(hwCurr, GwlStyle);
                var taskWindow = (lngStyle & IsTask) == IsTask;
                if (taskWindow)
                {
                    var length = GetWindowTextLength(new IntPtr(hwCurr));
                    var sb = new StringBuilder(2 * length + 1);
                    GetWindowText(hwCurr, sb, sb.Capacity);
                    var strTitle = sb.ToString();
                    if (!string.IsNullOrEmpty(strTitle))
                        apps.Add(strTitle);
                }
                hwCurr = GetWindow(hwCurr, GwHwndnext);
            }

            return apps;
        }

        #endregion

        public static List<PerformanceCounterCategory> GetAllCategories(bool isPrintRoot = true,
            bool isPrintTree = true)
        {
            var result = new List<PerformanceCounterCategory>();
            foreach (var category in PerformanceCounterCategory.GetCategories())
            {
                result.Add(category);
                if (isPrintRoot)
                {
                    PerformanceCounter[] categoryCounters;
                    switch (category.CategoryType)
                    {
                        case PerformanceCounterCategoryType.SingleInstance:
                            categoryCounters = category.GetCounters();
                            PrintCategoryAndCounters(category, categoryCounters, isPrintTree);
                            break;
                        case PerformanceCounterCategoryType.MultiInstance:
                            var categoryCounterInstanceNames = category.GetInstanceNames();
                            if (categoryCounterInstanceNames.Length > 0)
                            {
                                categoryCounters = category.GetCounters(categoryCounterInstanceNames[0]);
                                PrintCategoryAndCounters(category, categoryCounters, isPrintTree);
                            }

                            break;
                        case PerformanceCounterCategoryType.Unknown:
                            categoryCounters = category.GetCounters();
                            PrintCategoryAndCounters(category, categoryCounters, isPrintTree);
                            break;
                        //default: break;
                    }
                }
            }
            return result;
        }

        /// <summary>
        ///     获取本地所有磁盘
        /// </summary>
        /// <returns></returns>
        public static DriveInfo[] GetAllLocalDriveInfo()
        {
            return DriveInfo.GetDrives();
        }

        /// <summary>
        ///     获取本机所有进程
        /// </summary>
        /// <returns></returns>
        public static Process[] GetAllProcesses()
        {
            return Process.GetProcesses();
        }

        public static List<PerformanceCounter> GetAppointedCategorieCounters(
            PerformanceCategoryEnums.PerformanceCategoryEnum categorieEnum, bool isPrintRoot = true,
            bool isPrintTree = true)
        {
            var result = new List<PerformanceCounter>();
            var categorieName = PerformanceCategoryEnums.GetCategoryNameString(categorieEnum);
            if (PerformanceCounterCategory.Exists(categorieName))
            {
                var category = new PerformanceCounterCategory(categorieName);
                PerformanceCounter[] categoryCounters;
                switch (category.CategoryType)
                {
                    case PerformanceCounterCategoryType.Unknown:
                        categoryCounters = category.GetCounters();
                        result = categoryCounters.ToList();
                        if (isPrintRoot)
                            PrintCategoryAndCounters(category, categoryCounters, isPrintTree);
                        break;
                    case PerformanceCounterCategoryType.SingleInstance:
                        categoryCounters = category.GetCounters();
                        result = categoryCounters.ToList();
                        if (isPrintRoot)
                            PrintCategoryAndCounters(category, categoryCounters, isPrintTree);
                        break;
                    case PerformanceCounterCategoryType.MultiInstance:
                        var categoryCounterInstanceNames = category.GetInstanceNames();
                        if (categoryCounterInstanceNames.Length > 0)
                        {
                            categoryCounters = category.GetCounters(categoryCounterInstanceNames[0]);
                            result = categoryCounters.ToList();
                            if (isPrintRoot)
                                PrintCategoryAndCounters(category, categoryCounters, isPrintTree);
                        }
                        break;
                    //default: break;
                }
            }
            return result;
        }

        /// <summary>
        ///     获取指定磁盘可用大小
        /// </summary>
        /// <param name="drive"></param>
        /// <returns></returns>
        public static long GetDriveAvailableFreeSpace(DriveInfo drive)
        {
            return drive.AvailableFreeSpace;
        }

        /// <summary>
        ///     获取指定磁盘总空白大小
        /// </summary>
        /// <param name="drive"></param>
        /// <returns></returns>
        public static long GetDriveTotalFreeSpace(DriveInfo drive)
        {
            return drive.TotalFreeSpace;
        }

        /// <summary>
        ///     获取指定磁盘总大小
        /// </summary>
        /// <param name="drive"></param>
        /// <returns></returns>
        public static long GetDriveTotalSize(DriveInfo drive)
        {
            return drive.TotalSize;
        }

        #region 获取当前使用的IP,超时时间至少1s 

        /// <summary>
        ///     获取当前使用的IP,超时时间至少1s
        /// </summary>
        /// <returns></returns>
        public static string GetLocalIP(TimeSpan timeOut, bool isBelieveTimeOutValue = false, bool recordLog = true)
        {
            if (timeOut < new TimeSpan(0, 0, 1))
                timeOut = new TimeSpan(0, 0, 1);
            var isTimeOut = RunApp("route", "print", timeOut, out var result, recordLog);
            if (isTimeOut && !isBelieveTimeOutValue)
                try
                {
                    var tcpClient = new TcpClient();
                    tcpClient.Connect("www.baidu.com", 80);
                    var ip = ((IPEndPoint) tcpClient.Client.LocalEndPoint).Address.ToString();
                    tcpClient.Close();
                    return ip;
                }
                catch (Exception exception)
                {
                    Console.WriteLine(exception);
                    return null;
                }
            var getMatchedGroup = Regex.Match(result, @"0.0.0.0\s+0.0.0.0\s+(\d+.\d+.\d+.\d+)\s+(\d+.\d+.\d+.\d+)");
            if (getMatchedGroup.Success)
                return getMatchedGroup.Groups[2].Value;
            return null;
        }

        #endregion

        #region 获取本机主DNS

        /// <summary>
        ///     获取本机主DNS
        /// </summary>
        /// <returns></returns>
        public static string GetPrimaryDNS(bool recordLog = true)
        {
            RunApp("nslookup", "", new TimeSpan(0, 0, 1), out var result, recordLog, true); //nslookup会超时
            var getMatchedGroup = Regex.Match(result, @"\d+\.\d+\.\d+\.\d+");
            if (getMatchedGroup.Success)
                return getMatchedGroup.Value;
            return null;
        }

        #endregion

        /// <summary>
        ///     获取指定进程最大线程数
        /// </summary>
        /// <returns></returns>
        public static int GetProcessMaxThreadCount(Process process)
        {
            return process.Threads.Count;
        }

        /// <summary>
        ///     获取指定进程最大线程数
        /// </summary>
        /// <returns></returns>
        public static int GetProcessMaxThreadCount(string processName)
        {
            var maxThreadCount = -1;
            foreach (var process in Process.GetProcessesByName(processName))
                if (maxThreadCount < process.Threads.Count)
                    maxThreadCount = process.Threads.Count;
            return maxThreadCount;
        }

        private static void PrintCategoryAndCounters(PerformanceCounterCategory category,
            PerformanceCounter[] categoryCounters, bool isPrintTree)
        {
            Console.WriteLine($@"===============>{category.CategoryName}:[{categoryCounters.Length}]");
            if (isPrintTree)
                foreach (var counter in categoryCounters)
                    Console.WriteLine($@"   ""{category.CategoryName}"", ""{counter.CounterName}""");
        }

        /// <summary>
        ///     运行一个控制台程序并返回其输出参数。耗时至少1s
        /// </summary>
        /// <param name="filename">程序名</param>
        /// <param name="arguments">输入参数</param>
        /// <param name="result"></param>
        /// <param name="recordLog"></param>
        /// <param name="timeOutTimeSpan"></param>
        /// <param name="needKill"></param>
        /// <returns></returns>
        private static bool RunApp(string filename, string arguments, TimeSpan timeOutTimeSpan, out string result,
            bool recordLog = true, bool needKill = false)
        {
            try
            {
                var stopwatch = Stopwatch.StartNew();
                if (recordLog)
                    Console.WriteLine($@"{filename} {arguments}");
                if (timeOutTimeSpan < new TimeSpan(0, 0, 1))
                    timeOutTimeSpan = new TimeSpan(0, 0, 1);
                var isTimeOut = false;
                var process = new Process
                {
                    StartInfo =
                    {
                        FileName = filename,
                        CreateNoWindow = true,
                        Arguments = arguments,
                        RedirectStandardOutput = true,
                        RedirectStandardInput = true,
                        UseShellExecute = false
                    }
                };
                process.Start();
                using (var streamReader = new StreamReader(process.StandardOutput.BaseStream, Encoding.Default))
                {
                    if (needKill)
                    {
                        Thread.Sleep(200);
                        process.Kill();
                    }
                    while (!process.HasExited)
                        if (stopwatch.Elapsed > timeOutTimeSpan)
                        {
                            process.Kill();
                            stopwatch.Stop();
                            isTimeOut = true;
                            break;
                        }
                    result = streamReader.ReadToEnd();
                    streamReader.Close();
                    if (recordLog)
                        Console.WriteLine($@"返回[{result}]耗时：[{stopwatch.Elapsed}]是否超时：{isTimeOut}");
                    return isTimeOut;
                }
            }
            catch (Exception exception)
            {
                result = exception.ToString();
                Console.WriteLine($@"出错[{result}]");
                return true;
            }
        }

        #endregion

        #region 获取本地/远程所有内存信息

        /// <summary>
        ///     需要启动RPC服务,获取本地内存信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32PhysicalMemoryInfos(
            PhysicalMemoryInfoEnums.PhysicalMemoryInfoEnum eEnum = PhysicalMemoryInfoEnums.PhysicalMemoryInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                _LocalManagementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(_LocalManagementScope,
                        new SelectQuery(
                            $"SELECT {PhysicalMemoryInfoEnums.GetQueryString(eEnum)} FROM Win32_PhysicalMemory"))
                    .Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        PhysicalMemoryInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程内存信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32PhysicalMemoryInfos(
            string remoteHostName,
            PhysicalMemoryInfoEnums.PhysicalMemoryInfoEnum eEnum = PhysicalMemoryInfoEnums.PhysicalMemoryInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostName}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                        new SelectQuery(
                            $"SELECT {PhysicalMemoryInfoEnums.GetQueryString(eEnum)} FROM Win32_PhysicalMemory"))
                    .Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        PhysicalMemoryInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程内存信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32PhysicalMemoryInfos(
            IPAddress remoteHostIp,
            PhysicalMemoryInfoEnums.PhysicalMemoryInfoEnum eEnum = PhysicalMemoryInfoEnums.PhysicalMemoryInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostIp}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                        new SelectQuery(
                            $"SELECT {PhysicalMemoryInfoEnums.GetQueryString(eEnum)} FROM Win32_PhysicalMemory"))
                    .Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        PhysicalMemoryInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        #endregion

        #region 获取本地/远程所有原始性能计数器类别信息

        /// <summary>
        ///     需要启动RPC服务,获取本地所有原始性能计数器类别信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>>
            GetWin32PerfRawDataPerfOsMemoryInfos(
                PerfRawDataPerfOsMemoryInfoEnums.PerfRawDataPerfOsMemoryInfoEnum eEnum =
                    PerfRawDataPerfOsMemoryInfoEnums.PerfRawDataPerfOsMemoryInfoEnum.All,
                bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                _LocalManagementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(_LocalManagementScope,
                        new SelectQuery(
                            $"SELECT {PerfRawDataPerfOsMemoryInfoEnums.GetQueryString(eEnum)} FROM Win32_PerfRawData_PerfOS_Memory"))
                    .Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        PerfRawDataPerfOsMemoryInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有原始性能计数器类别信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>>
            GetWin32PerfRawDataPerfOsMemoryInfos(
                string remoteHostName,
                PerfRawDataPerfOsMemoryInfoEnums.PerfRawDataPerfOsMemoryInfoEnum eEnum =
                    PerfRawDataPerfOsMemoryInfoEnums.PerfRawDataPerfOsMemoryInfoEnum.All, bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostName}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                        new SelectQuery(
                            $"SELECT {PerfRawDataPerfOsMemoryInfoEnums.GetQueryString(eEnum)} FROM Win32_PerfRawData_PerfOS_Memory"))
                    .Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        PerfRawDataPerfOsMemoryInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有原始性能计数器类别信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>>
            GetWin32PerfRawDataPerfOsMemoryInfos(
                IPAddress remoteHostIp,
                PerfRawDataPerfOsMemoryInfoEnums.PerfRawDataPerfOsMemoryInfoEnum eEnum =
                    PerfRawDataPerfOsMemoryInfoEnums.PerfRawDataPerfOsMemoryInfoEnum.All, bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostIp}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                        new SelectQuery(
                            $"SELECT {PerfRawDataPerfOsMemoryInfoEnums.GetQueryString(eEnum)} FROM Win32_PerfRawData_PerfOS_Memory"))
                    .Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        PerfRawDataPerfOsMemoryInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        #endregion

        #region 获取本地/远程所有CPU信息

        /// <summary>
        ///     需要启动RPC服务,获取本地所有 CPU 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32ProcessorInfos(
            ProcessorInfoEnums.ProcessorInfoEnum eEnum = ProcessorInfoEnums.ProcessorInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                _LocalManagementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(_LocalManagementScope,
                    new SelectQuery($"SELECT {ProcessorInfoEnums.GetQueryString(eEnum)} FROM Win32_Processor")).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        ProcessorInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有 CPU 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32ProcessorInfos(
            string remoteHostName,
            ProcessorInfoEnums.ProcessorInfoEnum eEnum = ProcessorInfoEnums.ProcessorInfoEnum.All, bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostName}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                    new SelectQuery($"SELECT {ProcessorInfoEnums.GetQueryString(eEnum)} FROM Win32_Processor")).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        ProcessorInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有 CPU 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32ProcessorInfos(
            IPAddress remoteHostIp,
            ProcessorInfoEnums.ProcessorInfoEnum eEnum = ProcessorInfoEnums.ProcessorInfoEnum.All, bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostIp}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                    new SelectQuery($"SELECT {ProcessorInfoEnums.GetQueryString(eEnum)} FROM Win32_Processor")).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        ProcessorInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        #endregion

        #region 获取本地/远程所有进程信息

        /// <summary>
        ///     需要启动RPC服务,获取本地所有 进程 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32ProcessInfos(
            string processName = null, ProcessInfoEnums.ProcessInfoEnum eEnum = ProcessInfoEnums.ProcessInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                _LocalManagementScope.Connect();
                var selectQueryString = new SelectQuery(
                    $"SELECT {ProcessInfoEnums.GetQueryString(eEnum)} FROM Win32_Process");
                if (processName != null)
                    selectQueryString = new SelectQuery(
                        $"SELECT {ProcessInfoEnums.GetQueryString(eEnum)} FROM Win32_Process Where Name = '{processName}'");
                foreach (var managementBaseObject in new ManagementObjectSearcher(_LocalManagementScope,
                    selectQueryString).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        ProcessInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有 进程 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetRemoteWin32ProcessInfos(
            string remoteHostName, string processName = null,
            ProcessInfoEnums.ProcessInfoEnum eEnum = ProcessInfoEnums.ProcessInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostName}\\root\\cimv2");
                managementScope.Connect();
                var selectQueryString = new SelectQuery(
                    $"SELECT {ProcessInfoEnums.GetQueryString(eEnum)} FROM Win32_Process");
                if (processName != null)
                    selectQueryString = new SelectQuery(
                        $"SELECT {ProcessInfoEnums.GetQueryString(eEnum)} FROM Win32_Process Where Name = '{processName}'");
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                    selectQueryString).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        ProcessInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有 进程 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetRemoteWin32ProcessInfos(
            IPAddress remoteHostIp, string processName = null,
            ProcessInfoEnums.ProcessInfoEnum eEnum = ProcessInfoEnums.ProcessInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostIp}\\root\\cimv2");
                managementScope.Connect();
                var selectQueryString = new SelectQuery(
                    $"SELECT {ProcessInfoEnums.GetQueryString(eEnum)} FROM Win32_Process");
                if (processName != null)
                    selectQueryString = new SelectQuery(
                        $"SELECT {ProcessInfoEnums.GetQueryString(eEnum)} FROM Win32_Process Where Name = '{processName}'");
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                    selectQueryString).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        ProcessInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        #endregion

        #region 获取本地/远程所有系统信息

        /// <summary>
        ///     需要启动RPC服务,获取本地所有 系统 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32OperatingSystemInfos(
            OperatingSystemInfoEnums.OperatingSystemInfoEnum eEnum =
                OperatingSystemInfoEnums.OperatingSystemInfoEnum.All,
            bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                _LocalManagementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(_LocalManagementScope,
                    new SelectQuery(
                        $"SELECT {OperatingSystemInfoEnums.GetQueryString(eEnum)} FROM Win32_OperatingSystem")).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        OperatingSystemInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有 系统 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32OperatingSystemInfos(
            string remoteHostName,
            OperatingSystemInfoEnums.OperatingSystemInfoEnum eEnum =
                OperatingSystemInfoEnums.OperatingSystemInfoEnum.All, bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostName}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                    new SelectQuery(
                        $"SELECT {OperatingSystemInfoEnums.GetQueryString(eEnum)} FROM Win32_OperatingSystem")).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        OperatingSystemInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        /// <summary>
        ///     需要启动RPC服务,获取远程所有 系统 信息：
        /// </summary>
        /// <returns></returns>
        public List<Dictionary<string, string>> GetWin32OperatingSystemInfos(
            IPAddress remoteHostIp,
            OperatingSystemInfoEnums.OperatingSystemInfoEnum eEnum =
                OperatingSystemInfoEnums.OperatingSystemInfoEnum.All, bool isPrint = true)
        {
            var result = new List<Dictionary<string, string>>();
            try
            {
                var managementScope = new ManagementScope($"\\\\{remoteHostIp}\\root\\cimv2");
                managementScope.Connect();
                foreach (var managementBaseObject in new ManagementObjectSearcher(managementScope,
                    new SelectQuery(
                        $"SELECT {OperatingSystemInfoEnums.GetQueryString(eEnum)} FROM Win32_OperatingSystem")).Get())
                {
                    if (isPrint)
                        Console.WriteLine($@"<=========={managementBaseObject}===========>");
                    var thisObjectsDictionary = new Dictionary<string, string>();
                    foreach (var property in managementBaseObject.Properties)
                    {
                        if (isPrint)
                            Console.WriteLine(
                                $@"[{property.Name,40}] -> [{property.Value,-40}]//{
                                        OperatingSystemInfoEnums.GetDescriptionString(property.Name)
                                    }.");
                        thisObjectsDictionary.Add(property.Name, property.Value.ToString());
                    }
                    result.Add(thisObjectsDictionary);
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
            return result;
        }

        #endregion

        #region 单位转换进制

        private const int KbDiv = 1024;
        private const int MbDiv = 1024 * 1024;
        private const int GbDiv = 1024 * 1024 * 1024;

        #endregion

        #region 单个程序Cpu使用大小

        /// <summary>
        ///     获取进程一段时间内cpu平均使用率（有误差），最低500ms 内的平均值
        /// </summary>
        /// <returns></returns>
        public double GetProcessCpuProcessorRatio(Process process, TimeSpan interVal)
        {
            if (!process.HasExited)
            {
                var processorTime = new PerformanceCounter("Process", "% Processor Time", process.ProcessName);
                processorTime.NextValue();
                if (interVal.TotalMilliseconds < 500)
                    interVal = new TimeSpan(0, 0, 0, 0, 500);
                Thread.Sleep(interVal);
                return processorTime.NextValue() / Environment.ProcessorCount;
            }
            return 0;
        }

        /// <summary>
        ///     获取进程一段时间内的平均cpu使用率（有误差），最低500ms 内的平均值
        /// </summary>
        /// <returns></returns>
        public double GetProcessCpuProcessorTime(Process process, TimeSpan interVal)
        {
            if (!process.HasExited)
            {
                var prevCpuTime = process.TotalProcessorTime;
                if (interVal.TotalMilliseconds < 500)
                    interVal = new TimeSpan(0, 0, 0, 0, 500);
                Thread.Sleep(interVal);
                var curCpuTime = process.TotalProcessorTime;
                var value = (curCpuTime - prevCpuTime).TotalMilliseconds / (interVal.TotalMilliseconds - 10) /
                            Environment.ProcessorCount * 100;
                return value;
            }
            return 0;
        }

        #endregion

        #region 单个程序内存使用大小

        /// <summary>
        ///     获取关联进程分配的物理内存量,工作集(进程类)
        /// </summary>
        /// <returns></returns>
        public long GetProcessWorkingSet64Kb(Process process)
        {
            if (!process.HasExited)
                return process.WorkingSet64 / KbDiv;
            return 0;
        }

        /// <summary>
        ///     获取进程分配的物理内存量,公有工作集
        /// </summary>
        /// <returns></returns>
        public float GetProcessWorkingSetKb(Process process)
        {
            if (!process.HasExited)
            {
                var processWorkingSet = new PerformanceCounter("Process", "Working Set", process.ProcessName);
                return processWorkingSet.NextValue() / KbDiv;
            }
            return 0;
        }

        /// <summary>
        ///     获取进程分配的物理内存量,私有工作集
        /// </summary>
        /// <returns></returns>
        public float GetProcessWorkingSetPrivateKb(Process process)
        {
            if (!process.HasExited)
            {
                var processWorkingSetPrivate =
                    new PerformanceCounter("Process", "Working Set - Private", process.ProcessName);
                return processWorkingSetPrivate.NextValue() / KbDiv;
            }
            return 0;
        }

        #endregion

        #region 系统内存使用大小

        /// <summary>
        ///     系统内存使用大小Mb
        /// </summary>
        /// <returns></returns>
        public long GetSystemMemoryDosageMb()
        {
            return SystemMemoryUsed / MbDiv;
        }


        /// <summary>
        ///     系统内存使用大小Gb
        /// </summary>
        /// <returns></returns>
        public long GetSystemMemoryDosageGb()
        {
            return SystemMemoryUsed / GbDiv;
        }

        #endregion

        #region AIP声明

        [DllImport("IpHlpApi.dll")]
        public static extern uint GetIfTable(byte[] pIfTable, ref uint pdwSize, bool bOrder);

        [DllImport("User32")]
        private static extern int GetWindow(int hWnd, int wCmd);

        [DllImport("User32")]
        private static extern int GetWindowLongA(int hWnd, int wIndx);

        [DllImport("user32.dll")]
        private static extern bool GetWindowText(int hWnd, StringBuilder title, int maxBufSize);

        [DllImport("user32", CharSet = CharSet.Auto)]
        private static extern int GetWindowTextLength(IntPtr hWnd);

        #endregion
    }
}