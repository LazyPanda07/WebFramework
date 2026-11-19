#!/bin/bash

set -e 

FXR_PATH=$(printf ":%s" /usr/share/dotnet/host/fxr/10.*)
FXR_PATH=${FXR_PATH:1}

export LD_LIBRARY_PATH=$(pwd):${FXR_PATH}:${LD_LIBRARY_PATH}

./TaskBrokerTests ./TaskBrokerServer
