package ast

import (
	"fmt"
	"go/ast"
	"go/token"
	"log"
)

var GFset *token.FileSet

type Fixed struct {
	FuncDesc
}

var GFixedFunc map[string]Fixed //key的格式为Package.Func

//stmtCase 方法分析了语句的各种可能，尽量找全所有。
func stmtCase(stmt ast.Stmt, todo func(call *ast.CallExpr) bool) bool {
	switch t := stmt.(type) {
	case *ast.ExprStmt:
		log.Printf("表达式语句%+v at line:%v", t, GFset.Position(t.Pos()))
		if call, ok := t.X.(*ast.CallExpr); ok {
			return todo(call)
		}
	case *ast.ReturnStmt:
		for i, p := range t.Results {
			log.Printf("return语句%d:%v at line:%v", i, p, GFset.Position(p.Pos()))
			if call, ok := p.(*ast.CallExpr); ok {
				return todo(call)
			}
		}
	case *ast.AssignStmt:
		//函数体里的构造类型 999
		for _, p := range t.Rhs {
			switch t := p.(type) {
			case *ast.CompositeLit:
				for i, p := range t.Elts {
					switch t := p.(type) {
					case *ast.KeyValueExpr:
						log.Printf("构造赋值语句%d:%+v at line:%v", i, t.Value, GFset.Position(p.Pos()))
						if call, ok := t.Value.(*ast.CallExpr); ok {
							return todo(call)
						}
					}
				}
			}
		}
	default:
		log.Printf("不匹配的类型:%T", stmt)
	}
	return false
}

// AllCallCase 调用函数的N种情况
//对函数调用使用todo适配，并返回是否适配成功
//在AllCallCase方法中我们穷举了所有的调用函数的情况（ast.CallExpr），分别对应了000到ccc这13种情况。
func AllCallCase(n ast.Node, todo func(call *ast.CallExpr) bool) (find bool) {

	//函数体里的直接调用 000
	if fn, ok := n.(*ast.FuncDecl); ok {
		for i, p := range fn.Body.List {
			log.Printf("函数体表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
			find = find || stmtCase(p, todo)
		}

		log.Printf("func:%+v done", fn.Name.Name)
	}

	//if语句里
	if ifstmt, ok := n.(*ast.IfStmt); ok {
		log.Printf("if语句开始:%T %+v", ifstmt, GFset.Position(ifstmt.If))

		//if的赋值表达式 111
		if a, ok := ifstmt.Init.(*ast.AssignStmt); ok {
			for i, p := range a.Rhs {
				log.Printf("if语句赋值%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
				switch call := p.(type) {
				case *ast.CallExpr:
					c := todo(call)
					find = find || c
				}
			}
		}

		//if的花括号里面 222
		for i, p := range ifstmt.Body.List {
			log.Printf("if语句内部表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
			c := stmtCase(p, todo)
			find = find || c
		}

		//if的else里面 333
		if b, ok := ifstmt.Else.(*ast.BlockStmt); ok {
			for i, p := range b.List {
				log.Printf("if语句else表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
				c := stmtCase(p, todo)
				find = find || c
			}
		}

		log.Printf("if语句结束:%+v done", GFset.Position(ifstmt.End()))
	}

	//赋值语句 444
	if assign, ok := n.(*ast.AssignStmt); ok {
		log.Printf("赋值语句开始:%T %s", assign, GFset.Position(assign.Pos()))
		for i, p := range assign.Rhs {
			log.Printf("赋值表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
			switch t := p.(type) {
			case *ast.CallExpr:
				c := todo(t)
				find = find || c
			case *ast.CompositeLit:
				for i, p := range t.Elts {
					switch t := p.(type) {
					case *ast.KeyValueExpr:
						log.Printf("构造赋值%d:%+v at line:%v", i, t.Value, GFset.Position(p.Pos()))
						if call, ok := t.Value.(*ast.CallExpr); ok {
							c := todo(call)
							find = find || c
						}
					}
				}
			}
		}
	}

	if gostmt, ok := n.(*ast.GoStmt); ok {
		log.Printf("go语句开始:%T %s", gostmt.Call.Fun, GFset.Position(gostmt.Go))

		//go后面直接调用 555
		c := todo(gostmt.Call)
		find = find || c

		//go func里面的调用 666
		if g, ok := gostmt.Call.Fun.(*ast.FuncLit); ok {
			for i, p := range g.Body.List {
				log.Printf("go语句表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
				c := stmtCase(p, todo)
				find = find || c
			}
		}

		log.Printf("go语句结束:%+v done", GFset.Position(gostmt.Go))
	}

	if deferstmt, ok := n.(*ast.DeferStmt); ok {
		log.Printf("defer语句开始:%T %s", deferstmt.Call.Fun, GFset.Position(deferstmt.Defer))

		//defer后面直接调用 777
		c := todo(deferstmt.Call)
		find = find || c

		//defer func里面的调用 888
		if g, ok := deferstmt.Call.Fun.(*ast.FuncLit); ok {
			for i, p := range g.Body.List {
				log.Printf("defer语句内部表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
				c := stmtCase(p, todo)
				find = find || c
			}
		}

		log.Printf("defer语句结束:%+v done", GFset.Position(deferstmt.Defer))
	}

	if fostmt, ok := n.(*ast.ForStmt); ok {
		//for语句对应aaa和bbb
		log.Printf("for语句开始:%T %s", fostmt.Body, GFset.Position(fostmt.Pos()))
		for i, p := range fostmt.Body.List {
			log.Printf("for语句函数体表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
			c := stmtCase(p, todo)
			find = find || c
		}
	}

	if rangestmt, ok := n.(*ast.RangeStmt); ok {
		//range语句对应ccc
		log.Printf("range语句开始:%T %s", rangestmt.Body, GFset.Position(rangestmt.Pos()))
		for i, p := range rangestmt.Body.List {
			log.Printf("range语句函数体表达式%d:%T at line:%v", i, p, GFset.Position(p.Pos()))
			c := stmtCase(p, todo)
			find = find || c
		}
	}

	return
}

type FindContext struct {
	File      string
	Package   string
	LocalFunc *ast.FuncDecl
}

type FuncDesc struct {
	File    string
	Package string
	Name    string
}

func (f *FindContext) Visit(n ast.Node) ast.Visitor {
	if n == nil {
		return f
	}

	if fn, ok := n.(*ast.FuncDecl); ok {
		log.Printf("函数[%s.%s]开始 at line:%v", f.Package, fn.Name.Name, GFset.Position(fn.Pos()))
		f.LocalFunc = fn
	} else {
		log.Printf("类型%T at line:%v", n, GFset.Position(n.Pos()))
	}

	find := AllCallCase(n, f.FindCallFunc)

	if find {
		name := fmt.Sprintf("%s.%s", f.Package, f.LocalFunc.Name)
		GFixedFunc[name] = Fixed{FuncDesc: FuncDesc{f.File, f.Package, f.LocalFunc.Name.Name}}
	}

	return f
}

// FindCallFunc 方法首先看调用函数是不是选择结构，类似于a.b的结构
//然后对比了调用函数的a.b是不是我们关心的context.WithCancel；最后看第一个实参的名称是不是nil。
func (f *FindContext) FindCallFunc(call *ast.CallExpr) bool {
	if call == nil {
		return false
	}

	log.Printf("call func:%+v, %v", call.Fun, call.Args)

	if callFunc, ok := call.Fun.(*ast.SelectorExpr); ok {
		if fmt.Sprint(callFunc.X) == "context" && fmt.Sprint(callFunc.Sel) == "WithCancel" {
			if len(call.Args) > 0 {
				if argu, ok := call.Args[0].(*ast.Ident); ok {
					log.Printf("argu type:%T, %s", argu.Name, argu.String())
					if argu.Name == "nil" {
						location := fmt.Sprint(GFset.Position(argu.NamePos))
						log.Printf("找到关键函数:%s.%s at line:%v", callFunc.X, callFunc.Sel, location)
						return true
					}
				}
			}
		}
	}

	return false
}

/*
go/ast/ast.go中指明了ast节点的定义：
// All node types implement the Node interface.
type Node interface {
    Pos() token.Pos // position of first character belonging to the node
    End() token.Pos // position of first character immediately after the node
}

// All expression nodes implement the Expr interface.
type Expr interface {
    Node
    exprNode()
}

// All statement nodes implement the Stmt interface.
type Stmt interface {
    Node
    stmtNode()
}

// All declaration nodes implement the Decl interface.
type Decl interface {
    Node
    declNode()
}

语法有三个主体：表达式(expression)、语句(statement)、声明(declaration)，Node是基类，用于标记该节点的位置的开始和结束。
而三个主体的函数没有实际意义，只是用三个interface来划分不同的语法单位,如果某个语法是Stmt的话,就实现一个空的stmtNode函数即可。

列表如下：
普通Node,不是特定语法结构,属于某个语法结构的一部分.

    Comment 表示一行注释 // 或者 / /
    CommentGroup 表示多行注释
    Field 表示结构体中的一个定义或者变量,或者函数签名当中的参数或者返回值
    FieldList 表示以”{}”或者”()”包围的Filed列表

Expression & Types (都划分成Expr接口)

    BadExpr 用来表示错误表达式的占位符
    Ident 比如报名,函数名,变量名
    Ellipsis 省略号表达式,比如参数列表的最后一个可以写成arg...
    BasicLit 基本字面值,数字或者字符串
    FuncLit 函数定义
    CompositeLit 构造类型,比如{1,2,3,4}
    ParenExpr 括号表达式,被括号包裹的表达式
    SelectorExpr 选择结构,类似于a.b的结构
    IndexExpr 下标结构,类似这样的结构 expr[expr]
    SliceExpr 切片表达式,类似这样 expr[low:mid:high]
    TypeAssertExpr 类型断言类似于 X.(type)
    CallExpr 调用类型,类似于 expr()
    StarExpr 表达式,类似于 X
    UnaryExpr 一元表达式
    BinaryExpr 二元表达式
    KeyValueExp 键值表达式 key:value
    ArrayType 数组类型
    StructType 结构体类型
    FuncType 函数类型
    InterfaceType 接口类型
    MapType map类型
    ChanType 管道类型

Statements

    BadStmt 错误的语句
    DeclStmt 在语句列表里的申明
    EmptyStmt 空语句
    LabeledStmt 标签语句类似于 indent:stmt
    ExprStmt 包含单独的表达式语句
    SendStmt chan发送语句
    IncDecStmt 自增或者自减语句
    AssignStmt 赋值语句
    GoStmt Go语句
    DeferStmt 延迟语句
    ReturnStmt return 语句
    BranchStmt 分支语句 例如break continue
    BlockStmt 块语句 {} 包裹
    IfStmt If 语句
    CaseClause case 语句
    SwitchStmt switch 语句
    TypeSwitchStmt 类型switch 语句 switch x:=y.(type)
    CommClause 发送或者接受的case语句,类似于 case x <-:
    SelectStmt select 语句
    ForStmt for 语句
    RangeStmt range 语句

Declarations

    Spec type
        Import Spec
        Value Spec
        Type Spec
    BadDecl 错误申明
    GenDecl 一般申明(和Spec相关,比如 import “a”,var a,type a)
    FuncDecl 函数申明

Files and Packages

    File 代表一个源文件节点,包含了顶级元素.
    Package 代表一个包,包含了很多文件.
*/
