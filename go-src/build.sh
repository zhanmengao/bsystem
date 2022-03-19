#/bin/sh

cd go-go1.4-bootstrap-20171003/src

chmod u+x *.bash
GOOS=linux GOARCH=amd64 CGO_ENABLED=0 ./make.bash

cd ../../go-go1.15.12/src

chmod u+x *.bash

GOROOT_BOOTSTRAP=/home/wby/Space/C++space/go-src/go-go1.4-bootstrap-20171003 GO_GCFLAGS="-N -l" ./all.bash

