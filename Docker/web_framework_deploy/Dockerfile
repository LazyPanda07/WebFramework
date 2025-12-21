FROM ubuntu:24.04 AS build

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y cmake build-essential git uuid-dev ninja-build dotnet-sdk-8.0 software-properties-common
RUN add-apt-repository -y ppa:deadsnakes/ppa
RUN apt update
RUN apt install -y python3.14 python3.14-dev python3.14-venv libpython3.14

RUN git clone https://github.com/LazyPanda07/web_framework_server.git
RUN mkdir -p web_framework_server/build

WORKDIR /web_framework_server/build

RUN cmake -DWEB_FRAMEWORK_TAG=v3.2.0 -G "Ninja" ..
RUN cmake --build .
RUN cmake --install .
RUN mkdir /opt/web_framework_server && cp /web_framework_server/build/bin/web_framework_server /opt/web_framework_server

WORKDIR /

RUN rm -rf web_framework_server

FROM ubuntu:24.04 AS deploy

ENV DEBIAN_FRONTEND=noninteractive
ENV LD_LIBRARY_PATH="/usr/lib/dotnet/host/fxr/8.0.22":${LD_LIBRARY_PATH}

RUN apt update
RUN apt install -y curl bash gpg sudo dotnet-sdk-8.0 software-properties-common
RUN add-apt-repository -y ppa:deadsnakes/ppa
RUN apt update
RUN apt install -y python3.14 python3.14-venv libpython3.14

COPY --from=build /opt/web_framework_server /opt/web_framework_server

RUN curl -s --compressed "https://LazyPanda07.github.io/web_framework_ppa/KEY.gpg" | gpg --dearmor | tee /etc/apt/trusted.gpg.d/web_framework_ppa.gpg >/dev/null
RUN curl -s --compressed -o /etc/apt/sources.list.d/web_framework.list "https://LazyPanda07.github.io/web_framework_ppa/web_framework.list"
RUN apt update

RUN apt install -y web-framework=3.2.0

WORKDIR /opt/web_framework_server

ENTRYPOINT ["/opt/web_framework_server/web_framework_server", "configs/config.json"]
