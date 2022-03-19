using System;
using System.Collections.Generic;

namespace SmsComputerMonitor
{
    public static class ProcessInfoEnums
    {
        #region Fields and Properties

        private static readonly Dictionary<ProcessInfoEnum, Tuple<string, string>>
            ThisEnumDictionary = new Dictionary<ProcessInfoEnum, Tuple<string, string>>
            {
                {ProcessInfoEnum.Caption, new Tuple<string, string>("Caption", "标题")},
                {ProcessInfoEnum.CommandLine, new Tuple<string, string>("CommandLine", "命令行")},
                {ProcessInfoEnum.CreationClassName, new Tuple<string, string>("CreationClassName", "创建实例时使用的类或子类的名称")},
                {ProcessInfoEnum.CreationDate, new Tuple<string, string>("CreationDate", "创建日期")},
                {
                    ProcessInfoEnum.CsCreationClassName,
                    new Tuple<string, string>("CSCreationClassName", "范围界定计算机系统的创建类名称")
                },
                {ProcessInfoEnum.CsName, new Tuple<string, string>("CSName", "计算机系统名称")},
                {ProcessInfoEnum.Description, new Tuple<string, string>("Description", "描述")},
                {ProcessInfoEnum.ExecutablePath, new Tuple<string, string>("ExecutablePath", "进程的可执行文件的路径")},
                {ProcessInfoEnum.ExecutionState, new Tuple<string, string>("ExecutionState", "当前运行状况的过程")},
                {ProcessInfoEnum.Handle, new Tuple<string, string>("Handle", "流程标识符")},
                {ProcessInfoEnum.HandleCount, new Tuple<string, string>("HandleCount", "该进程拥有的打开句柄的总数")},
                {ProcessInfoEnum.InstallDate, new Tuple<string, string>("InstallDate", "安装对象的日期")},
                {ProcessInfoEnum.KernelModeTime, new Tuple<string, string>("KernelModeTime", "内核模式下的时间")},
                {
                    ProcessInfoEnum.MaximumWorkingSetSize,
                    new Tuple<string, string>("MaximumWorkingSetSize", "最大工作集大小的过程")
                },
                {
                    ProcessInfoEnum.MinimumWorkingSetSize,
                    new Tuple<string, string>("MinimumWorkingSetSize", "过程的最小工作集大小")
                },
                {ProcessInfoEnum.Name, new Tuple<string, string>("Name", "负责该进程的可执行文件的名称")},
                {ProcessInfoEnum.OsCreationClassName, new Tuple<string, string>("OSCreationClassName", "操作系统类别名称")},
                {ProcessInfoEnum.OsName, new Tuple<string, string>("OSName", "操作系统名称")},
                {ProcessInfoEnum.OtherOperationCount, new Tuple<string, string>("OtherOperationCount", "其他操作计数")},
                {ProcessInfoEnum.OtherTransferCount, new Tuple<string, string>("OtherTransferCount", "其他传输计数")},
                {ProcessInfoEnum.PageFaults, new Tuple<string, string>("PageFaults", "页面错误")},
                {ProcessInfoEnum.PageFileUsage, new Tuple<string, string>("PageFileUsage", "页面文件使用量")},
                {ProcessInfoEnum.ParentProcessId, new Tuple<string, string>("ParentProcessId", "创建进程的进程的唯一标识符")},
                {
                    ProcessInfoEnum.PeakPageFileUsage,
                    new Tuple<string, string>("PeakPageFileUsage", "进程生命周期中使用的最大页面文件空间量")
                },
                {ProcessInfoEnum.PeakVirtualSize, new Tuple<string, string>("PeakVirtualSize", "任何一个进程使用的最大虚拟地址空间")},
                {ProcessInfoEnum.PeakWorkingSetSize, new Tuple<string, string>("PeakWorkingSetSize", "峰值工作集大小")},
                {ProcessInfoEnum.Priority, new Tuple<string, string>("Priority", "调度操作系统中进程的优先级")},
                {
                    ProcessInfoEnum.PrivatePageCount,
                    new Tuple<string, string>("PrivatePageCount", "当前分配的页面只能由此Win32_Process实例表示的进程访问")
                },
                {ProcessInfoEnum.ProcessId, new Tuple<string, string>("ProcessId", "用于将一个进程与另一进程区分开的数字标识符")},
                {
                    ProcessInfoEnum.QuotaNonPagedPoolUsage,
                    new Tuple<string, string>("QuotaNonPagedPoolUsage", "非分页池使用配额")
                },
                {ProcessInfoEnum.QuotaPagedPoolUsage, new Tuple<string, string>("QuotaPagedPoolUsage", "分页池使用配额")},
                {
                    ProcessInfoEnum.QuotaPeakNonPagedPoolUsage,
                    new Tuple<string, string>("QuotaPeakNonPagedPoolUsage", "一个进程的非分页池使用量的峰值配额数量")
                },
                {
                    ProcessInfoEnum.QuotaPeakPagedPoolUsage,
                    new Tuple<string, string>("QuotaPeakPagedPoolUsage", "一个进程的分页池使用量的峰值配额量")
                },
                {ProcessInfoEnum.ReadOperationCount, new Tuple<string, string>("ReadOperationCount", "读取操作计数")},
                {ProcessInfoEnum.ReadTransferCount, new Tuple<string, string>("ReadTransferCount", "数据读取量")},
                {ProcessInfoEnum.SessionId, new Tuple<string, string>("SessionId", "会话标识")},
                {ProcessInfoEnum.Status, new Tuple<string, string>("Status", "状态")},
                {ProcessInfoEnum.TerminationDate, new Tuple<string, string>("TerminationDate", "终止日期")},
                {ProcessInfoEnum.ThreadCount, new Tuple<string, string>("ThreadCount", "进程中的活动线程数")},
                {ProcessInfoEnum.UserModeTime, new Tuple<string, string>("UserModeTime", "用户模式时间，以100纳秒为单位")},
                {
                    ProcessInfoEnum.VirtualSize,
                    new Tuple<string, string>("VirtualSize", "进程正在使用的虚拟地址空间的当前大小，而不是进程实际使用的物理或虚拟内存")
                },
                {ProcessInfoEnum.WindowsVersion, new Tuple<string, string>("WindowsVersion", "正在运行进程的Windows版本")},
                {ProcessInfoEnum.WorkingSetSize, new Tuple<string, string>("WorkingSetSize", "工作集大小")},
                {ProcessInfoEnum.WriteOperationCount, new Tuple<string, string>("WriteOperationCount", "写入操作计数")},
                {ProcessInfoEnum.WriteTransferCount, new Tuple<string, string>("WriteTransferCount", "写入传输计数")},
                {ProcessInfoEnum.All, new Tuple<string, string>("*", "所有")}
            };

        public enum ProcessInfoEnum
        {
            Caption, //标题
            CommandLine, //命令行
            CreationClassName, //创建实例时使用的类或子类的名称
            CreationDate, //创建日期
            CsCreationClassName, //范围界定计算机系统的创建类名称
            CsName, //计算机系统名称
            Description, //描述
            ExecutablePath, //进程的可执行文件的路径
            ExecutionState, //当前运行状况的过程
            Handle, //流程标识符
            HandleCount, //该进程拥有的打开句柄的总数
            InstallDate, //安装对象的日期
            KernelModeTime, //内核模式下的时间
            MaximumWorkingSetSize, //最大工作集大小的过程
            MinimumWorkingSetSize, //过程的最小工作集大小
            Name, //负责该进程的可执行文件的名称
            OsCreationClassName, //操作系统类别名称
            OsName, //操作系统名称
            OtherOperationCount, //其他操作计数
            OtherTransferCount, //其他传输计数
            PageFaults, //页面错误
            PageFileUsage, //页面文件使用量
            ParentProcessId, //创建进程的进程的唯一标识符
            PeakPageFileUsage, //进程生命周期中使用的最大页面文件空间量
            PeakVirtualSize, //任何一个进程使用的最大虚拟地址空间
            PeakWorkingSetSize, //峰值工作集大小
            Priority, //调度操作系统中进程的优先级
            PrivatePageCount, //当前分配的页面只能由此Win32_Process实例表示的进程访问
            ProcessId, //用于将一个进程与另一进程区分开的数字标识符
            QuotaNonPagedPoolUsage, //非分页池使用配额
            QuotaPagedPoolUsage, //分页池使用配额
            QuotaPeakNonPagedPoolUsage, //一个进程的非分页池使用量的峰值配额数量
            QuotaPeakPagedPoolUsage, //一个进程的分页池使用量的峰值配额量
            ReadOperationCount, //读取操作计数
            ReadTransferCount, //数据读取量
            SessionId, //会话标识
            Status, //状态
            TerminationDate, //终止日期
            ThreadCount, //进程中的活动线程数
            UserModeTime, //用户模式时间，以100纳秒为单位
            VirtualSize, //进程正在使用的虚拟地址空间的当前大小，而不是进程实际使用的物理或虚拟内存
            WindowsVersion, //正在运行进程的Windows版本
            WorkingSetSize, //工作集大小
            WriteOperationCount, //写入操作计数
            WriteTransferCount, //写入传输计数
            All //全部
        }

        #endregion

        #region  Methods

        public static string GetDescriptionString(string infoString)
        {
            foreach (var tupleValue in ThisEnumDictionary.Values)
                if (tupleValue.Item1.Equals(infoString))
                    return tupleValue.Item2;
            return "No such descriptionString!";
        }

        public static string GetQueryString(ProcessInfoEnum infoEnum)
        {
            if (ThisEnumDictionary.ContainsKey(infoEnum))
                return ThisEnumDictionary[infoEnum].Item1;
            return "";
        }

        #endregion
    }
}