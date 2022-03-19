using System;
using System.Globalization;
using System.Windows.Data;

namespace SmsComputerMonitor
{
    public class FunctionOpenedListConverter : IValueConverter
    {
        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            try
            {
                if (value is int functionsInt && int.TryParse(parameter?.ToString(), out int position))
                {

                    int baseBit = 0b0000_0000_0000_0001 << (position - 1);
                    if ((functionsInt & baseBit) == baseBit)
                    {
                        return true;
                    }
                }
                else
                {
                    LogHelper.LogError("Convert error! return false!");
                }
                return false;
            }
            catch (Exception exception)
            {
                LogHelper.LogError("FunctionOpenedListConverter Convert Error!", exception);
                return false;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            try
            {
                if (value is bool isChecked && int.TryParse(parameter?.ToString(), out int position))
                {
                    if (isChecked)
                    {
                        return position;
                    }
                    return -position;
                }
                LogHelper.LogError("ConvertBack error! return 0!");
                return 0;
            }
            catch (Exception exception)
            {
                LogHelper.LogError("FunctionOpenedListConverter ConvertBack Error!", exception);
                return 0;
            }
        }

        #endregion
    }
}