#!/bin/bash

set -e

export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

./cc_test WebFramework & sleep 1

curl http://127.0.0.1:8080 > out.json

md5sum reference.json
md5sum out.json
