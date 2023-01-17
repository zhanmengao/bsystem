package pidstat

import (
	"testing"
)

func TestPidStat(t *testing.T) {
	t.Log(CPUInfoByPidStat(320685))
}
func TestGetCpuTotalOccupy(t *testing.T) {
	t.Log(getCpuTotalOccupy())
}

func TestGetCpuProcOccupy(t *testing.T) {
	t.Log(getCpuProcOccupy(320685))
}
