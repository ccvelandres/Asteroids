#!docker
# FROM mcr.microsoft.com/windows/nanoserver:ltsc2022
FROM amd64/windows

# Install chocolatey
RUN @powershell -NoProfile -ExecutionPolicy unrestricted -Command "(iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))) >$null 2>&1"
RUN choco install mingw
CMD ["cmd.exe"]







# FROM ubuntu
# ENV TZ=Asia/Manila
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# RUN apt-get update; \
#     apt-get install -y --no-install-recommends \
#         build-essential cmake libsdl2-dev;
# WORKDIR /project
# ENTRYPOINT ["/bin/bash"]