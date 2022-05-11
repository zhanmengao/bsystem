using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp1
{
    class RemoteAppInfo : INotifyPropertyChanged
    {
        public int Index { get; set; }
        public string Name { get; set; }
        public string Path { get; set; }
        public string CommandLineSetting { get; set; }
        public string RequiredCommandLine { get; set; }
        public string Description { get; set; }
        public object ImageStr { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
