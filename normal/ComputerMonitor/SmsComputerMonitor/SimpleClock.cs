using System;
using System.Windows.Threading;

namespace SmsComputerMonitor
{
    public class SimpleClock
    {
        #region void DateTime Clock(ChangedWithEvent)

        private DateTime _Clock;

        public DateTime Clock
        {
            get => _Clock;
            set
            {
                _Clock = value;
                OnClockChanged(value);
            }
        }

        public delegate void ClockChangedHandler(DateTime clock);

        public event ClockChangedHandler ClockChanged;

        private void OnClockChanged(DateTime clock)
        {
            ClockChanged?.Invoke(clock);
        }

        #endregion

        public SimpleClock(int updateInSeconds)
        {
            Clock = DateTime.Now;
            DispatcherTimer timer = new DispatcherTimer {Interval = TimeSpan.FromSeconds(updateInSeconds)};
            timer.Tick += (sender, args) => Clock = DateTime.Now;
            timer.Start();
        }
    }
}