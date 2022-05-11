using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace software_to_go_GUI
{
    class Config : INotifyPropertyChanged
    {
        public static string CPU = "cpu_serial_number";
        public static string HOST_TYPE = "host_type";
        public static string HOST_IP = "host_ip";
        public static string WORK_TYPE = "work_type";
        public static string ETCD_ADDR = "etcd_addr";
        public static string ETCD_PORT = "etcd_port";
        private string etcd_port;
        private string host_type;
        private string work_type;
        private string host_ip;
        private string etcd_addr;
        private string cpu_serial_number;

        private string prefixKey;

        public string Cpu_serial_number
        {
            get
            {
                return cpu_serial_number;
            }
            set
            {
                cpu_serial_number = value;
                OnPropertyChanged("Cpu_serial_number");
            }
        }
        public string Host_type
        {
            get
            {
                return host_type;
            }
            set
            {
                host_type = value;
                OnPropertyChanged("Host_type");
            }
        }
        public string Work_type
        {
            get
            {
                return work_type;
            }
            set
            {
                work_type = value;
                OnPropertyChanged("Work_type");
            }
        }
        public string Host_ip
        {
            get
            {
                return host_ip;
            }
            set
            {
                host_ip = value;
                OnPropertyChanged("Host_ip");
            }
        }
        public string Etcd_addr
        {
            get
            {
                return etcd_addr;
            }
            set
            {
                etcd_addr = value;
                OnPropertyChanged("Etcd_addr");
            }
        }
        public string Etcd_port
        {
            get
            {
                return etcd_port;
            }
            set
            {
                etcd_port = value;
                OnPropertyChanged("Etcd_port");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected internal virtual void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
