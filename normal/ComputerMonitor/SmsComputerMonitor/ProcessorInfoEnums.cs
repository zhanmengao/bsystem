using System;
using System.Collections.Generic;

namespace SmsComputerMonitor
{
    public static class ProcessorInfoEnums
    {
        #region Fields and Properties

        private static readonly Dictionary<ProcessorInfoEnum, Tuple<string, string>>
            ThisEnumDictionary = new Dictionary<ProcessorInfoEnum, Tuple<string, string>>
            {
                {ProcessorInfoEnum.AddressWidth, new Tuple<string, string>("AddressWidth", "系统的位宽")},
                {ProcessorInfoEnum.Architecture, new Tuple<string, string>("Architecture", "架构")},
                {ProcessorInfoEnum.AssetTag, new Tuple<string, string>("AssetTag", "处理器的资产标签")},
                {ProcessorInfoEnum.Availability, new Tuple<string, string>("Availability", "可用性")},
                {ProcessorInfoEnum.Caption, new Tuple<string, string>("Caption", "标题")},
                {ProcessorInfoEnum.Characteristics, new Tuple<string, string>("Characteristics", "特点")},
                {
                    ProcessorInfoEnum.ConfigManagerErrorCode,
                    new Tuple<string, string>("ConfigManagerErrorCode", "Windows API配置管理器错误代码")
                },
                {
                    ProcessorInfoEnum.ConfigManagerUserConfig,
                    new Tuple<string, string>("ConfigManagerUserConfig", "用户定义的配置")
                },
                {
                    ProcessorInfoEnum.CpuStatus,
                    new Tuple<string, string>("CpuStatus", "处理器的当前状态 状态更改指示处理器使用情况，但不是处理器的物理状况")
                },
                {ProcessorInfoEnum.CreationClassName, new Tuple<string, string>("CreationClassName", "创建类别名称")},
                {
                    ProcessorInfoEnum.CurrentClockSpeed,
                    new Tuple<string, string>("CurrentClockSpeed", "处理器的当前速度，以MHz为单位")
                },
                {ProcessorInfoEnum.CurrentVoltage, new Tuple<string, string>("CurrentVoltage", "电压")},
                {ProcessorInfoEnum.DataWidth, new Tuple<string, string>("DataWidth", "位宽32位还是64位")},
                {ProcessorInfoEnum.Description, new Tuple<string, string>("Description", "描述")},
                {ProcessorInfoEnum.DeviceId, new Tuple<string, string>("DeviceID", "设备ID")},
                {ProcessorInfoEnum.ErrorCleared, new Tuple<string, string>("ErrorCleared", "报告的错误是否清除")},
                {ProcessorInfoEnum.ErrorDescription, new Tuple<string, string>("ErrorDescription", "错误信息")},
                {ProcessorInfoEnum.ExtClock, new Tuple<string, string>("ExtClock", "外部时钟")},
                {ProcessorInfoEnum.Family, new Tuple<string, string>("Family", "处理器信息结构")},
                {ProcessorInfoEnum.InstallDate, new Tuple<string, string>("InstallDate", "安装日期")},
                {ProcessorInfoEnum.L2CacheSize, new Tuple<string, string>("L2CacheSize", "二级处理器缓存的大小")},
                {ProcessorInfoEnum.L2CacheSpeed, new Tuple<string, string>("L2CacheSpeed", "二级处理器缓存的时钟速度")},
                {ProcessorInfoEnum.L3CacheSize, new Tuple<string, string>("L3CacheSize", "三级高速处理器缓存的大小")},
                {ProcessorInfoEnum.L3CacheSpeed, new Tuple<string, string>("L3CacheSpeed", "三级高速缓存时钟速度")},
                {ProcessorInfoEnum.LastErrorCode, new Tuple<string, string>("LastErrorCode", "最后一个错误代码")},
                {ProcessorInfoEnum.Level, new Tuple<string, string>("Level", "水平")},
                {ProcessorInfoEnum.LoadPercentage, new Tuple<string, string>("LoadPercentage", "负载能力")},
                {ProcessorInfoEnum.Manufacturer, new Tuple<string, string>("Manufacturer", "生产厂家")},
                {ProcessorInfoEnum.MaxClockSpeed, new Tuple<string, string>("MaxClockSpeed", "处理器的最大速度（MHz）")},
                {ProcessorInfoEnum.Name, new Tuple<string, string>("Name", "名称")},
                {ProcessorInfoEnum.NumberOfCores, new Tuple<string, string>("NumberOfCores", "处理器当前实例的内核数量")},
                {
                    ProcessorInfoEnum.NumberOfEnabledCore,
                    new Tuple<string, string>("NumberOfEnabledCore", "每个处理器插槽的已启用内核数量")
                },
                {
                    ProcessorInfoEnum.NumberOfLogicalProcessors,
                    new Tuple<string, string>("NumberOfLogicalProcessors", "处理器当前实例的逻辑处理器数量")
                },
                {
                    ProcessorInfoEnum.OtherFamilyDescription,
                    new Tuple<string, string>("OtherFamilyDescription", "处理器家族类型")
                },
                {ProcessorInfoEnum.PartNumber, new Tuple<string, string>("PartNumber", "零件号")},
                {ProcessorInfoEnum.PnpDeviceId, new Tuple<string, string>("PNPDeviceID", "Windows即插即用设备标识符的逻辑设备")},
                {
                    ProcessorInfoEnum.PowerManagementCapabilities,
                    new Tuple<string, string>("PowerManagementCapabilities", "逻辑设备的具体功率相关功能的阵列")
                },
                {
                    ProcessorInfoEnum.PowerManagementSupported,
                    new Tuple<string, string>("PowerManagementSupported", "管理设备的电源")
                },
                {ProcessorInfoEnum.ProcessorId, new Tuple<string, string>("ProcessorId", "处理器ID")},
                {ProcessorInfoEnum.ProcessorType, new Tuple<string, string>("ProcessorType", "处理器类型")},
                {ProcessorInfoEnum.Revision, new Tuple<string, string>("Revision", "系统修订级别")},
                {ProcessorInfoEnum.Role, new Tuple<string, string>("Role", "处理器的角色，中央处理器或数学处理器")},
                {
                    ProcessorInfoEnum.SecondLevelAddressTranslationExtensions,
                    new Tuple<string, string>("SecondLevelAddressTranslationExtensions", "虚拟化的地址转换扩展支持")
                },
                {ProcessorInfoEnum.SerialNumber, new Tuple<string, string>("SerialNumber", "序列号")},
                {ProcessorInfoEnum.SocketDesignation, new Tuple<string, string>("SocketDesignation", "电路上使用的芯片插座的类型")},
                {ProcessorInfoEnum.Status, new Tuple<string, string>("Status", "状态")},
                {ProcessorInfoEnum.StatusInfo, new Tuple<string, string>("StatusInfo", "逻辑设备的状态")},
                {ProcessorInfoEnum.Stepping, new Tuple<string, string>("Stepping", "处理器系列中处理器的修订级别")},
                {
                    ProcessorInfoEnum.SystemCreationClassName,
                    new Tuple<string, string>("SystemCreationClassName", "作用域计算机的创建类别名称")
                },
                {ProcessorInfoEnum.SystemName, new Tuple<string, string>("SystemName", "范围系统的名称")},
                {ProcessorInfoEnum.ThreadCount, new Tuple<string, string>("ThreadCount", "每个处理器套接字的线程数")},
                {ProcessorInfoEnum.UniqueId, new Tuple<string, string>("UniqueId", "唯一身份")},
                {ProcessorInfoEnum.UpgradeMethod, new Tuple<string, string>("UpgradeMethod", "处理器升级方法")},
                {ProcessorInfoEnum.Version, new Tuple<string, string>("Version", "处理器版本号")},
                {
                    ProcessorInfoEnum.VirtualizationFirmwareEnabled,
                    new Tuple<string, string>("VirtualizationFirmwareEnabled", "启用虚拟化扩展使能")
                },
                {
                    ProcessorInfoEnum.VmMonitorModeExtensions,
                    new Tuple<string, string>("VMMonitorModeExtensions", "支持英特尔或AMD虚拟机监视器扩展")
                },
                {ProcessorInfoEnum.VoltageCaps, new Tuple<string, string>("VoltageCaps", "处理器的电压能力")},
                {ProcessorInfoEnum.All, new Tuple<string, string>("*", "所有")}
            };

        public enum ProcessorInfoEnum
        {
            AddressWidth, //系统的位宽
            Architecture, //架构
            AssetTag, //处理器的资产标签
            Availability, //可用性
            Caption, //标题
            Characteristics, //特点
            ConfigManagerErrorCode, //Windows API配置管理器错误代码
            ConfigManagerUserConfig, //用户定义的配置
            CpuStatus, //处理器的当前状态 状态更改指示处理器使用情况，但不是处理器的物理状况
            CreationClassName, //创建类别名称
            CurrentClockSpeed, //处理器的当前速度，以MHz为单位
            CurrentVoltage, //电压
            DataWidth, //位宽32位还是64位
            Description, //描述
            DeviceId, //设备ID
            ErrorCleared, //报告的错误是否清除
            ErrorDescription, //错误信息
            ExtClock, //外部时钟
            Family, // 处理器信息”结构
            InstallDate, //安装日期
            L2CacheSize, //二级处理器缓存的大小
            L2CacheSpeed, //二级处理器缓存的时钟速度
            L3CacheSize, //三级高速处理器缓存的大小
            L3CacheSpeed, //三级高速缓存时钟速度
            LastErrorCode, //最后一个错误代码
            Level, //水平
            LoadPercentage, //负载能力
            Manufacturer, //生产厂家
            MaxClockSpeed, //处理器的最大速度（MHz）
            Name, //名称
            NumberOfCores, //处理器当前实例的内核数量
            NumberOfEnabledCore, //每个处理器插槽的已启用内核数量
            NumberOfLogicalProcessors, //处理器当前实例的逻辑处理器数量
            OtherFamilyDescription, //处理器家族类型
            PartNumber, //零件号
            PnpDeviceId, //Windows即插即用设备标识符的逻辑设备
            PowerManagementCapabilities, //逻辑设备的具体功率相关功能的阵列
            PowerManagementSupported, //管理设备的电源
            ProcessorId, //处理器ID
            ProcessorType, //处理器类型
            Revision, //系统修订级别
            Role, //处理器的角色，中央处理器或数学处理器
            SecondLevelAddressTranslationExtensions, //虚拟化的地址转换扩展支持
            SerialNumber, //序列号
            SocketDesignation, //电路上使用的芯片插座的类型
            Status, //状态
            StatusInfo, //逻辑设备的状态
            Stepping, //处理器系列中处理器的修订级别
            SystemCreationClassName, //作用域计算机的创建类别名称
            SystemName, //范围系统的名称
            ThreadCount, //每个处理器套接字的线程数
            UniqueId, //唯一身份
            UpgradeMethod, //处理器升级方法
            Version, //处理器版本号
            VirtualizationFirmwareEnabled, //启用虚拟化扩展使能
            VmMonitorModeExtensions, //支持英特尔或AMD虚拟机监视器扩展
            VoltageCaps, //处理器的电压能力
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

        public static string GetQueryString(ProcessorInfoEnum infoEnum)
        {
            if (ThisEnumDictionary.ContainsKey(infoEnum))
                return ThisEnumDictionary[infoEnum].Item1;
            return "";
        }

        #endregion
    }
}