#!/bin/bash

php_out_dir=../../wwwroot/application/ikpure/home/protocols/
go_out_dir=./generate/

#sed - i 's/optional//;1i syntax="proto3";'./ protos/*.proto
#sed -i 's/optional//;1i syntax="proto3";' ./protos/storage/*.proto

function clearPHP()
{
  if [ -d ${php_out_dir} ]; then
	  rm -rf ${php_out_dir}
	fi
}

function clearGo()
{
	rm -rf ${go_out_dir}
}

function buildPHP()
{
	echo "start build proto for php ..."

  clearPHP

	if [ ! -d ${php_out_dir} ]; then
		mkdir -p ${php_out_dir}
	fi

	protoc -I./pb/ ./pb/*.proto --php_out=${php_out_dir}
}

function buildGO()
{
	echo "start build proto for go ..."

    clearGo
	if [ ! -d ${go_out_dir} ]; then
		mkdir -p ${go_out_dir}
	fi

	protoc -I./pb/ ./pb/*.proto --gogofast_out=${go_out_dir}
}

function buildTable()
{
	echo "start build table ..."

	protoc -I./gaea/ ./gaea/table.proto --gogofast_out=./gaea/
}


if [ $# == 0 ]; then
	buildGO
	buildPHP
else
	if [ $1 == "php" ]; then
		buildPHP
	elif [ $1 == "go" ]; then
		buildGO
    elif [ $1 == "clearPHP" ]; then
		clearPHP
	elif [ $1 == "clearGo" ]; then
		clearGo
    elif [ $1 == "table" ]; then
		buildTable
	else
		echo "invaild params"
	fi
fi

