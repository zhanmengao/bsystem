FROM scratch

EXPOSE 5775/udp 6831/udp 6832/udp 5778
COPY agent-linux /go/bin/
ENTRYPOINT ["/go/bin/agent-linux"]
