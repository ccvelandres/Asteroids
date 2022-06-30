#!/bin/bash

docker build -t gamedev -f "$(pwd)/win.dockerfile" .
project_root=$(realpath "$(pwd)/../..")
docker run -v "$project_root":/project -dit --name ast gamedev