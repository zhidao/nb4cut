CC=gcc
CFLAGS=-Wall -O2

TARGET=nb4cut
all: $(TARGET)
%: %.c
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f *.o *~ $(TARGET)
