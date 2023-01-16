package pidstat

import (
	"testing"
)

func TestGetCpuTotalOccupy(t *testing.T) {
	t.Log(getCpuTotalOccupy())
}

func TestGetCpuProcOccupy(t *testing.T) {
	t.Log(getCpuProcOccupy(320685))
}
