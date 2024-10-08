#!/bin/bash

set -e

export LD_LIBRARY_PATH=$(pwd)/runtimes/linux-x64/native:${LD_LIBRARY_PATH}

./csharp_test & sleep 1

curl http://127.0.0.1:8080 > out.json

md5sum reference.json
md5sum out.json

diff reference.json out.json
