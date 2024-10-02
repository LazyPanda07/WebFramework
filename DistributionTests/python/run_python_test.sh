#!/bin/bash

set -e

python3 main.py & sleep 1

while [ ! -f start.txt ]
do
  sleep 1
done

curl http://127.0.0.1:8080 > out.json
