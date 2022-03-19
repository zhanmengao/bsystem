using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;

namespace WindowsApp
{
    class Class1
    {
        private IList<string> diskList = new List<string>();

        delegate void SetTextCallback(string s);
        delegate void ClearListBoxItem();
        private ListBox _listbox;
        private Form _form;
        private Thread thread = null;
        //private BackgroundWorker backgroundworker;

        public Class1()
        {

            System.Timers.Timer timer = new System.Timers.Timer(1000);
            timer.Enabled = true;
            timer.Elapsed += new System.Timers.ElapsedEventHandler(fi);
            timer.AutoReset = true;
        }
        public void filldata(Form form, Message m, ListBox listbox)
        {
            _listbox = listbox;
            _form = form;
            try
            {
                //WM_DEVICECHANGE，系统硬件改变发出的系统消息
                if (m.Msg == WndProMsgConst.WM_DEVICECHANGE)
                {
                    switch (m.WParam.ToInt32())
                    {
                        case WndProMsgConst.WM_DEVICECHANGE:
                            break;
                        //DBT_DEVICEARRIVAL，设备检测结束，并且可以使用
                        case WndProMsgConst.DBT_DEVICEARRIVAL:
                            scanUDisk();
                            _listbox.Items.Clear();
                            foreach (string s in diskList)
                            {
                                _listbox.Items.Add(s);
                            }
                            break;
                        //DBT_DEVICEREMOVECOMPLETE,设备卸载或者拔出
                        case WndProMsgConst.DBT_DEVICEREMOVECOMPLETE:
                            scanUDisk();
                            _listbox.Items.Clear();
                            foreach (string s in diskList)
                            {
                                _listbox.Items.Add(s);
                            }
                            break;

                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("当前盘不能正确识别，请重新尝试！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
        public void SetText(string text)
        {
            if (this._listbox.InvokeRequired)
            {
                if (_listbox.Items.Contains(text))
                    return;
                SetTextCallback d = new SetTextCallback(SetText);
                _form.Invoke(d, new object[] { text });
            }
            else
            {
                if (_listbox.Items.Contains(text))
                    return;
                this._listbox.Items.Add(text);
            }
        }
        void fi(object sender, System.Timers.ElapsedEventArgs e)
        {
            scanUDisk();
            //listBox1.Items.Clear();
            foreach (string s in diskList)
            {
                SetText(s);
            }
        }/// <summary>
         /// 扫描U口设备
         /// </summary>
         /// <param name="obj"></param>
        private void scanUDisk()
        {
            diskList.Clear();
            DriveInfo[] drives = DriveInfo.GetDrives();

            foreach (DriveInfo drive in drives)
            {
                if ((drive.DriveType == DriveType.Removable) && !drive.Name.Substring(0, 1).Equals("A"))
                {
                    try
                    {
                        diskList.Add(drive.Name);
                    }
                    catch
                    {
                        MessageBox.Show("当前盘不能正确识别，请重新尝试！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }

                }
            }
        }
    } /// <summary>
      /// windows消息的常量
      /// </summary>
    class WndProMsgConst
    {
        #region WndProc常量
        public const int WM_DEVICECHANGE = 0x219;
        public const int DBT_DEVICEARRIVAL = 0x8000;
        public const int DBT_CONFIGCHANGECANCELED = 0x0019;
        public const int DBT_CONFIGCHANGED = 0x0018;
        public const int DBT_CUSTOMEVENT = 0x8006;
        public const int DBT_DEVICEQUERYREMOVE = 0x8001;
        public const int DBT_DEVICEQUERYREMOVEFAILED = 0x8002;
        public const int DBT_DEVICEREMOVECOMPLETE = 0x8004;
        public const int DBT_DEVICEREMOVEPENDING = 0x8003;
        public const int DBT_DEVICETYPESPECIFIC = 0x8005;
        public const int DBT_DEVNODES_CHANGED = 0x0007;
        public const int DBT_QUERYCHANGECONFIG = 0x0017;
        public const int DBT_USERDEFINED = 0xFFFF;
        #endregion
    }
}