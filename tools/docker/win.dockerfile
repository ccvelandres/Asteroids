#!docker
# FROM mcr.microsoft.com/windows/nanoserver:ltsc2022
FROM ubuntu:jammy
RUN apt update
RUN apt install -y cmake ninja-build build-essential wget 
RUN apt install -y mingw-w64 binutils-mingw-w64





# FROM ubuntu
# ENV TZ=Asia/Manila
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# RUN apt-get update; \
#     apt-get install -y --no-install-recommends \
#         build-essential cmake libsdl2-dev;
# WORKDIR /project
# ENTRYPOINT ["/bin/bash"]