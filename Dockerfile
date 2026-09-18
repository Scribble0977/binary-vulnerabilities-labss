FROM ubuntu:20.04

# щоб apt не питав часовий пояс інтерактивно
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    binutils \
    nasm \
    python3 \
    python3-pip \
    netcat \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install pwntools

WORKDIR /lab2_shellcode