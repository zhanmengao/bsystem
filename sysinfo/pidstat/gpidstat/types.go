package pidstat

type TCPUInfo struct {
	Pid     uint64
	User    float64
	Sys     float64
	Guest   float64
	Cpu     float64
	CpuNum  int32
	Command string
}

type TTotalCpuOccupy struct {
	user   int
	nice   int
	system int
	idle   int
}

type TProcCpuOccupy struct {
	pid    int
	utime  int //user time
	stime  int //kernel time
	cutime int //all user time
	cstime int //all dead time
}
