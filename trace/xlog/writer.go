package xlog

import (
	"errors"
	"fmt"
	"os"
	"path"
	"sync"
	"time"
)

const DefaultLogFileName = "xlog"

type fileOp struct {
	fn string
	fp *os.File
}
type Writer struct {
	app    string
	dir    string
	name   string
	fileOp *fileOp
	slice  bool
	chExit chan bool
	sync.RWMutex
}

func NewWriter(app, dir, name string, slice bool) (wr *Writer, err error) {
	st, err := os.Stat(dir)
	if err != nil {
		if err = os.MkdirAll(dir, 0755); err != nil {
			return
		}
	} else if !st.IsDir() {
		err = errors.New("Err Server Internal Unknown")
		return
	}

	if name == "" {
		name = DefaultLogFileName
	}

	wr = &Writer{
		app: app,
		dir: dir,
		//fileOp:  *fileOp,
		slice:  slice,
		chExit: make(chan bool),
	}

	var fn string

	if slice {
		fn = wr.getFileName(name)
	} else {
		fn = path.Clean(fmt.Sprintf("%s/%s.log", dir, name))
	}

	fp, err := checkAndOpen(fn)
	if err != nil {
		return nil, err
	}
	wr.fileOp = &fileOp{fn: fn, fp: fp}

	go wr.checker()
	return
}

func (f *Writer) Write(data []byte) (n int, err error) {

	if f.slice {
		//new file open
		var fp *os.File
		f.Lock()
		fn := f.getFileName(f.name)
		if fp, err = checkAndOpen(fn); err != nil {
			f.Unlock()
			return
		}
		f.Unlock()

		fp = f.fileOp.fp

		n, err = fp.Write(data)
	} else {
		n, err = f.fileOp.fp.Write(data)
	}

	return
}

func (f *Writer) Close() {
	f.Lock()
	defer f.Unlock()

	f.fileOp.fp.Close()

	f.chExit <- true
}

func (f *Writer) checker() {
	tk := time.NewTicker(1 * time.Second)
	for {
		select {
		case <-tk.C:
			f.doCheck(f.fileOp)
		case <-f.chExit:
			return
		}
	}
}

func (f *Writer) getKeyByMod(mod string) string {
	return f.app + "_error_" + mod
}

func (f *Writer) getFileName(mod string) string {
	t := time.Now()
	key := f.getKeyByMod(mod)
	d := path.Clean(fmt.Sprintf("%s/%d%02d%02d/%s_%02d.log", f.dir, t.Year(), t.Month(), t.Day(), key, t.Hour()))
	return d

}

func (f *Writer) doCheck(fop *fileOp) {
	var (
		fn       string
		fp       *os.File
		st1, st2 os.FileInfo
		err      error
	)
	fn = f.getFileName(string(ModCommon))
	st1, err = os.Stat(fn)
	st2, _ = fop.fp.Stat()
	if fn != fop.fn || //文件名不同，代表文件发生了切换
		err != nil || //新的文件无法不存在，代表被删除
		!os.SameFile(st1, st2) { //文件不同，代表被删除
		if fp, err = checkAndOpen(fn); err != nil {
			return
		}
		tmpfp := fop.fp
		fop.fp = fp
		fop.fn = fn
		time.AfterFunc(2*time.Second, func() {
			tmpfp.Close()
		})
	}
}

func checkAndOpen(fn string) (fp *os.File, err error) {
	dir := path.Dir(fn)
	if _, err = os.Stat(dir); err != nil {
		if err = os.MkdirAll(dir, 0755); err != nil {
			return
		}
	}
	fp, err = os.OpenFile(fn, os.O_CREATE|os.O_APPEND|os.O_RDWR, 0644)
	return
}
