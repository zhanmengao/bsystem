package viperr

type BasicConfig struct {
	Platform        string `yaml:"platform" json:"platform"`                 // 平台
	LogLevel        string `yaml:"loglevel" json:"log_level"`                // 日志级别
	LogPath         string `yaml:"logpath" json:"log_path"`                  // 日志路径
	WorkerNum       int64  `yaml:"workernum" json:"worker_num"`              // 工作队列长度
	InnerTimeout    int64  `yaml:"innertimeout" json:"inner_timeout"`        // 全局的内网超时，单位：毫秒
	TimeZone        string `yaml:"timezone" json:"time_zone"`                // 服务器时区
	CoordinatorAddr string `yaml:"coordinatoraddr" json:"coordinator_addr"`  // zk等服务发现组件地址
	MonitorProcTime bool   `yaml:"monitorproctime" json:"monitor_proc_time"` // 是否开启proc超时监控
	Env             string `yaml:"env" json:"env"`                           // 当前的环境: test/beta/prod
	K8s             bool   `yaml:"k8s" json:"k8s"`                           // 是否运行在容器模式下
	PushGateway     string `yaml:"pushgateway" json:"pushgateway"`           //普罗米修斯网关
	FLogDAddr       string `yaml:"flogdaddr" json:"flogd_addr"`              //FLOGD地址
	ReqTimeout      int64  `yaml:"reqtimeout" json:"req_timeout"`
	GinMode         string `yaml:"ginmode" json:"gin_mode"`
	DBEngine        string `yaml:"dbengine" json:"db_engine"`
	MysqlLimit      int64  `yaml:"mysqllimit" json:"mysql_limit"`
	KafkaAddr       string `yaml:"kafkaaddr" json:"kafka_addr"`
	SessExpire      int64  `yaml:"sessexpire" json:"sess_expire"`
}

type Config struct {
	Mysql Mysql `yaml:"mysql"`
}

type Mysql struct {
	RdsHost string `yaml:"rds_host" mapstructure:"rds_host"`
	RdsPort int    `yaml:"rds_port" mapstructure:"rds_port"`
}
