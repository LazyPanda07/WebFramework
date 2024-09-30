#!/bin/bash

set -e

python3 main.py & sleep 1

curl http://127.0.0.1:8080 > out.json

diff reference.json out.json
