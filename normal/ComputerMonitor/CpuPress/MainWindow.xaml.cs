using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows;

namespace CpuPress
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : INotifyPropertyChanged
    {
        #region INotifyProperties

        #region OnPropertyChanged(INotifyPropertyChanged)

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

        #region bool? IsButtonEnable(INotifyPropertyChangedProperty)

        private bool? _IsButtonEnable;

        public bool? IsButtonEnable
        {
            get
            {
               return  _IsButtonEnable;
            }
            set
            {
                if (_IsButtonEnable != null && _IsButtonEnable.Equals(value)) return;
                _IsButtonEnable = value;
                OnPropertyChanged();

            }
        }

        #endregion

        #region string LogValue(INotifyPropertyChangedProperty)

        private string _LogValue;

        public string LogValue
        {
            get
            {
                return _LogValue;
            }
            set
            {
                if (_LogValue != null && _LogValue.Equals(value)) return;
                _LogValue = value;
                OnPropertyChanged();

            }
        }

        #endregion
        #endregion

        public MainWindow()
        {
            InitializeComponent();
        }

        private void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            Task.Factory.StartNew(() =>
            {
                IsButtonEnable = false;
                for (int i = 0; i < 100000000000000; i++)
                {
                    LogValue = $"Math.Log({i})={Math.Log(i).ToString("F5")}";
                }
                IsButtonEnable = true;
            });
        }
    }
}
