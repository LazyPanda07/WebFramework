FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y cmake build-essential git uuid-dev curl bash gpg sudo

RUN curl -s --compressed "https://LazyPanda07.github.io/web_framework_ppa/KEY.gpg" | gpg --dearmor | tee /etc/apt/trusted.gpg.d/web_framework_ppa.gpg >/dev/null
RUN curl -s --compressed -o /etc/apt/sources.list.d/web_framework.list "https://LazyPanda07.github.io/web_framework_ppa/web_framework.list"
RUN apt update

RUN apt install -y web-framework-static-dev=1.0.6
