FROM python

COPY files /tmp/files/

RUN mkdir /app && \
    chmod -R 755 /app && \
    mv /tmp/files/app / && \
    useradd app && \
    chown -R root:root /app && \
    sed -i 's/deb.debian.org/mirrors.tuna.tsinghua.edu.cn/' /etc/apt/sources.list && \
    sed -i '/security/d' /etc/apt/sources.list && \
    apt-get update -y
RUN apt install socat -y && \
	apt-get -y install software-properties-common python3-pip wget && \
    pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple && \
    pip install -r /tmp/files/requirements.txt && \
    rm -rf /tmp/files

EXPOSE 10000
WORKDIR /app
ENTRYPOINT socat TCP4-LISTEN:10000,tcpwrap=script,reuseaddr,fork EXEC:"/usr/bin/env python3 -u /app/app.py"
