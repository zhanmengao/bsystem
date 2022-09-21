package cfgtool

import (
	"config/f9cfg/internal/generator"
	"config/f9cfg/internal/ro"
	"testing"
)

func TestConfig(t *testing.T) {
	generator.Run("D:/go_workspace/MidPlatProto/go/pb", "forevernine.com/midplat/midplat_proto/go/pb", "pbconf", "")
}

func TestRO(t *testing.T) {
	ro.Run("D:/go_workspace/MidPlatProto/go/pb")
}
