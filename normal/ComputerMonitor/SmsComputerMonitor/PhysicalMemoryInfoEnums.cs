using System;
using System.Collections.Generic;

namespace SmsComputerMonitor
{
    public static class PhysicalMemoryInfoEnums
    {
        #region Fields and Properties

        private static readonly Dictionary<PhysicalMemoryInfoEnum, Tuple<string, string>> ThisEnumDictionary =
            new Dictionary<PhysicalMemoryInfoEnum, Tuple<string, string>>
            {
                {PhysicalMemoryInfoEnum.Attributes, new Tuple<string, string>("Attributes", "属性")},
                {PhysicalMemoryInfoEnum.BankLabel, new Tuple<string, string>("BankLabel", "标签库")},
                {PhysicalMemoryInfoEnum.Capacity, new Tuple<string, string>("Capacity", "获取内存容量（单位KB）")},
                {PhysicalMemoryInfoEnum.Caption, new Tuple<string, string>("Caption", "物理内存还虚拟内存")},
                {
                    PhysicalMemoryInfoEnum.ConfiguredClockSpeed,
                    new Tuple<string, string>("ConfiguredClockSpeed", "配置时钟速度")
                },
                {PhysicalMemoryInfoEnum.ConfiguredVoltage, new Tuple<string, string>("ConfiguredVoltage", "配置电压")},
                {
                    PhysicalMemoryInfoEnum.CreationClassName,
                    new Tuple<string, string>("CreationClassName", "创建类名（就是更换这个类的名字）")
                },
                {PhysicalMemoryInfoEnum.DataWidth, new Tuple<string, string>("DataWidth", "获取内存带宽")},
                {PhysicalMemoryInfoEnum.Description, new Tuple<string, string>("Description", "描述更Caption一样")},
                {PhysicalMemoryInfoEnum.DeviceLocator, new Tuple<string, string>("DeviceLocator", "获取设备定位器")},
                {PhysicalMemoryInfoEnum.FormFactor, new Tuple<string, string>("FormFactor", "构成因素")},
                {PhysicalMemoryInfoEnum.HotSwappable, new Tuple<string, string>("HotSwappable", "是否支持热插拔")},
                {PhysicalMemoryInfoEnum.InstallDate, new Tuple<string, string>("InstallDate", "安装日期（无值）")},
                {
                    PhysicalMemoryInfoEnum.InterleaveDataDepth,
                    new Tuple<string, string>("InterleaveDataDepth", "数据交错深度")
                },
                {PhysicalMemoryInfoEnum.InterleavePosition, new Tuple<string, string>("InterleavePosition", "交错的位置")},
                {PhysicalMemoryInfoEnum.Manufacturer, new Tuple<string, string>("Manufacturer", "生产商")},
                {PhysicalMemoryInfoEnum.MaxVoltage, new Tuple<string, string>("MaxVoltage", "最大电压")},
                {PhysicalMemoryInfoEnum.MemoryType, new Tuple<string, string>("MemoryType", "内存类型")},
                {PhysicalMemoryInfoEnum.MinVoltage, new Tuple<string, string>("MinVoltage", "最小电压")},
                {PhysicalMemoryInfoEnum.Model, new Tuple<string, string>("Model", "型号")},
                {PhysicalMemoryInfoEnum.Name, new Tuple<string, string>("Name", "名字")},
                {
                    PhysicalMemoryInfoEnum.OtherIdentifyingInfo,
                    new Tuple<string, string>("OtherIdentifyingInfo", "其他识别信息")
                },
                {PhysicalMemoryInfoEnum.PartNumber, new Tuple<string, string>("PartNumber", "零件编号")},
                {PhysicalMemoryInfoEnum.PositionInRow, new Tuple<string, string>("PositionInRow", "行位置")},
                {PhysicalMemoryInfoEnum.PoweredOn, new Tuple<string, string>("PoweredOn", "是否接通电源")},
                {PhysicalMemoryInfoEnum.Removable, new Tuple<string, string>("Removable", "是否可拆卸")},
                {PhysicalMemoryInfoEnum.Replaceable, new Tuple<string, string>("Replaceable", "是否可更换")},
                {PhysicalMemoryInfoEnum.SerialNumber, new Tuple<string, string>("SerialNumber", "编号")},
                {PhysicalMemoryInfoEnum.Sku, new Tuple<string, string>("SKU", "SKU号")},
                {PhysicalMemoryInfoEnum.SmbiosMemoryType, new Tuple<string, string>("SMBIOSMemoryType", "SMBIOS内存类型")},
                {PhysicalMemoryInfoEnum.Speed, new Tuple<string, string>("Speed", "速率")},
                {PhysicalMemoryInfoEnum.Status, new Tuple<string, string>("Status", "状态")},
                {PhysicalMemoryInfoEnum.Tag, new Tuple<string, string>("Tag", "唯一标识符的物理存储器")},
                {PhysicalMemoryInfoEnum.TotalWidth, new Tuple<string, string>("TotalWidth", "总宽")},
                {PhysicalMemoryInfoEnum.TypeDetail, new Tuple<string, string>("TypeDetail", "类型详细信息")},
                {PhysicalMemoryInfoEnum.Version, new Tuple<string, string>("Version", "版本信息")},
                {PhysicalMemoryInfoEnum.All, new Tuple<string, string>("*", "所有")}
            };

        public enum PhysicalMemoryInfoEnum
        {
            Attributes, //属性
            BankLabel, //标签库
            Capacity, //获取内存容量（单位KB）
            Caption, //物理内存还虚拟内存
            ConfiguredClockSpeed, //配置时钟速度
            ConfiguredVoltage, //配置电压
            CreationClassName, //创建类名（就是更换这个类的名字）
            DataWidth, //获取内存带宽
            Description, //描述更Caption一样
            DeviceLocator, //获取设备定位器
            FormFactor, //构成因素
            HotSwappable, //是否支持热插拔
            InstallDate, //安装日期（无值）
            InterleaveDataDepth, //数据交错深度
            InterleavePosition, //交错的位置
            Manufacturer, //生产商
            MaxVoltage, //最大电压
            MemoryType, //内存类型
            MinVoltage, //最小电压
            Model, //型号
            Name, //名字
            OtherIdentifyingInfo, //其他识别信息
            PartNumber, //零件编号
            PositionInRow, //行位置
            PoweredOn, //是否接通电源
            Removable, //是否可拆卸
            Replaceable, //是否可更换
            SerialNumber, //编号
            Sku, //SKU号
            SmbiosMemoryType, //SMBIOS内存类型
            Speed, //速率
            Status, //状态
            Tag, //唯一标识符的物理存储器
            TotalWidth, //总宽
            TypeDetail, //类型详细信息
            Version, //版本信息
            All //所有
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

        public static string GetQueryString(PhysicalMemoryInfoEnum infoEnum)
        {
            if (ThisEnumDictionary.ContainsKey(infoEnum))
                return ThisEnumDictionary[infoEnum].Item1;
            return "";
        }

        #endregion
    }
}