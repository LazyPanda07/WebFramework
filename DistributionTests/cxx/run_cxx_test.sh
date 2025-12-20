#!/bin/bash

set -e

export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

./cxx_test & sleep 1

while [ ! -f "start.txt" ]; do
    sleep 1
done

curl http://127.0.0.1:8080 > out.json

md5sum reference.json
md5sum out.json
