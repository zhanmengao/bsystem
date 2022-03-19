using ClassLibrary1;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace software_to_go_GUI
{
    class UIModel
    {
        public ObservableCollection<RemoteAppInfo> localSoftwareList;//= new ObservableCollection<RemoteAppInfo>();
        public ObservableCollection<RemoteAppInfo> remoteAppList;//= new ObservableCollection<RemoteAppInfo>();
        public IList selectedLocalSoftwareList; //=new List<RemoteAppInfo>();
        public IList selectedRemoteAppList;// =new List<RemoteAppInfo>();

        public UIModel()
        {
           localSoftwareList = new ObservableCollection<RemoteAppInfo>();
            remoteAppList = new ObservableCollection<RemoteAppInfo>();
            //selectedLocalSoftwareList = new List<RemoteAppInfo>();
            selectedRemoteAppList = new List<RemoteAppInfo>();
        }
    }
}
