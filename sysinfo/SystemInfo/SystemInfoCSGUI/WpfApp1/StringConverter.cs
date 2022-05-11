using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace WpfApp1
{
    class StringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var setting = value as string;
            string str;
            switch (setting)
            {
                case "0":
                    str = "不执行参数"; break;
                case "1": str = "可执行任何参数"; break;
                case "2":
                    str = "始终执行固定参数"; break;
                default: str = "other"; break;
            }
            return str;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
