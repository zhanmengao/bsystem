#!/bin/bash

GoSourceFile=$1
if [ "$GOPATH"  = "" ]; then
    GOPATH=$(go env GOPATH)
fi
echo "GOPATH=$GOPATH ---> PATH=$PATH"
INJECT_TAG() {
type protoc-go-inject-tag || go get github.com/favadi/protoc-go-inject-tag
  for FILENAME in auto_gen/*;do
    protoc-go-inject-tag -input="$FILENAME"
  done
}

rm -rf auto_gen

mkdir auto_gen

rm main.go


os_type=$(uname)

if [ "$os_type" = "Linux" ]; then
    plugin=gogofast
else
    plugin=gogoslick
fi

protoc -I../../proto/ ../../proto/*.proto --${plugin}_out=./auto_gen
INJECT_TAG

if [ $? -eq 0 ]; then
  echo "compile success"
else
  echo "compile failed"
  exit
fi

go build -o build_error

chmod +x build_error
./build_error

if [ $? -ne 0 ];then
    echo "generate errors failed###"
    exit
fi

if [ ! -f "$GoSourceFile" ]; then
    touch "$GoSourceFile"
fi

gofmt tmp.auto.gen.go > "$GoSourceFile"
rm -f tmp.auto.gen.go

rm -rf build_error
rm build_error.tpl
rm gen.sh
rm build_error.go

echo "generate all success !!, go go go"




