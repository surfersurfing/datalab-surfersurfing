FROM ubuntu:20.04

RUN sed -i 's/archive.ubuntu.com/mirrors.tuna.tsinghua.edu.cn/g' /etc/apt/sources.list && \
    sed -i 's/security.ubuntu.com/mirrors.tuna.tsinghua.edu.cn/g' /etc/apt/sources.list && \
    sed -i 's/archive.canonical.com/mirrors.tuna.tsinghua.edu.cn/g' /etc/apt/sources.list

RUN apt-get update && apt-get install -y \
    gcc \
    gcc-multilib \
    g++ \
    make \
    clang-format \
    git \
    python3 \
    python3-pip \
    python3-venv

COPY . /app

WORKDIR /app

RUN python3 -m venv venv && \
    . venv/bin/activate && \
    pip install pycparser -i  https://mirrors.aliyun.com/pypi/simple/

CMD ["/bin/bash"]