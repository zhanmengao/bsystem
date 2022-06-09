package yamll

type MidplatConfig struct {
	BaseCfg BasicConfig `yaml:"basecfg"`
}
type BasicConfig struct {
	Platform        string `yaml:"platform" json:"platform"`                               // 平台
	LogLevel        string `yaml:"log_level" json:"log_level" mapstructure:"log_level"`    // 日志级别
	LogPath         string `yaml:"log_path" json:"log_path" mapstructure:"log_path"`       // 日志路径
	WorkerNum       int64  `yaml:"worker_num" json:"worker_num" mapstructure:"worker_num"` // 工作队列长度
	InnerTimeout    int64  `yaml:"inner_timeout" json:"inner_timeout"`                     // 全局的内网超时，单位：毫秒
	TimeZone        string `yaml:"time_zone" json:"time_zone"`                             // 服务器时区
	CoordinatorAddr string `yaml:"coordinator_addr" json:"coordinator_addr"`               // zk等服务发现组件地址
	MonitorProcTime bool   `yaml:"monitor_proc_time" json:"monitor_proc_time"`             // 是否开启proc超时监控
	Env             string `yaml:"env" json:"env"`                                         // 当前的环境: test/beta/prod
	K8s             bool   `yaml:"k8s" json:"k8s"`                                         // 是否运行在容器模式下
	PushGateway     string `yaml:"pushgateway" json:"pushgateway"`                         //普罗米修斯网关
	FLogDAddr       string `yaml:"flogd_addr" json:"flogd_addr"`                           //FLOGD地址
	ReqTimeout      int64  `yaml:"req_timeout" json:"req_timeout"`
	GinMode         string `yaml:"gin_mode" json:"gin_mode"`
	DBEngine        string `yaml:"db_engine" json:"db_engine"`
	MysqlLimit      int64  `yaml:"mysql_limit" json:"mysql_limit"`
	KafkaAddr       string `yaml:"kafka_addr" json:"kafka_addr"`
	SessExpire      int64  `yaml:"sess_expire" json:"sess_expire"`
}

// Yaml struct of yaml
type Yaml struct {
	Mysql struct {
		User     string `yaml:"user"`
		Host     string `yaml:"host"`
		Password string `yaml:"password"`
		Port     string `yaml:"port"`
		Name     string `yaml:"name"`
	}
	Cache struct {
		Enable bool     `yaml:"enable"`
		List   []string `yaml:"list,flow"`
	}
}

// Yaml1 struct of yaml
type Yaml1 struct {
	SQLConf   Mysql `yaml:"mysql"`
	CacheConf Cache `yaml:"cache"`
}

// Yaml2 struct of yaml
type Yaml2 struct {
	Mysql `yaml:"mysql,inline"`
	Cache `yaml:"cache,inline"`
}

// Mysql struct of mysql conf
type Mysql struct {
	User     string `yaml:"user"`
	Host     string `yaml:"host"`
	Password string `yaml:"password"`
	Port     string `yaml:"port"`
	Name     string `yaml:"name"`
}

// Cache struct of cache conf
type Cache struct {
	Enable bool     `yaml:"enable"`
	List   []string `yaml:"list,flow"`
}

//Yaml the yaml file Cluster struct
type YamlArr struct {
	Testclu struct { //Testclu对应yaml文件里的testclu，yaml文件里的字母要小写，对大小写敏感
		Hostips     []string `yaml:"hostips,flow"` //Hostips是个数组，后面yaml:"要跟yaml文件里的hostips",否则找不到字段
		Port        string   `yaml:"port"`
		DBuser      string   `yaml:"dbuser"`
		Passwd      string   `yaml:"passwd"`
		ServiceName string   `yaml:"service_name"`
	}
}
