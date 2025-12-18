FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV ANDROID_VERSION=android-35
ENV SDK_INSTALL_NAME=platforms;android-35
ENV BUILD_TOOLS_NAME=build-tools;35.0.0
ENV FLUTTER_VERSION=3.38.5
ENV FLUTTER_PATH=/opt/flutter
ENV FLUTTER_BIN_PATH=${FLUTTER_PATH}/bin

RUN apt update
RUN apt install -y cmake build-essential git uuid-dev curl bash gpg sudo wget unzip zip openjdk-21-jdk
RUN apt install -y dotnet-sdk-8.0 python3

RUN wget https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip -O tools.zip
RUN unzip tools.zip
RUN rm -rf tools.zip
RUN mkdir latest && cd cmdline-tools && mv * ../latest/ && mv ../latest . && cd .. && mkdir -p Android/Sdk && mv cmdline-tools Android/Sdk && cd Android/Sdk/cmdline-tools/latest/bin && yes | ./sdkmanager --licenses

RUN wget https://storage.googleapis.com/flutter_infra_release/releases/stable/linux/flutter_linux_${FLUTTER_VERSION}-stable.tar.xz
RUN tar -xf flutter_linux_${FLUTTER_VERSION}-stable.tar.xz -C /opt/
RUN rm -rf flutter_linux_${FLUTTER_VERSION}-stable.tar.xz

ENV ANDROID_HOME=/Android/Sdk
ENV PATH=${FLUTTER_BIN_PATH}:${ANDROID_HOME}:${ANDROID_HOME}/cmdline-tools:${ANDROID_HOME}/platforms:${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platforms/${ANDROID_VERSION}:${PATH}

RUN sdkmanager "platform-tools" "${SDK_INSTALL_NAME}" "${BUILD_TOOLS_NAME}"

RUN git config --system --add safe.directory /opt/flutter
RUN flutter config --android-sdk ${ANDROID_HOME}

RUN curl -s --compressed "https://LazyPanda07.github.io/web_framework_ppa/KEY.gpg" | gpg --dearmor | tee /etc/apt/trusted.gpg.d/web_framework_ppa.gpg >/dev/null
RUN curl -s --compressed -o /etc/apt/sources.list.d/web_framework.list "https://LazyPanda07.github.io/web_framework_ppa/web_framework.list"
RUN apt update

RUN apt install -y web-framework-dev=3.2.0
