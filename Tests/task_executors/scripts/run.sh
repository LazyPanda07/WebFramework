#!/bin/bash

set -e 

export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

./task_broker_test ./server

exit 0

./server & sleep 1

curl http://127.0.0.1:64000/cxx
curl http://127.0.0.1:64000/cc
curl http://127.0.0.1:64000/python
curl http://127.0.0.1:64000/csharp

sleep 5

files=$(ls | grep .txt | wc -l)

if [ "${files}" -ne 16 ]; then
	exit 1
fi
