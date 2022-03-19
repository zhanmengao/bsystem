using System;
using System.Collections.Generic;

namespace SmsComputerMonitor
{
    public static class OperatingSystemInfoEnums
    {
        #region Fields and Properties

        private static readonly Dictionary<OperatingSystemInfoEnum, Tuple<string, string>>
            ThisEnumDictionary = new Dictionary<OperatingSystemInfoEnum, Tuple<string, string>>
            {
                {OperatingSystemInfoEnum.BootDevice, new Tuple<string, string>("BootDevice", "系统启动的磁盘驱动器的名称")},
                {OperatingSystemInfoEnum.BuildNumber, new Tuple<string, string>("BuildNumber", "操作系统内部编号")},
                {OperatingSystemInfoEnum.BuildType, new Tuple<string, string>("BuildType", "操作系统的构建类型,零售版")},
                {OperatingSystemInfoEnum.Caption, new Tuple<string, string>("Caption", "标题")},
                {OperatingSystemInfoEnum.CodeSet, new Tuple<string, string>("CodeSet", "代码集")},
                {OperatingSystemInfoEnum.CountryCode, new Tuple<string, string>("CountryCode", "国家代码")},
                {
                    OperatingSystemInfoEnum.CreationClassName,
                    new Tuple<string, string>("CreationClassName", "创建实例的继承链中的第一个具体类的名称")
                },
                {
                    OperatingSystemInfoEnum.CsCreationClassName,
                    new Tuple<string, string>("CSCreationClassName", "系统的创建类名称")
                },
                {OperatingSystemInfoEnum.CsdVersion, new Tuple<string, string>("CSDVersion", "Service Pack 版本号")},
                {OperatingSystemInfoEnum.CsName, new Tuple<string, string>("CSName", "系统的名称")},
                {
                    OperatingSystemInfoEnum.CurrentTimeZone,
                    new Tuple<string, string>("CurrentTimeZone", "操作系统偏离格林尼治平均时间")
                },
                {
                    OperatingSystemInfoEnum.DataExecutionPrevention32BitApplications,
                    new Tuple<string, string>("DataExecutionPrevention_32BitApplications",
                        "当数据执行保护硬件功能可用时，此属性表示该功能设置为适用于32位应用程序")
                },
                {
                    OperatingSystemInfoEnum.DataExecutionPreventionAvailable,
                    new Tuple<string, string>("DataExecutionPrevention_Available",
                        "数据执行预防是一种硬件功能，通过停止在数据类型的内存页面上执行代码来防止缓冲区溢出攻击")
                },
                {
                    OperatingSystemInfoEnum.DataExecutionPreventionDrivers,
                    new Tuple<string, string>("DataExecutionPrevention_Drivers", "当数据执行保护硬件功能可用时，此属性表示该功能设置为适用于驱动程序")
                },
                {
                    OperatingSystemInfoEnum.DataExecutionPreventionSupportPolicy,
                    new Tuple<string, string>("DataExecutionPrevention_SupportPolicy", "指示应用了哪个数据执行保护（DEP）设置")
                },
                {OperatingSystemInfoEnum.Debug, new Tuple<string, string>("Debug", "调试")},
                {OperatingSystemInfoEnum.Description, new Tuple<string, string>("Description", "操作系统的描述")},
                {OperatingSystemInfoEnum.Distributed, new Tuple<string, string>("Distributed", "分散式")},
                {OperatingSystemInfoEnum.EncryptionLevel, new Tuple<string, string>("EncryptionLevel", "安全交易的加密级别")},
                {
                    OperatingSystemInfoEnum.ForegroundApplicationBoost,
                    new Tuple<string, string>("ForegroundApplicationBoost", "前台应用程序的优先级提高")
                },
                {
                    OperatingSystemInfoEnum.FreePhysicalMemory,
                    new Tuple<string, string>("FreePhysicalMemory", "当前未使用和可用的物理内存的数量")
                },
                {
                    OperatingSystemInfoEnum.FreeSpaceInPagingFiles,
                    new Tuple<string, string>("FreeSpaceInPagingFiles", "映射到操作系统页面文件的空间")
                },
                {
                    OperatingSystemInfoEnum.FreeVirtualMemory,
                    new Tuple<string, string>("FreeVirtualMemory", "前未使用和可用的虚拟内存的数量")
                },
                {OperatingSystemInfoEnum.InstallDate, new Tuple<string, string>("InstallDate", "对象安装日期")},
                {
                    OperatingSystemInfoEnum.LargeSystemCache,
                    new Tuple<string, string>("LargeSystemCache", "此属性已过时，大系统缓存")
                },
                {
                    OperatingSystemInfoEnum.LastBootUpTime,
                    new Tuple<string, string>("LastBootUpTime", "操作系统上次重新启动的日期和时间")
                },
                {OperatingSystemInfoEnum.LocalDateTime, new Tuple<string, string>("LocalDateTime", "操作系统版本的当地日期和时间")},
                {OperatingSystemInfoEnum.Locale, new Tuple<string, string>("Locale", "操作系统使用的语言标识符")},
                {OperatingSystemInfoEnum.Manufacturer, new Tuple<string, string>("Manufacturer", "操作系统制造商的名称")},
                {
                    OperatingSystemInfoEnum.MaxNumberOfProcesses,
                    new Tuple<string, string>("MaxNumberOfProcesses", "操作系统可以支持的最大进程上下文数量")
                },
                {
                    OperatingSystemInfoEnum.MaxProcessMemorySize,
                    new Tuple<string, string>("MaxProcessMemorySize", "可以分配给进程的内存的最大数量（以千字节为单位）")
                },
                {
                    OperatingSystemInfoEnum.MuiLanguages,
                    new Tuple<string, string>("MUILanguages", "计算机上安装的多语言用户界面包（MUI包）语言")
                },
                {OperatingSystemInfoEnum.Name, new Tuple<string, string>("Name", "名称")},
                {
                    OperatingSystemInfoEnum.NumberOfLicensedUsers,
                    new Tuple<string, string>("NumberOfLicensedUsers", "操作系统的用户许可证数量")
                },
                {
                    OperatingSystemInfoEnum.NumberOfProcesses,
                    new Tuple<string, string>("NumberOfProcesses", "当前在操作系统上加载或运行的进程上下文的数量")
                },
                {
                    OperatingSystemInfoEnum.NumberOfUsers,
                    new Tuple<string, string>("NumberOfUsers", "当前操作系统正在存储状态信息的用户会话数")
                },
                {
                    OperatingSystemInfoEnum.OperatingSystemSku,
                    new Tuple<string, string>("OperatingSystemSKU", "操作系统的库存保持单元（SKU）号码")
                },
                {OperatingSystemInfoEnum.Organization, new Tuple<string, string>("Organization", "操作系统注册用户的公司名称")},
                {
                    OperatingSystemInfoEnum.OsArchitecture,
                    new Tuple<string, string>("OSArchitecture", "操作系统的体系结构，而不是处理器")
                },
                {OperatingSystemInfoEnum.OsLanguage, new Tuple<string, string>("OSLanguage", "操作系统的语言版本已安装")},
                {
                    OperatingSystemInfoEnum.OsProductSuite,
                    new Tuple<string, string>("OSProductSuite", "安装和授权的系统产品添加到操作系统")
                },
                {OperatingSystemInfoEnum.OsType, new Tuple<string, string>("OSType", "操作系统的类型")},
                {
                    OperatingSystemInfoEnum.OtherTypeDescription,
                    new Tuple<string, string>("OtherTypeDescription", "当前操作系统版本的附加说明")
                },
                {OperatingSystemInfoEnum.PaeEnabled, new Tuple<string, string>("PAEEnabled", "物理地址扩展使能")},
                {OperatingSystemInfoEnum.PlusProductId, new Tuple<string, string>("PlusProductID", "增强产品ID，不支持")},
                {
                    OperatingSystemInfoEnum.PlusVersionNumber,
                    new Tuple<string, string>("PlusVersionNumber", "增强版本编号，不支持")
                },
                {
                    OperatingSystemInfoEnum.PortableOperatingSystem,
                    new Tuple<string, string>("PortableOperatingSystem", "操作系统是否从外部USB设备引导")
                },
                {OperatingSystemInfoEnum.Primary, new Tuple<string, string>("Primary", "是否是主操作系统")},
                {OperatingSystemInfoEnum.ProductType, new Tuple<string, string>("ProductType", "产品类别")},
                {OperatingSystemInfoEnum.RegisteredUser, new Tuple<string, string>("RegisteredUser", "注册用户")},
                {
                    OperatingSystemInfoEnum.SerialNumber, new Tuple<string, string>("SerialNumber", "序列号")
                }, //QuantumLength
                {
                    OperatingSystemInfoEnum.ServicePackMajorVersion,
                    new Tuple<string, string>("ServicePackMajorVersion", "服务包的主要版本号")
                }, //QuantumType
                {
                    OperatingSystemInfoEnum.ServicePackMinorVersion,
                    new Tuple<string, string>("ServicePackMinorVersion", "服务包的次版本号")
                },
                {
                    OperatingSystemInfoEnum.SizeStoredInPagingFiles,
                    new Tuple<string, string>("SizeStoredInPagingFiles", "页面文件中的总字节数")
                },
                {OperatingSystemInfoEnum.Status, new Tuple<string, string>("Status", "状态")},
                {OperatingSystemInfoEnum.SuiteMask, new Tuple<string, string>("SuiteMask", "位标志，用于标识系统上可用的产品套件")},
                {OperatingSystemInfoEnum.SystemDevice, new Tuple<string, string>("SystemDevice", "操作系统的物理磁盘分区")},
                {OperatingSystemInfoEnum.SystemDirectory, new Tuple<string, string>("SystemDirectory", "操作系统的系统目录")},
                {OperatingSystemInfoEnum.SystemDrive, new Tuple<string, string>("SystemDrive", "操作系统所在的磁盘驱动器的字母")},
                {
                    OperatingSystemInfoEnum.TotalSwapSpaceSize,
                    new Tuple<string, string>("TotalSwapSpaceSize", "总交换空间以千字节为单位")
                },
                {
                    OperatingSystemInfoEnum.TotalVirtualMemorySize,
                    new Tuple<string, string>("TotalVirtualMemorySize", "以千字节为单位的虚拟内存的数量")
                },
                {
                    OperatingSystemInfoEnum.TotalVisibleMemorySize,
                    new Tuple<string, string>("TotalVisibleMemorySize", "操作系统可用的物理内存总量")
                },
                {OperatingSystemInfoEnum.Version, new Tuple<string, string>("Version", "操作系统的版本号")},
                {
                    OperatingSystemInfoEnum.WindowsDirectory,
                    new Tuple<string, string>("WindowsDirectory", "操作系统的Windows目录")
                },
                {OperatingSystemInfoEnum.All, new Tuple<string, string>("*", "所有")}
            };

        public enum OperatingSystemInfoEnum
        {
            BootDevice,
            BuildNumber,
            BuildType,
            Caption,
            CodeSet,
            CountryCode,
            CreationClassName,
            CsCreationClassName,
            CsdVersion,
            CsName,
            CurrentTimeZone,
            DataExecutionPrevention32BitApplications,
            DataExecutionPreventionAvailable,
            DataExecutionPreventionDrivers,
            DataExecutionPreventionSupportPolicy,
            Debug,
            Description,
            Distributed,
            EncryptionLevel,
            ForegroundApplicationBoost,
            FreePhysicalMemory,
            FreeSpaceInPagingFiles,
            FreeVirtualMemory,
            InstallDate,
            LargeSystemCache,
            LastBootUpTime,
            LocalDateTime,
            Locale,
            Manufacturer,
            MaxNumberOfProcesses,
            MaxProcessMemorySize,
            MuiLanguages,
            Name,
            NumberOfLicensedUsers,
            NumberOfProcesses,
            NumberOfUsers,
            OperatingSystemSku,
            Organization,
            OsArchitecture,
            OsLanguage,
            OsProductSuite,
            OsType,
            OtherTypeDescription,
            PaeEnabled,
            PlusProductId,
            PlusVersionNumber,
            PortableOperatingSystem,
            Primary,
            ProductType,
            RegisteredUser,
            SerialNumber,
            ServicePackMajorVersion,
            ServicePackMinorVersion,
            SizeStoredInPagingFiles,
            Status,
            SuiteMask,
            SystemDevice,
            SystemDirectory,
            SystemDrive,
            TotalSwapSpaceSize,
            TotalVirtualMemorySize,
            TotalVisibleMemorySize,
            Version,
            WindowsDirectory,
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

        public static string GetQueryString(OperatingSystemInfoEnum infoEnum)
        {
            if (ThisEnumDictionary.ContainsKey(infoEnum))
                return ThisEnumDictionary[infoEnum].Item1;
            return "";
        }

        #endregion
    }
}