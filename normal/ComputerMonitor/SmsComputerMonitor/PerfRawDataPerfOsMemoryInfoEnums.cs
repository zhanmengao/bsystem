using System;
using System.Collections.Generic;

namespace SmsComputerMonitor
{
    public static class PerfRawDataPerfOsMemoryInfoEnums
    {
        #region Fields and Properties

        private static readonly Dictionary<PerfRawDataPerfOsMemoryInfoEnum, Tuple<string, string>>
            ThisEnumDictionary = new Dictionary<PerfRawDataPerfOsMemoryInfoEnum, Tuple<string, string>>
            {
                {
                    PerfRawDataPerfOsMemoryInfoEnum.AvailableBytes,
                    new Tuple<string, string>("AvailableBytes", "可用B字节,只读")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.AvailableKBytes,
                    new Tuple<string, string>("AvailableKBytes", "可用KB字节,只读")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.AvailableMBytes,
                    new Tuple<string, string>("AvailableMBytes", "可用MB字节,只读")
                },
                {PerfRawDataPerfOsMemoryInfoEnum.CacheBytes, new Tuple<string, string>("CacheBytes", "缓存字节")},
                {
                    PerfRawDataPerfOsMemoryInfoEnum.CacheBytesPeak,
                    new Tuple<string, string>("CacheBytesPeak", "缓存字节最大值")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.CacheFaultsPersec,
                    new Tuple<string, string>("CacheFaultsPersec", "缓存每秒错误统计")
                },
                {PerfRawDataPerfOsMemoryInfoEnum.Caption, new Tuple<string, string>("Caption", "标题")},
                {PerfRawDataPerfOsMemoryInfoEnum.CommitLimit, new Tuple<string, string>("CommitLimit", "提交限制")},
                {PerfRawDataPerfOsMemoryInfoEnum.CommittedBytes, new Tuple<string, string>("CommittedBytes", "提交字节")},
                {
                    PerfRawDataPerfOsMemoryInfoEnum.DemandZeroFaultsPersec,
                    new Tuple<string, string>("DemandZeroFaultsPersec", "内存计数器每秒零花页面弥补错误平均速度")
                },
                {PerfRawDataPerfOsMemoryInfoEnum.Description, new Tuple<string, string>("Description", "描述")},
                {
                    PerfRawDataPerfOsMemoryInfoEnum.FreeAndZeroPageListBytes,
                    new Tuple<string, string>("FreeAndZeroPageListBytes", "空白零页表字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.FreeSystemPageTableEntries,
                    new Tuple<string, string>("FreeSystemPageTableEntries", "空白系统页表条目")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.FrequencyObject,
                    new Tuple<string, string>("Frequency_Object", "频率对象")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.FrequencyPerfTime,
                    new Tuple<string, string>("Frequency_PerfTime", "每秒频率")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.FrequencySys100Ns,
                    new Tuple<string, string>("Frequency_Sys100NS", "10000000频率耗时秒")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.LongTermAverageStandbyCacheLifetimes,
                    new Tuple<string, string>("LongTermAverageStandbyCacheLifetimes", "长期平均待机缓存寿命")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.ModifiedPageListBytes,
                    new Tuple<string, string>("ModifiedPageListBytes", "页修改列表字节")
                },
                {PerfRawDataPerfOsMemoryInfoEnum.Name, new Tuple<string, string>("Name", "名称，统计信息或指标已知的标签")},
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PageFaultsPersec,
                    new Tuple<string, string>("PageFaultsPersec", "每秒页面错误")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PageReadsPersec,
                    new Tuple<string, string>("PageReadsPersec", "每秒读页次数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PagesInputPersec,
                    new Tuple<string, string>("PagesInputPersec", "每秒页输入页数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PagesOutputPersec,
                    new Tuple<string, string>("PagesOutputPersec", "每秒页输出页数")
                },
                {PerfRawDataPerfOsMemoryInfoEnum.PagesPersec, new Tuple<string, string>("PagesPersec", "每秒读写页面数")},
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PageWritesPersec,
                    new Tuple<string, string>("PageWritesPersec", "每秒写页次数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PercentCommittedBytesInUse,
                    new Tuple<string, string>("PercentCommittedBytesInUse", "正在使用的字节数％")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PercentCommittedBytesInUseBase,
                    new Tuple<string, string>("PercentCommittedBytesInUse_Base", "正在使用的字节数％的基值")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PoolNonpagedAllocs,
                    new Tuple<string, string>("PoolNonpagedAllocs", "非换页池中的呼叫数量")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PoolNonpagedBytes,
                    new Tuple<string, string>("PoolNonpagedBytes", "非页面缓冲池中的字节数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PoolPagedAllocs,
                    new Tuple<string, string>("PoolPagedAllocs", "分页池中分配空间的次数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PoolPagedBytes,
                    new Tuple<string, string>("PoolPagedBytes", "页面缓冲池中的字节数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.PoolPagedResidentBytes,
                    new Tuple<string, string>("PoolPagedResidentBytes", "页面缓冲池的当前大小（以字节为单位）")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.StandbyCacheCoreBytes,
                    new Tuple<string, string>("StandbyCacheCoreBytes", "备用缓存核心字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.StandbyCacheNormalPriorityBytes,
                    new Tuple<string, string>("StandbyCacheNormalPriorityBytes", "备用缓存正常优先级字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.StandbyCacheReserveBytes,
                    new Tuple<string, string>("StandbyCacheReserveBytes", "备用缓存备用字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.SystemCacheResidentBytes,
                    new Tuple<string, string>("SystemCacheResidentBytes", "文件系统缓存中可分页操作系统代码的字节数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.SystemCodeResidentBytes,
                    new Tuple<string, string>("SystemCodeResidentBytes", "系统代码驻留字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.SystemCodeTotalBytes,
                    new Tuple<string, string>("SystemCodeTotalBytes", "系统代码总字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.SystemDriverResidentBytes,
                    new Tuple<string, string>("SystemDriverResidentBytes", "系统驱动程序常驻字节")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.SystemDriverTotalBytes,
                    new Tuple<string, string>("SystemDriverTotalBytes", "系统驱动程序总字节数")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.TimestampObject,
                    new Tuple<string, string>("Timestamp_Object", "对象定义的时间戳")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.TimestampPerfTime,
                    new Tuple<string, string>("Timestamp_PerfTime", "高性能计数器时间戳")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.TimestampSys100Ns,
                    new Tuple<string, string>("Timestamp_Sys100NS", "时间戳值以100纳秒为单位")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.TransitionFaultsPersec,
                    new Tuple<string, string>("TransitionFaultsPersec", "每秒恢复修改页面列表错误")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.TransitionPagesRePurposedPersec,
                    new Tuple<string, string>("TransitionPagesRePurposedPersec", "每秒恢复修改页面列表目标")
                },
                {
                    PerfRawDataPerfOsMemoryInfoEnum.WriteCopiesPersec,
                    new Tuple<string, string>("WriteCopiesPersec", "每秒写拷贝")
                },
                {PerfRawDataPerfOsMemoryInfoEnum.All, new Tuple<string, string>("*", "所有")}
            };

        public enum PerfRawDataPerfOsMemoryInfoEnum
        {
            AvailableBytes, //可用B字节,只读
            AvailableKBytes, //可用KB字节,只读
            AvailableMBytes, //可用MB字节,只读
            CacheBytes, //缓存字节
            CacheBytesPeak, //缓存字节最大值
            CacheFaultsPersec, //缓存每秒错误统计
            Caption, //标题
            CommitLimit, //提交限制
            CommittedBytes, //提交字节
            DemandZeroFaultsPersec, //内存计数器每秒零花页面弥补错误平均速度
            Description, //描述
            FreeAndZeroPageListBytes, //空白零页表字节
            FreeSystemPageTableEntries, //空白系统页表条目
            FrequencyObject, //频率对象
            FrequencyPerfTime, //每秒频率
            FrequencySys100Ns, //10000000频率耗时秒
            LongTermAverageStandbyCacheLifetimes, //长期平均待机缓存寿命
            ModifiedPageListBytes, //页修改列表字节
            Name, //名称，统计信息或指标已知的标签
            PageFaultsPersec, //每秒页面错误
            PageReadsPersec, //每秒读页次数
            PagesInputPersec, //每秒页输入页数
            PagesOutputPersec, //每秒页输出页数
            PagesPersec, //每秒读写页面数
            PageWritesPersec, //每秒写页次数
            PercentCommittedBytesInUse, //正在使用的字节数％
            PercentCommittedBytesInUseBase, //正在使用的字节数％的基值
            PoolNonpagedAllocs, //非换页池中的呼叫数量
            PoolNonpagedBytes, //非页面缓冲池中的字节数
            PoolPagedAllocs, //分页池中分配空间的次数
            PoolPagedBytes, //页面缓冲池中的字节数
            PoolPagedResidentBytes, //页面缓冲池的当前大小（以字节为单位）
            StandbyCacheCoreBytes, //备用缓存核心字节
            StandbyCacheNormalPriorityBytes, //备用缓存正常优先级字节
            StandbyCacheReserveBytes, //备用缓存备用字节
            SystemCacheResidentBytes, //文件系统缓存中可分页操作系统代码的字节数
            SystemCodeResidentBytes, //系统代码驻留字节
            SystemCodeTotalBytes, //系统代码总字节
            SystemDriverResidentBytes, //系统驱动程序常驻字节
            SystemDriverTotalBytes, //系统驱动程序总字节数
            TimestampObject, //对象定义的时间戳
            TimestampPerfTime, //高性能计数器时间戳
            TimestampSys100Ns, //时间戳值以100纳秒为单位
            TransitionFaultsPersec, //每秒恢复修改页面列表错误
            TransitionPagesRePurposedPersec, //每秒恢复修改页面列表目标
            WriteCopiesPersec, //每秒写拷贝
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

        public static string GetQueryString(PerfRawDataPerfOsMemoryInfoEnum infoEnum)
        {
            if (ThisEnumDictionary.ContainsKey(infoEnum))
                return ThisEnumDictionary[infoEnum].Item1;
            return "";
        }

        #endregion
    }
}