#!/bin/bash

set -e

export LD_LIBRARY_PATH=/usr/local/lib/python3.${PYTHON_MAJOR_VERSION}t/site-packages/web_framework_api/lib:${LD_LIBRARY_PATH}

python3 main.py & sleep 1

while [ ! -f start.txt ]
do
  sleep 1
done

curl http://127.0.0.1:8080 > out.json
