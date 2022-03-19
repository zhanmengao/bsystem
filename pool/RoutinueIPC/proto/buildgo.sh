#/bin/sh
#protoc --go_out=. *.proto

json=ignore protoc --gogofast_out=. *.proto
