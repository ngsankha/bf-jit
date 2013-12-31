LIBJIT_PATH = libjit
LIBJIT_INCLUDE_PATH = $(LIBJIT_PATH)/include
LIBJIT_LIB_PATH = $(LIBJIT_PATH)/jit/libs
LIBJIT_AR = $(LIBJIT_LIB_PATH)/libjit.a

CC = gcc
LD = gcc
CCOPT = -g -O0
CCFLAGS = -c $(CCOPT)
LDFLAGS = -lpthread -lm -ldl -lrt

all: bf-jit

bf-jit: bf-jit.o
	$(LD) $^ $(LIBJIT_AR) $(LDFLAGS) -o $@

bf-jit.o: src/bf-jit.c
	$(CC) -I$(LIBJIT_INCLUDE_PATH) -I. $(CCFLAGS) $^ -o $@

clean:
	rm -rf *.o bf-jit


