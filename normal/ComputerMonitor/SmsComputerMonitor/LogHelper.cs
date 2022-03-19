using System;
using NLog;

/*
Install-Package NLog -Version 4.4.12
*/
namespace SmsComputerMonitor
{
    public static class LogHelper
    {
        #region Fields and Properties

        private static class Logger
        {
            #region Fields and Properties

            private static ILogger _log;

            public static ILogger Log => _log ?? (_log = LogManager.GetCurrentClassLogger());

            #endregion
        }

        #endregion

        #region  Methods

        public static void LogError(string message, Exception ex, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Error(ex, message);
        }

        public static void LogError(string message, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Error(message);
        }

        public static void LogFatal(string message, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Fatal(message);
        }

        public static void LogFatal(string message, Exception ex, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Fatal(ex, message);
        }

        public static void LogInfo(string message, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Info(message);
        }

        public static void LogTrace(string message)
        {
            Logger.Log.Trace(message);
        }

        public static void LogTrace(long time, string message)
        {
            if (time > 1000)
                Logger.Log.Trace(message + ",花费时间:" + time);
        }

        public static void LogWarn(string message, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Warn(message);
        }

        public static void LogWarn(string message, Exception ex, bool isRecordLog = true)
        {
            if (isRecordLog)
                Logger.Log.Warn(ex, message);
        }

        #endregion

        //public static void SetConfig(FileInfo configFile)
        //{
        //    LogManager.Configuration = new XmlLoggingConfiguration(configFile.FullName);
        //}
    }
}