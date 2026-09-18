FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    gcc \
    gdb \
    python3 \
    python3-pip

RUN pip3 install pwntools

WORKDIR /lab1

CMD ["/bin/bash"]
