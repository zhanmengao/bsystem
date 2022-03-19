using System.Diagnostics;
using System.Drawing;
using System.Windows;
using System.Windows.Forms;

namespace SmsComputerMonitor
{
    public class NotifyIconFunction
    {
        #region Fields and Properties

        private MenuItem _menuItemCloseMainWindow;
        private MenuItem _menuItemShowMainWindow;
        private NotifyIcon _notifyIcon;

        #endregion

        #region  Methods

        public void SetNotifyIcon(Window mainWindow)
        {
            _objInstance._notifyIcon = new NotifyIcon
            {
                Icon = Icon.ExtractAssociatedIcon(Process.GetCurrentProcess().MainModule.FileName),
                Text = "",
                Visible = true,
                ContextMenu = new ContextMenu()
            };
            _objInstance._notifyIcon.DoubleClick +=
                (sender, args) => _objInstance.ShowAndActivateMainWindow(mainWindow);

            _objInstance._menuItemShowMainWindow = new MenuItem
            {
                Text = @"打开程序",
                DefaultItem = true
            };
            _objInstance._menuItemShowMainWindow.Click +=
                (sender, args) => _objInstance.ShowAndActivateMainWindow(mainWindow);
            _objInstance._menuItemCloseMainWindow = new MenuItem {Text = @"退出程序"};
            _objInstance._menuItemCloseMainWindow.Click += (sender, args) =>
            {
                _objInstance._notifyIcon.Dispose();
                Process.GetCurrentProcess().Kill();
            };

            _objInstance._notifyIcon.ContextMenu.MenuItems.Add(_menuItemShowMainWindow);
            _objInstance._notifyIcon.ContextMenu.MenuItems.Add("-");
            _objInstance._notifyIcon.ContextMenu.MenuItems.Add(_menuItemCloseMainWindow);

            mainWindow.Closing += (sender, args) =>
            {
                mainWindow.Hide();
                args.Cancel = true;
            };
        }

        private void ShowAndActivateMainWindow(Window mainWindow)
        {
            mainWindow.Show();
            mainWindow.Activate();
        }

        #endregion

        #region Singleton单体,由内部创建对象,全程只有一个实例

        private NotifyIconFunction()
        {
        }

        private static NotifyIconFunction _objInstance;

        public static NotifyIconFunction GetInstance()
        {
            return _objInstance ?? (_objInstance = new NotifyIconFunction());
        }

        #endregion
    }
}