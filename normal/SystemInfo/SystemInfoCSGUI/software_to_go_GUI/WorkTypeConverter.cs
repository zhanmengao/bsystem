using System;
using System.Diagnostics;
using System.Globalization;
using System.Windows.Data;

namespace software_to_go_GUI
{
    class WorkTypeConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return parameter.Equals(value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool isChecked = (bool)value;
            if (!isChecked)
            {
                return null;
            }
            return int.Parse(parameter.ToString());
        }
    }
}
