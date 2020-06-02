# C code snippets for Q/KDB+

# Info

## tcp

A simple TCP **client** for connecting to non-KDB endpoints, check [tcp.q](./tcp/tcp.q) for usage.

 - Only supports establishing 1 TCP connection per Q process.

## udp

A simple sender/receiver for UDP datagrams, check [udp.q](./udp/udp.q) for usage.

# Building

Build via Docker with:

```
mark@carbon:~/c$ docker build . -t builder
Sending build context to Docker daemon  195.1kB
Step 1/5 : FROM ubuntu:20.04
 ---> 1d622ef86b13
<snip>
Successfully tagged builder:latest

mark@carbon:~/c/udp$ docker run --rm -ti -v $(pwd):/app builder
mkdir -p l32
mkdir -p l64
gcc -fno-builtin -Wall -fno-omit-frame-pointer -shared -fPIC -DKXVER=3 -O3 udp.c -o l32/udp.so -m32
gcc -fno-builtin -Wall -fno-omit-frame-pointer -shared -fPIC -DKXVER=3 -O3 udp.c -o l64/udp.so

mark@carbon:~/c/tcp$ docker run --rm -ti -v $(pwd):/app builder
mkdir -p l32
mkdir -p l64
gcc -fno-builtin -Wall -fno-omit-frame-pointer -shared -fPIC -DKXVER=3 -O3 tcp.c -o l32/tcp.so -m32
gcc -fno-builtin -Wall -fno-omit-frame-pointer -shared -fPIC -DKXVER=3 -O3 tcp.c -o l64/tcp.so
```

# Running

After building, you can fire up the example `q` files:

## tcp.q
```q
q).tcp.Connect["localhost";12345]
3i
q).tcp.Send "Hello, World!"
13
```

## udp.q
```q
q).udp.Listen 8000
1b
q).udp.Send["localhost";8000;"Hello, World!"]
13i
q)"Received 13 bytes from 127.0.0.1"

q).udp.lp
0x48656c6c6f2c20576f726c6421
q)"c"$.udp.lp
"Hello, World!"
```
