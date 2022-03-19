package fortest

import (
	"database/sql"
	"log"
	"reflect"
	"strings"

	"forevernine.com/base/srvcore/config"

	"forevernine.com/base/srvcore/datamgr"
)

// MySQL "mysql:user-midplat_global;passwd-edec43a8f72ebeb88950746db71eee59;dbName-midplat_global;servAddr-172.25.16.17:12100;sql-select char_msg_unfiltered from t_chat_msg order by send_time limit 1000;type-string"
type MySQL struct {
	val     string
	db      *sql.DB
	sql     string
	retType reflect.Type

	user        string
	passwd      string
	dbName      string
	servAddr    string
}

func NewMysqlMatch(v string) *MySQL {
	v = strings.ReplaceAll(v, "mysql:", "")
	ret := &MySQL{}
	ss := strings.Split(v, ";")
	for _, v := range ss {
		s2 := strings.Split(v, "-")
		switch s2[0] {
		case "user":
			ret.user = s2[1]
		case "sql":
			ret.sql = s2[1]
		case "passwd":
			ret.passwd = s2[1]
		case "dbName":
			ret.dbName = s2[1]
		case "servAddr":
			ret.servAddr = s2[1]
		case "type":
			switch s2[1] {
			case "string":
				ret.retType = reflect.TypeOf("")
			case "int":
				ret.retType = reflect.TypeOf(1)
			case "float":
				ret.retType = reflect.TypeOf(0.1)
			}
		}
	}

	return ret
}

func (m *MySQL) GetMaybeList() []string {
	var err error
	cfg := &config.DBConfig{
		Addrs:       []string{m.servAddr},
		DBName:      m.dbName,
		Passwd:      m.passwd,
		User:        m.user,
		TimeoutMsec: 500,
	}
	if m.db, err = datamgr.AddMysqlPool(m.dbName, cfg); err != nil {
		log.Println(err)
		return nil
	}
	ret := make([]string, 0)
	var rows *sql.Rows
	rows, err = m.db.Query(m.sql)

	if err != nil {
		log.Println(err)
	} else {
		defer rows.Close()
		val := reflect.New(m.retType)
		for rows.Next() {
			if err = rows.Scan(val.Interface()); err != nil {
				log.Println(err)
			} else {
				ret = append(ret, getVal(val.Interface()))
			}
		}
	}
	return ret
}


