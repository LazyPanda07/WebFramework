FROM ubuntu:24.10 AS build

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y cmake build-essential git uuid-dev

RUN git clone https://github.com/LazyPanda07/web_framework_server.git
RUN mkdir -p web_framework_server/build

WORKDIR /web_framework_server/build

RUN cmake -DWEB_FRAMEWORK_TAG=v3.0.11 ..
RUN make install -j $(nproc)
RUN mkdir /opt/web_framework_server && cp /web_framework_server/build/bin/web_framework_server /opt/web_framework_server

WORKDIR /

RUN rm -rf web_framework_server

FROM ubuntu:24.10 AS deploy

RUN apt update
RUN apt install -y curl bash gpg sudo

COPY --from=build /opt/web_framework_server /opt/web_framework_server

RUN curl -s --compressed "https://LazyPanda07.github.io/web_framework_ppa/KEY.gpg" | gpg --dearmor | tee /etc/apt/trusted.gpg.d/web_framework_ppa.gpg >/dev/null
RUN curl -s --compressed -o /etc/apt/sources.list.d/web_framework.list "https://LazyPanda07.github.io/web_framework_ppa/web_framework.list"
RUN apt update

RUN apt install -y web-framework=1.0.6

WORKDIR /opt/web_framework_server

ENTRYPOINT ["/opt/web_framework_server/web_framework_server", "configs/config.json"]
