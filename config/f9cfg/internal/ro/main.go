package ro

import (
	"config/f9cfg/internal/meta"
	"config/f9cfg/internal/ro/visit"
	"go/ast"
	"go/parser"
	"go/token"
	"log"
	"os"
	"path"
)

var (
	s *visit.Scan
)

func Run(pbGoDir string) (pkgName string) {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	st, err := os.Stat(pbGoDir)
	if err != nil {
		panic(err)
	}
	if !st.IsDir() {
		panic(pbGoDir + " not a dir")
	}
	if err = os.Remove(path.Join(pbGoDir, "readonly.pb.go")); err != nil {
		log.Println(err)
	}
	tfs := token.NewFileSet()
	pkg, err := parser.ParseDir(tfs, pbGoDir, func(info os.FileInfo) bool {
		return true
	}, parser.AllErrors)
	if err != nil {
		panic(err)
	}
	log.Printf("total size :%d\n", len(pkg))
	s = &visit.Scan{
		MetaList:   make([]*meta.RoMeta, 0),
		AlreadyGen: make(map[string]struct{}),
	}
	if len(pkg) != 1 {
		panic("package illegal")
	}
	for k, v := range pkg {
		s.PkgName = k
		ast.Walk(s, v)
	}

	log.Printf("package name is:%s\n", s.PkgName)

	for _, v := range s.CfgList {
		m := meta.NewROMeta(v.Name.Name)
		meta.GenMeta(s.TypeMap, v.Type.(*ast.StructType).Fields, m)
		s.MetaList = append(s.MetaList, m)
	}

	//开始生成模板
	if err = genTemplateList(path.Join(pbGoDir, "readonly.pb.go")); err != nil {
		log.Fatal(err)
	}
	log.Println("ro gen success !")
	pkgName = s.PkgName
	return
}
