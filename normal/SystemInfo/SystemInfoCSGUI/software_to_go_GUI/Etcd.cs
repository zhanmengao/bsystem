using dotnet_etcd;

namespace software_to_go_GUI
{
    class Etcd
    {
        private string address;
        private EtcdClient client;
        public Etcd(string address)
        {
            this.address = address;
            client = client ?? (client = new EtcdClient(address));
        }
        public EtcdClient GetClient() {
            return client;
        }
    }

    /*
    EtcdClient etcdClient = new EtcdClient("https://192.168.50.208:2379");
    string etcdKey = "/pantyhose";
    etcdClient.Put(etcdKey, "hello human");
    9*/
}
