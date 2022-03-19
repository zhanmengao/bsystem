src=$(wildcard *.c)
obj=$(patsubst %.c,%.o,$(src))
out=$(patsubst %.c,%.out,$(src))

ALL:$(out)

%.out:%.o
	gcc $^ -o $@
%.o:%.c
	gcc $^ -o $@ -c
clean:
	-rm -rf $(obj) $(out)	
