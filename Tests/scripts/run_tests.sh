#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

./Server ${WEB_FRAMEWORK_SERVER_CONFIG} & sleep 5

./Basic ${WEB_FRAMEWORK_SERVER_CONFIG}

sleep 600
