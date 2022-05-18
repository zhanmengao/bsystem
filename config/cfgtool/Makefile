# LDFLAGS
REVISION=$(shell git rev-parse HEAD)
VERSION=$(shell cat .version)
LDFLAGS="-X forevernine.com/ikpure/server/common/global.revision=${REVISION} -X forevernine.com/ikpure/server/common/global.version=${VERSION}"
BRANCH=$(shell git rev-parse --abbrev-ref HEAD)

ENV=
ifeq ($(ENV),)
        ifeq ($(BRANCH),develop)
                ENV=dev
        else ifeq (feature,$(findstring feature,$(BRANCH)))
                ENV=dev
        else
                ENV=prd
        endif
endif

GOGO_PROTO=vendor/github.com/gogo/protobuf
SRV_DIR=/data/home/user00/syhm/srv
SRV_DIR_KING=/data/home/user00/syhm/srvking
SRV_DIR_EGG=/data/home/user00/syhm/srvegg
SRV_BIN_DIR=${GOPATH}/bin


.PHONY: proto pb table config error\
	test gatesrv gamesrv

all: proto gatesrv gamesrv

gamesrv:proto
	make gamesrv_build
	$(shell mkdir -p output/conf/gamesrv)
	cp -f conf/server/gamesrv.ini output/conf/gamesrv/gamesrv.ini

gatesrv:proto
	make gatesrv_build
	$(shell mkdir -p output/conf/gatesrv)
	cp -f conf/server/gatesrv.ini output/conf/gatesrv/gatesrv.ini

stealpoolsrv:proto
	make stealpoolsrv_build
	$(shell mkdir -p output/conf/stealpoolsrv)
	cp -f conf/server/stealpoolsrv.ini output/conf/stealpoolsrv/stealpoolsrv.ini

broadcastsrv:proto
	make broadcastsrv_build
	$(shell mkdir -p output/conf/broadcastsrv)
	cp -f conf/server/broadcastsrv.ini output/conf/broadcastsrv/broadcastsrv.ini

tqsrv:proto
	make tqsrv_build
	$(shell mkdir -p output/conf/tqsrv)
	cp -f conf/server/tqsrv.ini output/conf/tqsrv/tqsrv.ini

firepoolsrv:proto
	make firepoolsrv_build
	$(shell mkdir -p output/conf/firepoolsrv)
	cp -f conf/server/firepoolsrv.ini output/conf/firepoolsrv/firepoolsrv.ini
gmsrv:proto
	make gmsrv_build
	$(shell mkdir -p output/conf/gmsrv)
	cp -f conf/server/gmsrv.ini output/conf/gmsrv/gmsrv.ini

statcollector:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./tool/$@
	$(shell mkdir -p output/conf/statcollector)
	cp -f conf/server/statcollector.ini output/conf/statcollector/statcollector.ini

gamesrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/gamesrv
gatesrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/gatesrv
stealpoolsrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/stealpoolsrv
broadcastsrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/broadcastsrv
tqsrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/tqsrv
firepoolsrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/firepoolsrv
gmsrv_build:
	go build -o ./output/bin -ldflags ${LDFLAGS} --tags ${ENV} ./cmd/gmsrv
proto:
	@echo "gen proto"
	# git submodule update --init
	make pb
	make config
	make error
	$(shell mkdir -p output/bin)
pb:
	$(shell rm pbclass/pb/*)
	$(shell mkdir -p pbclass/pb)
	protoc -I./proto/pb/ ./proto/pb/*.proto --gogofast_out=pbclass/pb
config:
	cd tool && go build -o configparser.tool . && cd ..
	rm -f providers/configs/gamecfg/gamecfgvar/auto_loadConfig.go
	rm -f pbclass/pbconfig/*.go
	./tool/configparser.tool ./pbclass/pb/ ./pbclass/pbconfig/ ./tool/ deepCopy abTest
error:
	go build -o ./tool/errorparser.tool ./tool/errorparser
	./tool/errorparser.tool pbclass/pberror/gen_error_code.go


docker_img:
	docker build -t ikpure_server:latest .
docker_up:
	cp docker-conf/dbcfg.ini output/gaea/ikpure/ikpure_global/dbcfg.ini
	cp docker-conf/srvCfgAddress.ini output/gaea/ikpure/ikpure_global/srvCfgAddress.ini
	docker compose -p ikpure up -d

clean:
	go clean ./cmd/...


gogoproto:
	cd ${GOGO_PROTO} && make clean && make install


test:
	go test -v -count=1 ./test/test



# 本地或者调试用
pb+pull:
	cd proto && git checkout develop
	cd proto && git pull
	make pb

restart+gamesrv:
	$(call local+restart,gamesrv)
copy+restart+gamesrv:
	$(call local+copy,gamesrv)
	make restart+gamesrv

restart+gatesrv:
	$(call local+restart,gatesrv)
copy+restart+gatesrv:
	$(call local+copy,gatesrv)
	make restart+gatesrv

restart+tqsrv:
	$(call local+restart,tqsrv)
copy+restart+tqsrv:
	$(call local+copy,tqsrv)
	make restart+tqsrv

restart+broadcastsrv:
	$(call local+restart,broadcastsrv)
copy+restart+broadcastsrv:
	$(call local+copy,broadcastsrv)
	make restart+broadcastsrv

restart+stealpoolsrv:
	$(call local+restart,stealpoolsrv)
copy+restart+stealpoolsrv:
	$(call local+copy,stealpoolsrv)
	make restart+stealpoolsrv

restart+firepoolsrv:
	$(call local+restart,firepoolsrv)
copy+restart+firepoolsrv:
	$(call local+copy,firepoolsrv)
	make restart+firepoolsrv

define local+restart
	cd /data/servers && ./restart.sh $1
endef

define local+copy
	make $1_build
	cp -f ./output/bin/$1 ./output/$1_bak
	mv ./output/$1_bak /data/servers/bin/$1
endef

