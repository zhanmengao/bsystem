FROM scratch
ARG TARGETARCH
EXPOSE 8080 8081 8082 8083
COPY hotrod-linux-$TARGETARCH /go/bin/hotrod-linux
ENTRYPOINT ["/go/bin/hotrod-linux"]
CMD ["all"]
