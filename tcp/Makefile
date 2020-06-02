CCOPTS = -fno-builtin -Wall -fno-omit-frame-pointer -shared -fPIC -DKXVER=3 -O3
all:
	mkdir -p l32
	mkdir -p l64
	gcc $(CCOPTS) tcp.c -o l32/tcp.so -m32
	gcc $(CCOPTS) tcp.c -o l64/tcp.so
