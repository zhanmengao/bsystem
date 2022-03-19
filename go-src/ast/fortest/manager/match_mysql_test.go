package fortest

import (
	"fmt"
	"reflect"
	"testing"
)

func TestSplit(t *testing.T) {
	match := NewMysqlMatch("mysql:user-midplat_global;passwd-edec43a8f72ebeb88950746db71eee59;dbName-midplat_global;timeoutMsec-500;servAddr-172.25.16.17:12100;sql-select char_msg_unfiltered from t_chat_msg order by send_time limit 1000;type-string")
	fmt.Println(match)

	val := reflect.New(match.retType)
	pstr := val.Interface().(*string)
	str := "okok"
	*pstr = str
	fmt.Println(getVal(val.Interface()))
}
