#!/bin/bash

set -e 

export LD_LIBRARY=$(pwd):${LD_LIBRARY_PATH}

./Core --server_config multi_threaded_redis_config.json --run_redis_tests --run_arguments ./Server
