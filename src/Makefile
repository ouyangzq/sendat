all:sendat
 
OBJS = main.o openDev.o

sendat:$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lpthread
 
clean:
	rm -f sendat *.o
