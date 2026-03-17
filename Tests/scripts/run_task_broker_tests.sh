#!/bin/bash

set -e 

export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

./TaskBrokerTests ./TaskBrokerServer
