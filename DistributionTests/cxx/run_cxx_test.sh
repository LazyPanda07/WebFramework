#!/bin/bash

set -e

./cpp_test & sleep 1

curl http://127.0.0.1:8080 > out.json

md5sum reference.json
md5sum out.json
