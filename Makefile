
CFLAGS = -c -I/usr/include/SDL/
LDFLAGS = -lasound -lSDL
OBJS = alsa_seq.o main.o

all: $(OBJS) joy2midi midi_listen

.c.o:
	$(CC) $(CFLAGS) -o $@ $*.c

.cpp.o:
	$(CXX) $(CFLAGS) -o $@ $*.cpp

joy2midi: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

midi_listen: midi_listen.o
	$(CC) -o $@ $^ -lasound

clean:
	rm -f $(OBJS) midi_listen.o joy2midi midi_listen > /dev/null

