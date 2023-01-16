package pidstat

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"strconv"
	"strings"
	"time"
)

func CPUInfoByPidStat() (info *TCPUInfo, err error) {
	cmd := exec.Command("pidstat", "-u", "-p", strconv.Itoa(os.Getpid()))
	pidcpu, err := cmd.CombinedOutput()
	if err != nil {
		return
	}
	line := strings.Split(string(pidcpu), "\n")
	ss := strings.Split(line[3], "    ")
	for _, s := range ss {
		log.Println(s)
	}
	return
}

func CPUInfoByProc(pid int) float64 {

	totalcputime1 := getCpuTotalOccupy()
	procputime1 := getCpuProcOccupy(pid)

	time.Sleep(time.Duration(200000) * time.Microsecond)

	totalcputime2 := getCpuTotalOccupy()
	procputime2 := getCpuProcOccupy(pid)

	pcpu := 0.0
	if 0 != totalcputime2-totalcputime1 {
		pcpu = float64(100.0 * (procputime2 - procputime1) / (totalcputime2 - totalcputime1))
	}
	return pcpu
}

func getCpuTotalOccupy() int {
	body, err := ioutil.ReadFile("/proc/stat")
	if err != nil {
		return 0
	}
	name := ""
	info := &TTotalCpuOccupy{}
	_, err = fmt.Sscanf(string(body), "%s %d %d %d %d", &name, &info.user, &info.nice, &info.system, &info.idle)
	if err != nil {
		return 0
	}
	return info.user + info.nice + info.system
}

func getCpuProcOccupy(pid int) int {
	body, err := ioutil.ReadFile(fmt.Sprintf("/proc/%d/stat", pid))
	if err != nil {
		return 0
	}
	info := &TProcCpuOccupy{}
	str := string(body)
	ss := strings.Split(str, " ")
	_, err = fmt.Sscanf(str, "%d", &info.pid)
	if err != nil {
		return 0
	}
	//sscanf(q,"%ld %ld %ld %ld",&t.utime,&t.stime,&t.cutime,&t.cstime);
	if len(ss) >= 15 {
		info.utime, _ = strconv.Atoi(ss[14])
	}
	if len(ss) >= 16 {
		info.stime, _ = strconv.Atoi(ss[15])
	}
	if len(ss) >= 17 {
		info.cutime, _ = strconv.Atoi(ss[16])
	}
	if len(ss) >= 18 {
		info.cstime, _ = strconv.Atoi(ss[17])
	}
	return info.cstime + info.utime + info.stime + info.cutime
}
