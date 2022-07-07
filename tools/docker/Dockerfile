#!docker
FROM ubuntu:jammy
RUN apt update
# Common deps
RUN apt install -y cmake ninja-build build-essential wget curl pkg-config python3 python-is-python3 python3-pip
# Linux package deps
RUN apt install -y libsdl2-dev libsdl2-image-dev libglm-dev libvulkan-dev libglew-dev libopengl-dev
# mingw-w64 package deps
RUN apt install -y mingw-w64 binutils-mingw-w64
# Install mingw-w64 libraries
# RUN 
WORKDIR "/root/project"
ENTRYPOINT ["/bin/bash", "/root/project/tool/scripts/install_pkgs.sh"]
