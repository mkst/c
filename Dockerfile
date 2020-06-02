FROM ubuntu:20.04

RUN apt-get update && \
  apt-get install -y \
    gcc \
    libc6-dev-i386 \
    make

RUN mkdir -p /app

WORKDIR /app

CMD make
