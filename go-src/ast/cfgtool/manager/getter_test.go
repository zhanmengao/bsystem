package forevernine_com_config

import (
	"fmt"
	"testing"
)

func TestGetter(t *testing.T) {
	ui := defaultGetter("123")
	fmt.Println(ui.UID)

	var uid int64 = 2222
	ui = defaultGetter(&uid)
	fmt.Println(ui.UID)
}
