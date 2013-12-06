EXTRAFLAGS:= -Wall -Werror -pedantic -m32 --std=c99 -ggdb -O0
#OBJS=sched.o
#HEADS=sched.h
#INCLUDE=-include /usr/include/errno.h

.PHONY: all clean

#tester: $(OBJS) $(HEADS)
	 #gcc $(INCLUDE) -o tester $(EXTRACPPFLAGS) tester.cpp $(OBJS)
	 #gcc -c $(EXTRAFLAGS) -o tester $(OBJS)


tester:
	gcc $(EXTRAFLAGS) debug.c -o debug
	chmod +x debug
#gcc peron.o -o hello 
#main: sched.c $(OBJS) $(HEADS)
#	g++ $(INCLUDE) -o $@ $(EXTRACPPFLAGS) $< $(OBJS)

#%.o: %.c
#	gcc -c $(EXTRAFLAGS) -o $@ $<

#all: clean $(EXE) tester 
all: clean $(EXE) tester 

clean:
	rm -f $(EXE) *.o




