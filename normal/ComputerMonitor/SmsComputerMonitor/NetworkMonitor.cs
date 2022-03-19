// ***********************************************************************
// 
// 文件名：         Class1.cs
// 
// 创建日期：       2017/11/07
//  
// 功能说明：       网速监控
//  
// 作者：           高亚斌
// 
// ***********************************************************************

using System.Collections;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Timers;

namespace SmsComputerMonitor
{
    /// <summary>
    ///     The NetworkMonitor class monitors network speed for each network adapter on the computer,
    ///     using classes for Performance counter in .NET library.
    /// </summary>
    public class NetworkMonitor
    {
        #region Fields and Properties

        private readonly ArrayList _adapters; // The list of adapters on the computer.  
        private readonly ArrayList _monitoredAdapters; // The list of currently monitored adapters.  
        private readonly Timer _timer; // The timer event executes every second to refresh the values in adapters.  

        /// <summary>
        ///     Get instances of NetworkAdapter for installed adapters on this computer.
        /// </summary>
        public NetworkAdapter[] Adapters => (NetworkAdapter[]) _adapters.ToArray(typeof(NetworkAdapter));

        /// <summary>
        ///     Represents a network adapter installed on the machine.
        ///     Properties of this class can be used to obtain current network speed.
        /// </summary>
        public class NetworkAdapter : INotifyPropertyChanged
        {
            #region Fields and Properties

            internal PerformanceCounter DownLoadCounter, UpLoadCounter
                ; // Performance counters to monitor download and upload speed.  

            private long _downloadValue, _uploadValue; // Download/Upload counter value in bytes.  

            private long _downloadValueOld, _uploadValueOld
                ; // Download/Upload counter value one second earlier, in bytes.  


            /// <summary>
            ///     The name of the network adapter.
            /// </summary>
            public string Name { get; }

            #endregion

            #region  Constructors

            /// <summary>
            ///     Instances of this class are supposed to be created only in an NetworkMonitor.
            /// </summary>
            internal NetworkAdapter(string name)
            {
                Name = name;
            }

            #endregion

            #region  Methods

            /// <summary>
            ///     Overrides method to return the name of the adapter.
            /// </summary>
            /// <returns>The name of the adapter.</returns>
            public override string ToString()
            {
                return Name;
            }

            /// <summary>
            ///     Preparations for monitoring.
            /// </summary>
            internal void init()
            {
                // Since dlValueOld and ulValueOld are used in method refresh() to calculate network speed, they must have be initialized.  
                _downloadValueOld = DownLoadCounter.NextSample().RawValue;
                _uploadValueOld = UpLoadCounter.NextSample().RawValue;
            }

            /// <summary>
            ///     Obtain new sample from performance counters, and refresh the values saved in dlSpeed, ulSpeed, etc.
            ///     This method is supposed to be called only in NetworkMonitor, one time every second.
            /// </summary>
            internal void refresh()
            {
                _downloadValue = DownLoadCounter.NextSample().RawValue;
                _uploadValue = UpLoadCounter.NextSample().RawValue;

                // Calculates download and upload speed.  
                DownloadSpeed = _downloadValue - _downloadValueOld;
                UploadSpeed = _uploadValue - _uploadValueOld;

                _downloadValueOld = _downloadValue;
                _uploadValueOld = _uploadValue;
            }

            #endregion

            #region INotifyProperties

            #region OnPropertyChanged(INotifyPropertyChanged)

            public event PropertyChangedEventHandler PropertyChanged;

            protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }

            #endregion

            #region long DownloadSpeed(INotifyPropertyChangedProperty)

            private long _DownloadSpeed;

            /// <summary>
            ///     Current download speed in bytes per second.
            /// </summary>
            public long DownloadSpeed
            {
                get => _DownloadSpeed;
                private set
                {
                    if (_DownloadSpeed.Equals(value)) return;
                    _DownloadSpeed = value;
                    DownloadSpeedKbps = value / 1024.0;
                    OnPropertyChanged();
                }
            }

            #endregion

            #region long UploadSpeed(INotifyPropertyChangedProperty)

            private long _UploadSpeed;

            /// <summary>
            ///     Current upload speed in bytes per second.
            /// </summary>
            public long UploadSpeed
            {
                get => _UploadSpeed;
                private set
                {
                    if (_UploadSpeed.Equals(value)) return;
                    _UploadSpeed = value;
                    UploadSpeedKbps = value / 1024.0;
                    OnPropertyChanged();
                }
            }

            #endregion

            #region double DownloadSpeedKbps(INotifyPropertyChangedProperty)

            private double _DownloadSpeedKbps;

            /// <summary>
            ///     Current download speed in kbytes per second.
            /// </summary>
            public double DownloadSpeedKbps
            {
                get => _DownloadSpeedKbps;
                private set
                {
                    if (_DownloadSpeedKbps.Equals(value)) return;
                    _DownloadSpeedKbps = value;
                    OnPropertyChanged();
                }
            }

            #endregion

            #region double UploadSpeedKbps(INotifyPropertyChangedProperty)

            private double _UploadSpeedKbps;

            /// <summary>
            ///     Current upload speed in kbytes per second.
            /// </summary>
            public double UploadSpeedKbps
            {
                get => _UploadSpeedKbps;
                private set
                {
                    if (_UploadSpeedKbps.Equals(value)) return;
                    _UploadSpeedKbps = value;
                    OnPropertyChanged();
                }
            }

            #endregion

            #endregion
        }

        #endregion

        #region  Constructors

        public NetworkMonitor()
        {
            _adapters = new ArrayList();
            _monitoredAdapters = new ArrayList();
            EnumerateNetworkAdapters();

            _timer = new Timer(1000);
            _timer.Elapsed += timer_Elapsed;
        }

        #endregion

        #region  Methods

        /// <summary>
        ///     Enable the timer and add all adapters to the monitoredAdapters list,
        ///     unless the adapters list is empty.
        /// </summary>
        public void StartMonitoring()
        {
            if (_adapters.Count > 0)
            {
                foreach (NetworkAdapter adapter in _adapters)
                    if (!_monitoredAdapters.Contains(adapter))
                    {
                        _monitoredAdapters.Add(adapter);
                        adapter.init();
                    }

                _timer.Enabled = true;
            }
        }

        /// <summary>
        ///     Enable the timer, and add the specified adapter to the monitoredAdapters list
        /// </summary>
        public void StartMonitoring(NetworkAdapter adapter)
        {
            if (!_monitoredAdapters.Contains(adapter))
            {
                _monitoredAdapters.Add(adapter);
                adapter.init();
            }
            _timer.Enabled = true;
        }

        /// <summary>
        ///     Disable the timer, and clear the monitoredAdapters list.
        /// </summary>
        public void StopMonitoring()
        {
            _monitoredAdapters.Clear();
            _timer.Enabled = false;
        }

        /// <summary>
        ///     Remove the specified adapter from the monitoredAdapters list, and
        ///     disable the timer if the monitoredAdapters list is empty.
        /// </summary>
        public void StopMonitoring(NetworkAdapter adapter)
        {
            if (_monitoredAdapters.Contains(adapter))
                _monitoredAdapters.Remove(adapter);
            if (_monitoredAdapters.Count == 0)
                _timer.Enabled = false;
        }

        /// <summary>
        ///     Enumerates network adapters installed on the computer.
        /// </summary>
        private void EnumerateNetworkAdapters()
        {
            var category = new PerformanceCounterCategory("Network Interface");

            foreach (var name in category.GetInstanceNames())
            {
                // This one exists on every computer.  
                if (name == "MS TCP Loopback interface")
                    continue;
                // Create an instance of NetworkAdapter class, and create performance counters for it.  
                var adapter = new NetworkAdapter(name)
                {
                    DownLoadCounter = new PerformanceCounter("Network Interface", "Bytes Received/sec", name),
                    UpLoadCounter = new PerformanceCounter("Network Interface", "Bytes Sent/sec", name)
                };
                _adapters.Add(adapter); // Add it to ArrayList adapter  
            }
        }

        private void timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            foreach (NetworkAdapter adapter in _monitoredAdapters)
                adapter.refresh();
        }

        #endregion
    }
}