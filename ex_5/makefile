head=vrmp.h
srcs=vrmp.cc
objs=vrmp.o
opts=-w-g-c
all:
	vrmp
vrmp:
	$(objs)
	g++ $(objs)-o vrmp
vrmp.o:
	$(srcs)$(head)
	g++ $(opts)$(srcs)
clean:
	rm vrmp*.o
