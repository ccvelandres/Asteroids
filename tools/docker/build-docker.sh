#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_ROOT=$(realpath "${SCRIPT_DIR}/../..")

docker build -t gamedev -f "$(SCRIPT_DIR)/Dockerfile" .
docker run -v "$PROJECT_ROOT":/project -dit --name ast gamedev